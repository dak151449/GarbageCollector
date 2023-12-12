#pragma once

#include <json/json.h>
#include <iostream>
#include <fstream>

class Config
{
private:
    Json::Value conf;
    std::string getDBUser();
    std::string getDBPassword();
    std::string getDBHost();
    std::string getDBPort();
    std::string getDBName();
    std::string getDBSession();

    bool validate();

    std::string default_conf_name = "default_config.json";
public:
    Config(std::string file="config.json");

    std::string getConnectDB();
    std::vector<std::string> getPackRegexs();
    std::string getPatchDestination();
    bool getIsQuiet();
};
