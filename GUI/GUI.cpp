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
extern PROGRAMS programs_;
extern MIKROTIK mikrotik;
extern SCREEN grab_screen_buffer;

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
                        TabGrab->is_pressed = false;
                        TabProg->is_pressed = false;
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
                        TabGrab->is_pressed = false;
                        TabProg->is_pressed = false;
                        global.active_PCAP_tab();
                    };
                    if(a.type == UI_Action_Type::Deselect) {
                        printf("PcapIf Deselect\n");
                    };
                }
                if(a.id == TabGrab_id) {
                    if(a.type == UI_Action_Type::Select) {
                        printf("TabGrab Select\n");
                        TabPcap->is_pressed = false;
                        TabIf->is_pressed = false;
                        TabProg->is_pressed = false;
                        global.active_GRAB_tab();
                    };
                    if(a.type == UI_Action_Type::Deselect) {
                        printf("TabClicker Deselect\n");
                    };
                }
                if(a.id == TabProg_id) {
                    if(a.type == UI_Action_Type::Select) {
                        printf("TabGrab Select\n");
                        TabPcap->is_pressed = false;
                        TabIf->is_pressed = false;
                        TabGrab->is_pressed = false;
                        global.active_PROG_tab();
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
                        if(a._data_ == 23) { // tab
                            printf("tab %d:%d\n", View->green_rec.x, View->green_rec.y );
                            global.MousePress(View->green_rec.x, View->green_rec.y,1,0, 300,540);
                        }
                        if(a._data_ == 72) { // F6
                            printf("F6 - key press ios home\n");
                            global.KeyPressIosHome();
                        }
                        
                        if(a._data_ == 34 || a._data_ == 35) { // [
                            SpriteList->SpriteList_cursor_scroll(a._data_);
                        }
                    }
                }
                if(a.id == Mikrotik_to_txt_id) {
                    printf("Mikrotik_to_txt_id\n");
                    mikrotik.ip_list_to_txt((char *)"from_mikrotik.txt");
                    Mikrotik_to_txt->is_pressed = false;
                }
                if(a.id == Mikrotik_from_txt_id) {
                    printf("Mikrotik_from_txt_id\n");
                    mikrotik.ip_list_from_txt((char *)"to_mikrotik.txt");
                    Mikrotik_from_txt->is_pressed = false;
                }
                if(a.id == test1_id) {
                    test1->is_pressed = false;
                    global.test1();
                }
                if(a.id == BtnProgRun_id) {
                    if(BtnProgRun->is_pressed == true) {
                        programs_.item_[0].run();
                    } else {
                        programs_.item_[0].stop();
                    }
                }
                if(a.id == BtnProgLoad_id) {
                    programs_.item_[0].load();
                    BtnProgLoad->is_pressed = false;
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
    
    q = elements.add(this, 0, ELEMENT::Type::Button ,  115, 5, 45, 25, COLOR::ListBG, COLOR::ListBorder, "grab");
    TabGrab_id = q->id;

    q = elements.add(this, 0, ELEMENT::Type::Button ,  170, 5, 45, 25, COLOR::ListBG, COLOR::ListBorder, "prog");
    TabProg_id = q->id;

    ////////////////////////////////////////////////////////////////////////////
    
    q = elements.add(this, 0, ELEMENT::Type::Panel,  5, 35, 500, 680, COLOR::PanelBG, COLOR::PanelBorder, "PanelIf");
    PanelIf_id = q->id;

    q = elements.add(this, 0, ELEMENT::Type::Panel,  5, 35, 640, 680, COLOR::PanelBG, COLOR::PanelBorder, "PanelPcap");
    PanelPcap_id = q->id;

    q = elements.add(this, 0, ELEMENT::Type::Panel,  5, 35, 970, 855, COLOR::PanelBG, COLOR::PanelBorder, "PanelGrab");
    PanelGrab_id = q->id;

    q = elements.add(this, 0, ELEMENT::Type::Panel,  5, 35, 970, 855, COLOR::PanelBG, COLOR::PanelBorder, "PanelProg");
    PanelProg_id = q->id;
    
    q = elements.add(this, PanelProg_id, ELEMENT::Type::ProgramText ,  5, 115, 550, 735, COLOR::ProgTextBG, COLOR::ProgText, "ProgText");
    ProgText_id = q->id;
    
    q = elements.add(this, PanelProg_id, ELEMENT::Type::Print ,  570, 115, 350, 735, COLOR::ProgTextBG, COLOR::ProgText, "Print");
    Print_id = q->id;
    
    //q = elements.add(this, IfList_id, ELEMENT::Type::Button ,  55, 295, 65, 25, COLOR::ListBG, COLOR::ListBorder, "RUN");
    
    q = elements.add(this, PanelProg_id, ELEMENT::Type::Button ,  130, 35, 50, 25, COLOR::ListBG, COLOR::ListBorder, "Load");
    BtnProgLoad_id = q->id;
    
    q = elements.add(this, PanelProg_id, ELEMENT::Type::Button ,  130, 70, 50, 25, COLOR::ListBG, COLOR::ListBorder, "RUN");
    BtnProgRun_id = q->id;
    
    
    //BtnProgLoad_id = 0, BtnProgRun_id
    
    q = elements.add(this, PanelIf_id, ELEMENT::Type::List,  5, 25, 180, 325, COLOR::ListBG, COLOR::ListBorder, "Interface");
    IfList_id = q->id;
    
    q = elements.add(this, IfList_id, ELEMENT::Type::Button ,  55, 295, 65, 25, COLOR::ListBG, COLOR::ListBorder, "RUN");
    SnifRun_id = q->id;
        
    q = elements.add(this, PanelIf_id, ELEMENT::Type::MikrotikInfo ,  5, 370, 180, 25, COLOR::ListBG, COLOR::MemoBorder, "Info");
    Memo1_id = q->id;
    
    q = elements.add(this, PanelIf_id, ELEMENT::Type::Button ,  5, 400, 250, 25, COLOR::ListBG, COLOR::MemoBorder, "Mikrotik -> from_mikrotik.txt");
    Mikrotik_to_txt_id = q->id;
    
    q = elements.add(this, PanelIf_id, ELEMENT::Type::Button ,  5, 430, 250, 25, COLOR::ListBG, COLOR::MemoBorder, "Mikrotik <- to_mikrotik.txt");
    Mikrotik_from_txt_id = q->id;
    
    q = elements.add(this, PanelIf_id, ELEMENT::Type::Button ,  5, 460, 50, 25, COLOR::ListBG, COLOR::MemoBorder, "test 1");
    test1_id = q->id;
    
    q = elements.add(this, PanelPcap_id, ELEMENT::Type::List,  5, 25, 150, 200, COLOR::ListBG, COLOR::ListBorder, "Folders");
    FoldersList_id = q->id;

    q = elements.add(this, PanelPcap_id, ELEMENT::Type::List,  165, 25, 450, 640, COLOR::ListBG, COLOR::ListBorder, "Files");
    FilesList_id = q->id;

    q = elements.add(this, PanelPcap_id, ELEMENT::Type::PCAPinfo,  5, 380, 180, 150, COLOR::ListBG, COLOR::ListBorder, "PCAPinfo");
    PCAPinfo_id = q->id;
  
    q = elements.add(this, PanelProg_id, ELEMENT::Type::List,  5, 25, 100, 80, COLOR::ListBG, COLOR::ListBorder, "Programs");
    ProgramList_id = q->id;

    q = elements.add(this, PanelGrab_id, ELEMENT::Type::List,  110, 25, 230, 80, COLOR::ListBG, COLOR::ListBorder, "Windows");
    WindowList_id = q->id;
    
    q = elements.add(this, PanelGrab_id, ELEMENT::Type::Panel,  330, 25, 90, 30, COLOR::ListBG, COLOR::ListBorder, "Btn");    
    WindowListBtn_id = q->id;

    q = elements.add(this, WindowListBtn_id, ELEMENT::Type::Button,  5, 5, 80, 20, COLOR::ListBG, COLOR::ListBorder, "Start");
    WindowListBtnStart_id = q->id;

    
    q = elements.add(this, PanelGrab_id, ELEMENT::Type::View,  5, 115, 960, 540, COLOR::ListBG, COLOR::ListBorder, "View");
    View_id = q->id;
       
    q = elements.add(this, PanelGrab_id, ELEMENT::Type::SpriteList,  5, 660, 960, 190, COLOR::ListBG, COLOR::ListBorder, "SpriteList");
    SpriteList_id = q->id;
    
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
    TabGrab = elements.get_element_by_id(TabGrab_id);
    TabProg = elements.get_element_by_id(TabProg_id);
    
    ProgText = elements.get_element_by_id(ProgText_id);
    ProgText->program_idx = 0;// = &(programs.item_[0]);
    Print = elements.get_element_by_id(Print_id);
    Print->program_idx = 0;// = &(programs.item_[0]);
    
    PanelIf = elements.get_element_by_id(PanelIf_id);
    PanelPcap = elements.get_element_by_id(PanelPcap_id);
    PanelGrab = elements.get_element_by_id(PanelGrab_id);
    PanelProg = elements.get_element_by_id(PanelProg_id);
    
    View = elements.get_element_by_id(View_id);
    View->grab_screen = &grab_screen_buffer;
    View->program_idx = 0;// = &(programs.item_[0]);
    
    SpriteList = elements.get_element_by_id(SpriteList_id);
    SpriteList->program_idx = 0;// = &(programs.item_[0]);
    
    ProgramList = elements.get_element_by_id(ProgramList_id);
    WindowList = elements.get_element_by_id(WindowList_id);
    WindowListBtn = elements.get_element_by_id(WindowListBtn_id);
    WindowListBtn->x = WindowList->x + WindowList->w;
    WindowListBtn->h = WindowList->h;
    
    PanelPcap->is_visible = false;
    PanelGrab->is_visible = false;
    PanelProg->is_visible = false;
    
    TabIf->is_pressed = true;
    
    Mikrotik_to_txt = elements.get_element_by_id(Mikrotik_to_txt_id);
    Mikrotik_from_txt = elements.get_element_by_id(Mikrotik_from_txt_id);
    
    BtnProgLoad = elements.get_element_by_id(BtnProgLoad_id);
    BtnProgRun = elements.get_element_by_id(BtnProgRun_id);
    
    test1 = elements.get_element_by_id(test1_id);
    
    Memo1->item.push_back({this, ELEMENT_item::String, "str1"});
    Memo1->item.push_back({this, ELEMENT_item::String, "str2"});
    Memo1->item.push_back({this, ELEMENT_item::String, "str3"});
    
    
    ProgramList->item.push_back({this, ELEMENT_item::ListItem, "prg0"});
    ProgramList->item.push_back({this, ELEMENT_item::ListItem, "prg1"});
    ProgramList->item.push_back({this, ELEMENT_item::ListItem, "prg2"});
    
    
    
    global.mikrotik_info = Memo1;
    mikrotik.set_mikrotik_info(Memo1);
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

#ifdef __linux__
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
#endif