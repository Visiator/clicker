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

//#include "GUI.h"
#include "GUI_Primitives.h"

class ELEMENT;

class ELEMENT_item : public RECTANGLE {
public:

    enum Type 
    {
        Undefine, String, ListItem
    };
    bool is_pressed = false;
    Type type;
    ELEMENT* parent = nullptr;
    std::string text;
    void get_parent_xy(int *px, int *py);
    void paint(SCREEN_BUFFER *screen);
    
    bool MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y );
    
    void copy_from_src(const ELEMENT_item& src) {
        parent = src.parent;
        type = src.type;
        text = src.text;
        RECTANGLE::copy_from_src(src);
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
   
    ELEMENT_item(Type type, std::string text) : type(type), text(text) {
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
        Undefine, Edit, Memo, Field, List, Panel
    };
    bool is_visible = true;
    bool is_passive = true;
    unsigned int id = 0;
    unsigned int parent_id = 0;
    ELEMENT* parent = nullptr;
    Type type = Type::Undefine;
    std::string caption;
    std::vector<ELEMENT_item> item;
    void get_parent_xy(int *px, int *py);
    void paint(SCREEN_BUFFER *screen);
    
    void copy_from_src(const ELEMENT& src) {
        is_visible = src.is_visible;
        is_passive = src.is_passive;
        id = src.id;
        parent_id = src.parent_id;
        parent = src.parent;
        type = src.type;
        caption = src.caption;
        item = src.item;            
        RECTANGLE::copy_from_src(src); 
    }
    
    void MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y );
    
    ELEMENT& operator=(const ELEMENT& src) {
        return *this;
    }
    ELEMENT& operator=(ELEMENT&& src) {
        if (this != &src) {           
            copy_from_src(src);
        };
        return *this;
    }
    
    ELEMENT(unsigned int id, unsigned int parent_id, Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color,std::string caption) 
    : type(type) , RECTANGLE(x, y, w, h, bg_color, border_color), caption(caption), id(id), parent_id(parent_id)
    {
        if(type == Type::List) {
            is_passive = false;
        }
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
    
    ELEMENT* add(unsigned int parent_id, ELEMENT::Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color, std::string caption);
    
    void MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y );
    
    void paint(SCREEN_BUFFER *screen) {
        for(auto& a : item) {
            a.paint(screen);
        };
    }
    
    ELEMENTS(){};
};

#endif /* GUI_ELEMENT_H */

