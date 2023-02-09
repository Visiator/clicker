/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "pcap.h"
#include "frame.h"
#include "fragments_queue.h"

void custom_analiz_udp(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);

PCAP::PCAP() {
    
}

bool PCAP::decode_page_0a0d0d0a() {
    bool b;
    unsigned int p1;
    unsigned int i;
    i = 0;
    
    b = read_from_page_ui32(&i, &p1);
    if(p1 != 0x1a2b3c4d) {
        printf("corrupted! 3\n");
        return false;
    }
    
    return true;
}

void PCAP::decode_page_1() {
    
}

void PCAP::decode_page_5() {
    
}

void PCAP::decode_page_6(int frame_no) {
    counter.decode_page_6();
    bool b;
    unsigned int nomer_interface;
    unsigned long long timestamp;
    unsigned int packets_size;
    unsigned int raw_size;
    unsigned int i;
    i = 0;
    
    b = read_from_page_ui32(&i, &nomer_interface);
    b = read_from_page_ui64(&i, &timestamp);
    b = read_from_page_ui32(&i, &packets_size);
    b = read_from_page_ui32(&i, &raw_size);
    if(packets_size != raw_size) {
        printf("PCAP packets_size != raw_size \n");
    }
    if(frame_no == 211) {
        printf("211\n");
        
    }
    //frame.clean();
    analiz(frame_no, page_buff+20, packets_size);
    //sessions.add_to_session(&frame);   
    //sessions.save();
    
}

bool PCAP::read_page(FILE *f, unsigned int sz) {
    page_size = 0;
    if(page_buff_size < sz) {
        if(page_buff != NULL) delete[] page_buff;
        page_buff_size = sz * 2;
        page_buff = new unsigned char[page_buff_size];
    }
    
    int i;
    i = fread(page_buff, 1, sz, f);
    if(i == sz) {
        page_size = i;
        return true;
    };
    return false;
}

bool PCAP::read_ui32(FILE *f, unsigned int *hh) {
    unsigned char buf[4], *q;
    int i;
    *hh = 0;
    i = fread(buf, 1, 4, f);
    if(i != 4) {
        return false;
    };
    q = (unsigned char *)hh;
    q[0] = buf[0];
    q[1] = buf[1];
    q[2] = buf[2];
    q[3] = buf[3];
    return true;
}



void PCAP::load_from_file(std::string file_name) {
FILE *f;
    
    //w = fopen("wwww.pcap", "wb");
    
    
    f = fopen(file_name.c_str(), "rb");
    if(f == NULL) return;
    unsigned int hh, pp, ss;
    bool b;
    int frame_no = 1;
    do
    {
    
        b = read_ui32(f, &hh);
        if(b == false) {
            break;
        }
        b = read_ui32(f, &ss);

        b = read_page(f, ss-12);

        //write_ui32(w, hh);
        //write_ui32(w, ss);
        //fwrite(page_buff, 1, ss-12, w);
        //write_ui32(w, ss);
        
        
        if(hh == 0x0a0d0d0a) {
            decode_page_0a0d0d0a();
        } else {
          if(hh == 1) {
            decode_page_1();
            //fclose(w);
          } else {
            if(hh == 6) {
              decode_page_6(frame_no++);
            } else {
                if(hh == 5) {
                    decode_page_5();
                } else {
                    if(hh == 4) { // NRB
                        
                    } else {
                        printf("corrupted! 1\n");
                        break;
                    };
                };
            } 
          }   
        }
        b = read_ui32(f, &pp);
        if(ss != pp) {
            printf("corrupted! 2\n");
            break;
        }       
    } while(true);
    fclose(f);    
}

bool PCAP::read_from_page_ui32(unsigned int *idx, unsigned int *val) {
    *val = 0;
    if(*idx+4 > page_size) return false;
    unsigned char *q;
    q = (unsigned char *)val;
    q[0] = page_buff[*idx + 0];
    q[1] = page_buff[*idx + 1];
    q[2] = page_buff[*idx + 2];
    q[3] = page_buff[*idx + 3];
    
    *idx += 4;
    return true;
}

bool PCAP::read_from_page_ui64(unsigned int *idx, unsigned long long *val) {
    *val = 0;
    if(*idx+8 > page_size) return false;
    unsigned char *q;
    q = (unsigned char *)val;
    q[0] = page_buff[*idx + 0];
    q[1] = page_buff[*idx + 1];
    q[2] = page_buff[*idx + 2];
    q[3] = page_buff[*idx + 3];
    q[4] = page_buff[*idx + 4];
    q[5] = page_buff[*idx + 5];
    q[6] = page_buff[*idx + 6];
    q[7] = page_buff[*idx + 7];
    
    *idx += 8;
    return true;
};

void PCAP::analiz(int frame_no, unsigned char *buf, int buf_size) {
    FRAME frame;
    
    frame.clean();
    
    // 2022  (frame_no, buf, buf_size);
    
    if(frame_no == 210) {
        printf("210\n");
    }
    
    /*
    EthernetII *ethernet_ii;
    ethernet_ii = (EthernetII *)buf;
    ethernet_ii->set_endian();
    
    frame->ethernet_ii_type = ethernet_ii->type;
    frame->set_mac_src(ethernet_ii->mac_src);
    frame->set_mac_dst(ethernet_ii->mac_dst);
    */
    frame.set_mac_dst(buf);
    frame.set_mac_src(buf+6);
    frame.ethernet_ii_type = get_i16(buf[13], buf[12]);
    frame.frame_size = buf_size;
    
    frame.eth2_buf = buf;
    frame.eth2_buf_size = buf_size;
    
    counter.ethernet_ii_type(frame.ethernet_ii_type);
    
    if(frame.ethernet_ii_type == 0x0800) {
        
        analiz_ipv4(frame_no, buf + 6+6+2, buf_size-(6+6+2), &frame);
    } else if(frame.ethernet_ii_type == 0x8100) {    // Virtual LAN
        analiz_ipv4(frame_no, buf + 6+6+2+2+2, buf_size-(6+6+2+2+2), &frame);
    } else if(frame.ethernet_ii_type == 0x86dd) {    // ipv6
             
    } else if(frame.ethernet_ii_type == 0x0027) {    // stp

    } else if(frame.ethernet_ii_type == 0x88cc) {    // lldp

    } else if(frame.ethernet_ii_type == 0x74) {    // cdp

    } else if(frame.ethernet_ii_type == 0x0806) {    // ARP

    } else {
        //wtf("analiz", frame_no, buf, buf_size);
    }
 
    frame.save_pcap(frame_no, buf, buf_size);
    
    sessions.add_to_session__(&frame);   
    //sessions.save();
    

}


