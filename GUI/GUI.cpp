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

    q = elements.add(0, ELEMENT::Type::Panel,  30, 30, 640, 480, 0xccaacc, 0xff0000, "");
    
    q = elements.add(q->id, ELEMENT::Type::Memo,  5, 490, 50, 25, 0xaaffaa, 0xff0000, "");
    Memo1_id = q->id;
    
    elements.recalc_parent_id();
    Memo1 = elements.get_element_by_id(Memo1_id);
    
    //Memo1->string_list.push_back("str1");
    //Memo1->string_list.push_back("str2");
    //Memo1->string_list.push_back("str3");
}