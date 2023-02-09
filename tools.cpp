/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */
#include <sys/time.h>
#include <thread>
#include <stdlib.h>
#include <dirent.h>
#include "tools.h"
#include "frame.h"

#ifdef __linux__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#endif


#ifdef _WIN32

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_

#endif

extern bool GLOBAL_STOP;

#ifdef __linux__
unsigned long GetTickCount()
{
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    return (tv.tv_sec*1000+tv.tv_usec/1000);
}
#endif



unsigned int char_to_ipv4(char *buf) {

    unsigned int i1, i2, i3, i4, ii, i, k;
    ii = 0;
    i = 0;
    k = 1;
    
    i1 = 0;
    i2 = 0;
    i3 = 0;
    i4 = 0;
    
    while(buf[i] != 0) {
        if(buf[i] == '.') {
            k++;
        } else if(buf[i] >= '0' && buf[i] <= '9') {
            if(k == 1) { i1 *= 10; i1 += buf[i] - '0'; };
            if(k == 2) { i2 *= 10; i2 += buf[i] - '0'; };
            if(k == 3) { i3 *= 10; i3 += buf[i] - '0'; };
            if(k == 4) { i4 *= 10; i4 += buf[i] - '0'; };
        }
        
        i++;
    }
    ii = (i1<<24) | (i2<<16) | (i3<<8) | i4;
    return ii;
}

char *ipv4_to_char(unsigned int ip, char *buf) {
    buf[0] = 0;
    unsigned char *q;
    q = (unsigned char *)&ip;
    sprintf(buf, "%d.%d.%d.%d", q[3], q[2], q[1], q[0]);
    return buf;
}

unsigned short get_i16(unsigned char v1, unsigned char v2) {
    unsigned short r;
    unsigned char *q;
    q = (unsigned char *)&r;
    q[0] = v1;
    q[1] = v2;
    return r;
    
}

unsigned int get_i32(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4) {
    unsigned int r;
    unsigned char *q;
    q = (unsigned char *)&r;
    q[0] = v1;
    q[1] = v2;
    q[2] = v3;
    q[3] = v4;
    return r;
    
}
unsigned int get_i24(unsigned char v1, unsigned char v2, unsigned char v3) {
    unsigned int r = 0;
    unsigned char *q;
    q = (unsigned char *)&r;
    q[0] = v1;
    q[1] = v2;
    q[2] = v3;
    return r;
    
}

void usleep(int milliseconds) {
    if(milliseconds == 1) {
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        return;
    };
    if(milliseconds == 10) {
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        return;
    };
    if(milliseconds == 100) {
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
        return;
    };
    if(milliseconds == 333) {
        std::this_thread::sleep_for (std::chrono::milliseconds(333));
        return;
    };
    std::this_thread::sleep_for (std::chrono::milliseconds(milliseconds));
}

int my_strlen(const char *s1) {
    if(s1 == nullptr) return 0;
    int i=0;
    while(s1[i] != 0) {i++;};
    return i;
}

bool my_strcmp(const char *s1, int s1_max_size, const char *s2) {
    if(s1 == nullptr || s2 == nullptr) return false;
    int i;
    i = 0;
    while( i < s1_max_size && s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i]) {
        i++;
    }
    if( i >= s1_max_size ) return false;
    if(s1[i] == 0 && s2[i] == 0) return true;
    return false;
}

unsigned char _to_lower(unsigned char v) {
    if(v >= 0x41 && v <= 0x5a) return v+0x20;
    return v;
}

bool my_strcmp_lower(const char *s1, const char *s2) {
    if(s1 == nullptr || s2 == nullptr) return false;
    int i;
    i = 0;
    while(s1[i] != 0 && s2[i] != 0 && _to_lower(s1[i]) == _to_lower(s2[i])) {
        i++;
    }
    if(s1[i] == 0 && s2[i] == 0) return true;
    return false;
}


void set_GLOBAL_STOP(const wchar_t *str) {
    printf("set_GLOBAL_STOP\n");
    GLOBAL_STOP = true;
    
}

void wtf(const char *info, int frame_no, unsigned char *buf, int buf_size) {
    printf("wtf - %s frame_no=%d buf_size=%d\n", info, frame_no, buf_size);
}
void wtf(const char *info) {
    printf("wtf - %s\n", info);
}

