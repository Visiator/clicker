/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include <thread>
#include "tools.h"


void usleep(int milliseconds) {
    if(milliseconds == 1) {
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        return;
    };
    if(milliseconds == 10) {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        return;
    };
    if(milliseconds == 100) {
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
        return;
    };
    if(milliseconds == 333) {
        std::this_thread::sleep_for (std::chrono::milliseconds(333));
        return;
    };
    std::this_thread::sleep_for (std::chrono::milliseconds(milliseconds));
}

void set_GLOBAL_STOP(const wchar_t *str) {
    
}


void wtf(const wchar_t *str) {
    
}