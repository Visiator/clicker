/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   tools.h
 * Author: root
 *
 * Created on August 30, 2022, 6:01 PM
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <stdio.h>
#include <string.h>

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

class FRAME;

#ifdef __linux__
unsigned long GetTickCount();
#endif

char *ipv4_to_char(unsigned int ip, char *buf);
unsigned int char_to_ipv4(char *buf);

bool my_strcmp(const char *s1, int s1_max_size, const char *s2);
void usleep(int milliseconds);
void set_GLOBAL_STOP(const wchar_t *str);
void wtf(const char *info, int frame_no, unsigned char *buf, int buf_size);
void wtf(const char *str);
void memsetzero(unsigned char *destination, int n);

bool DirectoryExists( const char* pzPath );
unsigned short get_i16(unsigned char v1, unsigned char v2);
unsigned short rte_cpu_to_be_16(unsigned short v);
unsigned int get_i32(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4);
unsigned int get_i24(unsigned char v1, unsigned char v2, unsigned char v3);
char *decode_to_char_tcp_flag(unsigned char f, char *c);
void detect_ip(FRAME *frame);
int get_ui8(unsigned char *item, int idx, unsigned char *v, int index);
int get_ui16(unsigned char *item, int idx, unsigned short *v, int index);
int get_ui16_R(unsigned char *item, int idx, unsigned short *v, int index);
void save_textura_info(FILE *f, int x, int y, char *nic);
void save_textura_to_BMP_file_(FILE *f, unsigned int *bitmap, int w, int h);
void load_from_BMP_buffer(unsigned char *buf, unsigned int buf_size, std::vector<unsigned int> &bitmap, unsigned int &w, unsigned int &h);
bool is_ip(unsigned int ip, unsigned char i1, unsigned char i2, unsigned char i3, unsigned char i4);
std::string get_word(std::string s);
std::string del_word(std::string s);
std::string rl_trim(std::string s);
std::string remove_double_space(std::string s);

int my_atoi(const char *v);

bool is_local_ip(unsigned int ip);

void fatal_error(const char *v);

void wchar_to_char(wchar_t *src, char *dst, int dst_len);

#ifdef __linux__
int my_send(int sock, char *c, int len);
int my_recv(int sock, char *c, int len);
#endif

#ifdef _WIN32
int my_send(SOCKET sock, char *c, int len);
int my_recv(SOCKET sock, char *c, int len);
#endif


#endif /* TOOLS_H */

