/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   GUI.cpp
 * Author: root
 * 
 * Created on August 30, 2022, 3:05 PM
 */

#include "../program.h"
#include "GUI.h"
#include "../global.h"
#include "../tools.h"
#include "../mikrotik.h"


extern bool GLOBAL_STOP;
extern GLOBAL global;
extern PCAP pcap;
extern PROGRAMS programs;
extern MIKROTIK mikrotik;

GUI::GUI() {
}

GUI::GUI(const GUI& orig) {
}

GUI::~GUI() {
}
   
void GUI::wait_execute_close() {
    while(execute_is_run == true) {
        usleep(100);
    }
};    


void GUI::wait_run() {
    UI_ACTION a;
    
    while(GLOBAL_STOP == false) {
        if(ui_action.empty() == false) {
            a = ui_action.front();
            ui_action.pop();
            if(a.id != 0) {
                if(a.id == IfList_id) {
                    if(a.type == UI_Action_Type::Select) {
                        global.set_InterfaceSelect(a.value);
                    };
                }
                if(a.id == FoldersList_id) {
                    if(a.type == UI_Action_Type::Select) {
                        global.set_FoldersSelect(a.value);
                    }

                }
                if(a.id == FilesList_id) {
                    if(a.type == UI_Action_Type::DoubleClick ) {
                        global.set_FilesDoubleClick(a.value);
                    }
                }
                if(a.id == SnifRun_id) {
                    if(a.type == UI_Action_Type::Select) {
                        global.set_SnifferRun(true);
                    }
                    if(a.type == UI_Action_Type::Deselect) {
                        global.set_SnifferRun(false);
                    }
                }
                if(a.id == TabIf_id) {
                    if(a.type == UI_Action_Type::Select) {
                        printf("TabIf Select\n");
                        TabPcap->is_pressed = false;
                        TabClicker->is_pressed = false;
                        global.active_IF_tab();
                    };
                    if(a.type == UI_Action_Type::Deselect) {
                        printf("TabIf Deselect\n");
                    };
                }
                if(a.id == TabPcap_id) {
                    if(a.type == UI_Action_Type::Select) {
                        printf("PcapIf Select\n");
                        TabIf->is_pressed = false;
                        TabClicker->is_pressed = false;
                        global.active_PCAP_tab();
                    };
                    if(a.type == UI_Action_Type::Deselect) {
                        printf("PcapIf Deselect\n");
                    };
                }
                if(a.id == TabClicker_id) {
                    if(a.type == UI_Action_Type::Select) {
                        printf("TabClicker Select\n");
                        TabPcap->is_pressed = false;
                        TabIf->is_pressed = false;
                        global.active_CLICKER_tab();
                    };
                    if(a.type == UI_Action_Type::Deselect) {
                        printf("TabClicker Deselect\n");
                    };
                }
                if(a.id == WindowListBtnStart_id) {
                    if(a.type == UI_Action_Type::Select) {
                        printf("WindowListBtnStart Select\n");
                        global.set_WindowListBtnStart(true);
                    };
                    if(a.type == UI_Action_Type::Deselect) {
                        printf("WindowListBtnStart Deselect\n");
                        global.set_WindowListBtnStart(false);
                    };
                }
                if(a.id == WindowList_id) {
                    printf("WindowList_id %d\n", a._data_ );
                    global.WindowListId = a._data_;
                }
                if(a.id == View_id) {
                    if(a.type == UI_Action_Type::KeyEvent) {
                        printf("+++ %d\n", a._data_);
                        if(a._data_ == 25) { // arr up
                            if(View->green_rec.y > 0)
                                View->green_rec.y -= 5;
                        }
                        if(a._data_ == 39) { // arr down
                            if(View->grab_screen != nullptr &&
                               View->grab_screen->buffer != nullptr &&
                               View->grab_screen->w != 0 &&
                               View->grab_screen->h != 0 )
                            {
                                if((View->green_rec.y+View->green_rec.h+5) < View->grab_screen->h) {
                                    View->green_rec.y += 5;
                                }
                            };
                            
                        }
                        if(a._data_ == 40) { // arr right
                            if(View->grab_screen != nullptr &&
                               View->grab_screen->buffer != nullptr &&
                               View->grab_screen->w != 0 &&
                               View->grab_screen->h != 0 )
                            {
                                if((View->green_rec.x+View->green_rec.w+5) < View->grab_screen->w) {
                                    View->green_rec.x += 5;
                                }
                            };
                        }
                        if(a._data_ == 38) { // arr left
                            if(View->green_rec.x > 0)
                                View->green_rec.x -= 5;
                        }
                        if(a._data_ == 26) {
                            if(View->grab_screen != nullptr &&
                               View->grab_screen->buffer != nullptr &&
                               View->grab_screen->w != 0 &&
                               View->grab_screen->h != 0 )
                            {
                                if((View->green_rec.x+View->green_rec.w+5) < View->grab_screen->w) {
                                    View->green_rec.w += 5;
                                }
                            };
                            
                        }
                        if(a._data_ == 24) {
                            if(View->green_rec.w >= 15)
                                View->green_rec.w -= 5;
                        }
                        if(a._data_ == 54) {
                            if(View->grab_screen != nullptr &&
                               View->grab_screen->buffer != nullptr &&
                               View->grab_screen->w != 0 &&
                               View->grab_screen->h != 0 )
                            {
                                if((View->green_rec.y+View->green_rec.h+5) < View->grab_screen->h) {
                                    View->green_rec.h += 5;
                                }
                            };
                        }
                        if(a._data_ == 52) {
                            if(View->green_rec.h >= 15)
                                View->green_rec.h -= 5;
                        }
                        if(a._data_ == 49) {
                            View->set_need_save_bmp();
                        }
                    }
                }
            }
        }
        this->screen.need_update = true;
        usleep(10);
    }
};  

