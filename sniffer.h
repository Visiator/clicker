/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   sniffer.h
 * Author: root
 *
 * Created on September 22, 2022, 8:10 PM
 */

#ifndef SNIFFER_H
#define SNIFFER_H

#include <queue>
#include <set>

class SNIFFER {
public:
    
    void set_local_stop(bool val) {
        local_stop = val;
    }
    bool local_stop = false;
    void start_sniff(int InterfaceId, const char *InterfaceName);
    SNIFFER() {
        
    };
};

#endif /* SNIFFER_H */