void memsetzero(unsigned char *destination, int n) {
    int i;
    i = 0;
    while(i < n) {
        destination[i] = 0;
        i++;
    }
}


bool DirectoryExists( const char* pzPath )
{
    if ( pzPath == NULL) return false;

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL)
    {
        bExists = true;    
        (void) closedir (pDir);
    }

    return bExists;
}

char *decode_to_char_tcp_flag(unsigned char f, char *c) {
    unsigned char tcp_flags_cwr = (f&0x80)>>7;
    unsigned char tcp_flags_ecn = (f&0x40)>>6;
    unsigned char tcp_flags_urg = (f&0x20)>>5;
    unsigned char tcp_flags_ack = (f&0x10)>>4;
    unsigned char tcp_flags_push = (f&0x08)>>3;
    unsigned char tcp_flags_reset = (f&0x04)>>2;
    unsigned char tcp_flags_syn = (f&0x02)>>1;
    unsigned char tcp_flags_fin = (f&0x01);
    sprintf(c, "%c%c%c%c%c%c%c%c", tcp_flags_cwr == 0 ? '-' : 'c'
                         , tcp_flags_ecn == 0 ? '-' : 'e'
                         , tcp_flags_urg == 0 ? '-' : 'u'
                         , tcp_flags_ack == 0 ? '-' : 'A'
            
                         , tcp_flags_push == 0 ? '-' : 'p'
                         , tcp_flags_reset == 0 ? '-' : 'r'
                         , tcp_flags_syn == 0 ? '-' : 'S'
                         , tcp_flags_fin == 0 ? '-' : 'f'
            );
    return c;
}

