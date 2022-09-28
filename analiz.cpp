/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "sessions.h"
#include "mikrotik.h"
#include "global.h"
#include "tools.h"

extern GLOBAL global;
extern MIKROTIK mikrotik;

void analiz_to_block(SESSION* session) {

}

bool is_local_ip(unsigned int ip) {
    // 192 = c0
    // 168 = a8
    // 10 = 0a

    if((ip & 0xff000000) == 0xc0000000 &&
       (ip & 0x00ff0000) == 0x00a80000 ) 
    {
        return false;
    };
    return true;
}

void custom_analiz_udp(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    
    if(is_local_ip(frame->ipv4_dst_ip)) return;
    
    if(frame->ipv4_src_port == 443) {
        if(frame->session_packet_count == 0) {
            if(frame->payload_size > 4) {
                if(frame->payload[0] == 0x01) {
                global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_src_ip);

                printf("ddd\n");
                };
            };
        };
    }
    
    if(frame->ipv4_dst_port == 443) {
        if(frame->session_packet_count == 0) {
            if(frame->payload_size > 4) {
                if(frame->payload[0] == 0x01 && frame->payload[1] == 0x00 && frame->payload[2] == 0x00 && frame->payload[3] == 0x00) {
                    global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
                }
            }
            if(frame->payload_size == 86 && frame->frame_size == 128) {
                global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
            }        
        }
    }
    
    if(frame->ipv4_dst_port == 4500) {
            
            global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
        
    } else {
        //global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
    }
    
    // 0x00:00:65:00:00:01:00:00:66:00:00:20:02:00:00:00
    
    /*
    if(buf_size >= 16) {
        if(buf[0] == 0 &&
           buf[1] == 0 &&
           buf[2] == 0x65 &&
           buf[3] == 0 &&
           buf[4] == 0 &&
           buf[5] == 0x01 &&
           buf[6] == 0 &&
           buf[7] == 0 &&
           buf[8] == 0x66 &&
           buf[9] == 0 &&
           buf[10] == 0 &&
           buf[11] == 0x20 &&
           buf[12] == 0x02 &&
           buf[13] == 0x00 &&
           buf[14] == 0x00 &&
           buf[15] == 0x00                 
          )
        {
            printf("1\n");
        }
    }
    */
    
    /*char username[10];
    int i, padding;
    i = 0;
    unsigned long long *tie;
    unsigned int mg1, ms_app_id;
    unsigned short record_type, record_length, block_type, block_length;
    record_type = get_i16(buf[i+1], buf[i]); i+=2;
    record_length  = get_i16(buf[i+1], buf[i]); i+=2;
    if(record_length + 20 == buf_size) {
        mg1  = get_i32(buf[i+3], buf[i+2], buf[i+1], buf[i]); i+=4;
        if(mg1 == 0x2112a442) {
            i += 12;
            while(i+4 < buf_size) {
                block_type = get_i16(buf[i+1], buf[i]); i+=2;
                block_length  = get_i16(buf[i+1], buf[i]); i+=2;
                if(i + block_length <= buf_size) {
                    if(block_type == 0x0006) { // USERNAME
                        if(block_length == 9) {
                            for(unsigned int j=0;j<9;j++) {
                                username[j] = buf[i+j];
                            }
                        }
                    }
                    if(block_type == 0x802a) { // ICE-CONTROLLONG
                        if(block_length == 8) {
                            tie = (unsigned long long *)(buf + i);
                            if(*tie == 0x90394a3c01000000) { // 0x000000013c4a3990
                                printf("tie ok\n");
                            }
                        }
                            
                    }
                    if(block_type == 0x8037) { // MS-APP-AD
                        if(block_length == 4) {
                            ms_app_id = get_i32(buf[i+3], buf[i+2], buf[i+1], buf[i]);
                            if(ms_app_id == 0x00000003) {
                                printf("ms_app_id ok");
                            }
                        }
                    }
                } 
                padding = block_length % 4;
                if(padding > 0) {
                    i += block_length + (4-padding);
                } else {
                    i += block_length;
                };
            }
        }
    }*/
    
    
    /*
    unsigned char c;
    c = buf[0] & 0x3f;
    if(c != 0) {
        return;    
    }
    int i,j,k;
    
    unsigned short msg_type, msg_len, atr_type, atr_len;
    char atr_val[500];
    unsigned int mag;
    
    msg_type = get_i16(buf[1], buf[0]) & 0x3f;    
    msg_len = get_i16(buf[3], buf[2]);
    
    mag = get_i32(buf[7], buf[6], buf[5], buf[4]);
    
    j = msg_len;
    i = 8+12;
    while(j > 0) {
        atr_type = get_i16(buf[i+1], buf[i]); i+= 2;
        atr_len = get_i16(buf[i+1], buf[i]); i+=2;
        if(atr_len > 100 || atr_len > j) {
            return;
        }
        for(k=0;k<atr_len;k++) atr_val[k] = buf[i++];
        atr_val[k] = 0;
        
        j -= atr_len;
    }
    */
}

