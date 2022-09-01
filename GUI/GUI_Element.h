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
        Edit, Memo, Field
    };
    Type type;
    std::string caption;
    
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
            type = src.type;
            x = src.x;
            y = src.y;
            w = src.w;
            h = src.h;
            
            bg_color = src.bg_color;
            border_color = src.border_color;
            
            caption = src.caption;
        };
        return *this;
    }
    ELEMENT(ELEMENT *parent, Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color,std::string caption) 
    : type(type) , RECTANGLE(x, y, w, h, bg_color, border_color), caption(caption)
    {
        
    }
    ELEMENT(ELEMENT&& src) {
        *this = std::move(src);
    }
    ELEMENT(const ELEMENT& src) {
        
    }
};

class ELEMENTS {
public:
    std::vector<ELEMENT> item;
    
    ELEMENT* add(ELEMENT* parent, ELEMENT::Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color, std::string caption);
    
    void paint(SCREEN_BUFFER *screen) {
        item[0].paint(screen);
    }
    
    ELEMENTS(){};
};

#endif /* GUI_ELEMENT_H */

