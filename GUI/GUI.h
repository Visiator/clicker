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

#include <thread>
#include <queue>

#ifndef GUI_H
#define GUI_H



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

enum UI_Action_Type {
    Undefine,
    Select,
    DoubleClick,
    Deselect,
    KeyEvent,
};

class UI_ACTION {
public:
    
    UI_Action_Type type;
    int id;
    std::string value;
    unsigned int _data_;
    
    void copy_from_src(const UI_ACTION& src) {
        _data_ = src._data_;
        type = src.type;
        id = src.id;
        value = src.value;
    }
    
    UI_ACTION& operator=(UI_ACTION&& src) {
        if (this != &src) {           
            copy_from_src(src);
        };
        return *this;
    }
    UI_ACTION& operator=(const UI_ACTION& src) {
        if (this != &src) {
            copy_from_src(src);
        }
        return *this;
    }
    UI_ACTION(UI_Action_Type type, int id, std::string value, unsigned int _data_) : type(type), id(id), value(value), _data_(_data_) {
        
    }
    UI_ACTION() : type(UI_Action_Type::Undefine), id(-1), value(""), _data_(0) {};
    UI_ACTION(UI_ACTION&& src){
        if (this != &src) {
            copy_from_src(src);
        }
    }
    UI_ACTION(const UI_ACTION& src) {
        if (this != &src) {
            copy_from_src(src);
        }
    }
    
};

#ifdef __linux__
class LINUX_PARAM {
public:
    Display     *display_ = nullptr;
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
    unsigned int DetectKeyTypeLinux(unsigned int v) {
        return v;
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
    
    #ifdef __linux__
    Display* get_display() { return linux.display_; };
    #endif
    
    std::queue<UI_ACTION> ui_action;
    
    unsigned int TabIf_id = 0, TabPcap_id = 0, TabClicker_id = 0, PanelPcap_id = 0, PanelClicker_id = 0, View_id = 0
    , Mikrotik_to_txt_id = 0, Mikrotik_from_txt_id = 0, test1_id = 0;
    
    ELEMENT *TabIf = nullptr, *TabPcap = nullptr, *TabClicker = nullptr
            , *PanelIf = nullptr, *PanelPcap = nullptr, *PanelClicker = nullptr, *View = nullptr
            , *Mikrotik_to_txt = nullptr, *Mikrotik_from_txt = nullptr, *test1 = nullptr;
    
    unsigned int WindowList_id = 0, WindowListBtn_id = 0, PanelIf_id = 0, Memo1_id = 0, ProgramList_id = 0, IfList_id = 0, FoldersList_id = 0, FilesList_id = 0, PCAPinfo_id = 0, SnifRun_id = 0;
    unsigned int WindowListBtnStart_id = 0;
    
    ELEMENT *WindowListBtnStart = nullptr;
            
    ELEMENT *WindowList = nullptr, *WindowListBtn = nullptr, *Memo1 = nullptr, *ProgramList = nullptr, *IfList = nullptr, *FoldersList = nullptr, *FilesList = nullptr, *PCAPinfo = nullptr, *SnifRun = nullptr;
    
    ELEMENT* focus = nullptr;
    
    GUI();
    SCREEN_BUFFER screen;
    
    _FRAME start_position;
    
    ELEMENTS elements;
    
    void MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y );
    void KeyPressEvent(unsigned int key);
    
    void create_elements();
    
    bool low_level_init();
    void finish();
    void wait_run();
    void wait_execute_close();
    
    void set_ui_action(UI_Action_Type type, int id, std::string val, unsigned int window_id);
    
    bool execute_is_run = false;
    std::thread* execute_thread = nullptr;
    void execute();
    
    
    
    unsigned int find_window(Display *display, ELEMENT *_WindowList);
    
    GUI(const GUI& orig);
    virtual ~GUI();
private:

};

#endif /* GUI_H */