void PCAP::analiz_ipv4(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    int i = 0;
    IPV4_QUEUE_ITEM *ipv4_queue_item;
    unsigned char ver, header_len;
    ver = (buf[i] & 0xf0)>>4;
    header_len = (buf[i++] & 0x0f) * 4;
    if(ver != 4) {
        wtf("analiz_ipv4", frame_no, buf, buf_size);
        return;
    }
    unsigned char ip_dsfield_dscp = (buf[i] & 0xfc)>>2;
    unsigned char ip_dsfield_ecn = (buf[i++] & 0x03);
    
    unsigned short ip_total_len = get_i16(buf[i++], buf[i++]);
    unsigned short ip_id = get_i16(buf[i++], buf[i++]);
    unsigned short b1, b2;
    b1 = buf[i];
    b2 = buf[i+1];
    unsigned char ip_flag = buf[i++];
    unsigned char ip_flag_more_fragment = (ip_flag & 0x20) == 0x20 ? 1 : 0;
    unsigned short fragment_offset = (unsigned short)buf[i++];
    fragment_offset |= ((b1 & 0x1f)<<8);
    fragment_offset *= 8;
    unsigned char ip_ttl = buf[i++];

    unsigned char ip_proto = buf[i++]; 
    unsigned short ip_checksum = get_i16(buf[i++], buf[i++]);
    
    unsigned int ip_src = get_i32(buf[i++], buf[i++], buf[i++], buf[i++]);
    unsigned int ip_dst = get_i32(buf[i++], buf[i++], buf[i++], buf[i++]);
    
    frame->ip_proto = ip_proto;
    
    frame->set_ipv4_src_ip(ip_src);
    frame->set_ipv4_dst_ip(ip_dst);

    //save_ip_flow(frame_no, ip_proto, ip_src, ip_dst, ip_id, buf_size, ip_flag_more_fragment);
    
    
    if(ip_total_len == 40 && buf_size == 46) {
        buf_size = ip_total_len;
    };
    if(ip_total_len == 36 && buf_size == 46) {
        buf_size = ip_total_len;
    };
    
    if(ip_total_len == 32 && buf_size == 46) {
        buf_size = ip_total_len;
    };
    
    int ip_payload_size = buf_size - header_len;
    
    if(ip_total_len > buf_size) {
        printf("?");
    } else {
        if(ip_total_len != buf_size) {
            printf("?");

        }
    };
    if(frame_no == 9406) {
        char cc[100];
        ipv4_to_char(ip_dst, cc);
        printf("-");
        
    }
    
    if(ip_flag_more_fragment != 0) {
                
        printf("fragment\n");
        frame->is_ipv4_fragment = true;
        ipv4_queue_item = queue.add_to_ipv4_queue(ip_proto, ip_src, ip_dst, ip_id, fragment_offset, buf+header_len, ip_payload_size);
        if(ipv4_queue_item == nullptr) {
            wtf("analiz_ipv4 ipv4_queue_item == nullptr", frame_no, buf, ip_total_len);
            return;
        }
        return;
    } else {
        
            ipv4_queue_item = queue.find_ipv4_queue(ip_proto, ip_src, ip_dst, ip_id);
            
            if(ipv4_queue_item != nullptr) {
                
                ipv4_queue_item->add_fragment(buf+header_len, ip_payload_size, fragment_offset);
                int sz = fragment_offset + (ip_payload_size);
                if(ipv4_queue_item->check_compleet(sz) == true) {
                    if(queue.current_ipv4_items_count>0) queue.current_ipv4_items_count--;
                    if(ip_proto == 6) {       
                        analiz_ipv4_tcp(frame_no, ipv4_queue_item->get_buf(sz), sz, frame);
                    } else if(ip_proto == 17) {        
                        analiz_ipv4_udp(frame_no, ipv4_queue_item->get_buf(sz), sz, frame);
                    }
                    ipv4_queue_item->clean();
                    return;
                } else {
                    wtf("analiz_ipv4 check_compleet==false", frame_no, buf, ip_payload_size);
                    
                }
                
                return;
            } else {
                //wtf("analiz_ipv4 ipv4_queue_item==nullptr(2)", frame_no, buf, ip_total_len);
                //return;
            }
        
    }
    
    if(header_len != 20) {
        //wtf("analiz_ipv4 ip_total_len != 40", frame_no, buf, ip_total_len);
    }
    
    if(ip_proto == 6) {       
        analiz_ipv4_tcp(frame_no, buf+header_len, ip_payload_size, frame);
    } else if(ip_proto == 17) {        
        analiz_ipv4_udp(frame_no, buf+header_len, ip_payload_size, frame);
    } else {
        //wtf("analiz_ipv4", frame_no, buf, ip_total_len);
    }
   
}

