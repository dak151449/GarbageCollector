#include "Config.h"

Config::Config(std::string file)
{
    homedir = getenv("HOME");
    if ( homedir == NULL ) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    std::cout << homedir << std::endl;
    if (homedir != NULL)
        homedir = (std::string(homedir) + "/GarbageCollector/").c_str();

    try {
        std::cout << "home " << homedir + file << "\n";
        std::ifstream F(homedir + file, std::ifstream::binary);
        F >> conf;
        if (!validate()) {
            std::cout << "WARNING! config file is incorrect, the default config is used" << std::endl;
            std::ifstream F_def(default_conf_name, std::ifstream::binary);
            conf.clear();
            F_def >> conf;
        }
    }
    catch(std::exception e){
        std::cout << "etc " << etcdir + default_conf_name << "\n";
        std::cout << "WARNING! config file is incorrect, the default config is used" << std::endl;
        std::ifstream F_def(etcdir + default_conf_name, std::ifstream::binary);
        conf.clear();
        F_def >> conf;
    }
}

std::string Config::getConnectDB()
{   
    std::string user = "user=" + getDBUser();
    std::string passw = "password=" + getDBPassword();
    std::string host = "host=" + getDBHost();
    std::string port = "port=" + getDBPort();
    std::string dbname = "dbname=" + getDBName();
    std::string target_session_attrs = "target_session_attrs=" + getDBSession();     

    return user + " " + passw + " " + host + " " + port + " " + dbname + " " + target_session_attrs;
}

std::vector<std::string> Config::getPackRegexs()
{   
    std::vector<std::string> out;
    for (auto r: conf["pack_regexs"]) {
        out.push_back(r.asString());
    }
    return out;
}

std::string Config::getDBUser()
{
    return conf["user"].asString();
}

std::string Config::getDBPassword()
{
    return conf["password"].asString();
}

std::string Config::getDBHost()
{
    return conf["host"].asString();
}

std::string Config::getDBPort()
{
    return conf["port"].asString();
}

std::string Config::getDBName()
{
    return conf["dbname"].asString();
}

std::string Config::getDBSession()
{
    return conf["target_session_attrs"].asString();
}

std::string Config::getPatchDestination()
{
    return conf["patch_destination"].asString();
}

bool Config::getIsQuiet()
{
return conf["is_quiet"].asBool();
}

bool Config::validate() {

    if (getDBUser() == "" || 
        getDBPassword() == "" || 
        getDBHost() == "" ||
        getDBPort() == "" ||
        getDBName() == "" ||
        getDBSession() == "") {
        return false;
    }
    return true;
}
