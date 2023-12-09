#include "StatusBar.h"

void StatusBar::print_status(std::string info)
{   
    if (is_quiet)
        return;

    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) <<"%\t" << info;
    for (int i = 0; i < last_len - int(info.size()); i++) {
        std::cout << " ";
    }

    std::cout << "\r";
    std::cout.flush();

    progress += step;
    
    last_len = info.size();
}

void StatusBar::end_status()
{   
    if (is_quiet)
        return;

    progress = 1.0;
    print_status("DONE");
    std::cout << std::endl;
}
