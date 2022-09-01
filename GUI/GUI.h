/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   GUI.h
 * Author: root
 *
 * Created on August 30, 2022, 3:05 PM
 */

#ifndef GUI_H
#define GUI_H

#include <thread>

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
#endif

#include "font.h"
#include "GUI_Element.h"


#ifdef __linux__
class LINUX_PARAM {
public:
    Display     *display = nullptr;
    Window      window = 0;
    int         screen_id = 0;
    GC          graph_ctx = 0;
    XGCValues   *gc_values = nullptr;
    XImage      *image;
};
#endif

class GUI {
private:
    #ifdef __linux__
    LINUX_PARAM linux;
    #endif    
public:
    
    unsigned int Memo1_id = 0;
    
    ELEMENT *Memo1 = nullptr;
    
    GUI();
    SCREEN_BUFFER screen;
    
    FRAME start_position;
    
    ELEMENTS elements;
    
    void create_elements();
    
    bool low_level_init();
    void finish();
    void wait_run();
    void wait_execute_close();
    
    bool execute_is_run = false;
    std::thread* execute_thread = nullptr;
    void execute();
    
    GUI(const GUI& orig);
    virtual ~GUI();
private:

};

#endif /* GUI_H */

