/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   mikrotik.h
 * Author: root
 *
 * Created on September 25, 2022, 6:29 PM
 */

#ifndef MIKROTIK_H
#define MIKROTIK_H

#include <string>
#include <vector>
#include <set>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

class ELEMENT;

class MIKROTIK {
public:
    ELEMENT *mikrotik_info = nullptr;
    std::set<unsigned int> ip_in_mikrotik;
    
    int sock;
    sockaddr_in s_address{};

    std::string _ip, _login, _pass;

    bool login();

    bool set_firewall_ip(unsigned int ip);
    void set_ip_login_pass(const char *ip, const char *login, const char *pass);
    bool get_firewall_ip_list();
    bool send_login();
    bool send_command(const char *command);
    bool send_command(const char *command, const char *p1, const char *p2, const char *p3);
    bool read_responce(std::vector<std::string> &v);
    bool _connect();
    void _disconnect();

    bool buf_is(char *buf, char *v);

    MIKROTIK();
};


#endif /* MIKROTIK_H */

