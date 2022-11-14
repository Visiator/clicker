/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   global.h
 * Author: root
 *
 * Created on September 15, 2022, 4:24 PM
 */




#ifndef GLOBAL_H
#define GLOBAL_H

#include <queue>
#include <thread>
#include <set>
#include <mutex>



class GUI;
class ELEMENT;
class SCREEN;

class GLOBAL {
public:
    
    std::mutex m_queue_mutex;
    
    ELEMENT *mikrotik_info = nullptr;
    std::queue<unsigned int> ip_to_add_q;
    std::set<unsigned int> ip_to_add_s;
    
    void add_ip_to_queue_to_send_mikrotik(unsigned int ip);
    unsigned int get_ip_to_queue_to_send_mikrotik();
    
    std::string FolderName;
    
    GUI *gui = nullptr;
    
    void init();
    void wait_execute_close();
    bool need_run_sniffer = false;
    std::string need_load_from_file_FileName;
    bool need_load_from_file(std::string file_name) {
        if(need_load_from_file_FileName != "") return false;
        need_load_from_file_FileName = file_name;
        return true;
    };
    
    std::string InterfaceSelect;
    int InterfaceSelectId;
    void set_InterfaceSelect(std::string val);
    void set_FoldersSelect(std::string val);
    void set_FilesDoubleClick(std::string val);
    void set_SnifferRun(bool val);
    
    bool execute_is_run = false;
    std::thread *execute_thread ;
    void execute();
    
    bool execute2_is_run = false;
    std::thread *execute2_thread ;
    void execute2();
    
    bool MousePress(int mx_, int my_, int mk_, int double_click_, int scr_w, int scr_h);
    
    bool mouse_to_left_top();
    bool mouse_to_right_top();
    bool mouse_to_left_bottom();
    bool mouse_to_right_bottom();
    
    bool mouse_move_from_left_top(int x, int y, int k, int double_click);
    bool mouse_move_from_right_bottom(int x, int y, int k, int double_click);
    
    bool mouse_move_from_right_top(int x, int y, int k, int double_click);
    
    void mouse_press();
    void mouse_unpress();
    
    void set_gui(GUI *val){
        gui = val;
    }
    
    void active_IF_tab();
    void active_PCAP_tab();
    void active_GRAB_tab();
    void active_PROG_tab();
    
    unsigned int WindowListId = 0;
    bool WindowListBtnStart = false;
    void set_WindowListBtnStart(bool v) {
        WindowListBtnStart = v;
    }
    bool test1();
    int f_serial = -1;
    void serial_io();
    unsigned char need_write_serial_5bytes[6];
    
    void colibrate_mouse();
    
    GLOBAL() {
        for(int i=0;i<6;i++) need_write_serial_5bytes[i] = 0;
    }
};

#endif /* GLOBAL_H */

