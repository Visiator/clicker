/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "program.h"
#include "global.h"
#include "tools.h"
#include "GUI/GUI.h"

extern bool GLOBAL_STOP;
extern GLOBAL global;
extern GUI gui;

void PROGRAM::init() {
    
}


void PROGRAMS::init() {
    execute_is_run = true;
    execute_thread = new std::thread(&PROGRAMS::execute, this);
}

void PROGRAMS::execute() {
    Window w = 0;
    while(GLOBAL_STOP == false) {
        
        if(global.WindowListBtnStart) {
            Grab_Sprites(global.WindowListId);
        } else {
            
        }
        
        usleep(1000);
    }
    execute_is_run = false;
}

void PROGRAMS::wait_execute_close() {
    while(execute_is_run == true) {
        usleep(10);
    }
}

unsigned int avg(unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4) {
    unsigned int r, g, b;
    r = ((v1 & 0xff) + (v2 & 0xff) + (v3 & 0xff) + (v4 & 0xff)) / 4;
    g = (((v1 & 0xff00) + (v2 & 0xff00) + (v3 & 0xff00) + (v4 & 0xff00))>>8) / 4;
    b = (((v1 & 0xff0000) + (v2 & 0xff0000) + (v3 & 0xff0000) + (v4 & 0xff0000))>>16) / 4;

    return r | (g<<8) | (b<<16);
}


void PROGRAMS::Grab_Sprites(Window w) {

    if(w == 0) {
        return;
    }
    XWindowAttributes window_attr;
    Display *d = XOpenDisplay(getenv("DISPLAY"));

    XImage      *imageS;

    //d = gui.get_display();
    
    if(!XGetWindowAttributes(d , w, &window_attr)) {
        printf("XGetWindowAttributes(0 false\n");
        return;
    }
    
    
    if(window_attr.width/2 != grab_screen_buffer.w || window_attr.height/2 != grab_screen_buffer.h) {
        //g_mutex.lock();
        grab_screen_buffer.set_size(window_attr.width/2, window_attr.height/2);
        //g_mutex.unlock();
    };

    imageS = XGetImage(d, w, 0, 0, window_attr.width, window_attr.height, AllPlanes, ZPixmap);

    if(imageS == nullptr) {
        w = 0;
    } else {

        unsigned int *qq, *q;
        qq = (unsigned int *)imageS->data;
        q = grab_screen_buffer.buffer;
        int k = 0;
        for(int j=0;j<window_attr.height/2;j++) {

            for(int i=0;i<window_attr.width/2;i++) {
                *q++ = avg( *qq, *(qq+1) , *(qq+window_attr.width), *(qq+window_attr.width+1));
                qq+=2;
            }
            qq += window_attr.width;
            //q += window_attr.width/2;
        }

        XFree(imageS);
    };
    
    XCloseDisplay(d);
}