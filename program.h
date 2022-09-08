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

#include <vector>

class PROGRAM_line {
public:
    int id;
    std::string raw;
    
    void clean() {
        id = -1;
        raw = "";
    }
    
    PROGRAM_line() {};
    ~PROGRAM_line() {};
};

class PROGRAM {
public:
    std::vector<PROGRAM_line> line;
    PROGRAM(){};
    ~PROGRAM(){};
};

class PROGRAMMS {
public:
    std::vector<PROGRAM> item;
    PROGRAMMS(){};
    ~PROGRAMMS(){};
};

#endif /* PROGRAM_H */

