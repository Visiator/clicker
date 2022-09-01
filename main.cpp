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

#include "GUI/GUI.h"
#include "webcam.h"
#include "tools.h"

bool GLOBAL_STOP = false;

GUI gui;
WEBCAMS webcams;

int main(int argc, char** argv) {

    
    
    gui.create_elements();
    
    if(gui.low_level_init()==false) return -1;
    
    webcams.init(&(gui.Memo1->string_list));
    
    
    
    gui.wait_run();
    
    set_GLOBAL_STOP(L"main");
       
    gui.wait_execute_close();
    webcams.wait_execute_close();
    
    gui.finish();
    
    return 0;
}

