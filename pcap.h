/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   pcap.h
 * Author: smorodin
 *
 * Created on September 18, 2022, 12:44 PM
 */

#ifndef PCAP_H
#define PCAP_H

#include <string>
#include <vector>

#include "sessions.h"
#include "fragments_queue.h"

class PCAP_COUNTER {
public:
    unsigned long _decode_page_6 = 0, ethernet_ii_type_0800 = 0, ethernet_ii_type_86dd = 0, ethernet_ii_type_0027 = 0, ethernet_ii_type_88cc = 0, ethernet_ii_type_0074 = 0, ethernet_ii_type_0806 = 0, ethernet_ii_type_else = 0
    , _ipv4_tcp_egress = 0, _ipv4_udp_egress = 0
    , _ipv4_tcp_ingress = 0, _ipv4_udp_ingress = 0;
    void clear() {
        _decode_page_6 = 0;
        ethernet_ii_type_0800 = 0;
        ethernet_ii_type_86dd = 0;
        ethernet_ii_type_0027 = 0;
        ethernet_ii_type_88cc = 0;
        ethernet_ii_type_0074 = 0;
        ethernet_ii_type_0806 = 0;
        ethernet_ii_type_else = 0;
        _ipv4_tcp_egress = 0;
        _ipv4_tcp_ingress = 0;
        _ipv4_udp_egress = 0;
        _ipv4_udp_ingress = 0;        
    }
    void ipv4_tcp(DIRECTION d) {
        if(d == DIRECTION::egress) {
            _ipv4_tcp_egress++;
        } else { 
            _ipv4_tcp_ingress++; }
    }
    void ipv4_udp(DIRECTION d) {
        if(d == DIRECTION::egress) {
            _ipv4_udp_egress++;
        } else {
            _ipv4_udp_ingress++;
        };
    }
    void decode_page_6() {
        _decode_page_6++;
    }
    void ethernet_ii_type(unsigned short v) {
        if(v == 0x0800) {        
            ethernet_ii_type_0800++;
        } else if(v == 0x86dd) {    // ipv6
            ethernet_ii_type_86dd++;
        } else if(v == 0x0027) {    // stp
            ethernet_ii_type_0027++;
        } else if(v == 0x88cc) {    // lldp
            ethernet_ii_type_88cc++;
        } else if(v == 0x74) {    // cdp
            ethernet_ii_type_0074++;
        } else if(v == 0x0806) {    // ARP
            ethernet_ii_type_0806++;
        } else {
            ethernet_ii_type_else++;
        }
    }
};

class PCAP {
public:
    PCAP_COUNTER counter;
    
    SESSIONS sessions;
    FRAGMENTS_QUEUE queue;
    unsigned char *page_buff =  NULL;
    int page_buff_size = 0;
    int page_size = 0;
    
    
    void load_from_file(std::string file_name);
    
    bool read_ui32(FILE *f, unsigned int *hh);
    bool read_page(FILE *f, unsigned int sz);
    
    bool decode_page_0a0d0d0a();
    void decode_page_1();
    void decode_page_5();
    void decode_page_6(int frame_no);
    
    bool read_from_page_ui32(unsigned int *idx, unsigned int *val);
    bool read_from_page_ui64(unsigned int *idx, unsigned long long *val);
    
    void analiz(int frame_no, unsigned char *buf, int buf_size);
    void analiz_ipv4(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void analiz_ipv4_tcp(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void analiz_ipv4_udp(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void analiz_ipv4_tcp_payload(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void analiz_ipv4_udp_payload(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void analiz_tls_160301(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    bool it_is_160301(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    bool it_is_160303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    bool it_is_170303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void analiz_tls_160303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void read_dns_name(int lvl, int frame_no, unsigned char *buf, int buf_size, char *rr, int &i);
    void decode_tls_handshake_version_0301(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void decode_tls_handshake_version_0303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void decode_sertificates(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void decode_tls2_handshake_version_0303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);
    void decode_server_name(int frame_no, unsigned char *buf, int buf_size,char *nn);
    void decode_server_51(unsigned char *buf, int buf_size, char *nn, int *nn_len);
    void decode_sertificat(int frame_no, unsigned char *buf, int buf_size, CERT *cert);
    void decode_tls_cert(unsigned char *item, int idx, int cert_len, CERT *cert, std::string path);
    std::string decode_object(unsigned char v[], unsigned int len);    
    int decode_bit_string_tls_cert(unsigned char *item, int idx, int cert_len, CERT *cert, std::string path, unsigned char *result1, unsigned char *result2);
    
    PCAP();
};



#endif /* PCAP_H */