void PCAP::analiz_ipv4_tcp(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    
    int i = 0;
    unsigned short tcp_src_port = get_i16(buf[i++], buf[i++]); 
    unsigned short tcp_dst_port = get_i16(buf[i++], buf[i++]); 
    
    frame->set_ipv4_src_port(tcp_src_port);
    frame->set_ipv4_dst_port(tcp_dst_port);
    
    frame->detect_direction();    
    frame->session_id = frame->generate_id_from_FRAME_ipv4(frame);
    frame->get_packet_no_from_session(sessions);
    
    counter.ipv4_tcp(frame->direction);
    
    //frame->generate_session_id_by_ipv4_tcp();
    
    unsigned int tcp_seq_raw = get_i32(buf[i++], buf[i++], buf[i++], buf[i++]);
    unsigned int tcp_ack_raw = get_i32(buf[i++], buf[i++], buf[i++], buf[i++]);
    
    unsigned char tcp_hdr_len = ((buf[i]&0xf0)>>4)*4;
   
    // flags
    unsigned char tcp_flags_res = ((buf[i]&0xe));
    unsigned char tcp_flags_ns = ((buf[i++]&0x1));
   
    unsigned char ff = buf[i];
    frame->tcp_flags_cwr = (buf[i]&0x80)>>7;
    frame->tcp_flags_ecn = (buf[i]&0x40)>>6;
    frame->tcp_flags_urg = (buf[i]&0x20)>>5;
    frame->tcp_flags_ack = (buf[i]&0x10)>>4;
    frame->tcp_flags_push = (buf[i]&0x08)>>3;
    frame->tcp_flags_reset = (buf[i]&0x04)>>2;
    frame->tcp_flags_syn = (buf[i]&0x02)>>1;
    frame->tcp_flags_fin = (buf[i++]&0x01);
    
    unsigned short tcp_window_size_value = get_i16(buf[i++], buf[i++]);
    unsigned short tcp_checksum = get_i16(buf[i++], buf[i++]);
    unsigned short tcp_urgent_pointer = get_i16(buf[i++], buf[i++]);
    
    frame->payload = buf+tcp_hdr_len;
    frame->payload_size = buf_size - tcp_hdr_len;


    
    i += (tcp_hdr_len - 20);
    
    char c1[100], c2[100], s[1000], c3[100], c4[100];
    std::string ss;
    
    sprintf(s, "%d %s - %s sz_pl=%05d %s [%s] seq=%u ack=%u "
            , frame_no, ipv4_to_char(frame->ipv4_src_ip, c1), ipv4_to_char(frame->ipv4_dst_ip, c2)
            , (int)(buf_size - tcp_hdr_len), frame->direction_to_char(c3), decode_to_char_tcp_flag(ff, c4)
            , tcp_seq_raw, tcp_ack_raw);
    
    ss = s;
    // net option 12 byte
    printf("%s\n", s);
    FILE *f;
    /*f = fopen("seq_ack.txt", "ab");
    if(f != NULL) {
        fprintf(f, "%s\n", s);
        fclose(f);
    }*/
    
    unsigned int d1, d2;
    bool bb;
    
    TCP_QUEUE_ITEM *q;
    int flg;
    
    flg = 0;
    
    if(frame->tcp_flags_syn == 0) {
    
        
        q = queue.add_to_tcp_queue(frame->direction, frame->ipv4_src_ip, frame->ipv4_src_port, frame->ipv4_dst_ip, frame->ipv4_dst_port, tcp_seq_raw, tcp_ack_raw, buf + i, buf_size - i);
        if(q == nullptr) {
            wtf("q = queue.add_to_tcp_queue");
            return;
        }
        if(frame->tcp_flags_push != 0) { // fragment finish
            
            d1 = tcp_seq_raw - q->seq_first;
            bb = q->check_compleet(d1+frame->payload_size);
            if(bb == true) {
                unsigned char *v;
                v = q->get_buf(d1+frame->payload_size);
                analiz_ipv4_tcp_payload(frame_no, v, d1+frame->payload_size, frame);
                frame->save_sess(frame_no, v, d1+frame->payload_size);
                detect_ip(frame);
                flg = 1;
                
            } else {
                wtf("analiz_ipv4_tcp(1)");
            }
            q->clean();  
            
        } 
        
    }
    
    
    //detect_ip(frame);
    
    //frame->payload = buf + i;
    //analiz_ipv4_tcp_payload(frame_no, buf + i, buf_size - i, frame);
    
    //frame->save_first_packert();
    
    
    //sessions.add_frame(frame);
}

void PCAP::analiz_ipv4_udp(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    
    int i = 0;
    unsigned short udp_src_port = get_i16(buf[i++], buf[i++]); 
    unsigned short udp_dst_port = get_i16(buf[i++], buf[i++]); 

    if(udp_dst_port == 37008 && buf_size > 0 && buf[8] == 1) {
        // 2022 analiz_TZSP(++frame_no, buf+8, buf_size-8);
        return;
    }
    
    frame->set_ipv4_src_port(udp_src_port);
    frame->set_ipv4_dst_port(udp_dst_port);
    
    frame->detect_direction();    
    frame->session_id = frame->generate_id_from_FRAME_ipv4(frame);
    frame->get_packet_no_from_session(sessions);
    
    counter.ipv4_udp(frame->direction);
    
    //frame->generate_session_id_by_ipv4_udp();
    
    unsigned short udp_length = get_i16(buf[i++], buf[i++]); 
    
    frame->payload = buf+8;
    frame->payload_size = buf_size - 8;
    
    //test_payload(0, frame->payload, frame->payload_size);
    
    analiz_ipv4_udp_payload(frame_no, buf + 8, buf_size - 8, frame);
    frame->save_sess(frame_no, frame->payload, frame->payload_size);
    detect_ip(frame);
    frame->save_first_packert();
    //sessions.add_frame(frame);
}

void analiz_c1(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);

void decode_http_GET(int frame_no, unsigned char *buf, int buf_size, FRAME *frame);

