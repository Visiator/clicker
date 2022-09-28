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

#include <stdio.h>
#include <string.h>

class FRAME;

char *ipv4_to_char(unsigned int ip, char *buf);
unsigned int char_to_ipv4(char *buf);

void usleep(int milliseconds);
void set_GLOBAL_STOP(const wchar_t *str);
void wtf(const char *info, int frame_no, unsigned char *buf, int buf_size);
void wtf(const char *str);
void memsetzero(unsigned char *destination, int n);
unsigned long GetTickCount();
bool DirectoryExists( const char* pzPath );
unsigned short get_i16(unsigned char v1, unsigned char v2);
unsigned int get_i32(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4);
unsigned int get_i24(unsigned char v1, unsigned char v2, unsigned char v3);
char *decode_to_char_tcp_flag(unsigned char f, char *c);
void detect_ip(FRAME *frame);
int get_ui8(unsigned char *item, int idx, unsigned char *v, int index);
int get_ui16(unsigned char *item, int idx, unsigned short *v, int index);
int get_ui16_R(unsigned char *item, int idx, unsigned short *v, int index);
void save_textura_to_BMP_file_(FILE *f, unsigned int *bitmap, int w, int h);
bool is_ip(unsigned int ip, unsigned char i1, unsigned char i2, unsigned char i3, unsigned char i4);

#endif /* TOOLS_H */

