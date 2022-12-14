/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   GUI_Element.h
 * Author: root
 *
 * Created on August 31, 2022, 3:23 PM
 */

#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <string>
#include <vector>

#include "../pcap.h"
//#include "GUI.h"
#include "GUI_Primitives.h"

class GUI;
//class PCAP;
class ELEMENT;
class PROGRAM;

class ELEMENT_item : public RECTANGLE {
public:

    enum Type 
    {
        Undefine, String, ListItem, WindowList
    };
    unsigned int window_id = 0, program_id = 0;
    bool is_pressed = false;
    Type type;
    ELEMENT* parent = nullptr;
    GUI *gui = nullptr;
    std::string text;
    void get_parent_xy(int *px, int *py);
    void paint(SCREEN_BUFFER *screen);
    
    bool MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y );
    
    void copy_from_src(const ELEMENT_item& src) {
        window_id = src.window_id;
        program_id = src.program_id;
        gui = src.gui;
        parent = src.parent;
        type = src.type;
        text = src.text;
        RECTANGLE::copy_from_src(src);
    }
    
    
    bool operator<(const ELEMENT_item &r) const
    {
        std::string s1, s2;
        int a1, a2, v1, v2;
        a1 = text.find(':');
        if(a1 > 0) {
            a2 = r.text.find(':');
            if(a2 > 0) {
                s1 = text.substr(0, a1);
                s2 = r.text.substr(0, a2);
                v1 = std::stoi(s1);
                v2 = std::stoi(s2);
                return v1 < v2;
            }
        } 
        return text < r.text;        
    }
    bool operator>(const ELEMENT_item &r) const
    {
        std::string s1, s2;
        int a1, a2, v1, v2;
        a1 = text.find(':');
        if(a1 > 0) {
            a2 = r.text.find(':');
            if(a2 > 0) {
                s1 = text.substr(0, a1);
                s2 = r.text.substr(0, a2);
                v1 = std::stoi(s1);
                v2 = std::stoi(s2);
                return v1 > v2;
            }
        } 
        return text > r.text;
    }
    ELEMENT_item& operator=(ELEMENT_item&& src) {
        copy_from_src(src);
        return *this;
    }
    ELEMENT_item& operator=(const ELEMENT_item& src) {
        
        return *this;
    }   
    
    void init() {
        if(type == Type::ListItem) {
            bg_color = 0xffff00;
            bg_color_pressed = 0x00ffff;
        }
    }
   
    ELEMENT_item(GUI *gui, Type type, std::string text) : gui(gui), type(type), text(text) {
        init();
    }
    
    ELEMENT_item(ELEMENT_item&& src) {
        *this = std::move(src);
    }
    ELEMENT_item(const ELEMENT_item& src) {
        if(this != &src) {
            copy_from_src(src);
        }
    }
};

class ELEMENT : public RECTANGLE {
public:
    
    enum Type 
    {
        Undefine, Edit, Memo, Field, List, Panel, PCAPinfo, Button, View, MikrotikInfo, SpriteList, ProgramText, Print
    };
    _FRAME green_rec;
    GUI *gui;
    bool is_visible = true;
    bool is_passive = true;
    bool is_pressed = false;
    unsigned int id = 0;
    unsigned int parent_id = 0;
    ELEMENT* parent = nullptr;
    PCAP *pcap = nullptr;
    Type type = Type::Undefine;
    std::string caption;
    SCREEN *grab_screen = nullptr;
    bool need_save_bmp = false;
    unsigned int info1, info2;
    PROGRAM *program = nullptr;
    
    int SpriteList_cursor = 0;
    void SpriteList_cursor_scroll(int key);
    
    std::vector<ELEMENT_item> item;
    void get_parent_xy(int *px, int *py);
    void paint(SCREEN_BUFFER *screen);
    void paint_green_rec(SCREEN_BUFFER *screen, int x, int y);
    void paint_green_rec_x2(SCREEN_BUFFER *screen, int x, int y);
    
    void add_to_window_list(unsigned int window_id, int wx, int wy, int ww, int wh);
    
    void copy_from_src(const ELEMENT& src) {
        info1 = src.info1;
        info2 = src.info2;
        need_save_bmp = src.need_save_bmp;
        grab_screen = src.grab_screen;
        green_rec = src.green_rec;
        gui = src.gui;
        is_visible = src.is_visible;
        is_passive = src.is_passive;
        is_pressed = src.is_pressed;
        id = src.id;
        parent_id = src.parent_id;
        parent = src.parent;
        type = src.type;
        caption = src.caption;
        item = src.item;            
        program = src.program;
        RECTANGLE::copy_from_src(src); 
    }
    
    void MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y );
    void KeyPressEvent(unsigned int key);
    
    void set_need_save_bmp();
    
    ELEMENT& operator=(const ELEMENT& src) {
        return *this;
    }
    ELEMENT& operator=(ELEMENT&& src) {
        if (this != &src) {           
            copy_from_src(src);
        };
        return *this;
    }
    
    ELEMENT(GUI *gui, unsigned int id, unsigned int parent_id, Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color,std::string caption) 
    : gui(gui), type(type) , RECTANGLE(x, y, w, h, bg_color, border_color), caption(caption), id(id), parent_id(parent_id), need_save_bmp(false), info1(0), info2(0)
    {
        if(type == Type::List || type == Type::Button) {
            is_passive = false;
        }
        green_rec.set_xywh(50, 50, 80, 30);
    }
    ELEMENT(ELEMENT&& src) {
        *this = std::move(src);
    }
    ELEMENT(const ELEMENT& src) {
        if (this != &src) {
            copy_from_src(src);
        }
    }
};

class ELEMENTS {
public:
    std::vector<ELEMENT> item;
    
    unsigned int generate_new_id() {
        unsigned int old;
        old = 0;
        for(const auto& a : item) {
            if(a.id > old) old = a.id;
        }
        return old+1;
    }
    
    void recalc_parent_id();
    ELEMENT* get_element_by_id(unsigned int element_id);
    
    ELEMENT* add(GUI *gui, unsigned int parent_id, ELEMENT::Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color, std::string caption);
    
    void MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y );
    void KeyPressEvent(unsigned int key);
    
    void paint(SCREEN_BUFFER *screen) {
        for(auto& a : item) {
            a.paint(screen);
        };
    }
    
    ELEMENTS(){};
};

#endif /* GUI_ELEMENT_H */