void PCAP::analiz_ipv4_tcp_payload(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {

    //test_payload(1, buf, buf_size);
    
    if(buf_size == 0) return;
    
    int i = 0;
    
    if(frame_no == 6) {
        //analiz_c1(frame_no, buf, buf_size, frame);
    }
    
    if(buf_size > 10) {
        if(buf[0] == 'G' && buf[1] == 'E' && buf[2] == 'T' && buf[3] == ' ') {
            printf("detect GET \n");
            decode_http_GET(frame_no, buf, buf_size, frame);
        }
    }
    
    if(buf[0] == 0x16) {
        if(buf[1] == 0x03 && buf[2] == 0x01) {
            printf("0x16-0x03-0x01\n");
            if(it_is_160301(frame_no, buf, buf_size, frame)) {
                analiz_tls_160301(frame_no, buf, buf_size, frame);
            } else {
                wtf("no 160301", frame_no, buf, buf_size);
            }
        } else if(buf[1] == 0x03 && buf[2] == 0x03) {
            if(it_is_160303(frame_no, buf, buf_size, frame)) {
                analiz_tls_160303(frame_no, buf, buf_size, frame);
            } else {
                wtf("no 160303", frame_no, buf, buf_size);
            }
        } else {
            printf("0x16\n");
        }
        
    } else if(buf[0] == 0x17) {
        if(buf[1] == 0x03 && buf[2] == 0x03) {
          printf("0x17-0x03-0x03\n");
          if(it_is_170303(frame_no, buf, buf_size, frame)) {
              
          }
        } else {
          printf("0x17-?\n");  
        }
    } else if(buf[0] == 0x14) {
        printf("0x14\n");
    } else {
        
    }
    
}

void PCAP::analiz_ipv4_udp_payload(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    
    
    
    
    int i = 0;
    
    unsigned short dns_id=0, dns_flags=0, dns_count_queries=0, dns_count_answers=0, dns_count_auth_rr=0, dns_count_add_rr=0;
    unsigned short dns_qry_type = 0, dns_qry_class = 0;
    unsigned int dns_resp_ttl, dns_soa_serial_number;
    unsigned short dns_resp_len;
    unsigned int ip = 0, xx;
    
    if(buf_size == 148) {
        if(i+3 >= buf_size) return;
        xx = get_i32(buf[i+0], buf[i+1], buf[i+2], buf[i+3]);
        if(xx == 0x00000001) { // Wireguard client -> server
            frame->set_wg_sender(buf+4);
            frame->set_wg_ephemeral(buf+8);
        } else {
            wtf("analiz_ipv4_udp_payload(1)", frame_no, buf, buf_size);
        }
    };
    if(buf_size == 92) {
        if(i+3 >= buf_size) return;
        xx = get_i32(buf[i+0], buf[i+1], buf[i+2], buf[i+3]);
        if(xx == 0x00000002) { // Wireguard server -> client
            frame->is_wireguard_sc = true;
        } else {
            //wtf("analiz_ipv4_udp_payload(1)", frame_no, buf, buf_size);
        }
    }
    
    if(frame->ipv4_dst_port == 68 && frame->mac_dst[0] == 0xff) {
        frame->dhcp_request = true;
        return;
    }
    
    char rr[1000], dns_cname[1000];
    
    if(frame->ipv4_dst_port == 53 || frame->ipv4_src_port == 53) {
        if(i+4 >= buf_size) return;
        dns_id = get_i16(buf[i++], buf[i++]);
        dns_flags = get_i16(buf[i++], buf[i++]);
        if((dns_flags & 0x8000) == 0x8000) {
            frame->dns_responce = 1;    
        } else {
            frame->dns_request = 1;
        }
        if(i+4 >= buf_size) return;
        dns_count_queries = get_i16(buf[i++], buf[i++]);
        dns_count_answers = get_i16(buf[i++], buf[i++]);
        if(i+4 >= buf_size) return;
        dns_count_auth_rr = get_i16(buf[i++], buf[i++]);
        dns_count_add_rr = get_i16(buf[i++], buf[i++]);
        /*if(dns_count_queries == 1)// && dns_count_answers == 0 && dns_count_auth_rr == 0 && dns_count_add_rr == 0) {
            for(int i=0;i<1000;i++) rr[i] = 0;
            read_dns_name(frame_no, buf, buf_size, rr, i);
            
            //printf("DNS request [%s]\n", rr);
            dns_qry_type = get_i16(buf[i++], buf[i++]);
            dns_qry_class = get_i16(buf[i++], buf[i++]);
            frame->set_dns_request(rr, dns_qry_type, dns_qry_class);
        }*/
        while(dns_count_queries > 0)
        {
                for(int i=0;i<1000;i++) rr[i] = 0;
                read_dns_name(0, frame_no, buf, buf_size, rr, i);
                frame->add_dns_request(rr);
                if(i >= buf_size) {
                    wtf("i >= buf_size");
                    return;
                }
                if(i+4 >= buf_size) return;
                dns_qry_type = get_i16(buf[i++], buf[i++]);
                dns_qry_class = get_i16(buf[i++], buf[i++]);
                frame->set_dns_request(rr, dns_qry_type, dns_qry_class);
                dns_count_queries--;
        };
        while(dns_count_answers > 0)// && dns_count_answers == 0 && dns_count_auth_rr == 0 && dns_count_add_rr == 0) 
        {
                for(int i=0;i<1000;i++) rr[i] = 0;
                read_dns_name(0, frame_no, buf, buf_size, rr, i);
                if(i+4 >= buf_size) return;
                dns_qry_type = get_i16(buf[i++], buf[i++]);
                dns_qry_class = get_i16(buf[i++], buf[i++]);
                if(i+6 >= buf_size) return;
                dns_resp_ttl = get_i32(buf[i++], buf[i++], buf[i++], buf[i++]);
                dns_resp_len = get_i16(buf[i++], buf[i++]);
                
                if(dns_qry_type == 6) {
                    
                } else if(dns_qry_type == 5){
                    
                } else if(dns_qry_type == 1){
                    if(i+4 >= buf_size) return;
                    ip = get_i32(buf[i+3], buf[i+2], buf[i+1], buf[i+0]);
                    frame->dns_responce = true;
                    frame->add_dns_responce(rr, ip);
                    if(dns_resp_len != 4) {
                        wtf("analiz_ipv4_udp_payload (2)", frame_no, buf, buf_size);
                    }
                } else {
                    
                }
                i += dns_resp_len;

                //dns_cname
                        
                dns_count_answers--;
        };
        while(dns_count_auth_rr > 0)
        {
                for(int i=0;i<1000;i++) rr[i] = 0;
                read_dns_name(0, frame_no, buf, buf_size, rr, i);
                if(i+4 >= buf_size) return;
                dns_qry_type = get_i16(buf[i++], buf[i++]);
                dns_qry_class = get_i16(buf[i++], buf[i++]);
                if(i+6 >= buf_size) return;
                dns_resp_ttl = get_i32(buf[i++], buf[i++], buf[i++], buf[i++]);
                dns_resp_len = get_i16(buf[i++], buf[i++]);
                
                if(dns_qry_type == 6) {
                    
                } else if(dns_qry_type == 5) {

                } else if(dns_qry_type == 1) {
                    if(i+4 >= buf_size) return;
                    ip = get_i32(buf[i+3], buf[i+2], buf[i+1], buf[i+0]);
                    frame->add_dns_responce(rr, ip);
                    if(dns_resp_len != 4) {
                        wtf("analiz_ipv4_udp_payload (3)", frame_no, buf, buf_size);
                    }
                }
                i += dns_resp_len;
                
                if(i == buf_size) {
                    
                    
                }
                
                dns_count_auth_rr--;
        };
        
            
        //    wtf("analiz_ipv4_udp_payload (1)", frame_no, buf, buf_size);
        
        return;
    }

}


void PCAP::analiz_tls_160301(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    int i = 0;
    unsigned short tls_ver, tls_block_len;
    unsigned int tls_handshake_length;
    while(i < buf_size) {
        
        tls_ver = get_i16(buf[i+1], buf[i+2]);
        tls_block_len = get_i16(buf[i+4], buf[i+3]);
        if(tls_ver != 0x0103 || tls_block_len >= buf_size-i) {
            wtf("analiz_tls_160301(1)", frame_no, buf, buf_size);
            return;
        }
        i += 5;
        unsigned char tls_handshake_type = buf[i+0];
        tls_handshake_length = get_i24(buf[i+3],buf[i+2],buf[i+1]);
        if(tls_handshake_length > buf_size-i-4) return;
        unsigned short tls_handshake_version = get_i16(buf[i+4], buf[i+5]);
        if(tls_handshake_version == 0x0303) {
            decode_tls_handshake_version_0303(frame_no, buf+i+6, buf_size-i-6, frame);
        }
        if(tls_handshake_version == 0x0103) {
            decode_tls_handshake_version_0301(frame_no, buf+i+6, buf_size-i-6, frame);
        }
        i += tls_block_len;
    }
};

bool PCAP::it_is_160301(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    int i = 0;
    unsigned short tls_ver, tls_block_len;
    tls_ver = get_i16(buf[i+1], buf[i+2]);
    tls_block_len = get_i16(buf[i+4], buf[i+3]);
    if(tls_ver != 0x0103 || tls_block_len >= buf_size-i) {
        return false;
    };
    
    return true;
}

bool PCAP::it_is_160303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    int i = 0;
    unsigned short tls_ver, tls_block_len;
    tls_ver = get_i16(buf[i+1], buf[i+2]);
    tls_block_len = get_i16(buf[i+4], buf[i+3]);
    if(tls_ver != 0x0303 || tls_block_len >= buf_size-i) {
        return false;
    };
    return true;
}

