/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: root
 *
 * Created on August 30, 2022, 3:03 PM
 */


#include <cstdlib>
#include <dirent.h>
#include <algorithm> 

#include "GUI/GUI.h"
#include "pcap.h"
#include "global.h"
#include "webcam.h"
#include "tools.h"
#include "sniffer.h"
#include "clicker.h"
#include "program.h"
#include "mikrotik.h"

bool GLOBAL_STOP = false;

GLOBAL global;
GUI gui;
PCAP pcap;
SNIFFER sniffer;
CLICKER clicker;
PROGRAMS programs;
MIKROTIK mikrotik;
WEBCAMS webcams;

void Load_Folders(ELEMENT* list) {
    list->item.push_back({&gui, ELEMENT_item::ListItem, "Downloads"});
    list->item.push_back({&gui, ELEMENT_item::ListItem, "Pictures"});
    list->item.push_back({&gui, ELEMENT_item::ListItem, "Music"});
    list->item.push_back({&gui, ELEMENT_item::ListItem, "tmp"});
}

void Load_NetIfList(ELEMENT* list){
    std::string s;
    DIR *dir;
    int k;
    char c[500], n[10];
    dir = opendir("/sys/class/net");
    struct dirent *entry;
    
    while ( (entry = readdir(dir)) != NULL) {
        //printf("{%d} - (%s) [%d] %d\n", entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
        sprintf(c, "/sys/class/net/%s/ifindex", entry->d_name);
        FILE *f;
        f = fopen(c, "rb");
        if(f != NULL) {
            for(int i=0;i<10;i++) n[i] = 0;
            k = fread(n, 1, 10, f);
            if(k >= 1 && k <= 9 && n[k-1] == '\n') n[k-1] = 0;
            fclose(f);
            sprintf(c, "%s:%s", n, entry->d_name);
            
            list->item.push_back({&gui, ELEMENT_item::ListItem, c});
        }
    };    
   closedir(dir);
   
   std::sort( list->item.begin(), list->item.end() );
}

void testtt();

int main(int argc, char** argv) {
    
    
    std::vector<std::string> list;

    mikrotik.set_ip_login_pass("192.168.5.5", "admin", "Qq1233!!");
    
    programs.init();
    
    gui.create_elements();
    global.set_gui(&gui);
    global.init();
    
    if(gui.low_level_init()==false) return -1;
    
    Load_NetIfList(gui.IfList);
    Load_Folders(gui.FoldersList);
    
    std::vector<std::string> lst;
    
    webcams.init(&lst, &programs.item_[0].grab_screen_buffer, &programs.item_[0]);
    
    clicker.init();
    
    
    //global.active_PROG_tab();
    
    gui.wait_run();
    
    set_GLOBAL_STOP(L"main");
    programs.wait_execute_close();   
    gui.wait_execute_close();
    global.wait_execute_close();
    webcams.wait_execute_close();
    
    gui.finish();
    
    return 0;
}


