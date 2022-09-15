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

#include "GUI/GUI.h"
#include "webcam.h"
#include "tools.h"

bool GLOBAL_STOP = false;

GUI gui;
//WEBCAMS webcams;

void Load_NetIfList(ELEMENT* list){
    std::string s;
    DIR *dir;
    int k;
    char c[500], n[10];
    dir = opendir("/sys/class/net");
    struct dirent *entry;
    
    while ( (entry = readdir(dir)) != NULL) {
        printf("{%d} - (%s) [%d] %d\n", entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
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
    
   (void)closedir(dir);
}

int main(int argc, char** argv) {

    
    
    gui.create_elements();
    
    if(gui.low_level_init()==false) return -1;
    
    Load_NetIfList(gui.IfList);
    //->item.push_back({ELEMENT_item::ListItem, "If1"});
    
    //webcams.init(&(gui.Memo1->string_list));
    
    
    
    gui.wait_run();
    
    set_GLOBAL_STOP(L"main");
       
    gui.wait_execute_close();
    //webcams.wait_execute_close();
    
    gui.finish();
    
    return 0;
}