bool PCAP::it_is_170303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    frame->tls_170303 = true;
    int i=0;
    
    
    return true;
}

void PCAP::analiz_tls_160303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    int i = 0;
    unsigned short tls_ver, tls_block_len;
    unsigned short tls_handshake_version;
    

    
    while(i < buf_size) {
        
        tls_ver = get_i16(buf[i+1], buf[i+2]);
        tls_block_len = get_i16(buf[i+4], buf[i+3]);
        if(tls_ver != 0x0303 || tls_block_len >= buf_size-i) {
            //wtf("analiz_tls_160303(1)", frame_no, buf, buf_size);
            return;
        }
        i += 5;
        unsigned char tls_handshake_type = buf[i+0];
        unsigned int tls_handshake_length = get_i24(buf[i+3],buf[i+2],buf[i+1]);
        if(tls_handshake_length > buf_size-i-4) return;
        if(tls_handshake_type == 11) {
            unsigned int certificates_length = get_i24(buf[i+6],buf[i+5],buf[i+4]);
            if(certificates_length > buf_size-i-4) return;
            
            decode_sertificates(frame_no, buf+i+7, certificates_length,  frame);
        } else if(tls_handshake_type == 2) {
            tls_handshake_version = get_i16(buf[i+4], buf[i+5]);
            if(tls_handshake_version == 0x0303) {
                decode_tls2_handshake_version_0303(frame_no, buf+i+6, tls_handshake_length, frame);
            }
        } else if(tls_handshake_type == 1) {
            tls_handshake_version = get_i16(buf[i+4], buf[i+5]);
            if(tls_handshake_version == 0x0303) {
                decode_tls1_handshake_version_0303(frame_no, buf+i+6, tls_handshake_length, frame);
            }
        } else {
            wtf("tls_handshake_type");
        }
        i += tls_block_len;
    }
};

void PCAP::read_dns_name(int lvl, int frame_no, unsigned char *buf, int buf_size, char *rr, int &i) {
    int jj=0;
    while(jj<1000-2 && rr[jj] != 0) jj++;
    
    int k;

    if(lvl > 20) {
         wtf("lvl > 20");
        return;
    }
    
    if(i >= buf_size) { 
        wtf("read_dns_name i > buf_size"); 
        return; 
    };
    unsigned char cc = buf[i++];
    while(cc > 0) {
        
        if((cc & 0xc0) == 0xc0)
        {
            k = (cc & 0x3f);
            if(i >= buf_size) { 
                wtf("read_dns_name (2)", frame_no, buf, buf_size); 
                return;
            }
            if(i >= buf_size) { wtf("read_dns_name i > buf_size"); return; };
            
            cc = buf[i++];
            k += cc;
            read_dns_name(lvl+1, frame_no, buf, buf_size, rr, k);
            return;
        }

        
        if(jj > 0) rr[jj++] = '.';
        for(unsigned int j=0;j<cc;j++)
        {
            
            if(i >= buf_size) { wtf("read_dns_name i > buf_size"); return; };
            if(jj>=1000-10) {
                return;
            }
            
            int vv;
            vv = buf[i];
            
            if( (buf[i] >= 'a' && buf[i] <= 'z') ||
                (buf[i] >= '0' && buf[i] <= '9') ||
                 buf[i] == '-' || buf[i] == '.' ) 
            {
            
            } else {
                printf("----\n");
                if(vv > 127 || vv < 32) {
                    return;
                }
            }
            rr[jj++] = buf[i++]; 
        };
        
        if(i >= buf_size) { wtf("read_dns_name i > buf_size"); return; };
        cc = buf[i++];
    }
}

void PCAP::decode_tls_handshake_version_0301(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {

}

void PCAP::decode_tls_handshake_version_0303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    char nn[1000];
    int i=0;
    
    i += 32; // tls.handshake.random
    if(i >= buf_size) return;
    unsigned char tls_handshake_session_id_length = buf[i++];
    i += tls_handshake_session_id_length; // tls.handshake.session_id
    if(i+1 >= buf_size) return;
    unsigned short tls_handshake_cipher_suites_length = get_i16(buf[i++], buf[i++]);
    i += tls_handshake_cipher_suites_length;
    if(i >= buf_size) return;
    unsigned char tls_handshake_comp_methods_length = buf[i++];
    i += tls_handshake_comp_methods_length;
    if(i+1 >= buf_size) return;
    unsigned short tt, ll, tls_handshake_extensions_length = get_i16(buf[i++], buf[i++]);
    int el;
    el = tls_handshake_extensions_length;
    while( el > 0 ) 
    {
                if(i+3 >= buf_size) return;
                tt = get_i16(buf[i++], buf[i++]);
                ll = get_i16(buf[i++], buf[i++]);
                if(tt == 0) { // server name
                    for(int j=0;j<1000;j++) nn[j] = 0;
                    decode_server_name(frame_no, buf+i, ll, nn);
                    frame->set_SNI(nn);
                }
                if(tt == 51) {
                    for(int j=0;j<1000;j++) nn[j] = 0;
                    int nn_len = 0;
                    decode_server_51(buf+i, ll, nn, &nn_len);
                    frame->set_ClientHello_Key51((unsigned char *)nn, nn_len);
                }
                i += ll;
                el -= (4 + ll);
    }
    
}

void PCAP::decode_sertificates(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    int i = 0;
    CERT cert;
    while(i < buf_size) {
        unsigned int certificate_length = get_i24(buf[i+2],buf[i+1],buf[i+0]); 
        i+= 3;
        if(certificate_length > buf_size) return;
        
        cert.clean();
        decode_sertificat(frame_no, buf+i, certificate_length, &cert);
        frame->add_cert(cert);
        i += certificate_length;
    };
}

void decode_sni_extension_sni(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    char b[100];
    b[0] = 0;
    int i = 0;
    uint16_t len = get_i16(buf[1], buf[0]);
    if(len+2 != buf_size) return;
    while(i < len) {
        b[i] = buf[i+2];
        b[i+1] = 0;
        i++;
    }
    frame->SNI.push_back(b);
}

void decode_sni_extension(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    if(buf_size < 3) return;
    uint16_t len = get_i16(buf[1], buf[0]); 
    uint8_t type = buf[2];
    if(len+2 == buf_size && type == 0) {
        decode_sni_extension_sni(frame_no, buf+3, buf_size-3, frame);
    }
}

