#include "LostArkHoningCalculator/utils/Http.h"
#include <sstream>
#include <zlib.h>
#include <curl/curl.h>
#include <iostream>
#include <string>

#ifdef _MSC_VER
#include <boost/config/compiler/visualc.hpp>
#endif
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "LostArkHoningCalculator/utils/StringManipulation.h"
#include "LostArkHoningCalculator/utils/Functions.h"

namespace utils {

	static void printTree(boost::property_tree::ptree const& pt)
	{
		using boost::property_tree::ptree;
		ptree::const_iterator end = pt.end();
		for ( ptree::const_iterator it = pt.begin(); it != end; ++it )
		{
			std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
			printTree(it->second);
		}
	}

	static void printTree(boost::property_tree::wptree const& pt)
	{
		using boost::property_tree::wptree;
		wptree::const_iterator end = pt.end();
		for ( wptree::const_iterator it = pt.begin(); it != end; ++it )
		{
			std::wcout << it->first << L": " << it->second.get_value<std::wstring>() << std::endl;
			printTree(it->second);
		}
	}

	static std::string decompressGzip(const std::string& input)
	{
		std::string output;
		output.reserve(input.size());
		z_stream stream;
		memset(&stream, 0, sizeof(stream));
		inflateInit2(&stream, 16 + MAX_WBITS);
		stream.next_in = (unsigned char*) input.data();
		stream.avail_in = input.size();
		int ret;
		char buffer[10000];
		do
		{
			stream.next_out = reinterpret_cast<unsigned char*>( buffer );
			stream.avail_out = sizeof(buffer);
			ret = inflate(&stream, 0);
			if ( output.size() < stream.total_out )
			{
				output.append(buffer, stream.total_out - output.size());
			}
		}
		while ( ret == Z_OK );
		inflateEnd(&stream);
		if ( ret != Z_STREAM_END )
		{
			throw std::runtime_error("decompression error");
		}
		return output;
	}

	std::string urlEncode(std::string str)
	{
		std::string new_str = "";
		char c;
		int ic;
		const char* chars = str.c_str();
		char bufHex[10];
		int len = strlen(chars);

		for ( int i = 0; i < len; i++ )
		{
			c = chars[i];
			ic = c;
			if ( c == ' ' )
				new_str += '+';
			else if ( isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' )
				new_str += c;
			else
			{
				sprintf(bufHex, "%X", c);
				if ( ic < 16 )
					new_str += "%0";
				else
					new_str += "%";
				new_str += bufHex;
			}
		}
		return new_str;
	}

	std::string urlDecode(std::string str)
	{
		std::string ret;
		char ch;
		int i, ii, len = str.length();

		for ( i = 0; i < len; i++ )
		{
			if ( str[i] != '%' )
			{
				if ( str[i] == '+' )
					ret += ' ';
				else
				{
					if ( str[i] == '&' && i < str.length() - 3 && str[i + 1] == '#' )
					{
						auto semicolonPos = str.find(';', i);
						if ( semicolonPos != std::string::npos )
						{
							int numberOfAdditionalCharacters = semicolonPos - i;
							auto code = str.substr(i + 2, numberOfAdditionalCharacters - 2);
							int codeValue = 0;
							try
							{
								codeValue = std::stoi(code);
							}
							catch ( const std::exception& e )
							{

							}
							if ( codeValue > 0 )
							{
								ret += (char) codeValue;
								i += numberOfAdditionalCharacters;
							}
							else
							{
								ret += str[i];
							}
						}
						else
						{
							ret += str[i];
						}
					}
					else
					{
						ret += str[i];
					}
				}
			}
			else
			{
				sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
				ch = static_cast<char>( ii );
				ret += ch;
				i = i + 2;
			}
		}
		return ret;
	}

	size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data)
	{
		data->append((char*) ptr, size * nmemb);
		return size * nmemb;
	}

