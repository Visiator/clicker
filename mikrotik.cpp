/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "mikrotik.h"
#include <string.h>
#include "tools.h"
#include "GUI/GUI_Element.h"

MIKROTIK::MIKROTIK() {

}

void MIKROTIK::set_ip_login_pass(const char *ip, const char *login, const char *pass) {
    this->_ip = ip;
    this->_login = login;
    this->_pass = pass;
}

bool MIKROTIK::_connect() {

    sock = socket(AF_INET, SOCK_STREAM, 0);

    s_address.sin_family = AF_INET;
    s_address.sin_addr.s_addr = inet_addr("192.168.5.5");
    s_address.sin_port = htons(8728);

    int r;

    r = connect(sock, (struct sockaddr *)&s_address, sizeof(s_address));
    if(r < 0) {
        return false;
    }

    int flags = fcntl(sock , F_GETFL, 0);
    fcntl(sock , F_SETFL, flags|O_NONBLOCK);
    int imode = 1;
    ioctl(sock , FIONBIO, &imode);

    return true;
}

void MIKROTIK::_disconnect() {
    close(sock);
}

bool MIKROTIK::send_login() {
    char cc[5010];

    sprintf(cc, "%c/login%c=name=%s%c=password=%s%c", 6, (int)_login.length() +6, _login.c_str(), (int)_pass.length()+10, _pass.c_str(), 0);
    int k = 1 + 6 + 1 + (int)_login.length()+6 + 1 + (int)_pass.length()+10 + 1;
    int j = send(sock, cc, k, 0);
    if(j != k) return false;

    return true;
}

bool MIKROTIK::read_responce(std::vector<std::string> &v) {

    unsigned long t0, t1, t2, t3;
    t0 = GetTickCount();
    t1 = 0;
    unsigned char c, b[1000];
    int sz, i, k, s1, s2, s3;

    while(true) {
        // read text len
        sz = 0;
        while(sz == 0)
        {
            k = recv(sock, &c, 1, 0);
            if(k == 1) {
                if(c == 0) {
                    /*if(buf_is(b, (char *)"!done")) {
                        return true;
                    }
                    continue;*/
                    //return true;
                    k = recv(sock, &c, 1, 0);
                    if(k == -1) return true;
                }
                if(c <= 127) {
                    if(c == 0) {
                        return true;
                    }
                    sz = c;
                } else {
                    printf("mikrotik c > 127\n");
                    return false;
                }
            }
            t1 = GetTickCount() - t0;
            if(t1 > 300000) {
                return false;
            }
        };
        // read text
        s1 = 0;
        s2 = 0;
        s3 = 0;

        i = 0;
        b[0] = 0;
        while( i < sz) {
            k = recv(sock, &c, 1, 0);
            if(k == -1) {
                s1++;
            } else if (k == 0) {
                s2++;
            } else if (k == 1) {
                s3++;
                b[i] = c;
                if(i < 1000-1) i++;

            } else {
                printf("wtf? k");
                return false;
            }
        }
        b[i] = 0;
        v.push_back((char *)b);
   }
    return false;
}

bool MIKROTIK::login_() {
    if(!send_login()) {
        return false;
    }
    std::vector<std::string> v;
    bool r;
    r = read_responce(v);
    if(v.size() == 1) {
        if(v[0] == "!done") {
            return true;
        }
    }
    return false;
}

bool MIKROTIK::get_firewall_ip_list() {

    lock();
    
    ip_in_mikrotik.clear();
    
    unsigned long t0, t1, t2, t3;

    t0 = GetTickCount();

    if(!_connect()) {
        unlock();
        return false;
    }
    t1 = GetTickCount() - t0;
    if(!login_()) {
        unlock();
        return false;
    }
    if(!send_command("/ip/firewall/filter/print")) {
        unlock();
        return false;
    }
    unsigned int iip;
    bool r;
    std::vector<std::string> v;
    r = read_responce(v);

    if(v.size() > 0) {
        if(v[0] == "!re") {
            for(const auto& s : v) {
                if(s.length() >= 13) {
                    std::string a;
                    a = s.substr(0, 13);
                    if(a == "=dst-address=") {
                        a = s.substr(13);    
                        iip = char_to_ipv4((char *)a.c_str());
                        ip_in_mikrotik.insert(iip);
                    }
                }
                printf("%s\n", s.c_str());
            }
        }
    }
    _disconnect();
    if(mikrotik_info != nullptr) {
        mikrotik_info->info2 = ip_in_mikrotik.size();
    }
    
    unlock();
    return true;
}


bool MIKROTIK::buf_is(char *buf, char *v) {
    int i;
    i = 0;
    while(v[i] != 0) {
        if(buf[i] != v[i]) return false;
        i++;
    }
    return true;
}