void detect_ip(FRAME *frame) {
    unsigned char ip_proto_;
    int ip0_, port0_, ip1_, port1_, frame_size_;
    
    if( !(frame->ipv4_dst_ip, 192, 168, 1, 6) &&
        !is_ip(frame->ipv4_src_ip, 192, 168, 1, 6) ) {
        
        return;
        
    }
    
    if(frame->direction == egress) {
        ip1_ = frame->ipv4_dst_ip;
        port1_ = frame->ipv4_dst_port;
        
        ip0_ = frame->ipv4_src_ip;
        port0_ = frame->ipv4_src_port;
        
    } else {
        ip1_ = frame->ipv4_src_ip;
        port1_ = frame->ipv4_src_port;
        
        ip0_ = frame->ipv4_dst_ip;
        port0_ = frame->ipv4_dst_port;
    }
    frame_size_ = frame->payload_size;
    
    if(ip1_ == 0) return;
    if(ip1_ == 0xffffffff) return;
    if((ip1_ & 0xff000000) == 0xc0000000 &&
       (ip1_ & 0xff0000) == 0xa80000 )
    {
        
        return;
    }

    if(ip1_ == 0x08080808 || ip0_ == 0x08080808) {
        return;
    }
    
    char ss[500], ip0__[100], ip1__[100];
    ipv4_to_char(ip1_, ip1__);
    ipv4_to_char(ip0_, ip0__);
    
    if(port1_ == 53 || port0_ == 53) {
        if(ip1_ == 0xad20962 || ip0_ == 0xad20962) { // 10.210.9.98
            return;
        }
    }
    
    sprintf(ss, "/var/www/html/sniffer_web/ip3/%s:%d_%s:%d", ip1__, port1_, ip0__, port0_);
    
    char tcp_e[1000], udp_e[1000], sni[1000], cert[1000], dns_name[1000], http_get[1000];
    char tcp_i[1000], udp_i[1000];
    tcp_e[0] = 0;
    udp_e[0] = 0;
    tcp_i[0] = 0;
    udp_i[0] = 0;
    sni[0] = 0;
    cert[0] = 0;
    dns_name[0] = 0;
    http_get[0] = 0;
    int i,j,jj,k,v;
    
    FILE *f;
    f = fopen(ss, "rb");
    if( f != NULL ){
        i = fgetc(f);
        j = 0;
        jj = 0;
        k = 0;
        v = 0;
        while(i != EOF) {
            if(i == '\n') {
                j = 0;
                jj = 0;
                v = 0;
                if(k < 5) k++;
                
            } else {
                if(i == ':') {
                    v++;
                } else {
                    if(v==1) {
                        if(k == 0) { tcp_e[j] = i; tcp_e[j+1] = 0; };
                        if(k == 1) { udp_e[j] = i; udp_e[j+1] = 0; };
                        if(k == 2) { sni[j] = i; sni[j+1] = 0; };
                        if(k == 3) { cert[j] = i; cert[j+1] = 0; };
                        if(k == 4) { dns_name[j] = i; dns_name[j+1] = 0; };
                        if(k == 5) { 
                            http_get[j] = i; 
                            http_get[j+1] = 0; 
                        };
                        if(j<1000-5) j++;
                    };
                    if(v==2) {
                        if(k == 0) { tcp_i[jj] = i; tcp_i[jj+1] = 0; };
                        if(k == 1) { udp_i[jj] = i; udp_i[jj+1] = 0; };
                        if(jj<1000-5) jj++;
                    };
                }
            }
            i = fgetc(f);
        }
        fclose(f);
        //rewrite_in_file_A(ss, ip_proto, frame_size);
        
        //return;
    }
    
    f = fopen(ss, "wb");
    if(f == nullptr) {
        printf("f == nullptr\n");
        return;
    }
    
    int tcp_val_e = atoi(tcp_e);
    int udp_val_e = atoi(udp_e);
    int tcp_val_i = atoi(tcp_i);
    int udp_val_i = atoi(udp_i);
    
    
    if(frame->ip_proto == 6) { 
        if(frame->direction == egress) {
            tcp_val_e += frame_size_;
        } else {
            tcp_val_i += frame_size_;
        }
    } else if(frame->ip_proto == 17) { 
        if(frame->direction == egress) {
            udp_val_e += frame_size_;
        } else {
            udp_val_i += frame_size_;
        }
    } else {
        
    }

    std::string aa;
    if(sni[0] == 0) {
        for(auto &a: frame->SNI) {
            if(sni[0] != 0 && strlen(sni) < 1000-100) strcat(sni, ",");
            if(strlen(sni) + strlen(a.c_str()) < 1000-10) strcat(sni, a.c_str());
        }
    };
   
    
    if(cert[0] == 0) {
        
        
            int xx = frame->get_cert_count();
            
            if(xx > 0) {
                sprintf(cert, "%d", xx);
            };
        
    };
    

    
    if(http_get[0] == 0) {
        if(frame->http.get.length() > 0) {
            strcpy(http_get, frame->http.get.c_str());
        }
    }
    
    if(sni[0] == 'd') {
        printf("d\n");
    }
    
    
    fprintf(f, "tcp:%05d:%05d\n", tcp_val_e, tcp_val_i);
    fprintf(f, "udp:%05d:%05d\n", udp_val_e, udp_val_i);   
    fprintf(f, "sni:%s\n", sni);
    fprintf(f, "cert:%s\n", cert);
    fprintf(f, "dns_name:%s\n", frame->session_dns_name.c_str());
    fprintf(f, "http_get:%s\n", http_get);
    
    fclose(f);
    
    //analiz_by_patterns(frame);
}

int get_ui8(unsigned char *item, int idx, unsigned char *v, int index) {
    *v = item[idx + index + 0];
    return 1;
};

int get_ui16(unsigned char *item, int idx, unsigned short *v, int index) {
    unsigned char *c;
    c = (unsigned char *)v;
    c[0] = item[idx + index + 0];
    c[1] = item[idx + index + 1];

    return 2;
  };
int get_ui16_R(unsigned char *item, int idx, unsigned short *v, int index) {
    unsigned char *c;
    c = (unsigned char *)v;
    c[0] = item[idx + index + 1];
    c[1] = item[idx + index + 0];
    return 2;
  };

void write2(FILE *f, unsigned short v) {
    fwrite(&v, 1, 2, f);
}
void write4(FILE *f, unsigned int v) {
    fwrite(&v, 1, 4, f);
}
    
void write3(FILE *f, unsigned char b, unsigned char g, unsigned char r) {
    fprintf(f, "%c%c%c", b, g, r);
}

void save_textura_info(FILE *f, int x, int y, char *nic) {
    fprintf(f, "x:%d\n", x);
    fprintf(f, "y:%d\n", y);
    fprintf(f, "mouse_press_target_percent_w:50\n");
    fprintf(f, "mouse_press_target_percent_h:50\n");
    fprintf(f, "nic:%s\n", nic);
    fprintf(f, "nopress:0\n");
    fprintf(f, "double_click:0\n");
}

