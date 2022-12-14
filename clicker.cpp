/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#ifdef __linux__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#endif

#include "GUI/GUI.h"
#include "clicker.h"
#include "tools.h"

#ifdef _WIN32

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_

#endif

extern GUI gui;
extern bool GLOBAL_STOP;

void CLICKER::init() {
    execute_is_run = true;
    execute_thread = new std::thread(&CLICKER::execute, this);
}

void CLICKER::execute() {
    /*Display *d = nullptr;
    while(GLOBAL_STOP == false && d == nullptr) {
        d = gui.get_display();
        usleep(100);
    }*/
    
    
    while(GLOBAL_STOP == false) {
        
        //find_window(gui.get_display());    
        
        usleep(3000);
    }
}