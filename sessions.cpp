/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "sessions.h"
#include <vector>

void custom_analiz(SESSION *s, FRAME *frame);

extern std::map<unsigned int, std::string> global_dns_list;

SESSIONS::SESSIONS() {
    
}

void SESSIONS::save() {
    FILE *f;
    f = fopen("sessions.txt", "wb");
    if(f != NULL){
        for(auto& a : items) {
            a.second.ssave(f);
            
        }
        fclose(f);
    }
}



void SESSIONS::add_to_session__(FRAME *frame) {
    
    
    
    if(frame->stored_to_session_ == true) return;
    if(frame->is_ipv4_fragment == true) return;
    
    if(frame->ip_proto == 0) return;
    if(frame->ipv4_dst_ip == 0 || frame->ipv4_src_ip == 0 ||
       frame->ipv4_dst_ip == 0xffffffff || frame->ipv4_src_ip == 0xffffffff ) return;
    if(frame->session_id == "" && frame->ip_proto != 6 && frame->ip_proto != 17) {
        frame->session_id = std::to_string(frame->ip_proto);
    }
    if(frame->session_id == "") {
        wtf("SESSIONS::add_to_session");
        return;
    }
    
    /*std::string cc;
    
    for(auto const& it: frame->cert_list) {
        cc += it.commonName;
    }*/
    
    //std::pair<std::string, SESSION> p;
    
    
    
    
    auto it = items.find(frame->session_id);
    if(it == items.end()) {
        std::string s = "";
        std::map<unsigned int, std::string> :: iterator dns_it;
        dns_it = global_dns_list.find(frame->ipv4_dst_ip);
        if(dns_it != global_dns_list.end()) {           
            s = dns_it->second;
        }
        items.insert(std::make_pair(frame->session_id, SESSION(s)));
        it = items.find(frame->session_id);
        if(it == items.end()) {
            return;
        }
        
    }
    it->second.add_to_session(frame);
    
    if(frame->tcp_flags_fin) {
        printf("fin\n");
        items.erase(frame->session_id);
    }
    
    SESSION *s = &(it->second);
    
    custom_analiz(s, frame);
}