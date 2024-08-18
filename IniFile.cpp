#include "IniFile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

std::string PreparingString(std::string str)
{
    std::string prepared = "";

    for (int i = 0; i < str.size(); i++) 
    {
        if (str[i] == ';') 
        {
            return prepared;
        }
        else if (str[i] != ' ') 
        {
            prepared += str[i];
        }
    }
    return prepared;
}

IniFile::IniFile(const std::string& path)
{
    std::ifstream file;
    std::string str;
    file.open(path);
    std::string section;
    if (file.is_open())
    {
        while (std::getline(file, str))
        {
            str = PreparingString(str);
            if (str[0] == '[' && str[str.size() - 1] == ']')
            {
                int end = str.find(']');
                str = str.substr(1, end - 1);
                if (str.find('[') == -1 && str.find(']') == -1)
                {
                    section = str;
                }
            }
            else
            {
                if (section != "" && str.find('=') != -1)
                {
                    int equal_pos = str.find('=');
                    std::string key = str.substr(0, equal_pos);
                    std::string value = str.substr(equal_pos + 1, str.size() - equal_pos - 1);
                    if (key != "" && value != "") 
                    {
                        data[section][key] = value;
                    }
                }
            }
        }
    }
    file.close();
}
IniFile::~IniFile()
{
    save();
}

void IniFile::save() 
{
    std::ofstream file;
    file.open(filePath);
    for (const auto& section : data) 
    {
        file << "[" << section.first << "]" << std::endl;
        for (const auto& pair : section.second) 
        {
            file << pair.first << "=" << pair.second << std::endl;
        }
    }
}


int IniFile::readInt(const std::string& section, const std::string& key, int def)
{
    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
        if (it->first == section)
        {
            for (auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
            {
                if (nit->first == key)
                {
                    int ans = std::stoi(nit->second);
                    return ans;
                }
            }
            return def;
        }

    }
    return def;
}

double IniFile::readDouble(const std::string& section, const std::string& key, double def)
{
    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
        if (it->first == section)
        {
            for (auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
            {
                if (nit->first == key)
                {
                    double ans = std::stod(nit->second);
                    return ans;
                }
            }
            return def;
        }

    }
    return def;
}

std::string IniFile::readString(const std::string& section, const std::string& key, const std::string& def)
{
    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
        if (it->first == section)
        {
            for (auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
            {
                if (nit->first == key)
                {
                    return nit->second;
                }
            }
            return def;
        }

    }
    return def;
}


bool IniFile::readBool(const std::string& section, const std::string& key, bool def)
{
    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
        if (it->first == section)
        {
            for (auto nit = it->second.cbegin(); nit != it->second.cend(); ++nit)
            {
                if (nit->first == key)
                {
                    for (int i = 0; i < trueValues.size(); i++) {
                        if (nit->second == trueValues[i]) {
                            return true;
                        }
                    }
                    return false;
                }
            }
            return def;
        }

    }
    return def;
}


void IniFile::writeInt(const std::string& section, const std::string& key, int value)
{
    data[section][key] = std::to_string(value);
}

void IniFile::writeDouble(const std::string& section, const std::string& key, double value)
{
    data[section][key] = std::to_string(value);
}
void IniFile::writeString(const std::string& section, const std::string& key, const std::string& value)
{
    data[section][key] = value;
}
void IniFile::writeBool(const std::string& section, const std::string& key, bool value)
{
    data[section][key] = std::to_string(value);
}

bool IniFile::isSectionExist(const std::string& section)
{
    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
        if (it->first == section)
            return true;
    }
    return false;
}

bool IniFile::isKeysExist(const std::string& section, const std::string& key)
{
    for (auto it = data[section].cbegin(); it != data[section].cend(); ++it)
    {
        if (it->first == key)
            return true;
    }
    return false;
}

size_t IniFile::getSectionsCount()
{
    size_t count = 0;
    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
        if (!it->second.empty())
            count++;
    }
    return count;
}

size_t IniFile::getKeysCount(const std::string& section)
{
    size_t count = 0;

    for (auto it = data[section].cbegin(); it != data[section].cend(); ++it)
    {
        count++;
    }
    return count;
}

SectionsMap IniFile::getSections() const
{
    return data;
}

bool IniFile::deleteSection(const std::string& section)
{
    return data.erase(section);
}

bool IniFile::deleteKey(const std::string& section, const std::string& key)
{
    bool ans = false;
    for (auto it = data[section].cbegin(); it != data[section].cend(); ++it)
    {
        if (key == it->first)
            ans = true;
    }
    if (ans)
    {
        data[section].erase(key);
    }
    return ans;
}

bool IniFile::addNewSection(const std::string& section)
{
    if (data.count(section) == 0)
    {
        data[section] = KeysMap();
        return true;
    }
    return false;
}
