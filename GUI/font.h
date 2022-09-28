/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   font.h
 * Author: smorodin
 *
 * Created on August 4, 2022, 1:27 PM
 */

#ifndef FONT_H
#define FONT_H

#include <string>
#include <vector>
#include <map>


class SCREEN_BUFFER;

void _printf(const char *v);

//void memo_counter(int val);

class BUKVA {
public:
    unsigned short w = 0, h = 0;
    unsigned int idx = 0;
    unsigned char *buf = nullptr;

    int paint(SCREEN_BUFFER *screen, int x, int y, uint32_t color);

    
    BUKVA& operator=(const BUKVA& src) {
        if (this != &src) {
            //_printf("BUKVA operator=&\n");
        };
        return *this;
    }
    BUKVA& operator=(BUKVA&& src) {
        if (this != &src) {
            //_printf("BUKVA operator=&&\n");
            
            this->idx  = std::move(src.idx);
            this->w  = std::move(src.w);
            this->h  = std::move(src.h);
            this->buf = src.buf;
            src.buf = nullptr;
        };
        return *this;
    }
    void set_len(int start, int len, int pix) {
        for(int i=0;i<len;i++) {
          if(pix == 1) { buf[start + i] = '1'; }
          else { buf[start + i] = '0'; };
        }
    }

    BUKVA(unsigned int idx_, unsigned short w_, unsigned short h_, unsigned short *src_buf) noexcept : idx(idx_), w(w_), h(h_), buf(nullptr){
        unsigned int len = 0, pix = 0, i = 0;
        buf = new unsigned char[w * h];
        //memo_counter(w*h);
        while( len < w*h )
        {
            set_len(len, src_buf[i], pix);
            if(pix == 0) { pix = 1; } else { pix = 0; };
            len += src_buf[i];
            i++;
        };
        if(len != w*h )
        {
            //_printf("BUKVA() constructor err\n");    
            return;
        };

        //_printf("BUKVA() constructor\n");
    }
    BUKVA(const BUKVA& src) noexcept {
        //_printf("BUKVA() const constructor\n");
    }
    BUKVA(BUKVA&& src) noexcept {
        //_printf("BUKVA() move constructor\n");
        *this = std::move(src);

    }
    ~BUKVA() {
        //_printf("BUKVA() destructor\n");
        
        if(buf != nullptr) {
            //memo_counter(w*h*-1);
            delete[] buf;
            buf = nullptr;
        };
    }
};

class FONT {
public:
    
    std::string name;
    unsigned int size;
    std::map<unsigned int, BUKVA> bukva;
    
    bool set_from_buffer_LZ(uint16_t *buf);
    
    FONT& operator=(const FONT& src) {
        if (this != &src) {
            //_printf("operator=&\n");
        };
        return *this;
    }
    FONT& operator=(FONT&& src) {
        if (this != &src) {
            //_printf("operator=&&\n");
            this->name  = std::move(src.name);
            this->size  = std::move(src.size);
            this->bukva = std::move(src.bukva);
        };
        return *this;
    }
    FONT(std::string name_, unsigned int size_, uint16_t *buf) noexcept : name(name_), size(size_){
        set_from_buffer_LZ(buf);
        //_printf("FONT() constructor\n");
    }
    FONT(const FONT& src) noexcept : size(0)  {
        //_printf("FONT() const constructor\n");
    }
    FONT(FONT&& src) noexcept : size(0) {
        //_printf("FONT() move constructor\n");
        *this = std::move(src);

    }
    
    ~FONT() {
        //_printf("FONT() destructor\n");
    }
};

class FONTS {
public:
    SCREEN_BUFFER *screen = nullptr;
    std::vector<FONT> list;
    
    void print(int x, int y, const char *font_name, int font_size, const char* txt, unsigned int color);
    void print_w(int x, int y, int max_w, const char *font_name, int font_size, const char* txt, unsigned int color);
    int  text_width(const char *font_name, int font_size, std::string& txt);
    int  text_height(const char *font_name, int font_size);

    
    void add_new_from_buffer(unsigned short *buf);
    void init();
    FONTS(SCREEN_BUFFER *screen) : screen(screen) {
        init();
    }
};

#endif /* FONT_H */