void PCAP::decode_tls1_handshake_version_0303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    int i = 0;
    i += 32;
    if( i+3 >= buf_size) return;
    uint8_t tls_handshake_session_id_length = buf[i++];
    uint16_t tls_handshake_cipher_suites_length = get_i16(buf[i++], buf[i++]);
    if(i + tls_handshake_cipher_suites_length + 1 >= buf_size) return;
    
    i += tls_handshake_cipher_suites_length;
    
    uint8_t tls_handshake_comp_methods_length = buf[i++];
    if(tls_handshake_comp_methods_length != 1) return;
    if(i >= buf_size) return;
    uint8_t tls_handshake_comp_methods = buf[i++];
    
    if(i+1 >= buf_size) return;
    uint16_t tls_handshake_extensions_length = get_i16(buf[i++], buf[i++]);

    while(tls_handshake_extensions_length > 0) {
    
        if(i+4 >= buf_size) return;
        uint16_t tls_handshake_extension_type = get_i16(buf[i++], buf[i++]);
        uint16_t tls_handshake_extension_len = get_i16(buf[i++], buf[i++]);

        printf("type=%X len=%d\r\n", tls_handshake_extension_type, tls_handshake_extension_len);
        
        if(tls_handshake_extension_len > tls_handshake_extensions_length) return;
        
        if(tls_handshake_extension_type == 0) { // server name
            decode_sni_extension(frame_no, buf+i, tls_handshake_extension_len, frame);
        }
        
        tls_handshake_extensions_length -= tls_handshake_extension_len;
        i += tls_handshake_extension_len;
    }
    
    
}


void PCAP::decode_tls2_handshake_version_0303(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    int i = 0;
    
    
}

void PCAP::decode_server_name(int frame_no, unsigned char *buf, int buf_size,char *nn) {
    int i=0;
    unsigned short server_name_list_len = get_i16(buf[i++], buf[i++]);
    unsigned char server_name_type = buf[i++];
    if(server_name_type == 0) {
        unsigned short server_name_len = get_i16(buf[i++], buf[i++]);
        unsigned short j = 0;
        while(j < server_name_len) {
            nn[j] = buf[i++];
            j++;
        }
        nn[j] = 0;
        return;
    }
    wtf("decode_server_name", frame_no, buf, buf_size);
}

void PCAP::decode_server_51(unsigned char *buf, int buf_size, char *nn, int *nn_len) {
    int i = 0;
    unsigned short j;
    unsigned short key_share_client_length = get_i16(buf[i++], buf[i++]);
    j = 0;
    while( j < key_share_client_length ) {
        unsigned short key_share_group = get_i16(buf[i++], buf[i++]);
        unsigned short key_share_key_exchange_length  = get_i16(buf[i++], buf[i++]);
        if(key_share_group == 29) { // key
            if(key_share_key_exchange_length <= 256) {
                int k;
                *nn_len = key_share_key_exchange_length;
                k = 0;
                while(k<key_share_key_exchange_length) {
                    nn[k] = buf[i+k];
                    k++;
                }
            }
        }
        i += key_share_key_exchange_length;
        j += key_share_key_exchange_length+4;
    };
    
}

void PCAP::decode_sertificat(int frame_no, unsigned char *buf, int buf_size, CERT *cert) {
    int i=0;
    decode_tls_cert(buf, 0, buf_size, cert, "root");
}

