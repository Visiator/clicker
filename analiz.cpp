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

#define data8(p, index)  p[index]
#define data16(p, index) *(uint16_t*)(&(p)[index])
#define data32(p, index) *(uint32_t*)(&(p)[index])
#define data64(p, index) *(uint64_t*)(&(p)[index])


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
                //global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_src_ip);

                printf("ddd\n");
                };
            };
        };
    }
    
    if(frame->ipv4_dst_port == 443) {
        if(frame->session_packet_count == 0) {
            if(frame->payload_size > 4) {
                if(frame->payload[0] == 0x01 && frame->payload[1] == 0x00 && frame->payload[2] == 0x00 && frame->payload[3] == 0x00) {
                    //global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
                }
            }
            if(frame->payload_size == 86 && frame->frame_size == 128) {
                //global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
            }        
        }
    }
    
    if(frame->ipv4_dst_port == 4500) {
            
            //global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
        
    } else {
        //global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
    }
    
}

void pprint8(unsigned char *p) {
    if(p == nullptr) {
        return;
    }
    printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", (unsigned int)p[0], (unsigned int)p[1], (unsigned int)p[2], (unsigned int)p[3], (unsigned int)p[4], (unsigned int)p[5], (unsigned int)p[6], (unsigned int)p[7]);
}

void decode_sni_20(char *v) 
{
    unsigned int a, *q, i1, i2, i3;
    i1 = 0;
    i2 = 0;
    i3 = 0;
    a = strlen(v);
    if(a == 51) {
        q = (unsigned int *)v;
        if(*q == 0x65737361) { // "asse"
            printf("api.\n");
            q = (unsigned int *)&(v[a-4]);
            if(*q == 0x6d6f632e) // 
            {
                printf(".com\n");
                for(int i=7;i<47;i++)
                {
                    if(v[i] >= '0' && v[i] <= '9') {
                        i1++;
                    } else if(v[i] >= 'a' && v[i] <= 'f') {
                        i2++;
                    } else {
                        i3++;
                    }
                    v[i] = '+';
                }
                if(i1 >= 7 && i2 >= 5 && i3 == 0 && i1 + i2 == 40) {
                    printf("+++\n");
                }
            }
        }
    }
}