	HttpResult httpRequest(const std::string& hostName, const std::string& queryString, const std::string& cookies, const std::string& encoding, bool sendData, const std::string& postData, const std::string& contentType)
	{
		HttpResult result;
		long responseCode = -1;
		CURLcode code = CURLcode::CURL_LAST;
		try
		{
			CURL* curl = curl_easy_init();
			if ( curl )
			{
				curl_easy_setopt(curl, CURLOPT_URL, queryString.c_str());

				curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
				curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
				curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L);

				curl_easy_setopt(curl, CURLOPT_CAINFO, "res/cacert.pem");//use local certificate
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);//disable ssl checking
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);//also disable ssl checking

				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeFunction);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result.resultData);
				curl_easy_setopt(curl, CURLOPT_HEADERDATA, &result.headerLines);

				struct curl_slist* headers = NULL;
				headers = curl_slist_append(headers, std::string("Host: " + hostName).c_str());
				headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml,application/json;q=0.9,*/*;q=0.8");
				headers = curl_slist_append(headers, std::string("Accept-Encoding: " + encoding).c_str());
				headers = curl_slist_append(headers, std::string("Accept-Language: de,en-US;q=0.7,en;q=0.3").c_str());

				if ( contentType.size() > 0 )
				{
					headers = curl_slist_append(headers, std::string("Content-Type: " + contentType).c_str());
				}

				if ( cookies.size() > 0 )
				{
					headers = curl_slist_append(headers, std::string("Cookie: " + cookies).c_str());
				}

				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

				if ( sendData )
				{
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());//post data
				}

				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);//no progress bar shown=1
				curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);//log info on=1, or off
				code = curl_easy_perform(curl);
				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
				curl_easy_cleanup(curl);

				if ( code != CURLE_OK )
				{
					result.statusCode = 10000 + code;
				}
				else
				{
					result.statusCode = responseCode;
					if ( result.headerLines.find("Content-Encoding: gzip") != std::string::npos && result.resultData.length() > 1 )
					{
						result.resultData = decompressGzip(result.resultData);
					}
				}
			}
			else
			{
				result.statusCode = -1;
			}
		}
		catch ( const std::exception& e )
		{
			result.statusCode = -1;
			std::cout << "Web request failed: " << e.what() << std::endl;
		}
		return result;
	}

	HttpResult httpPost(const std::string& hostName, const std::string& queryString, const std::string& data, const std::string& cookies, const std::string& contentType, const std::string& encoding)
	{
		return httpRequest(hostName, queryString, cookies, encoding, true, data, contentType);
	}

	HttpResult httpGet(const std::string& hostName, const std::string& queryString, const std::string& cookies, const std::string& encoding)
	{
		return httpRequest(hostName, queryString, cookies, encoding, false, "", "");
	}

	std::wstring HttpResult::getHeaderLinesWString()
	{
		return utils::utf8ToWstring(headerLines);
	}

	std::wstring HttpResult::getResultDataWString()
	{
		return utils::utf8ToWstring(resultData);
	}

	bool HttpResult::isValid() const
	{
		if ( statusCode < 0 || statusCode > 308 )
			return false;
		return true;
	}

	void HttpResult::throwIfNotValid(const std::string& errorInfo)
	{
		if ( !isValid() )
		{
			throw std::runtime_error(errorInfo + ". statusCode: " + std::to_string(statusCode));
		}
	}

	void HttpResult::throwIfNotValid(const std::wstring& errorInfo)
	{
		if ( !isValid() )
		{
			throw std::runtime_error(utils::wStringToUtf8(errorInfo + L". statusCode: " + std::to_wstring(statusCode)));
		}
	}

	std::string HttpResult::getHeaderValue(const std::string& name)
	{
		auto lines = utils::splitString(headerLines, '\n');
		for ( auto& line : lines )
		{
			if ( line[line.size() - 1] == '\r' )
				line.pop_back();
			auto values = utils::splitString(line, ':', 2);
			if ( values[0] == name )
			{
				return values[1];
			}
		}
		return "";
	}

	std::string HttpResult::getCookie(const std::string& name)
	{
		auto cookies = utils::splitString(getHeaderValue("Set-Cookie"), ';');
		for ( auto& cookie : cookies )
		{
			auto values = utils::splitString(cookie, '=', 2);
			std::string cookieName = values[0];
			if ( cookieName[0] == ' ' )
				cookieName.erase(cookieName.begin());
			if ( cookieName == name )
			{
				return values[1];
			}
		}
		return "";
	}

}