void PCAP::decode_tls_cert(unsigned char *item, int idx, int cert_len, CERT *cert, std::string path) {
    std::string object_name;
    std::string object_value;
    
  //AnsiString aa, zz, rr;
  char xxx[100];
  unsigned long long ii64;
  int val;
  unsigned char ver, serial[100], l8, vl[1000], r1[1024], r2[50];
  unsigned short ll;
  //ss += " len="+String(cert_len);
  int ii, i;
  bool flag;
  char start[1000];
  //sprintf(start, "%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-"
  //, item[idx+0].v, item[idx+1].v, item[idx+2].v, item[idx+3].v, item[idx+4].v, item[idx+5].v
  //, item[idx+6].v, item[idx+7].v, item[idx+8].v, item[idx+9].v, item[idx+10].v, item[idx+11].v
  //);

  int lvl = 0;

  i = 0;
  while(i < cert_len)
  {
      lvl++;
    flag = false;

    if(flag == false && item[idx+i] == 0x5 && item[idx+i+0]) // NULL
    {
      flag = true;
      i += 2;
    };

    if(flag == false && item[idx+i] == 0x31)
    {

      flag = true;
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        if(item[idx+i+1] == 0x82)
        {
          ii = get_ui16_R(item, idx+i, &ll, 2);
          
          decode_tls_cert(item, idx+i+4, ll, cert, path+"|"+std::to_string(lvl));
          i += ll+4;
        }
        else
        {
          if(item[idx+i+1] == 0x81)
          {
            ii = get_ui8(item, idx+i, &l8, 2);
            decode_tls_cert(item , idx+i+3, l8, cert, path+"|"+std::to_string(lvl));
            i += ll+3;
          }
          else
          {
            wtf(" wtf?");
            return;
          };
        };
      }
      else
      {
        ii = get_ui8(item, idx+i, &l8, 1);
        decode_tls_cert(item, idx+i+2, l8, cert, path+"|"+std::to_string(lvl));
        i += 2 + l8;
      }
    }


    if(flag == false && item[idx+i] == 0x30)
    {
      flag = true;
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        if(item[idx+i+1] == 0x82)
        {
          ii = get_ui16_R(item, idx+i, &ll, 2);
          decode_tls_cert(item, idx+i+4, ll, cert, path+"|"+std::to_string(lvl));
          i += ll+4;
        }
        else
        {
          if(item[idx+i+1] == 0x81)
          {
            ii = get_ui8(item, idx+i, &l8, 2);
            decode_tls_cert(item, idx+i+3, l8, cert , path+"|"+std::to_string(lvl));
            i += l8+3;
          }
          else
          {
            ///ss += " wtf?";
              wtf("?");
            return;
          };
        };
      }
      else
      {
        ii = get_ui8(item, idx+i, &l8, 1);
        decode_tls_cert(item, idx+i+2, l8, cert, path+"|"+std::to_string(lvl));
        i += 2 + l8;
      }
    }
    if(flag == false && item[idx+i] == 0xA0)
    {
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        wtf(" wtf?");
        return;
      }
      else
      {
        ii = get_ui8(item, idx+i, &l8, 1);
        decode_tls_cert(item, idx+i+2, l8, cert, path+"|"+std::to_string(lvl));
        i += l8+2;
        flag = true;
      };
    };

    if(flag == false && item[idx+i] == 0x02)
    {
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        wtf(" wtf?");
        return;
      }
      else
      {
        ii = get_ui8(item, idx+i, &l8, 1);
        if(l8 == 1)
        {

          val = item[idx+i+2];
          cert->set_ver(val);
          //ss += path + "|0x02 val=" + String(val);
        }
        else
        {
          if(l8 == 9)
          {
            //set_label(item, idx, i+3, 8, 0xaa0000, 0x00ffff,"i64", 0x009900, 0x00);
            ii64 = 0;
            unsigned char *q;
            q = (unsigned char *)&ii64;
            //rr = "";
            for(int j=0;j<8;j++)
            {
              *q++ = item[idx+i+10-j];
              sprintf(xxx, "%02X", item[idx+i+2+j]);
              //rr += String(xxx);
            };
            //ss += " " + path + "|0x02 val=" + String(ii64);
          }
          else
          {
            if(l8 == 20)
            {
              //AnsiString zz;
              //zz += path + "|0x02";

              //rr = "";
              for(int j=0;j<20;j++) {
                serial[j] = item[idx+i+2+j];
                sprintf(xxx, "%02X", serial[j]);
                //rr += String(xxx);
              };
              //if(zz == "root|0x30|0x30|0x02")
              {
                //frame->sert_serial(rr);
              };
            }
            else
            {
                if(l8 == 16)
                {
                    for(int j=0;j<16;j++) {
                        serial[j] = item[idx+i+2+j];
                        //sprintf(xxx, "%02X", serial[j]);
                    };
                    cert->set_serial16(serial);
                } else {
                    wtf(" wtf?");
                    return;
                };
            };
          };
        }
        i += l8+2;
        flag = true;
      };
    };
    if(flag == false && item[idx+i] == 0x06) // OBJECT IDENTIFIER
    {
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        wtf(" wtf?");
        return;
      }
      else
      {
        ii = get_ui8(item, idx+i, &l8, 1);

        for(int j=0;j<l8;j++)
        {
          vl[j] = item[idx+i+j+2];
        };

        
        
        object_name = decode_object(vl, l8);

        i += l8+2;
        flag = true;
      };
    };

    if(flag == false && item[idx+i] == 0x13)
    {
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        wtf(" wtf?");
        return;
      }
      else
      {
        ii = get_ui8(item, idx+i, &l8, 1);
        //aa = "";
        object_value = "";
        for(int j=0;j<l8;j++)
        {
          //aa += String((char)item[idx+i+j+2]);
            object_value += (char)item[idx+i+j+2];
        };
        //ss += "{"+aa+"}";


        i += l8+2;
        flag = true;
      };
    };
    if(flag == false && item[idx+i] == 0x0C)
    {
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        wtf(" wtf?");
        return;
      }
      else
      {
        ii = get_ui8(item, idx+i, &l8, 1);
        
        object_value = "";
        for(int j=0;j<l8;j++)
        {
          object_value += (char)item[idx+i+j+2];
        };
        //ss += "{"+aa+"}";


        i += l8+2;
        flag = true;
      };
    };
    if(flag == false && item[idx+i] == 0x17) // date time
    {
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        wtf(" wtf?");
        return;
      }
      else
      {
        ii = get_ui8(item, idx+i, &l8, 1);
        //aa = "";
        for(int j=0;j<l8;j++)
        {
          //aa += String((char)item[idx+i+j+2]);
        };
        //ss += "{"+aa+"}";


        i += l8+2;
        flag = true;
      };
    };

    if(flag == false && item[idx+i] == 0x03) // bit string
    {
      flag = true;
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        if(item[idx+i+1] == 0x82)
        {
          ii = get_ui16_R(item, idx+i, &ll, 2);
          //decode_tls_cert(item, idx+i+2, l8, cert, path+"|"+std::to_string(lvl));
          int res;
          res = decode_bit_string_tls_cert(item, idx+i+4, ll, cert, path+"|"+std::to_string(lvl), r1, r2);
          if(res == 1) {
              cert->detect_bitstring2048_m3(r1, r2);
          } else if(res == 2) { 
              cert->detect_bitstring2048(r1);
          } else if(res == 3) {
              cert->detect_bitstring4096(r1);
          } else if(res == 4) {
              cert->detect_bitstring4096_m3(r1, r2);
          } else {
              wtf("?(1)");
          }
          i += ll+4;
        }
        else
        {
          if(item[idx+i+1] == 0x81)
          {
            ii = get_ui8(item, idx+i, &l8, 2);
            int res;
            res = decode_bit_string_tls_cert(item, idx+i+3, l8, cert, path+"|"+std::to_string(lvl), r1, r2);
            //decode_tls_cert(idx+i+3, ss, l8);
            i += ll+3;
          }
          else
          {
            wtf(" wtf?");
            return;
          };
        };
      }
      else
      {
        ii = get_ui8(item, idx+i, &l8, 1);
        //decode_tls_cert(idx+i+2, ss, l8);
        i += 2 + l8;
      }
    }

    if(flag == false && item[idx+i] == 0x04) // octet string
    {
       flag = true;
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        if(item[idx+i+1] == 0x82)
        {
          ii = get_ui16_R(item, idx+i, &ll, 2);
          /// decode_tls_cert(item, idx+i+4, ll, cert, path+"|"+std::to_string(lvl));
          
          i += ll+4;
        }
        else
        {
          if(item[idx+i+1] == 0x81)
          {
            //ii = get_ui8(idx+i, &l8, 2);
            l8 = item[idx+i+2];
            /// decode_tls_cert(item, idx+i+3, l8, cert, path+"|"+std::to_string(lvl));
            //decode_tls_cert(idx+i+3, ss, l8);
            i += ll+3;
          }
          else
          {
            wtf(" wtf?");
            return;
          };
        };
      }
      else
      {
        l8 = item[idx+i+1];
        //decode_tls_cert(idx+i+2, ss, l8);
        i += 2 + l8;
      } 
    }
    
    if(flag == false && item[idx+i] == 0x01) // bool 
    {
        flag = true;
        l8 = 1;
        i += 2 + l8;
    }
    
    if(flag == false && item[idx+i] == 0xA3)
    {
      flag = true;
      if((item[idx+i+1] & 0x80) == 0x80)
      {
        if(item[idx+i+1] == 0x82)
        {
          ii = get_ui16_R(item, idx+i, &ll, 2);
          decode_tls_cert(item, idx+i+4, ll, cert, path+"|"+std::to_string(lvl));
          
          i += ll+4;
        }
        else
        {
          if(item[idx+i+1] == 0x81)
          {
            //ii = get_ui8(idx+i, &l8, 2);
            l8 = item[idx+i+2];
            decode_tls_cert(item, idx+i+3, l8, cert, path+"|"+std::to_string(lvl));
            //decode_tls_cert(idx+i+3, ss, l8);
            i += l8+3;
          }
          else
          {
            wtf(" wtf?");
            return;
          };
        };
      }
      else
      {
        l8 = item[idx+i+1];
        //decode_tls_cert(idx+i+2, ss, l8);
        i += 2 + l8;
      }
    }

    if(flag == false)
    {
      wtf(" wtf?");
      return;
    };


    if(i == cert_len)
    {
        if(object_name != "") {
            cert->set_object(object_name, object_value );
        } 
      return;
    };
    if(i > cert_len)
    {
      wtf(" wtf?");
      return;
    };
  };
};