void GUI::create_elements() {

    ELEMENT *q;

    q = elements.add(this, 0, ELEMENT::Type::Button ,  5, 5, 45, 25, COLOR::ListBG, COLOR::ListBorder, "snif");
    TabIf_id = q->id;
    
    q = elements.add(this, 0, ELEMENT::Type::Button ,  60, 5, 45, 25, COLOR::ListBG, COLOR::ListBorder, "pcap");
    TabPcap_id = q->id;
    
    q = elements.add(this, 0, ELEMENT::Type::Button ,  115, 5, 45, 25, COLOR::ListBG, COLOR::ListBorder, "click");
    TabClicker_id = q->id;

    ////////////////////////////////////////////////////////////////////////////
    
    q = elements.add(this, 0, ELEMENT::Type::Panel,  5, 40, 500, 680, COLOR::PanelBG, COLOR::PanelBorder, "PanelIf");
    PanelIf_id = q->id;

    q = elements.add(this, 0, ELEMENT::Type::Panel,  5, 40, 640, 680, COLOR::PanelBG, COLOR::PanelBorder, "PanelPcap");
    PanelPcap_id = q->id;

    q = elements.add(this, 0, ELEMENT::Type::Panel,  5, 40, 640, 680, COLOR::PanelBG, COLOR::PanelBorder, "PanelClicker");
    PanelClicker_id = q->id;

    q = elements.add(this, PanelIf_id, ELEMENT::Type::List,  5, 25, 180, 325, COLOR::ListBG, COLOR::ListBorder, "Interface");
    IfList_id = q->id;
    
    q = elements.add(this, IfList_id, ELEMENT::Type::Button ,  55, 295, 65, 25, COLOR::ListBG, COLOR::ListBorder, "RUN");
    SnifRun_id = q->id;
        
    q = elements.add(this, PanelIf_id, ELEMENT::Type::MikrotikInfo ,  5, 370, 180, 25, COLOR::ListBG, COLOR::MemoBorder, "Info");
    Memo1_id = q->id;
    
    q = elements.add(this, PanelPcap_id, ELEMENT::Type::List,  5, 25, 150, 100, COLOR::ListBG, COLOR::ListBorder, "Folders");
    FoldersList_id = q->id;

    q = elements.add(this, PanelPcap_id, ELEMENT::Type::List,  5, 150, 250, 350, COLOR::ListBG, COLOR::ListBorder, "Files");
    FilesList_id = q->id;

    q = elements.add(this, PanelPcap_id, ELEMENT::Type::PCAPinfo,  5, 380, 180, 150, COLOR::ListBG, COLOR::ListBorder, "PCAPinfo");
    PCAPinfo_id = q->id;
  
    q = elements.add(this, PanelClicker_id, ELEMENT::Type::List,  5, 25, 100, 100, COLOR::ListBG, COLOR::ListBorder, "Programs");
    ProgramList_id = q->id;

    q = elements.add(this, PanelClicker_id, ELEMENT::Type::List,  110, 25, 230, 100, COLOR::ListBG, COLOR::ListBorder, "Windows");
    WindowList_id = q->id;
    
    q = elements.add(this, PanelClicker_id, ELEMENT::Type::Panel,  330, 25, 90, 30, COLOR::ListBG, COLOR::ListBorder, "Btn");    
    WindowListBtn_id = q->id;

    q = elements.add(this, WindowListBtn_id, ELEMENT::Type::Button,  5, 5, 80, 20, COLOR::ListBG, COLOR::ListBorder, "Start");
    WindowListBtnStart_id = q->id;

    
    q = elements.add(this, PanelClicker_id, ELEMENT::Type::View,  55, 155, 550, 500, COLOR::ListBG, COLOR::ListBorder, "View");
    View_id = q->id;
       
    
    elements.recalc_parent_id();
    
    Memo1 = elements.get_element_by_id(Memo1_id);
    //Memo1->is_visible = false;
    FoldersList = elements.get_element_by_id(FoldersList_id);
    FilesList = elements.get_element_by_id(FilesList_id);
    IfList = elements.get_element_by_id(IfList_id);
    SnifRun = elements.get_element_by_id(SnifRun_id);
    PCAPinfo = elements.get_element_by_id(PCAPinfo_id);
    PCAPinfo->pcap = &pcap;
    PCAPinfo->is_visible = false;
    
    TabIf = elements.get_element_by_id(TabIf_id);
    TabPcap = elements.get_element_by_id(TabPcap_id);
    TabClicker = elements.get_element_by_id(TabClicker_id);
    
    PanelIf = elements.get_element_by_id(PanelIf_id);
    PanelPcap = elements.get_element_by_id(PanelPcap_id);
    PanelClicker = elements.get_element_by_id(PanelClicker_id);
    
    View = elements.get_element_by_id(View_id);
    View->grab_screen = &programs.grab_screen_buffer;
    
    ProgramList = elements.get_element_by_id(ProgramList_id);
    WindowList = elements.get_element_by_id(WindowList_id);
    WindowListBtn = elements.get_element_by_id(WindowListBtn_id);
    WindowListBtn->x = WindowList->x + WindowList->w;
    WindowListBtn->h = WindowList->h;
    
    PanelPcap->is_visible = false;
    PanelClicker->is_visible = false;
    
    TabIf->is_pressed = true;
    
    Memo1->item.push_back({this, ELEMENT_item::String, "str1"});
    Memo1->item.push_back({this, ELEMENT_item::String, "str2"});
    Memo1->item.push_back({this, ELEMENT_item::String, "str3"});
    
    
    ProgramList->item.push_back({this, ELEMENT_item::ListItem, "prg1"});
    ProgramList->item.push_back({this, ELEMENT_item::ListItem, "prg2"});
    ProgramList->item.push_back({this, ELEMENT_item::ListItem, "prg3"});
    
    global.mikrotik_info = Memo1;
    mikrotik.mikrotik_info = Memo1;
}

