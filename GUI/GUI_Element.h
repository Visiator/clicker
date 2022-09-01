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


#include "GUI_Primitives.h"

class ELEMENT : public RECTANGLE {
public:
    
    enum Type 
    {
        Undefine, Edit, Memo, Field, Panel
    };
    unsigned int id = 0;
    unsigned int parent_id = 0;
    ELEMENT* parent = nullptr;
    Type type = Type::Undefine;
    std::string caption;
    std::vector<std::string> string_list;
    void get_parent_xy(int *px, int *py);
    void paint(SCREEN_BUFFER *screen);
    
    ELEMENT& operator=(const ELEMENT& src) {
        return *this;
    }
    ELEMENT& operator=(ELEMENT&& src) {
        if (this != &src) {
            //this->idx  = std::move(src.idx);
            //this->w  = std::move(src.w);
            //this->h  = std::move(src.h);
            //this->buf = src.buf;
            //src.buf = nullptr;
            id = src.id;
            parent_id = src.parent_id;
            type = src.type;
            x = src.x;
            y = src.y;
            w = src.w;
            h = src.h;
            
            bg_color = src.bg_color;
            border_color = src.border_color;
            
            caption = src.caption;
            
            string_list = src.string_list;
        };
        return *this;
    }
    
    ELEMENT(unsigned int id, unsigned int parent_id, Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color,std::string caption) 
    : type(type) , RECTANGLE(x, y, w, h, bg_color, border_color), caption(caption), id(id), parent_id(parent_id)
    {
        
    }
    ELEMENT(ELEMENT&& src) {
        *this = std::move(src);
    }
    ELEMENT(const ELEMENT& src) {
        if (this != &src) {
            id = src.id;
            parent_id = src.parent_id;
            type = src.type;
            x = src.x;
            y = src.y;
            w = src.w;
            h = src.h;
            
            bg_color = src.bg_color;
            border_color = src.border_color;
            
            caption = src.caption;
            
            string_list = src.string_list;
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
    
    void paint(SCREEN_BUFFER *screen) {
        for(auto& a : item) {
            a.paint(screen);
        };
    }
    
    ELEMENTS(){};
};

#endif /* GUI_ELEMENT_H */