std::string PCAP::decode_object(unsigned char v[], unsigned int len)
{
    std::string ss;
    
  int i;
  unsigned char v1[] = {0x2a ,0x86 ,0x48 ,0x86 ,0xf7 ,0x0d ,0x01 ,0x01 ,0x0b};
  unsigned char v2[] = {0x2a ,0x86 ,0x48 ,0x86 ,0xf7 ,0x0d ,0x01 ,0x01 ,0x01};

  unsigned char v3_1[] = {0x55, 0x04, 0x06};


  if(len == 9)
  {
    i = 0;
    while(i<9 && v[i] == v2[i]) i++;
    if(i == 9) {
      return "rsaEncryption";
      
    };
    i = 0;
    while(i<9 && v[i] == v1[i]) i++;
    if(i == 9) {
      return "sha256WithRSAEncryption";
    };
  };

  if(len == 3)
  {
    if(v[0] == 0x55 && v[1] == 0x4)
    {

      if(v[2] == 0x6) { return "countryName";  };
      if(v[2] == 0x7) { return "localityName";  };
      if(v[2] == 10) { return "orgName";  };
      if(v[2] == 11) { return "orgUnitName";  };
      if(v[2] == 3) { return "commonName";  };
      if(v[2] == 8) { return "stateName";  };

    };
  };

  return "?";

};

int PCAP::decode_bit_string_tls_cert(unsigned char *item, int idx, int cert_len, CERT *cert, std::string path, unsigned char *result1, unsigned char *result2) {
    int i=0, j;
    unsigned char unused_bits = item[idx+i++];
    cert_len--;
    if(unused_bits != 0) {
        wtf("unused_bits != 0");
        return 0;
    }
    if(cert_len == 526 && item[idx+i] == 0x30 && item[idx+i+1] == 0x82) {
        i += 2;
        unsigned short l1, l2, kl;
        j = get_ui16_R(item, idx+i, &l1, 0);
        if(item[idx+i+2] != 0x02 || item[idx+i+3] != 0x82) {
            wtf("?cert(2)");
            return 0;
        }
        j = get_ui16_R(item, idx+i+4, &kl, 0);
        if(kl!=513) {
            wtf("?cert(3)");
            return 0;
        }
        unused_bits = item[idx+i+6];
        if(unused_bits != 0) {
            wtf("unused_bits != 0(2)");
            return 0;
        }
        i+=7;
        for(int x=0;x<512;x++) result1[x] = item[idx+i+x];
        i += 512;
        if(item[idx+i] != 0x02 || item[idx+i+1] != 0x03) {
            wtf("?cert(4)");
            return 0;
        }
        i += 2;
        for(int x=0;x<3;x++) result2[x] = item[idx+i+x];
        return 4;
    }
    if(cert_len == 270 && item[idx+i] == 0x30 && item[idx+i+1] == 0x82) {
        i += 2;
        unsigned short l1, l2, kl;
        j = get_ui16_R(item, idx+i, &l1, 0);
        if(item[idx+i+2] != 0x02 || item[idx+i+3] != 0x82) {
            wtf("?cert(2)");
            return 0;
        }
        j = get_ui16_R(item, idx+i+4, &kl, 0);
        if(kl!=257) {
            wtf("?cert(3)");
            return 0;
        }
        unused_bits = item[idx+i+6];
        if(unused_bits != 0) {
            wtf("unused_bits != 0(2)");
            return 0;
        }
        i+=7;
        for(int x=0;x<256;x++) result1[x] = item[idx+i+x];
        i += 256;
        if(item[idx+i] != 0x02 || item[idx+i+1] != 0x03) {
            wtf("?cert(4)");
            return 0;
        }
        i += 2;
        for(int x=0;x<3;x++) result2[x] = item[idx+i+x];
        return 1;
    }
    if(cert_len == 256) {
        for(int x=0;x<256;x++) result1[x] = item[idx+i+x];
        return 2;
    }
    if(cert_len == 512) {
        for(int x=0;x<512;x++) result1[x] = item[idx+i+x];
        return 3;
    }
    wtf("?cert?");
    return 0;
}


void analiz_c1(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
 
    int i = 0;
    unsigned short block1, block2;    
    unsigned int tls_handshake_length;
    
    unsigned char tls_handshake_type = buf[i+0]; i++;
            
    tls_handshake_length = get_i24(buf[i+2],buf[i+1],buf[i+0]); i += 3;
    if(tls_handshake_length == buf_size - 4) {
        
        block1 = get_i16(buf[i+1], buf[i]); i += 2;
        block2 = get_i16(buf[i+1], buf[i]); i += 2;

        if(block1 == 0x3 && block2 == tls_handshake_length - 20) {
            i += 16;
            do
            {
                block1 = get_i16(buf[i+1], buf[i]); i += 2;
                block2 = get_i16(buf[i+1], buf[i]); i += 2;
                
                    i+= block2;
            } while(true);

        }
        
    };
    
}


bool _my_left_strcmp(const char *s1, const char *s2) {
    if(s1 == nullptr || s2 == nullptr) return false;
    int i;
    i = 0;
    while(s1[i] != 0 && s2[i] != 0 && s1[i] == s2[i]) {
        i++;
    }
    if(s1[i] == 0) return true;
    return false;
}

void decode_http_GET(int frame_no, unsigned char *buf, int buf_size, FRAME *frame) {
    char m[20][500];
    int i,j,k;
    for(int i=0;i<20;i++) m[i][0] = 0;
    i = 0;
    k = 0;
    j = 0;
    while(i < buf_size) {
        if(buf[i] == '\r') {
            if(j<20-2) j++;
            k = 0;
        } else {
            if(buf[i] != '\n') {
                m[j][k] = _to_lower(buf[i]);
                if(k<500-2) k++;
                m[j][k] = 0;
            }
        }
        i++;
    }
    for(i=0;i<20;i++) {
        if(_my_left_strcmp("host: ", m[i])) {
            std::string s;
            s = m[i]+6;
            frame->http.get = s;
        }
    }
}