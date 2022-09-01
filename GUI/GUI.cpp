/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   GUI.cpp
 * Author: root
 * 
 * Created on August 30, 2022, 3:05 PM
 */

#include "GUI.h"
#include "../tools.h"

extern bool GLOBAL_STOP;

GUI::GUI() {
}

GUI::GUI(const GUI& orig) {
}

GUI::~GUI() {
}
   
void GUI::wait_execute_close() {
    while(execute_is_run == true) {
        usleep(100);
    }
};    


void GUI::wait_run() {
    while(GLOBAL_STOP == false) {
        this->screen.need_update = true;
        usleep(100);
    }
};  

void GUI::create_elements() {

    ELEMENT *q;
    
    q = elements.add((ELEMENT *)nullptr, ELEMENT::Type::Memo,  10, 30, 150, 25, 0xaaffaa, 0xff0000, "caption1");
    q->x = 200;
}