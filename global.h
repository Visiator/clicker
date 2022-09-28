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
    
    
    void set_gui(GUI *val){
        gui = val;
    }
    
    void active_IF_tab();
    void active_PCAP_tab();
    void active_CLICKER_tab();
    
    unsigned int WindowListId = 0;
    bool WindowListBtnStart = false;
    void set_WindowListBtnStart(bool v) {
        WindowListBtnStart = v;
    }
    
    GLOBAL() {
        
    }
};

#endif /* GLOBAL_H */

