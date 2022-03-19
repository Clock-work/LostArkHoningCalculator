#pragma once 
#include <string>

namespace network {

	struct HttpResult
	{
		//statusCode = -1 means that an exception occoured | 200, or 302, etc means success, otherwise its the http status code | wikipedia.org/wiki/HTTP-Statuscode | if its above 10000, the errorcode is a curl error 
		int statusCode = -1;
		std::string headerLines;
		//could be encoded
		std::string resultData;
		//converts utf8/16 characters
		std::wstring getHeaderLinesWString();
		//converts utf8/16 characters
		std::wstring getResultDataWString();
		//checks the statusCode 
		bool isValid() const;
		//throws a web exception if the HttpResult is not valid
		void throwIfNotValid(const std::string& errorInfo);
		//throws a web exception if the HttpResult is not valid
		void throwIfNotValid(const std::wstring& errorInfo);
		/*returns the header value with the given name(headerLines are 1 value per line in the format name:value)
		returns an empty string if it was not found*/
		std::string getHeaderValue(const std::string& name);
		/*returns the cookie value for the given name cookies are in the format name1:value1;name2:value2
		returns an empty string if it was not found*/
		std::string getCookie(const std::string& name);
	};

	std::string urlEncode(std::string str);

	std::string urlDecode(std::string str);

	/*hostname will be top level domain, for example: google.com
	queryString will be full subdomain with additional parameters if needed for example http://google.com/search?var1=value1&var2=value2
	|data will be additional values to set for example dat1=val1&dat2=val2
	cookies might be empty, or in the format cook1=val1;cook2=val2
	contentType might be "application/x-www-form-urlencoded", or  "application/json" */
	HttpResult httpPost(const std::string& hostName, const std::string& queryString, const std::string& data, const std::string& cookies = "", const std::string& contentType = "application/x-www-form-urlencoded", const std::string& encoding = "gzip, deflate");

	/*hostname will be top level domain, for example: google.com
	queryString will be full subdomain with additional parameters if needed for example http://google.com/search?var1=value1&var2=value2
	cookies might be empty, or in the format cook1=val1;cook2=val2 */
	HttpResult httpGet(const std::string& hostName, const std::string& queryString, const std::string& cookies = "", const std::string& encoding = "gzip, deflate");

}