#pragma once
#include <iostream>
#include <string>
#include <set>
#include <unistd.h>
#include <mutex>
#include <json/json.h>
#include <optional>
#include <chrono>
#include <vector>
#include <pqxx/pqxx>

class PostgreHandler {
public:
    std::mutex ph_lock;
    int test = 0;
	PostgreHandler();
    void reconnect();
    
    // добавить пакет в бд
    bool addDeprecated(std::string name, bool data); 

    // получить значение по имени пакета или nullopt 
    std::optional<bool> getDeprecated(std::string name);
private:
    std::string table_name;
    std::optional<pqxx::connection> connect;
};