bool check_ja3_v2(unsigned char *payload, size_t payload_size) {
    unsigned int *v;
    v = (unsigned int *)payload;
    if((*v & 0x00ffffff) == 0x00010316) {
        if(payload[5] == 0x01) {
            unsigned int l;
            unsigned short ss;
            ss = data16(payload,7);
            l = rte_cpu_to_be_16(data16(payload,7));
            if(l + 9 == payload_size) {
                unsigned short *s;
                s = (unsigned short *)(payload+9);
                if(*s == 0x0303) {
                    unsigned long *q;
                    q = (unsigned long *)(payload+76);
                    if(*q == 0x0313011302135c00) {
                        q++;
                        if(*q == 0xa8cca9cc30c02cc0) {
                            q++;
                            if(*q == 0x61c05dc0adc0afc0) {
                                q++;
                                if(*q == 0xacc0aec02fc02bc0) {
                                    q++;
                                    if(*q == 0x28c024c060c05cc0) {
                                        q++;
                                        if(*q == 0x27c023c077c073c0) {
                                            q++;    
                                            if(*q == 0x14c00ac076c072c0) {
                                                q++;
                                                if(*q == 0xa1c09d0013c009c0) {
                                                    q++;
                                                    if(*q == 0xa0c09c0051c09dc0) {
                                                        q++;
                                                        if(*q == 0xc0003d0050c09cc0) {
                                                            q++;
                                                            if(*q == 0x84003500ba003c00) {
                                                                return true;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    };
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool test_sni_eq_ip(unsigned char *s, unsigned short sz, unsigned int ip) {
    unsigned short sz1, type1, sni_len, i, k, v;
    unsigned int ii = 0;
    sz1 = rte_cpu_to_be_16(data16(s, 0));
    type1 = s[2];
    if(type1 == 0) { // host_name
        sni_len = rte_cpu_to_be_16(data16(s, 3));
        if(sni_len + 5 == sz) {
            printf("+5\n");
            s += 5;
            i = 0;
            v = 0;
            while(i < sni_len) {
                if(*s == '.') {
                    ii = ii << 8;
                    ii = ii | v;
                    k++;
                    v = 0;
                } else {
                    if(*s >= '0' && *s <= '9') {
                        v *= 10;
                        v += (*s - '0');
                    } else {
                        return false;
                    }
                }
                s++;
                i++;
            }
            ii = ii << 8;
            ii = ii | v;
            if(ii == ip) {
                return true;
            }
        }
    }
    
    return false;
}

bool check_ja3_v3(unsigned char *payload, size_t payload_size, unsigned int ip) {
    
    unsigned int *v;
    v = (unsigned int *)payload;
    if((*v & 0x00ffffff) == 0x00010316) {
        if(payload[5] == 0x01) {
            unsigned int l;
            l = rte_cpu_to_be_16(data16(payload,7));
            if(l + 9 == payload_size) {
                unsigned short *s;
                s = (unsigned short *)(payload+9);
                if(*s == 0x0303) {
                    if(payload[43] == 32) { // session_id_length
                        if(rte_cpu_to_be_16(data16(payload,43 + 1 + 32 )) == 74) { // cipher_suites_length
                            unsigned long *q;
                            q = (unsigned long *)(payload+78);
                                     
                            if( *q    == 0x2fc0a9cc2cc02bc0 && // cipher_suites
                               *(q+1) == 0x9f009e00a8cc30c0 &&
                               *(q+2) == 0x9ec0adc0acc0aacc &&
                               *(q+3) == 0x27c024c023c09fc0 &&
                               *(q+4) == 0x09c06b00670028c0 &&
                               *(q+5) == 0x330014c013c00ac0 &&
                               *(q+6) == 0x9cc09d009c003900 &&
                               *(q+7) == 0x35003c003d009dc0 &&
                               *(q+8) == 0x0313021301132f00) 
                            {
                                bool flag_supported_groups = false;
                                unsigned char *server_name_block = nullptr;
                                unsigned short server_name_block_sz;
                                unsigned short comp_methods_length = rte_cpu_to_be_16(data16(payload,78 + 74 ));
                                unsigned short extensions_length = rte_cpu_to_be_16(data16(payload,78 + 74 + 2 ));
                                if(extensions_length + 156 == payload_size) {
                                    unsigned short i, type, sz;
                                    i = 0;
                                    while(extensions_length + i < payload_size) { // TLS extensions
                                        type = rte_cpu_to_be_16(data16(payload,78 + 74 + 2 + 2 + i)); 
                                        sz = rte_cpu_to_be_16(data16(payload,78 + 74 + 2 + 4 + i));
                                        if(type == 0) { // server name
                                            if(i + 4 + sz <  payload_size) {
                                                server_name_block = (unsigned char *)(payload + 78 + 74 + 2 + 2 + i + 4);
                                                server_name_block_sz = sz;
                                            }
                                        }
                                        if(type == 10) { // supported groups
                                            if(sz == 20) {
                                                unsigned long *e;
                                                e = (unsigned long *)(payload + 78 + 74 + 2 + 4 + i + 4);
                                                if(*e == 0x010117001e001d00) {
                                                    e++;
                                                    if(*e == 0x401030118000201) {
                                                        flag_supported_groups = true;
                                                        if(server_name_block != nullptr) {
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        i += 4 + sz;
                                    }
                                    if(flag_supported_groups) {
                                        if(test_sni_eq_ip(server_name_block, server_name_block_sz, ip)) {
                                            return true;
                                        }
                            
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}


void custom_analiz(SESSION *s, FRAME *frame) {


    
    /*if(frame->payload_size == 54) {
        if(frame->ipv4_dst_port == 25000 ||
           frame->ipv4_dst_port == 5566 ||
           frame->ipv4_dst_port == 80 ||
           frame->ipv4_dst_port == 11211 ||
           frame->ipv4_dst_port == 11194 ||
           frame->ipv4_dst_port == 443 ||
           frame->ipv4_dst_port == 1194 
                ) {
            if(frame->payload[0] == 0x38) {
                global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
            }
        }
    }*/
    
    /*if(frame->payload_size == 54) {
        if(frame->ipv4_dst_port == 5566 || 
           frame->ipv4_dst_port == 11211
          ) 
        {
            if(frame->payload[0] == 0x38) {
                
                    global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
                
            }
        }
    }*/
    
    /*if(frame->payload_size > 0) {        
        //check_ja3_v2(frame->payload, frame->payload_size);
        //check_ja3_v3(frame->payload, frame->payload_size, frame->ipv4_dst_ip);
        if(frame->SNI.size() > 0) {
            unsigned char *q;
            unsigned int *x;
            
            q = (unsigned char *)frame->SNI[0].c_str();
            x = (unsigned int *)q;
            printf("%x %x\n", x[0], x[1]);
        }
    };
    printf("111111\n");*/
    /*
    if(frame->ipv4_dst_port == 3306 && frame->payload_size > 0 && s->packet_count == 4) {
        
        //printf("qqqqq\n");
        if(    frame->payload[0] == 0x00
            && frame->payload[frame->payload_size-4] == 0
            && frame->payload[frame->payload_size-3] == 0
            && frame->payload[frame->payload_size-2] == 0
            && frame->payload[frame->payload_size-1] == 0
            
            ) 
        {
            
            global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
        }
    }

            
    
    if(frame->ipv4_dst_port == 54783 && frame->payload_size > 0 && s->packet_count <=1 && frame->payload[0] == 0x38)
    {
        if(    frame->payload[frame->payload_size-4] == 0
            && frame->payload[frame->payload_size-3] == 0
            && frame->payload[frame->payload_size-2] == 0
            && frame->payload[frame->payload_size-1] == 0
            
            ) 
        {
            //(udp.payload[0:4] == 00:00:00:00 && udp.payload[21:1] == 0x20 && udp.payload[-1:1] == 0x5e ) && (udp.dstport in {4500, 500})
            global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
        }
    }
    if(frame->ipv4_dst_port == 4500 && frame->payload_size > 0 && s->packet_count <=1) {
         if(    frame->payload[0] == 0
            && frame->payload[1] == 0
            && frame->payload[2] == 0
            && frame->payload[3] == 0
            && frame->payload[21] == 0x20
            
            ) 
        {
            // (udp.payload[0:4] == 00:00:00:00 && udp.payload[21:1] == 0x20) && (udp.dstport == 4500)
            global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
        }
    }
    */
    if(frame->ipv4_dst_port == 443 && frame->payload_size > 0 && s->packet_count == 4) {
        
        if(frame->SNI.size() > 0 && frame->SNI[0].length() > 15) {
            std::string s;
            s = frame->SNI[0].substr(14, 255);
            if(s == ".cloudfront.net") {
                //printf("+\n");
                global.add_ip_to_queue_to_send_mikrotik(frame->ipv4_dst_ip);
            }
        }
        
        unsigned int *v, l;
        unsigned short *s;
        unsigned long *q;
        v = (unsigned int *)frame->payload;
        if((*v & 0x00ffffff) == 0x00010316) {
            if(frame->payload[5] == 0x01) {
                l = get_i16(frame->payload[8], frame->payload[7]);
                if(l == frame->payload_size - 9) {
                    s = (unsigned short *)(frame->payload+9);
                    if(*s == 0x0303) {
                        unsigned short cipher_suites_length = rte_cpu_to_be_16(data16(frame->payload,76 ));

                        if(cipher_suites_length == 32) {
                            q = (unsigned long *)(frame->payload+78);
                            if(q[0] == 0x0313021301136a6a &&
                               q[1] == 0x30c02cc02fc02bc0 &&
                               q[2] == 0x14c013c0a8cca9cc && 
                               q[3] == 0x35002f009d009c00) { //
                                            printf("+++\n");
                                uint8_t comp_methods_length = frame->payload[78 + cipher_suites_length];
                                if(comp_methods_length == 1) {
                                    unsigned short extensions_length = rte_cpu_to_be_16(data16(frame->payload,78 + cipher_suites_length + 2 ));
                                    if(78 + cipher_suites_length + extensions_length + 4 == frame->payload_size) {
                                        printf("1\n");
                                        
                                        unsigned short f, list_length;
                                        unsigned short *s;
                                        
                                        bool flag_supported_groups = false, flag_supported_versions = false;
                                        unsigned short i, max_i, type, sz;
                                        i = 78 + cipher_suites_length + 4;
                                        
                                        while(i < frame->payload_size) { // TLS extensions
                                            type = rte_cpu_to_be_16(data16(frame->payload,i)); 
                                            sz = rte_cpu_to_be_16(data16(frame->payload,i+2));
                                            if(i + 4 + sz >= frame->payload_size) {
                                                break;
                                            }
                                            if(type == 10) { // supported groups
                                                
                                                f = 0;
                                                list_length = rte_cpu_to_be_16(data16(frame->payload,i+4));
                                                s = (unsigned short *)(frame->payload+i+6);
                                                if(list_length == sz-2) {
                                                    while(list_length > 0) {
                                                        if(*s == 0x1d00) {
                                                            f |= 0x01;
                                                        } else if(*s == 0x1700) {
                                                            f |= 0x02;
                                                        } else if(*s == 0x1800) {
                                                            f |= 0x04;
                                                        } 
                                                        s++;
                                                        list_length -= 2;
                                                        if(list_length == 1) break;
                                                    }
                                                    if(f == 7) {
                                                        flag_supported_groups = true;
                                                    }
                                                }

                                            } else if(type == 43) { // supported_version
                                                f = 0;
                                                uint8_t sv_length = frame->payload[i+4];
                                                if(sv_length+1 == sz) {
                                                    f = 0;
                                                    s = (unsigned short *)(frame->payload+i+5);
                                                    while(sv_length > 0) {
                                                        if(*s == 0x0403) {
                                                            f |= 1;
                                                        } else if (*s == 0x0303) {
                                                            f |= 2;
                                                        }
                                                        s++;
                                                        sv_length -= 2;
                                                        if(sv_length == 1) break;
                                                    }
                                                    if(f == 3) {
                                                        flag_supported_versions = true;
                                                    };
                                                }
                                            }
                                            
                                            
                                            i += 4 + sz;
                                        }
                                        if(flag_supported_groups && flag_supported_versions) {
                                            printf("flag_supported_groups && flag_supported_versions\n");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    

    

}