bool MIKROTIK::send_command(const char *command) {
    char cc[5010];
    int j;
    if(strlen(command) == 0) return false;
    if(strlen(command) <= 127) {
        sprintf(cc, "%c%s%c", (int)strlen(command), command, 0);
        j = send(sock, cc, (int)strlen(command)+2, 0);
        if(j == strlen(command)+2) return true;
        return false;
    }
    printf("???????????\n");
    return false;

}

bool MIKROTIK::send_command(const char *command, const char *p1, const char *p2, const char *p3) {
    char cc[5010];
    int j, ll;
    if(strlen(command) == 0) return false;
    if(strlen(command) <= 127) {
        sprintf(cc, "%c%s%c%s%c%s%c%s%c"
                , (int)strlen(command), command
                , (int)strlen(p1), p1
                , (int)strlen(p2), p2
                , (int)strlen(p3), p3
                , 0);
        ll = 1+(int)strlen(command)+1+(int)strlen(p1)+1+(int)strlen(p2)+1+(int)strlen(p3)+1;
        j = send(sock, cc, ll , 0);
        if(j == ll) return true;
        return false;
    }
    printf("???????????\n");
    return false;

}


bool MIKROTIK::set_firewall_ip(unsigned int _ip) {
    
    lock();
    
    if(ip_in_mikrotik.find(_ip) != ip_in_mikrotik.end()) {
        unlock();
        return true;
    }
    unlock();
    get_firewall_ip_list();
    lock();
    if(ip_in_mikrotik.find(_ip) != ip_in_mikrotik.end()) {
        unlock();
        return true;
    }
    
    
    //////////////////////////////////
    //
    unsigned long t0, t1, t2, t3;
    char ip[50];
    ipv4_to_char(_ip, ip);

    t0 = GetTickCount();

    if(!_connect()) {
        unlock();
        return -1;
    }
    t1 = GetTickCount() - t0;
    if(!login_()) {
        unlock();
        return false;
    }
    std::string s;
    s = "=dst-address=";
    s += ip;
    if(!send_command("/ip/firewall/filter/add"
            , "=chain=forward"
            , "=action=drop"
            , s.c_str()
            )) {
        unlock();
        return false;
    }

    
    
    bool r, ret;
    ret = false;
    std::vector<std::string> v;
    r = read_responce(v);

    for(const auto& s : v) {
        if(s == "!done") {
            ret = true;
        }
        printf("%s\n", s.c_str());
    }

    _disconnect();
    
    //
    //////////////////////////////////////////////////////////
    
    unlock();
    return ret;
   
}

void MIKROTIK::lock() {
    mutex.lock();
}
    
void MIKROTIK::unlock() {
    mutex.unlock();
}

void MIKROTIK::set_mikrotik_info(ELEMENT *v) {
    mikrotik_info = v;
}

bool MIKROTIK::ip_is_exists_in_list(unsigned int ip) {
    if(ip_in_mikrotik.find(ip) != ip_in_mikrotik.end()) {
        return true;
    }
    return false;
}

void MIKROTIK::ip_list_to_txt(char *file_name) {
    get_firewall_ip_list();
    char s[100];
    FILE *f;
    f = fopen(file_name, "wb");
    if(f == NULL) {
        printf("open file error!\n");
        return;
    }
    for(auto& a : ip_in_mikrotik) {
        ipv4_to_char(a, s);
        fprintf(f, "%s\n", s);
    }
    fclose(f);
}
    
void MIKROTIK::ip_list_from_txt(char *file_name) {
    FILE *f;
    f = fopen(file_name, "rb");
    if(f == NULL) {
        printf("open file error!\n");
        return;
    }
    
    _connect();

    login_();
    
    char cc[100];
    unsigned int ii;
    int i, j;
    i = fgetc(f);
    j = 0;
    while(i != EOF)
    {
        if(i == 13 || i == 10) {
            
            printf("%s\n", cc);

            ii = char_to_ipv4(cc);
            
            //set_firewall_ip(ii);
            //////////////////////////////////////////////////////////
            //
            
            std::string s;
            s = "=dst-address=";
            s += cc;
            if(!send_command("/ip/firewall/filter/add"
                    , "=chain=forward"
                    , "=action=drop"
                    , s.c_str()
                    )) {
                unlock();
                return ;
            }


            std::vector<std::string> v;
            v.clear();
            bool r, ret;
            r = read_responce(v);

            ret = false;
            
            for(const auto& s : v) {
                if(s == "!done") {
                    ret = true;
                }
                printf("%s\n", s.c_str());
            }
            if(ret == false) {
                
                return;
            }
            //
            //////////////////////////////////////////////////////////
            
            
            
            j = 0;
            cc[0];
        } else {
            if( i != 10 && i != 13) {
                cc[j] = (char)i;
                if(j < 100-5) j++;
                cc[j] = 0;
            }
        }
                
        i = fgetc(f);
    }

    fclose(f);
    
    _disconnect();
}