void save_textura_to_BMP_file_(FILE *f, unsigned int *bitmap, int w, int h) {

    int i = 0, sz;

    sz = ((w*3) + (w % 4))*h;

    unsigned short bfType, bfReserved1, bfReserved2;
    unsigned int bfSize, bfOffBits;

    bfType = 0x4d42;
    bfSize = sz + 54;
    bfReserved1 = 0;
    bfReserved2 = 0;
    bfOffBits = 54;

    write2(f, bfType);
    write4(f, bfSize);
    write2(f, bfReserved1);
    write2(f, bfReserved2);
    write4(f, bfOffBits);

    unsigned int BITMAPINFO_ver = 40;

    write4(f, BITMAPINFO_ver);

    unsigned int biWidth = w;
    unsigned int biHeight = h;
    unsigned short biBitCount = 24;
    unsigned int biCompression = 0;
    unsigned int biSizeImage = sz;
    unsigned int biClrUsed = 0;
    unsigned int biClrImportant = 0;
    write4(f, biWidth);
    write4(f, biHeight);
    write2(f, 1);
    write2(f, biBitCount);
    write4(f, biCompression);
    write4(f, biSizeImage);
    write4(f, 1);
    write4(f, 1);
    write4(f, biClrUsed);
    write4(f, biClrImportant);
    int j, x, y;
    for (y = h - 1; y >= 0; y--) {
            j = 0;
            for (x = 0; x < w; x++) {

                unsigned int r, g, b;
                r = 0;
                g = 0;
                b = 0;
                b = bitmap[y*w+x] & 0xff;
                g = (bitmap[y*w+x] & 0xff00)>>8;
                r = (bitmap[y*w+x] & 0xff0000)>>16;
                write3(f, b, g, r);

                    //i += 3;
                    j += 3;
            }
            j = w % 4;
            while(j>0) {
                fprintf(f, "%c", 0);
                j--;
            }
            i += j;
    };
}

bool is_ip(unsigned int ip, unsigned char i1, unsigned char i2, unsigned char i3, unsigned char i4) {
    unsigned int ii;
    ii = (i1<<24) | (i2<<16) | (i3<<8) | i4;
    if(ip == ii) return true;
    return false;
}

unsigned short rte_cpu_to_be_16(unsigned short v) {
    unsigned char c1, c2, *c;
    c = (unsigned char *)&v;
    c1 = c[0];
    c[0] = c[1];
    c[1] = c1;
    return v;
}

void load_from_BMP_buffer(unsigned char *buf, unsigned int buf_size, std::vector<unsigned int> &bitmap, unsigned int &w, unsigned int &h) {
        int inverse = 0;

        if (buf == nullptr) return;
        //if (bitmap != nullptr) delete[] bitmap;

        //bitmap = nullptr;
        w = 0;
        h = 0;

        if(buf_size < 50) return;

        unsigned short *bfType, *bfReserved1, *bfReserved2;
        unsigned int *bfSize, *bfOffBits;

        bfType = (unsigned short *)&(buf[0]);
        bfSize = (unsigned int *)&(buf[2]);
        bfReserved1 = (unsigned short *)&(buf[6]);
        bfReserved2 = (unsigned short *)&(buf[8]);
        bfOffBits = (unsigned int *)&(buf[10]);


        if (*bfType != 0x4d42) return;


        unsigned int *BITMAPINFO_ver, *biWidth, *biHeight, *biCompression, *biSizeImage, *biClrUsed, *biClrImportant;
        unsigned short *biBitCount;

        int i, j, delta, x, y;
        //unsigned int *q;
        delta = *bfOffBits;

        BITMAPINFO_ver = (unsigned int *)&(buf[14]);

        if (*BITMAPINFO_ver == 40) {
                biWidth = (unsigned int *)&(buf[14 + 0x04]);
                biHeight = (unsigned int *)&(buf[14 + 0x08]);
                biBitCount = (unsigned short *)&(buf[14 + 0x0e]);
                biCompression = (unsigned int *)&(buf[14 + 0x10]);
                biSizeImage = (unsigned int *)&(buf[14 + 0x14]);
                biClrUsed = (unsigned int *)&(buf[14 + 0x20]);
                biClrImportant = (unsigned int *)&(buf[14 + 0x24]);
                if (*biCompression == 0 && *biBitCount == 24) {
                    
                        w = *biWidth;
                        h = *biHeight;

                        bitmap.resize(w * h);
                        
                        //bitmap = new unsigned int[w * h]; 
                        
                        w = *biWidth;
                        h = *biHeight;
                        
                        //q = (unsigned int *)bitmap;
                        char cc[4];
                        unsigned int *qq;
                        qq = (unsigned int *)cc;
                        
                        i = delta;
                        for (y = h - 1; y >= 0; y--) {
                                j = 0;
                                for (x = 0; x < w; x++) {
                                
                                    if(i+3 >= buf_size) {
                                        
                                        return;
                                    }
                                    
                                        if (inverse == 0) {
                                
                                                cc[3] = 0;
                                                cc[0] = buf[i];
                                                cc[1] = buf[i + 1];
                                                cc[2] = buf[i + 2];
                                        }
                                        else {
                                                cc[3] = 0;
                                                cc[2] = buf[i];
                                                cc[1] = buf[i + 1];
                                                cc[0] = buf[i + 2];
                                        }
                                        bitmap[y*w + x] = *qq;
                                        i += 3;
                                        j += 3;
                                }
                                j = w % 4;

                                i += j;
                        };
                        return;
                }
                return;
        }
};

