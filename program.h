/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   program.h
 * Author: smorodin
 *
 * Created on September 2, 2022, 2:45 PM
 */

#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <string>
#include <thread>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>


#include "GUI/GUI_Primitives.h"

class PROGRAM_line {
public:
    int id;
    std::string raw;
    
    void clean() {
        id = -1;
        raw = "";
    }
    
    PROGRAM_line() {};
    ~PROGRAM_line() {};
};

class PROGRAM {
public:
    
    void init();
    
    std::vector<PROGRAM_line> line;
    PROGRAM(){};
    ~PROGRAM(){};
};

class PROGRAMS {
public:
    std::vector<PROGRAM> item;
    SCREEN grab_screen_buffer;
    
    void init();
    
    bool execute_is_run = false;
    std::thread* execute_thread = nullptr;
    void execute();    
    
    void wait_execute_close();
    
    void Grab_Sprites(Window w);
    
    PROGRAMS(){};
    ~PROGRAMS(){};
};

#endif /* PROGRAM_H */

