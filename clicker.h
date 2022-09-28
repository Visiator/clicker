/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   clicker.h
 * Author: root
 *
 * Created on September 23, 2022, 5:15 PM
 */

#ifndef CLICKER_H
#define CLICKER_H

#include <thread>

class CLICKER {
public:
    
    void init();
    
    bool execute_is_run = false;
    std::thread* execute_thread = nullptr;
    void execute();
    
    CLICKER(){};    
};

#endif /* CLICKER_H */