std::string get_word(std::string s) {
    
    if(s.length()>=2) {
        if(s[0] == '=' && s[1] == '=') return "==";
        if(s[0] == '!' && s[1] == '=') return "!=";
        if(s[0] == '>' && s[1] == '=') return ">=";
        if(s[0] == '<' && s[1] == '=') return "<=";
        if(s[0] == '<' && s[1] == '>') return "<>";

        if(s[0] == '+' && s[1] == '=') return "+=";
        if(s[0] == '-' && s[1] == '=') return "-=";
        if(s[0] == '/' && s[1] == '=') return "/=";
        if(s[0] == '*' && s[1] == '=') return "*=";

    }
    if(s.length()>=1) {
        if(s[0] == '=') return "=";
        if(s[0] == '+') return "+";
        if(s[0] == '-') return "-";
        if(s[0] == '/') return "/";
        if(s[0] == '*') return "*";
        if(s[0] == '<') return "<";
        if(s[0] == '>') return ">";
    }
    
    std::string r;
    int i, k;
    i = 0;
    k = 0;
    while(i < s.length()) {
        if(s[i] == '\"') {
            if(k == 0) {
                k = 1;
            } else {
                k = 0;
            }
        }
        if(k == 0) {
            if(s[i] == ' ' || s[i] == '=' || s[i] == '+' || s[i] == '.' || s[i] == ',' || s[i] == '/' || s[i] == '*') {
                return r;
            }
        }
        r += s[i];
        i++;
    }
    return s;
}

std::string del_word(std::string s) {
    std::string w, ss;
    w = get_word(s);
    if(s.length() == w.length()) return "";
    
    ss = s.substr(w.length(), s.length());

    if(ss.length() > 1 && ss[0] == ' ') ss = ss.substr(1, ss.length());
    
    return ss;
}
        
std::string rl_trim(std::string s) {
    while(s.length() > 0 && s[0] == ' ') {
        s = s.substr(1, s.length());
    }
    
    while(s.length() > 0 && s[s.length()-1] == ' ') {
        s = s.substr(0, s.length()-1);
    };
    
    return s;
}

std::string remove_double_space(std::string s) {

    std::string s1, s2;
    
    int i = 0;
    while( i+1 < s.length()) {
        while(i+1 < s.length() && (s[i] == ' ' || s[i] == 9) && (s[i+1] == ' ' || s[i+1] == 9)) {
            s1 = s.substr(0, i);
            s2 = s.substr(i+1, s.length());
            s = s1 + s2;
        }
        i++;
    }
    
    return s;
}

int my_atoi(const char *v) {
    int r, i, z;
    z = 0;
    r = 0;
    i = 0;
    if(v == nullptr) return 0;
    if(v[i] == '-') { z = 1; i++; };
    while(v[i] != 0) {
        if(v[i] == '.' || v[i] == ',') break;
        if(v[i] >= '0' && v[i] <= '9') {
            r *= 10;
            r += (v[i] - '0');
        }
        i++;
    }
    if(z == 1) r *= -1;
    
    return r;
}

