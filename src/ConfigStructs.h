#pragma once 
#include "Utils.h"
#include <tuple>

class ConfigLine
{

public:
	std::string name;
	static const char delimiter = ':';
	static const char valueDelimiter = ',';

	ConfigLine(const std::string& line)
	{
		if ( line.find(delimiter) != std::string::npos )
		{
			name = line.substr(0, line.find(delimiter));
		}
		else
		{
			name = line;
		}
	}

	virtual ~ConfigLine()
	{

	}

	virtual std::string getLine() const
	{
		return name;
	}

	//can be an empty, or only information config line
	virtual bool hasValue() const
	{
		return false;
	}

	static bool hasValuePart(const std::string& line)
	{
		return line.find(delimiter) != std::string::npos;
	}

protected:
	std::string getValuePart(const std::string& line) const
	{
		std::string value = "";
		if ( line.find(delimiter) != std::string::npos )
		{
			value = line.substr(line.find(delimiter) + 1);
		}
		return value;
	}

};

class ValueConfigLine
	: public ConfigLine
{

public:
	std::string value;

	ValueConfigLine(const std::string& line)
		: ConfigLine(line), value(getValuePart(line))
	{

	}

	virtual std::string getLine() const override
	{
		return name + delimiter + value;
	}

	//can be an empty, or only information config line
	virtual bool hasValue() const override
	{
		return true;
	}

	void setBoolValue(bool boolValue)
	{
		value = boolValue ? "true" : "false";
	}
	bool getBoolValue()
	{
		std::string mostRecentPart = value;
		if ( value.find(valueDelimiter) != std::string::npos )
		{
			std::vector<std::string> parts = splitString(value, valueDelimiter);
			mostRecentPart = parts.at(parts.size() - 1);
		}
		return ( mostRecentPart == "True" || mostRecentPart == "true" ) ? true : false;
	}
	void setFloatValue(float floatValue)
	{
		value = to_string_noZeros((double) floatValue);
	}
	float getFloatValue()
	{
		std::string mostRecentPart = value;
		if ( value.find(valueDelimiter) != std::string::npos )
		{
			std::vector<std::string> parts = splitString(value, valueDelimiter);
			mostRecentPart = parts.at(parts.size() - 1);
		}
		return std::stod(mostRecentPart);
	}

};

class BoolConfigLine
	: public ConfigLine
{

public:
	bool value;

	BoolConfigLine(const std::string& line)
		: ConfigLine(line)
	{
		std::string valuePart = getValuePart(line);
		value = valuePart == "True" || valuePart == "true" ? true : false;
	}

	virtual std::string getLine() const override
	{
		return name + delimiter + ( value ? "true" : "false" );
	}

	//can be an empty, or only information config line
	virtual bool hasValue() const override
	{
		return true;
	}

};

class ConfigFile
{
private:
	const std::string m_configPath;
	float m_oldConfigVersion;
	//first line is always the config version
	std::vector<ConfigLine*> m_configLines;
	bool m_hasChanged;
	int m_currentLineCounter;

	ConfigLine* createConfigLine(const std::string& line)
	{
		if ( ConfigLine::hasValuePart(line) )
		{
			return new ValueConfigLine(line);
		}
		else
		{
			return new ConfigLine(line);
		}
	}

	void addNewConfigLine(const std::string& line)
	{
		m_hasChanged = true;
		m_configLines.insert(m_configLines.begin() + m_currentLineCounter++, createConfigLine(line));
	}

	//also increases line counter 
	ValueConfigLine* getNextValueConfigLine()
	{
		while ( m_currentLineCounter < m_configLines.size() )
		{
			if ( m_configLines.at(m_currentLineCounter)->hasValue() )
				break;
			m_currentLineCounter++;
		}
		if ( m_currentLineCounter >= m_configLines.size() )
			throw std::runtime_error("end of config file");
		return dynamic_cast<ValueConfigLine*>( m_configLines.at(m_currentLineCounter++) );
	}

	void readConfigVersionLine(float newConfigVersion)
	{
		const std::string versionName = "Config Version(Don't change!)";
		if ( m_configLines.size() >= 1 && m_configLines.at(0)->name == versionName )
		{
			ValueConfigLine* versionLine = getNextValueConfigLine();
			m_oldConfigVersion = versionLine->getFloatValue();
			if ( isMore(newConfigVersion, m_oldConfigVersion) )
			{
				m_hasChanged = true;
				versionLine->setFloatValue(newConfigVersion);
			}
		}
		else
		{
			addNewConfigLine(versionName + ConfigLine::delimiter + to_string_noZeros((double) newConfigVersion));
			m_hasChanged = true;
		}
	}

public:
	ConfigFile(const std::string& configPath, float newConfigVersion)
		: m_configPath(configPath), m_oldConfigVersion(0.1f), m_hasChanged(false), m_currentLineCounter(0)
	{
		std::vector <std::string> lines = readUtf8FileToLines(configPath);
		m_configLines.reserve(lines.size());
		for ( const std::string& line : lines )
		{
			m_configLines.push_back(createConfigLine(line));
		}
		if ( lines.size() == 0 )
		{
			m_oldConfigVersion = 0.0f;//no config file, so not the 0.1f for the config file before the version system
		}
		readConfigVersionLine(newConfigVersion);
	}

	~ConfigFile()
	{
		for ( ConfigLine* configLine : m_configLines )
		{
			delete configLine;
		}
	}

	float getNextValueFloat(float minConfigVersion, float defaultValue, const std::string& defaultName)
	{
		if ( isMore(minConfigVersion, m_oldConfigVersion) )
		{
			addNewConfigLine(defaultName + ConfigLine::delimiter + to_string_noZeros((double) defaultValue));
			return defaultValue;
		}
		return getNextValueConfigLine()->getFloatValue();
	}

	bool getNextValueBool(float minConfigVersion, bool defaultValue, const std::string& defaultName)
	{
		if ( isMore(minConfigVersion, m_oldConfigVersion) )
		{
			addNewConfigLine(defaultName + ConfigLine::delimiter + (defaultValue ? "true" : "false"));
			return defaultValue;
		}
		return getNextValueConfigLine()->getBoolValue();
	}

	std::tuple<float, float> getNextTwoValuesFloat(float minConfigVersion, float firstDefaultValue, const std::string& firstDefaultName, float secondDefaultValue, const std::string& secondDefaultName)
	{
		if ( isMore(minConfigVersion, m_oldConfigVersion) )
		{
			addNewConfigLine(firstDefaultName + ConfigLine::delimiter + to_string_noZeros((double) firstDefaultValue));
			addNewConfigLine(secondDefaultName + ConfigLine::delimiter + to_string_noZeros((double) secondDefaultValue));
			return std::make_tuple(firstDefaultValue, secondDefaultValue);
		}
		float value1 = getNextValueConfigLine()->getFloatValue();
		float value2 = getNextValueConfigLine()->getFloatValue();
		return std::make_tuple(value1, value2);
	}

	//writes the changes to the config file (if new options have been added) and returns if changes happened
	bool writeChanges()
	{
		if ( m_hasChanged )
		{
			std::vector <std::string> lines;
			lines.reserve(m_configLines.size());
			for ( ConfigLine* configLine : m_configLines )
			{
				lines.push_back(configLine->getLine());
			}
			writeUtf8FileFromLines(m_configPath, lines);
		}
		return m_hasChanged;
	}

};