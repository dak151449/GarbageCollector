#pragma once

#include <iostream>
#include <unistd.h>

class StatusBar
{
private:
    float step = 0.2; 
    int last_len = 0;
    float progress = 0;
public:
    StatusBar(std::string info, float s){
        if (!is_quiet)
            std::cout << info << "\n";
            
        step = s;
    };

    void print_status(std::string info = "");
    void end_status();

    inline static bool is_quiet;
};

