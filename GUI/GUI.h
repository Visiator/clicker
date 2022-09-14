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



#include "GUI_Primitives.h"
#include "GUI_Element.h"

class ELEMENT;

enum COLOR {
    black = 0,
    PanelBG = 0xcccccc,
    PanelBorder = 0x777777,
    MemoBG = 0xeecccc,
    MemoBorder = 0xee0000,
    ListBG = 0xcceecc,
    ListBorder = 0x00ee00,
    
            
};

#ifdef __linux__
class LINUX_PARAM {
public:
    Display     *display = nullptr;
    Window      window = 0;
    int         screen_id = 0;
    GC          graph_ctx = 0;
    XGCValues   *gc_values = nullptr;
    XImage      *image;
    MOUSE_BUTTON_TYPE DetectButtonTypeLinux(unsigned int v) {
        if(v == 1) return MOUSE_BUTTON_TYPE::left;
        if(v == 2) return MOUSE_BUTTON_TYPE::muddle;
        if(v == 3) return MOUSE_BUTTON_TYPE::right;
        return MOUSE_BUTTON_TYPE::left;
    }
};
#endif

class GUI {
private:
    #ifdef __linux__
    LINUX_PARAM linux;
    Atom  atom1, atom2;
    #endif    
public:
    
    
    
    unsigned int Panel1_id = 0, Memo1_id = 0, ProgramList_id = 0;
    
    ELEMENT *Memo1 = nullptr, *ProgramList = nullptr;
    
    GUI();
    SCREEN_BUFFER screen;
    
    FRAME start_position;
    
    ELEMENTS elements;
    
    void MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y );
    
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

