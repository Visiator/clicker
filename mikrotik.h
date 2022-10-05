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
#include <mutex>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

class ELEMENT;

class MIKROTIK {
private:
    ELEMENT *mikrotik_info = nullptr;
    std::set<unsigned int> ip_in_mikrotik;
    
    int sock;
    sockaddr_in s_address{};

    std::string _ip, _login, _pass;

    std::mutex mutex;
    
    void lock();
    void unlock();
    
    bool login_();

    
    
    
    bool send_login();
    bool send_command(const char *command);
    bool send_command(const char *command, const char *p1, const char *p2, const char *p3);
    bool read_responce(std::vector<std::string> &v);
    bool _connect();
    void _disconnect();

    bool buf_is(char *buf, char *v);

public:
    void set_ip_login_pass(const char *ip, const char *login, const char *pass);
    bool ip_is_exists_in_list(unsigned int ip);
    void set_mikrotik_info(ELEMENT *v);
    bool set_firewall_ip(unsigned int ip);
    
    bool set_firewall_ip_raw(unsigned int ip);
    
    bool get_firewall_ip_list();
    
    void ip_list_to_txt(char *file_name);
    void ip_list_from_txt(char *file_name);
    
    MIKROTIK();
};


#endif /* MIKROTIK_H */

