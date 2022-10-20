/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   program.h
 * Author: smorodin
 *
 * Created on September 2, 2022, 2:45 PM
 */

#ifndef PROGRAM_H
#define PROGRAM_H

#include <mutex>
#include <vector>
#include <string>
#include <thread>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>


#include "GUI/GUI_Primitives.h"

class SPRITE {
public:  
    std::string file_name;
    std::vector<unsigned int> bitmap;
    bool is_detected = false;
    int is_detected_x = 0, is_detected_y = 0;
    unsigned int w = 0, h = 0, delta = 0;
    void load_from_bmp(std::string& file_name_);
    bool eq(SCREEN *src, int x, int y);
    bool detect_sprite(SCREEN *src);
    bool operator<(const SPRITE &r) const
    {
        return file_name < r.file_name;        
    }
    bool operator>(const SPRITE &r) const
    {
        return file_name > r.file_name;        
    }
    
    SPRITE(std::string& file_name_){ load_from_bmp(file_name_); };
    SPRITE(char* file_name_){ 
        std::string fn;
        fn = file_name_;
        load_from_bmp(fn); 
    };
};

enum CMD {
    Undef, Label, Goto, If, Else, Endif, Print, Set, Stop
};

class PROGRAM_line {
public:
    int idx = -1, next_idx = -1, else_idx = -1;
    std::string raw, info, s1, s2, s3, s4;
    CMD cmd = CMD::Undef;
    
    void decode_CMD() {
        if(s1.length() > 1) {
            if(s1[0] == ':') {
                cmd = CMD::Label;
                return;
            }
            if(s1 == "stop") {
                cmd = CMD::Stop;
                return;
            }
            if(s1 == "if") {
                cmd = CMD::If;
                return;
            }
            if(s1 == "else") {
                cmd = CMD::Else;
                return;
            }
            if(s1 == "endif") {
                cmd = CMD::Endif;
                return;
            }
            if(s1 == "print") {
                cmd = CMD::Print;
                return;
            }
            if(s1 == "goto") {
                cmd = CMD::Goto;
                return;
            }            
            if(s1 == "set") {
                cmd = CMD::Set;
                return;
            }
            cmd = CMD::Undef;
        }
    }
    
    void clean() {
        idx = -1;
        raw = "";
        cmd = CMD::Undef;
    }
    
    PROGRAM_line(std::string raw) : raw(raw) {};
    ~PROGRAM_line() {};
};

class PROGRAM {
public:
    int ttimer[10];
    std::vector<SPRITE> sprite;
    std::map<std::string, std::string> vars;
    int idx = 0;
    
    std::vector<std::string> print_out;
    std::mutex p_mutex;
    void print_out_lock();
    void print_out_unlock();
    void print_out_add(std::string s);
    
    void load();
    void load_text(std::string& FolderName);
    void detect_sprites(SCREEN *src);
    void compile();
    
    bool is_run_program = false;
    bool need_run = false;
    bool need_stop = false;
    void run()  { print_out_add("run"); need_run = true;  };
    void stop() { print_out_add("stop"); need_stop = true; };
    
    int get_label_idx(std::string s);
    int get_else_idx(int v);
    
    void init() {
        for(int i=0;i<10;i++) ttimer[i] = 0;        
    }
    
    int next_step = -1;
    void execute_next_step();
    
    void print(char *s);
    void exec_set(std::string v1, std::string v2, std::string v3);
    std::string calc_value(std::string e);
    bool it_is_timer(std::string name);
    void set_timer(std::string name, std::string val);

    
    std::vector<PROGRAM_line> line;
    
    void copy_from_src(const PROGRAM& src) {
        while(is_run_program) {
            usleep(1);
        }
        for(int i=0;i<10;i++) ttimer[i] = src.ttimer[i];
        sprite = src.sprite;
        idx = src.idx;
        vars = src.vars;
        print_out = src.print_out;
        //p_mutex = src.p_mutex;
    }
    PROGRAM& operator=(PROGRAM&& src) {
        copy_from_src(src);
        return *this;
    }
    PROGRAM& operator=(const PROGRAM& src) {
        
        return *this;
    }   
    PROGRAM(int idx) : idx(idx), is_run_program(false) {};
    PROGRAM(PROGRAM&& src) {
        *this = std::move(src);
    }
    PROGRAM(const PROGRAM& src) {
        if(this != &src) {
            copy_from_src(src);
        }
    }
    ~PROGRAM(){};
};

class PROGRAMS {
public:
    
    std::vector<PROGRAM> item_;
    SCREEN grab_screen_buffer;
    
    void init();
    
    bool execute_is_run = false;
    std::thread* execute_thread = nullptr;
    void execute();    
    
    void wait_execute_close();
    
    void Grab_Sprites(Window w);
    
    PROGRAMS(){};
    ~PROGRAMS(){};
};

#endif /* PROGRAM_H */

