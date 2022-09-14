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

    q = elements.add(0, ELEMENT::Type::Panel,  30, 30, 640, 480, COLOR::PanelBG, COLOR::PanelBorder, "Panel1");
    Panel1_id = q->id;
    
    q = elements.add(Panel1_id, ELEMENT::Type::Memo,  5, 25, 150, 125, COLOR::MemoBG, COLOR::MemoBorder, "Memo1");
    Memo1_id = q->id;
    
    q = elements.add(Panel1_id, ELEMENT::Type::List,  160, 25, 150, 125, COLOR::ListBG, COLOR::ListBorder, "List1");
    ProgramList_id = q->id;
    
    elements.recalc_parent_id();
    Memo1 = elements.get_element_by_id(Memo1_id);
    ProgramList = elements.get_element_by_id(ProgramList_id);
    
    Memo1->item.push_back({ELEMENT_item::String, "str1"});
    Memo1->item.push_back({ELEMENT_item::String, "str2"});
    Memo1->item.push_back({ELEMENT_item::String, "str3"});
    
    
    ProgramList->item.push_back({ELEMENT_item::ListItem, "str1"});
    ProgramList->item.push_back({ELEMENT_item::ListItem, "str2"});
    ProgramList->item.push_back({ELEMENT_item::ListItem, "str3"});
}

void GUI::MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y ) {
    elements.MouseButtonEvent(MouseButtonType, mouse_x, mouse_y);
}