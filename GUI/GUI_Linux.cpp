/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include <thread>
#include "GUI.h"
#include "../tools.h"

extern bool GLOBAL_STOP;

#ifdef __linux__

    char msg [256]={"test"};
    

    void GUI::execute() {
    
        int cnt = 0;
        
        std::string str;
        str = "1233";
        
        XEvent      exppp;
        XEvent      event;
        while(GLOBAL_STOP == false) {
            
            if (XPending(linux.display)) 
            {
                try {
                    XNextEvent(linux.display, &event);
                } catch(...) {
                    printf("catch 1\n");
                }
                
                if (event.type == Expose) // Перерисовываем окно
                {
                    screen.fill_all(0xffffff);
                    
                    elements.paint(&screen);
                    
                    cnt++;
                    
                    str = "tik=" + std::to_string(cnt);
                    screen.fonts.print(5, 5, "arial",12,str, 0xff);
                    
                    XPutImage(linux.display, linux.window, linux.graph_ctx, linux.image, 0, 0, 0, 0, screen.w, screen.h);//DISPLAY_WIDTH, DISPLAY_HEIGHT);

                    XSetForeground (linux.display, linux.graph_ctx, 0x337700);
                    //XDrawString (linux.display, linux.window, linux.graph_ctx, 50, 50, msg, strlen (msg));
                
                    
                    
                    
                    
                } else {
                    if (event.type == ConfigureNotify) {
                        printf("ConfigureNotify\n");
                    } else {
                        if (event.type == ReparentNotify) {
                            printf("ReparentNotify\n");
                        } else {
                            if (event.type == MapNotify) {
                                printf("MapNotify\n");
                            } else {
                                if (event.type == MotionNotify) {
                                    //printf("MotionNotify\n");
                                } else {
                                    if (event.type == ClientMessage) {
                                        printf("ClientMessage\n");
                                        if( event.xclient.message_type == atom1 &&
                                            event.xclient.data.l[0] == atom2)
                                         {
                                             XDestroyWindow(linux.display, linux.window);
                                         }
                                    } else {
                                        if (event.type == DestroyNotify) 
                                        {
                                            //printf("Window has been destroyed\n");
                                            set_GLOBAL_STOP(L"DestroyNotify");
                                            //XCloseDisplay (linux.display);
                                        } else {
                                            if(event.type == ButtonPress) {
                                                MouseButtonEvent(linux.DetectButtonTypeLinux(event.xbutton.button), event.xbutton.x,event.xbutton.y );
                                                printf("ButtonPress %d:%d %d\n", event.xbutton.x,event.xbutton.y, event.xbutton.button);
                                            } else {
                                                printf("else\n");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(screen.need_update && GLOBAL_STOP == false) {
                    screen.need_update = false;
                    memset(&exppp, 0, sizeof(exppp));
                    exppp.type = Expose;
                    exppp.xexpose.window = linux.window;
                    XSendEvent(linux.display, linux.window,False,ExposureMask,&exppp);
                }
                usleep(10);
            }
        }
        execute_is_run = false;
    }

    bool GUI::low_level_init() {
        
        start_position.set_xywh(50, 50, 750, 750);
        
        if ((linux.display = XOpenDisplay(getenv("DISPLAY"))) == NULL)
        {
            wtf(L"Can't connect X server");//, strerror(errno));
            return false;
        }
        
        linux.screen_id = XDefaultScreen(linux.display);
        
        linux.window = XCreateSimpleWindow(linux.display, RootWindow(linux.display, linux.screen_id), start_position.x, start_position.y, start_position.w, start_position.h, 10, XBlackPixel(linux.display, linux.screen_id), XWhitePixel(linux.display, linux.screen_id));
 
        linux.graph_ctx=XCreateGC(linux.display, linux.window, 0, linux.gc_values);
 
        
        
        atom1 = XInternAtom(linux.display, "WM_PROTOCOLS", 0);
        
        atom2 = XInternAtom(linux.display, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(linux.display, linux.window, &atom2, 1);

        // StructureNotify 
        
        XSelectInput(linux.display, linux.window, ExposureMask | StructureNotifyMask | KeyPressMask | PointerMotionMask | ButtonPressMask );
 
        //Показываем окно на экране
        XMapWindow(linux.display, linux.window);
        
        screen.set_size(start_position.w, start_position.h);
        
        linux.image = XCreateImage(linux.display,DefaultVisual(linux.display,DefaultScreen(linux.display)),24,ZPixmap,0,(char*) screen.buffer,start_position.w, start_position.h,32,(start_position.w)*4);
    
        
        execute_is_run = true;
        execute_thread = new std::thread(&GUI::execute, this);
 
        return true;
    };
    
    void GUI::finish() {
        XCloseDisplay(linux.display);
        
    }
    
#endif