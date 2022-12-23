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
    int idx = 0;
    std::string file__name;
    std::string nic;
    std::vector<unsigned int> bitmap;
    bool is_detected = false;
    uint64_t is_detected_time_ = 0;
    int is_detected_x = 0, is_detected_y = 0;
    int mouse_press_target_percent_w = 50, mouse_press_target_percent_h = 50;
    int double_click = 0, nopress = 0;
    unsigned int w = 0, h = 0, delta = 0;
    void load_from_bmp(std::string& file_name_);
    bool eq(SCREEN *src, int x, int y);
    bool detect_sprite(SCREEN *src);
    
    int get_detected_time();
    
    std::string print() {
        std::string s;
        s = "sprite:" + std::to_string(idx) + ":" + nic + ":" + (is_detected ? "detect" : "-") + ":" + std::to_string(is_detected_x) + "-" + std::to_string(is_detected_y);
        return s;
    }
    
    bool operator<(const SPRITE &r) const
    {
        return file__name < r.file__name;        
    }
    bool operator>(const SPRITE &r) const
    {
        return file__name > r.file__name;        
    }
    
    SPRITE(std::string& file_name_, int idx_){ idx = idx_; load_from_bmp(file_name_); };
    SPRITE(char* file_name_, int idx_){ 
        std::string fn;
        fn = file_name_;
        idx_ = idx;
        load_from_bmp(fn); 
    };
};

enum CMD {
    Undef, Label, Goto, If, Else, Endif, Print, Set, Stop, Comment, MousePress, MouseMove, KeyPress_, KeyPressRaw, KeyPressIosHome
};

class PROGRAM_line {
public:
    int idx = -1, next_idx = -1, else_idx = -1;
    std::string raw, info, s1, s2, s3, s4;
    CMD cmd = CMD::Undef;
    
    void decode_CMD() {
        
        if(s1.length() == 1) {
            if(s1[0] == '#') {
                cmd = CMD::Comment;
                return;
            }
        }
        
        if(s1.length() > 1) {
            if(s1[0] == '#') {
                cmd = CMD::Comment;
                return;
            }
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
            
            if(s1 == "mouse_press" || s1 == "mousepress") {
                cmd = CMD::MousePress;
                return;
            }
            
            if(s1 == "mouse_move" || s1 == "mousemove") {
                cmd = CMD::MouseMove;
                return;
            }

            if(s1 == "key_press_raw") {
                cmd = CMD::KeyPressRaw;
                return;
            }
            
            if(s1 == "key_press") {
                cmd = CMD::KeyPress_;
                return;
            }

            if(s1 == "key_press_ios_home") {
                cmd = CMD::KeyPressIosHome;
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
    SCREEN *grab_screen_buffer = nullptr;
    int sprite_detected_idx = 0;
    int ttimer[10];
    void timers_increase();
    std::vector<SPRITE> sprite;
    std::map<std::string, std::string> vars;
    int idx = 0;
    std::string detail;
    
    std::vector<std::string> print_out;
    std::mutex p_mutex;
    void print_out_lock();
    void print_out_unlock();
    void print_out_add(std::string s);
    
    void clear() {
        for(int i=0;i<10;i++) ttimer[i] = 0;
        sprite.clear();
        vars.clear();
        detail = "";
        print_out.clear();
        line.clear();
        
    }
    
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
    
    
    std::string getDetectedSprite();
    void init() {
        for(int i=0;i<10;i++) ttimer[i] = 0;        
    }
    
    int next_step = -1;
    void execute_next_step();
    
    void print(char *s);
    void exec_set(std::string v1, std::string v2, std::string v3);
    void exec_print(std::string v1, std::string v2, std::string v3);
    void exec_MousePress(std::string v1, std::string v2, int mk);
    void exec_MouseMove(std::string v1, std::string v2);
    void exec_KeyPressRaw(std::string v1, std::string v2);
    void exec_KeyPress(std::string v1, std::string v2);
    void exec_KeyPressIosHome();
    
    bool calc_boolean(std::string s);
    
    std::string calc_value_function_sprite_by_nic_is_detected(std::string p);
    void set_value_function_sprite_by_nic_is_detected_time(std::string p, std::string val);
    std::string calc_value_function_sprite_by_nic_is_detected_time(std::string p);
    std::string calc_value_function(std::string e);
    std::string calc_value(std::string e);
    std::string calc_value(std::string v1, std::string v2, std::string v3);
    
    bool it_is_timer(std::string name);
    void set_timer(std::string name, std::string val);
    std::string get_timer(std::string name);

    bool it_is_var(std::string name);
    std::string get_var(std::string name);
    
    bool it_is_sprite_name(std::string name);
    
    bool it_is_integer(std::string val);
    bool it_is_sprite(std::string val);
    
    std::string get_sprite_value(std::string s);
    bool get_XY_from_sprite_by_idx(std::string s,int &x, int &y, int &double_click);
    bool get_XY_from_sprite_by_name(std::string s,int &x, int &y, int &double_click);
    
    std::vector<PROGRAM_line> line;
    int get_sprite_max_id();
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
    
    void detect_sprites(SCREEN *src);
    
    void init();
    
    bool execute_is_run = false;
    std::thread* execute_thread = nullptr;
    void execute();    
    
    bool execute_timers_is_run = false;
    std::thread* execute_timers_thread = nullptr;
    void execute_timers();
    
    void wait_execute_close();
    
    //void Grab_Sprites(Window w);
    
    PROGRAMS(){};
    ~PROGRAMS(){};
};

#endif /* PROGRAM_H */