bool is_local_ip(unsigned int ip) {
    // 192 = c0
    // 168 = a8
    // 10 = 0a

    if((ip & 0xff000000) == 0xc0000000 &&
       (ip & 0x00ff0000) == 0x00a80000 ) 
    {
        return true;
    };
    return false;
}

void fatal_error(const char *v) {
    printf("fatal_error [%s]\n", v);
}

char wchar_to_ascii_(wchar_t p) {

	if (p == 0x401) return (char)168;
	if (p == 0x451) return (char)184;
	if (p >= 0x410 && p <= 0x44f) {
		return (0xC0 + (char)((int)p - (int)0x410));
	};
	if (p >= 0x20 && p <= 0x7f)
	{
		return (char)p;
	};
	if (p == 8226) return (char)150;
	return (char)'?';
}

void wchar_to_char(wchar_t *src, char *dst, int dst_len) {
    if(dst == nullptr || dst_len <= 0 || dst_len > 2000) return;
    dst[0] = 0;
    if(src == nullptr) return;
    int i;
    i = 0;
    while(i < dst_len-1 && src[i] != 0) {
        dst[i] = wchar_to_ascii_(src[i]);
        i++;
    }
    dst[i] = 0;
}

#ifdef __linux__
int my_send(int sock, char *c, int len) {
    return send(sock, c, len, 0);
}

int my_recv(int sock, char *c, int len) {
    return recv(sock, c, len, 0);
}
#endif

#ifdef _WIN32
int my_send(SOCKET sock, char *c, int len) {
    return send(sock, c, len, 0);
}

int my_recv(SOCKET sock, char *c, int len) {
    return recv(sock, c, len, 0);
}
#endif



    
bool GETHTTP::connect(const char *ip, uint16_t port) {
    
    if(execute_is_run == false) {
        execute_is_run = true;
        execute_thread = new std::thread(&GETHTTP::execute, this);
    };
    
    sock = socket(AF_INET, SOCK_STREAM, 0);

    s_address.sin_family = AF_INET;
    s_address.sin_addr.s_addr = inet_addr(ip);
    s_address.sin_port = htons(port);

    int r;

    current_comand = Connect;
    current_comand_start = GetTickCount();
    r = ::connect(sock, (struct sockaddr *)&s_address, sizeof(s_address));
    current_comand = Empty;
    if(r < 0) {
        return false;
    }

    int flags = fcntl(sock , F_GETFL, 0);
    fcntl(sock , F_SETFL, flags|O_NONBLOCK);
    int imode = 1;
    ioctl(sock , FIONBIO, &imode);
    return true;
}

int  GETHTTP::send(uint8_t *buf, uint32_t buf_len) {
    int r;
    
    current_comand = Send;
    current_comand_start = GetTickCount();
    r = ::send(sock, (char *)buf, buf_len, 0);
    current_comand = Empty;
    return r;
}

bool check_http_end(uint8_t *buf, uint32_t buf_len, uint8_t *&body, uint32_t &body_len) {
    body = nullptr;
    body_len = 0;
    
    uint8_t name[1000];uint8_t value[1000];
    memsetzero(name, 1000);
    memsetzero(value, 1000);
    int i, j, mode, content_length = -1, start_body = -1;
    i = 0;
    j = 0;
    mode = 0;
    while(i < buf_len) {
        if(buf[i] == '\n') {
            
            
            if(i > 4) {
                if(i+1 < buf_len && buf[i-3] == '\r' && buf[i-2] == '\n' && buf[i-1] == '\r' && buf[i-0] == '\n') {
                    start_body = i+1;
                    if(buf_len - start_body == content_length) {
                        body = &(buf[start_body]);
                        body_len = content_length;
                        //printf("start_body = %d\n", start_body);
                        return true;
                    }
                }
            }

            if(my_strcmp_lower((const char *)name, "content-length")) {
                content_length = my_atoi((const char *)value);
                //printf("++++\n");
            } else {
                //printf("----\n");
            }
            
            memsetzero(name, 1000);
            memsetzero(value, 1000);
            j = 0;
            mode = 0;
        } else {
            if(buf[i] != '\r') {
                
                    if(mode == 0 && buf[i] == ':') {
                        mode++;
                        j = 0;
                    } else {
                        if(mode == 0) { name[j]  = buf[i]; name[j+1]  = 0; };
                        if(mode == 1) { value[j] = buf[i]; value[j+1] = 0; };
                        if(j < 1000-5) j++;
                    }
                
            } 
            
        }
        i++;
    }
    return false;
}