void pprint8(unsigned char *p) {
    if(p == nullptr) {
        return;
    }
    printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", (unsigned int)p[0], (unsigned int)p[1], (unsigned int)p[2], (unsigned int)p[3], (unsigned int)p[4], (unsigned int)p[5], (unsigned int)p[6], (unsigned int)p[7]);
}

void custom_analiz(SESSION *s, FRAME *frame) {
    
    if(frame->ipv4_dst_port == 443) {
        if(frame->payload[0] == 0x16 &&
                frame->payload[1] == 0x03 &&
                frame->payload[2] == 0x03 
          ) 
        {
            unsigned short sz1, ver1, sz3, sz4;
            unsigned int sz2, ofset;
            
            sz1 = get_i16(frame->payload[4], frame->payload[3]);
            if(sz1 + 5 == frame->payload_size) {
                if(frame->payload[5] == 0x01) {
                    sz2 = get_i24(frame->payload[8], frame->payload[7], frame->payload[6]);        
                    if(sz2 + 4 == sz1) {
                        ver1 = get_i16(frame->payload[10], frame->payload[9]);
                        if(ver1 == 0x0303) {
                            if(frame->payload[43] == 0 ) {
                                sz3 = get_i16(frame->payload[45], frame->payload[44]);
                                if(sz3 == 50) {
                                    unsigned long *d;
                                    d = (unsigned long *)(frame->payload + 46);
                                    if(*d == 0x2cc0031302130113) {
                                        d++;
                                        if(*d == 0xa9cc2fc030c02bc0) {
                                            d++;
                                            if(*d == 0x23c027c0aacca8cc) {
                                                d++;
                                                if(*d == 0x09c00ac024c028c0) {
                                                    d++;
                                                    if(*d == 0x13cc14cc13c014c0) {
                                                        d++;
                                                        if(*d == 0xadccacccabcc15cc) {
                                                            ofset = 46 + 50;
                                                            unsigned short type, sz, *v1 = (unsigned short *)(frame->payload + ofset); ofset+=2;
                                                            if(*v1 == 0x0001) {
                                                                sz4 = get_i16(frame->payload[ofset+1], frame->payload[ofset]); ofset += 2;
                                                                if(sz4 + ofset == frame->payload_size) {
                                                                    
                                                                    while(sz4 > 0) {
                                                                        type = get_i16(frame->payload[ofset+1], frame->payload[ofset]); ofset += 2;
                                                                        sz   = get_i16(frame->payload[ofset+1], frame->payload[ofset]); ofset += 2;
                                                                        if(type == 0x002b) { // tls.handshake.extension.type   supported_version
                                                                            if(sz == 3) {
                                                                                if(frame->payload[ofset] == 0x02 &&
                                                                                   frame->payload[ofset+1] == 0x03 &&
                                                                                   frame->payload[ofset+2] == 0x04 
                                                                                  )
                                                                                {
                                                                                    printf("supported version only TLS 1.3\n");
                                                                                }
                                                                            }
                                                                        }
                                                                        sz4 -= 4;                                                                        
                                                                        ofset += sz;
                                                                        sz4 -= sz;
                                                                    }
                                                                    
                                                                    printf("++++\n");
                                                                }                                                                
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }                                        
                                    }
                                    printf("160303\n"); 
                                    /*130113021303c02c 
                                      c02bc030c02fcca9
                                      cca8ccaac027c023
                                      c028c024c00ac009
                                      c014c013cc14cc13
                                      cc15ccabccacccad
                                      c037*/
                                }
                            }
                        }
                    }
                }
                
            }
            
        }
    }
    
    if(frame->ipv4_dst_port == 80) {
        if(frame->payload_size >= 38 && frame->payload_size < 255) {
            if(frame->payload[0] == 0x82) {
                unsigned char sz = frame->payload[1] & 0x3f;
                if(sz+6 == frame->payload_size) {
                    unsigned int *mask = (unsigned int *)&frame->payload[2];
                    unsigned int *src, dst;
                    src = (unsigned int *)(frame->payload+6);
                    dst = (*src ^ *mask);
                    if(dst == 0x00650000) { // 0x00006500
                        src++;
                        dst = (*src ^ *mask);
                        if(dst == 0x00000100) { // 0x00006500 00010000
                            src++;
                            dst = (*src ^ *mask);
                            if(dst == 0x20000066) { // 0x00006500 00010000 66000020
                                src++;
                                dst = (*src ^ *mask);
                                if(dst == 0x00000002) { // 0x00006500000100006600002002000000
                                    printf("+++\n");
                                }
                            }   
                        }
                    }
                    pprint8(frame->payload);
                    pprint8(frame->payload+8);
                }
            }
        }
    }
    
    return;
    
    if(s->packet_count == 0) {
        printf("0\n");
    }
    if(frame->ip_proto == 6) {
        if(frame->ipv4_dst_port == 443 || frame->ipv4_src_port == 443) {
            if(s->packet_with_payload_count == 1 ) {
                
                if(is_ip(frame->ipv4_dst_ip, 192,168,1,86) ||
                   is_ip(frame->ipv4_src_ip, 192,168,1,86) 
                  ) 
                {
                    pprint8(frame->payload);
                    printf("1\n");            
                }
            }
        }
    }


    if( is_ip(frame->ipv4_dst_ip, 192,168,1,175) ||
        is_ip(frame->ipv4_src_ip, 192,168,1,175) 
      ) 
    {
        if(frame->ip_proto == 17) {
            pprint8(frame->payload);
            printf("1\n");            
        }
        return;
    }
    if( is_ip(frame->ipv4_dst_ip, 192,168,1,5) ||
        is_ip(frame->ipv4_src_ip, 192,168,1,5) 
      ) 
    {
        if(frame->ip_proto == 17) {
            pprint8(frame->payload);
            printf("1\n");            
        }
        return;
    }
    
    if( is_ip(frame->ipv4_dst_ip, 192,168,1,195) ||
        is_ip(frame->ipv4_src_ip, 192,168,1,195) 
      ) 
    {
        if(frame->ip_proto == 17) {
            
            
            
            if( is_ip(frame->ipv4_src_ip, 192,168,1,195) ) {
                if(frame->ipv4_dst_port == 4500) {
                    printf("4500\n");
                    global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
                }
                
                if(frame->ipv4_dst_port == 443) {
                    if(s->packet_count == 1) {
                        if(  (frame->payload[0] >= 0x01 && frame->payload[0] <= 0x04)
                                && frame->payload[1] == 0x00 
                                && frame->payload[2] == 0x00 
                                && frame->payload[3] == 0x00
                           ) 
                        {
                            //pprint8(frame->payload);
                            //printf("1\n"); 
                            global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
                        }
                    }
                }                
            }
            
            if( is_ip(frame->ipv4_dst_ip, 192,168,1,195) ) {
                if(frame->ipv4_src_port == 443) {
                    if(        frame->payload[0] >= 0x01 && frame->payload[0] <= 0x04
                            && frame->payload[1] == 0x00 
                            && frame->payload[2] == 0x00 
                            && frame->payload[3] == 0x00) {
                        //pprint8(frame->payload);
                        //printf("1\n");
                        global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_src_ip);
                    }
                }
            }
        }
        if(frame->ip_proto == 6) {
            printf("1\n");
            if(frame->SNI.size() > 0) {
                for(auto& s : frame->SNI) {
                    printf("[%s]\n", s.c_str());
                }
            }
        }
        return;
    }
    
    char cc[100];
    
    
}