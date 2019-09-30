#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class ConfigParser {
public:
    ConfigParser(const std::string& filename, char delim = '=', char comment = '#', bool debugOutput = false)
        : debugOutput(debugOutput)
        , errorFlag(false)
    {
        std::ifstream file;
        file.open(filename);

        if (!file.is_open()) {
            if (debugOutput) {
                std::cerr << "ERROR: Can not open config file " << filename << "!"
                          << std::endl;
            }
            errorFlag = true;
        } else {
            std::string line, key, value;
            while (std::getline(file, line)) {
                if (file.fail()) {
                    // Extracted nothing or max_size characters
                    if (debugOutput) {
                        std::cerr << "ERROR: Reading line of config file "
                                  << filename << " failed!" << std::endl;
                    }

                    errorFlag = true;
                    break;
                }

                // Remove trailing white space characters
                ltrim(line);
                // Check if line is empty or a comment
                if (line.size() != 0 && line.front() != comment) {
                    std::istringstream issLine(line);
                    std::getline(issLine, key, delim);
                    if (!issLine.fail() && rtrim(key).size() != 0 && key.find(comment) == std::string::npos) {
                        std::getline(issLine, value, comment);
                        // Ignore empty values
                        if (!issLine.fail() && trim(value).size() != 0) {
                            // Ignore duplicate entries
                            auto res = values.emplace(key, value);
                            if (!res.second && debugOutput) {
                                std::cerr << "WARNING: Possible duplicate key " << key << " in config file!" << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }

    virtual ~ConfigParser() {}

    std::pair<bool, std::string> getString(const std::string& key, bool optional = false)
    {
        auto val = values.find(key);

        if (val != values.end()) {
            return std::make_pair(true, val->second);
        }

        if (debugOutput && !optional) {
            std::cerr << "ERROR: Can not find " << key << " in config!"
                      << std::endl;
        }

        if (!optional)
            errorFlag = true;
        return std::make_pair(false, std::string());
    }

    std::pair<bool, std::string> getString(const std::string& key, const std::string& fallback)
    {
        auto result = getString(key, true);
        if (result.first)
            return result;
        return std::make_pair(false, fallback);
    }

    std::pair<bool, int> getInt(const std::string& key, bool optional = false)
    {
        auto res = getString(key, optional);
        if (res.first) {
            try {
                return std::make_pair(true, std::stoi(res.second));
            } catch (std::exception&) {
                if (debugOutput && !optional) {
                    std::cerr << "ERROR: Can not parse inter from key " << key
                              << " in config!" << std::endl;
                }
            }
        }

        if (!optional)
            errorFlag = true;
        return std::make_pair(false, -1);
    }

    std::pair<bool, int> getInt(const std::string& key, int fallback)
    {
        auto result = getInt(key, true);
        if (result.first)
            return result;
        return std::make_pair(false, fallback);
    }

    std::pair<bool, unsigned> getUInt(const std::string& key, bool optional = false)
    {
        auto res = getString(key, optional);
        if (res.first) {
            try {
                return std::make_pair(true, std::stoul(res.second));
            } catch (std::exception&) {
                if (debugOutput && !optional) {
                    std::cerr << "ERROR: Can not parse unsigned integer from key "
                              << key << " in config!" << std::endl;
                }
            }
        }

        if (!optional)
            errorFlag = true;
        return std::make_pair(false, 0);
    }

    std::pair<bool, unsigned> getUInt(const std::string& key, unsigned fallback)
    {
        auto result = getUInt(key, true);
        if (result.first)
            return result;
        return std::make_pair(false, fallback);
    }

    std::pair<bool, long> getLong(const std::string& key, bool optional = false)
    {
        auto res = getString(key, optional);
        if (res.first) {
            try {
                return std::make_pair(true, std::stol(res.second));
            } catch (std::exception&) {
                if (debugOutput && !optional) {
                    std::cerr << "ERROR: Can not parse long from key " << key
                              << " in config!" << std::endl;
                }
            }
        }

        if (!optional)
            errorFlag = true;
        return std::make_pair(false, 0);
    }

    std::pair<bool, long> getLong(const std::string& key, long fallback)
    {
        auto result = getLong(key, true);
        if (result.first)
            return result;
        return std::make_pair(false, fallback);
    }

    std::pair<bool, float> getFloat(const std::string& key, bool optional = false)
    {
        auto res = getString(key, optional);
        if (res.first) {
            try {
                return std::make_pair(true, std::stof(res.second));
            } catch (std::exception&) {
                if (debugOutput && !optional) {
                    std::cerr << "ERROR: Can not parse float from key " << key
                              << " in config!" << std::endl;
                }
            }
        }

        if (!optional)
            errorFlag = true;
        return std::make_pair(false, 0);
    }

    std::pair<bool, float> getFloat(const std::string& key, float fallback)
    {
        auto result = getFloat(key, true);
        if (result.first)
            return result;
        return std::make_pair(false, fallback);
    }

    std::pair<bool, double> getDouble(const std::string& key, bool optional = false)
    {
        auto res = getString(key, optional);
        if (res.first) {
            try {
                return std::make_pair(true, std::stof(res.second));
            } catch (std::exception&) {
                if (debugOutput && !optional) {
                    std::cerr << "ERROR: Can not parse double from key " << key
                              << " in config!" << std::endl;
                }
            }
        }

        if (!optional)
            errorFlag = true;
        return std::make_pair(false, 0);
    }

    std::pair<bool, double> getDouble(const std::string& key, double fallback)
    {
        auto result = getDouble(key, true);
        if (result.first)
            return result;
        return std::make_pair(false, fallback);
    }

    inline const std::map<std::string, std::string>& getMap() const { return values; }
    inline bool hasErrors() const { return errorFlag; }

private:
    bool debugOutput;
    bool errorFlag;
    std::map<std::string, std::string> values;

    std::string& rtrim(std::string& str,
        const std::string& chars = "\t\n\v\f\r ") const
    {
        // Remove all follow-up white space characters
        std::size_t pos = str.find_last_not_of(chars);
        if (pos != std::string::npos && (pos + 1) < str.size())
            str.erase(pos + 1);
        return str;
    }

    std::string& ltrim(std::string& str,
        const std::string& chars = "\t\n\v\f\r ") const
    {
        // Remove trailing white space characters
        std::size_t pos = str.find_first_not_of(chars);
        if (pos != std::string::npos)
            str.erase(0, pos);
        // In case the string consists only of white space characters return an empty string
        else
            str.clear();
        return str;
    }

    std::string& trim(std::string& str,
        const std::string& chars = "\t\n\v\f\r ") const
    {
        return rtrim(ltrim(str));
    }
};

#endif