void GUI::KeyPressEvent(unsigned int key) {
    if(focus != nullptr) {
        focus->KeyPressEvent(key);
    }
}

void GUI::MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y ) {
        elements.MouseButtonEvent(MouseButtonType, mouse_x, mouse_y);
}

void GUI::set_ui_action(UI_Action_Type type, int id, std::string val, unsigned int window_id) {
    ui_action.push({type, id, val, window_id});
}

unsigned int GUI::find_window(Display *display, ELEMENT *_WindowList) {

    return 0;
    
    int status;
    Window parent, rw;
    Window *children;
    unsigned int nNumChildren, r = 0;
    XWindowAttributes window_attr;

    status = XQueryTree(display, DefaultRootWindow(display), &rw, &parent, &children, &nNumChildren);
    if (status == 0) return 0;
    if (nNumChildren == 0) return 0;

    for (int i = 0; i < nNumChildren; i++)
    {
        int er = 0;
        if(children[i] == 0) {
            er = 2;
        }
        try
        {
          XGetWindowAttributes(display, children[i], &window_attr);
        } catch(...)
        {
            er = 1;
        }
        
        
                
        if((window_attr.width == 820 && window_attr.height == 655) ||
           (window_attr.width == 500 && window_attr.height == 695))
        {
            _WindowList->add_to_window_list(children[i], window_attr.x, window_attr.y, window_attr.width, window_attr.height);
            //printf("%ld - %d:%d - %d:%d\n", children[i], window_attr.x, window_attr.y, window_attr.width, window_attr.height);
            
            //r = children[i];
            //XFree((char*) children);
            //return r;
        } else {
            /*if(window_attr.width > 300 && window_attr.width < 600) {
                printf("%d:%d %d:%d\n", window_attr.x, window_attr.y, window_attr.width, window_attr.height);
            }*/
        }
    }
    //printf("------\n");
    XFree((char*) children);
    return 0;
}