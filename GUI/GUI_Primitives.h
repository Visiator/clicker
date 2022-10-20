/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   GUI_Primitives.h
 * Author: root
 *
 * Created on August 31, 2022, 3:21 PM
 */

#ifndef GUI_PRIMITIVES_H
#define GUI_PRIMITIVES_H

#include <mutex>
#include "font.h"

class SCREEN {
public:
    std::mutex g_mutex;

    unsigned int *buffer;
    unsigned int w, h;
    void lock() {
        g_mutex.lock();
    }
    void unlock() {
        g_mutex.unlock();
    }
    void line_h(unsigned int x, unsigned int y, unsigned int ww, unsigned int color) {
        if(x+ww >= w ) return;
        if(y >= h ) return;
        unsigned int *q, i;
        q = buffer + y*w + x;
        for(i = 0;i<ww;i++) *q++ = color;
    }
    
    void line_v(unsigned int x, unsigned int y, unsigned int hh, unsigned int color) {
        if(x >= w ) return;
        if(y+hh >= h ) return;
        unsigned int *q, i;
        q = buffer + y*w + x;
        for(i = 0;i<hh;i++) {
            *q = color;
            q += w;
        };
    }
    
    void set_size(unsigned int w_, unsigned int h_) {
        if(w_ == w && h_ == h) return;
        lock();
        w = w_;
        h = h_;
        if(buffer != nullptr) delete[] buffer;
        buffer = new unsigned int[w*h];
        unlock();
    }
    
    void fill_all(unsigned int col) {
        if(buffer == nullptr) return;
        int j = w*h;
        for(int i=0;i<j;i++) {
            buffer[i] = col;
        }
    }

    void fill(unsigned int xx, unsigned int yy, unsigned int ww, unsigned int hh, unsigned int color) {
        if(xx+ww >= w ) return;
        if(yy+hh >= h ) return;
        unsigned int *q;
        unsigned int i, j;
        for(j = 0; j < hh; j++) {
            q = buffer + (yy+j)*w + xx;
            for(i = 0; i < ww; i++) {
                *q++ = color;
            }
        }

    }

    SCREEN() {
        buffer = nullptr;
        w = 0;
        h = 0;
    }
};


enum class MOUSE_BUTTON_TYPE : short { left, muddle, right };

class TEXTURA {
public:
    unsigned int *buf = nullptr;
    unsigned int w = 0, h = 0;
    void set_size(unsigned int ww, unsigned int hh) {
        if(ww == w && hh == h) { return; }
        w = ww;
        h = hh;
        if(buf != nullptr) delete[] buf;
        buf = new unsigned int[w * h];
    }
};

class SCREEN_BUFFER {
public:
    FONTS fonts{this};
    bool need_update = false;
    unsigned int *buffer;
    unsigned int w, h;
    void line_h(unsigned int x, unsigned int y, unsigned int ww, unsigned int color) {
        if(x+ww >= w ) return;
        if(y >= h ) return;
        unsigned int *q, i;
        q = buffer + y*w + x;
        for(i = 0;i<ww;i++) *q++ = color;
    }
    void line_v(unsigned int x, unsigned int y, unsigned int hh, unsigned int color) {
        if(x >= w ) return;
        if(y+hh >= h ) return;
        unsigned int *q, i;
        q = buffer + y*w + x;
        for(i = 0;i<hh;i++) {
            *q = color;
            q += w;
        };
    }
    void rectangle(unsigned int x, unsigned int y, unsigned int ww, unsigned int hh, unsigned int color) {
        line_h(x, y, ww, color);
        line_h(x, y+hh-1, ww, color);
        line_v(x, y, hh, color);
        line_v(x+ww-1, y, hh, color);
        
    }
    void set_size(unsigned int w_, unsigned int h_) {
        if(w_ == w && h_ == h) return;
        w = w_;
        h = h_;
        if(buffer != nullptr) delete[] buffer;
        buffer = new unsigned int[w*h];
    }
    void fill_all(unsigned int col) {
        if(buffer == nullptr) return;
        int j = w*h;
        for(int i=0;i<j;i++) {
            buffer[i] = col;
        }
    }
    
    void fill(unsigned int xx, unsigned int yy, unsigned int ww, unsigned int hh, unsigned int color) {
        if(xx+ww >= w ) return;
        if(yy+hh >= h ) return;
        
        unsigned int *q;
        unsigned int i, j;
        for(j = 0; j < hh; j++) {
            q = buffer + (yy+j)*w + xx;
            for(i = 0; i < ww; i++) {
                *q++ = color;
            }
        }
        
    }
    
    void fill(unsigned int xx, unsigned int yy, unsigned int ww, unsigned int hh, std::vector<unsigned int> &bitmap) {
        if(xx+ww >= w ) return;
        if(yy+hh >= h ) return;
        
        unsigned int *q;
        unsigned int i, j, k = 0;
        for(j = 0; j < hh; j++) {
            q = buffer + (yy+j)*w + xx;
            for(i = 0; i < ww; i++) {
                *q++ = bitmap[k++];
            }
        }
        
    }
    
    
    SCREEN_BUFFER() {
        buffer = nullptr;
        w = 0;
        h = 0;

    }
};

class _FRAME {
private:
    
public:
    int x, y, w, h;
    int gx, gy, gw, gh;
    
    int getx() { return x; };
    int gety() { return y; };
    int getw() { return w; };
    int geth() { return h; };
    void set_xywh(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        
        gx = 0;
        gy = 0;
        gw = 0;
        gh = 0;
        
    }
    void copy_from_src(const _FRAME &src) {
        x = src.x;
        y = src.y;
        w = src.w;
        h = src.h;        
    }
    bool its_me(int xx, int yy) {
        if(xx >= x && xx <=x+w-1 &&
           yy >= y && yy <=y+h-1) return true;
        return false;
    }
    _FRAME(unsigned int x, unsigned int y, unsigned int w, unsigned int h) : x(x), y(y), w(w), h(h) {};
    _FRAME() : x(0), y(0), w(0), h(0) {};
    ~_FRAME() {};
};

class RECTANGLE : public _FRAME {
public:
    unsigned int bg_color = 0, border_color = 0x006600, bg_color_pressed = 0x00aa00;
    void copy_from_src(const RECTANGLE &src) {
        bg_color = src.bg_color;
        border_color = src.border_color;
        bg_color_pressed = src.bg_color_pressed;
        _FRAME::copy_from_src(src);
    }
    void paint(SCREEN_BUFFER *screen);
    RECTANGLE(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color) : _FRAME(x, y, w, h), bg_color(bg_color), border_color(border_color) {};
    RECTANGLE() {};
    ~RECTANGLE(){};
};

#endif /* GUI_PRIMITIVES_H */

