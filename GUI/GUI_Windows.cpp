/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include <thread>
#include "GUI.h"
#include "../tools.h"

extern bool GLOBAL_STOP;

#ifdef _WIN32

    char msg [256]={"test"};
    

    void GUI::execute() {
    
        while(GLOBAL_STOP == false) {
            
            
            usleep(10);
        }
        execute_is_run = false;
    }

    bool GUI::low_level_init() {
        
        start_position.set_xywh(50, 50, 1000, 900);
        /*
        if ((linux.display_ = XOpenDisplay(getenv("DISPLAY"))) == NULL)
        {
            wtf("Can't connect X server");//, strerror(errno));
            return false;
        }
        
        linux.screen_id = XDefaultScreen(linux.display_);
        
        linux.window = XCreateSimpleWindow(linux.display_, RootWindow(linux.display_, linux.screen_id), start_position.getx(), start_position.gety(), start_position.getw(), start_position.geth(), 10, XBlackPixel(linux.display_, linux.screen_id), XWhitePixel(linux.display_, linux.screen_id));
 
        linux.graph_ctx=XCreateGC(linux.display_, linux.window, 0, linux.gc_values);
        
        atom1 = XInternAtom(linux.display_, "WM_PROTOCOLS", 0);
        
        atom2 = XInternAtom(linux.display_, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(linux.display_, linux.window, &atom2, 1);

        // StructureNotify 
        
        XSelectInput(linux.display_, linux.window, ExposureMask | StructureNotifyMask | KeyPressMask | PointerMotionMask | ButtonPressMask );
 
        //Показываем окно на экране
        XMapWindow(linux.display_, linux.window);
        
        screen.set_size(start_position.getw(), start_position.geth());
        
        linux.image = XCreateImage(linux.display_,DefaultVisual(linux.display_,DefaultScreen(linux.display_)),24,ZPixmap,0,(char*) screen.buffer,start_position.getw(), start_position.geth(),32,(start_position.getw())*4);
        */
        
        execute_is_run = true;
        execute_thread = new std::thread(&GUI::execute, this);
 
        return true;
    };
    
    void GUI::finish() {
        
        
    }
    
#endif