int  GETHTTP::recv(uint8_t *buf, uint32_t buf_len, uint8_t *&body, uint32_t &body_len) {
    int r = 0, rr = 0;
    
    bool read_ok = false;
    while(!read_ok) {
        if(rr >= buf_len) {
            printf("rr >= buf_len\n");
            return -1;
        }
        
        current_comand = Send;
        current_comand_start = GetTickCount();
        r = my_recv(sock, (char *)buf+rr, buf_len-rr);
        current_comand = Empty;
        if(r == 0) {
            //printf("r == 0\n");
            usleep(500);
        } else if(r < 0) {
            //printf("r < 0 %d\n", r);
            usleep(500);
        } else if(r > 0){
            rr += r;
            printf("#");
            fflush(stdout);
            if(check_http_end(buf, rr, body, body_len)) {
                printf("\nrecv %d ok - http detected\n", rr);
                return rr;
            } else {
                //printf("check_http_end fail\n");
            }
            
        }
    }
    printf("\nrecv %d ok\n", rr);
    return 0;
}

void GETHTTP::disconnect() {
    need_stop = true;
    while(execute_is_run) {
        usleep(1);
    }
}

GETHTTP::GETHTTP() {
    
}

GETHTTP::~GETHTTP() {
    
}



static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(BYTE c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(BYTE const* buf, unsigned int bufLen) {
  std::string ret;
  int i = 0;
  int j = 0;
  BYTE char_array_3[3];
  BYTE char_array_4[4];

  while (bufLen--) {
    char_array_3[i++] = *(buf++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';
  }

  return ret;
}

std::vector<BYTE> base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  BYTE char_array_4[4], char_array_3[3];
  std::vector<BYTE> ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
          ret.push_back(char_array_3[i]);
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
  }

  return ret;
}

void GETHTTP::execute() {
    
    while(need_stop == false) {

        if(current_comand == Connect) {
            if(current_comand_start + 5000 < GetTickCount()) {
                printf("connect timeout\n");
                shutdown(sock,SHUT_RDWR);
                close(sock);

                current_comand_start = GetTickCount();
            }
        }

        if(current_comand == Send) {
            if(current_comand_start + 5000 < GetTickCount()) {
                printf("send timeout\n");
                shutdown(sock,SHUT_RDWR);
                close(sock);

                current_comand_start = GetTickCount();
            }
        }

        if(current_comand == Recv) {
            if(current_comand_start + 5000 < GetTickCount()) {
                printf("recv timeout\n");
                shutdown(sock,SHUT_RDWR);
                close(sock);

                current_comand_start = GetTickCount();
            }
        }
        
        usleep(10);
    }
    execute_is_run = false;
}


OPENVPN::OPENVPN() {
    
}

OPENVPN::~OPENVPN() {
    
}

void OPENVPN::close() {
    need_stop = true;
    while(execute_is_run == true) {
        usleep(1);
    }
}

void OPENVPN::init() {
    if(execute_is_run == false) {
        execute_is_run = true;
        execute_thread = new std::thread(&OPENVPN::execute, this);
    };    
}

void OPENVPN::execute() {
    
    while(need_stop == false) {

        if(needstatus == NEEDRUN) {
            needstatus = EMPTY;
            currentstatus = STARTS;
            
            pid_t child = fork();
            if(child < 0) {
                printf("OPENVPN::execute() error child = fork()\n");
                currentstatus = RUNERROR;
            } else if (child == 0) {
                printf("im is child++\n");
                execl("/usr/sbin", "openvpn", "/etc/openvpn/client/client.conf", (char *) 0);
                usleep(1000);
                printf("im is child exit(0)\n"); 
                exit(0);
            } else {
                printf("im is owner\n");
                currentstatus = RUNOK;
            }
            
            currentstatus = RUNERROR;            
        }
        
        usleep(10);
    }
    execute_is_run = false;
}


bool OPENVPN::run() {
    needstatus = NEEDRUN;
    while(currentstatus != RUNOK) {
        if(currentstatus == RUNERROR) {
            return false;
        }
        usleep(100);
    }
    return true;
}
   
bool OPENVPN::stop() {
    needstatus = NEEDSTOP;
    while(currentstatus != STOPOK) {
        if(currentstatus == STOPERROR) {
            return false;
        }
        usleep(100);
    }
    return true;
}