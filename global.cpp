
#include <cstddef>
#include <dirent.h>
#include <algorithm> 



#include "GUI/GUI.h"
#include "GUI/GUI_Element.h"
#include "global.h"
#include "pcap.h"
#include "sniffer.h"
#include "mikrotik.h"

extern PCAP pcap;
extern bool GLOBAL_STOP;
extern SNIFFER sniffer;
extern MIKROTIK mikrotik;

void GLOBAL::init() {
    execute_is_run = true;
    execute_thread = new std::thread(&GLOBAL::execute, this);
    
    execute2_is_run = true;
    execute2_thread = new std::thread(&GLOBAL::execute2, this);
}
void GLOBAL::wait_execute_close() {
    while(execute_is_run == true) {
        usleep(100);
    }
    while(execute2_is_run == true) {
        usleep(100);
    }
};    
void GLOBAL::execute() {
    while(GLOBAL_STOP == false) {
        if(need_load_from_file_FileName != "") {
            pcap.counter.clear();
            pcap.load_from_file(need_load_from_file_FileName);
            need_load_from_file_FileName = ""; 
        }
        if(need_run_sniffer) {
            printf("need_run_sniffer [%d] %s\n", InterfaceSelectId, InterfaceSelect.c_str());
            
            sniffer.start_sniff(InterfaceSelectId, InterfaceSelect.c_str());
            
            need_run_sniffer = false;
            gui->SnifRun->is_pressed = false;
        }
        usleep(100);
    }
    execute_is_run = false;
}

void GLOBAL::execute2() {
    
    unsigned int iip;
    
    while(GLOBAL_STOP == false) {
        
         serial_io();
        
        iip = get_ip_to_queue_to_send_mikrotik();
        if(iip != 0) {
            mikrotik.set_firewall_ip(iip);
        }
        
        usleep(100);
    }
    execute2_is_run = false;
}


void GLOBAL::set_InterfaceSelect(std::string val) {
    int p;
    std::string a1, a2;
    p = val.find_first_of(':');
    if(p != std::string::npos) {
        a1 = val.substr(0, p);
        InterfaceSelect = val.substr(p+1, 5555);
        try
        {
            this->InterfaceSelectId = std::stoi(a1);
        } catch(...) {
            this->InterfaceSelectId = 0;
        }
    }
    printf("111\n");
}

bool it_is_log(char *v) {
    if(v == nullptr) return false;
    int i;
    i = strlen(v);
    if(i > 4) {
        if( v[i-1] == 'g' && 
            v[i-2] == 'o' && 
            v[i-3] == 'l' && 
            v[i-4] == '.' 
                )
        {
            return true;
        }
    }
    return false;
}

bool it_is_pcapng(char *v) {
    if(v == nullptr) return false;
    int i;
    i = strlen(v);
    if(i > 7) {
        if( v[i-1] == 'g' && 
            v[i-2] == 'n' && 
            v[i-3] == 'p' && 
            v[i-4] == 'a' && 
            v[i-5] == 'c' && 
            v[i-6] == 'p' && 
            v[i-7] == '.' 
                )
        {
            return true;
        }
        if( v[i-1] == 'p' && 
            v[i-2] == 'a' && 
            v[i-3] == 'c' && 
            v[i-4] == 'p' && 
            v[i-5] == '.' 
                )
        {
            return true;
        }

    }
    return false;
}

void GLOBAL::set_FoldersSelect(std::string val) {

    
    
    gui->FilesList->item.clear();
    
    std::string h = getenv("HOME");
    h = "/home/smorodin";
    FolderName = h + "/" + val;
    if(val == "tmp") {
        FolderName = "/tmp";
    }
    DIR *dir;
    int k;
    char *c;
    dir = opendir(FolderName.c_str());
    struct dirent *entry;
    if(dir == 0) return;
    while ( (entry = readdir(dir)) != NULL) {
        //printf("{%d} - (%s) [%d] %d\n", entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
        c = entry->d_name;
        if(!((c[0] == '.' && c[1] == 0) ||
             (c[0] == '.' && c[1] == '.' && c[2] == 0) ||
              entry->d_type == DT_DIR ||
              it_is_log(entry->d_name)
            )
          )
        {
            if(it_is_pcapng(entry->d_name)) {
                gui->FilesList->item.push_back({gui, ELEMENT_item::ListItem, entry->d_name});
            }
        }
        
    };
    
   closedir(dir);
    
   std::sort(gui->FilesList->item.begin(), gui->FilesList->item.end());
}

void GLOBAL::set_FilesDoubleClick(std::string val) {
    printf("DoubleClick %s\n", val.c_str());
    need_load_from_file(FolderName + "/" + val);
}

void GLOBAL::set_SnifferRun(bool val) {
    if(val == true) {
        need_run_sniffer = true;
    } else {
        need_run_sniffer = false;
        sniffer.set_local_stop(true);
    }
}

void GLOBAL::active_IF_tab() {
    gui->PanelIf->is_visible = true;
    gui->PanelPcap->is_visible = false;
    gui->PanelClicker->is_visible = false;
    gui->focus = nullptr;
}

void GLOBAL::active_PCAP_tab() {
    gui->PanelIf->is_visible = false;
    gui->PanelPcap->is_visible = true;
    gui->PanelClicker->is_visible = false;
    gui->focus = nullptr;
}

void GLOBAL::active_CLICKER_tab() {
    gui->PanelIf->is_visible = false;
    gui->PanelPcap->is_visible = false;
    gui->PanelClicker->is_visible = true;
    gui->focus = gui->View;
}

void GLOBAL::add_ip_to_queue_to_send_mikrotik(unsigned int ip) {
    if(ip_to_add_s.find(ip) != ip_to_add_s.end()) {
        return;
    }

    if(mikrotik.ip_is_exists_in_list(ip))
    {
        return;
    }
    
    m_queue_mutex.lock();
    ip_to_add_s.insert(ip);
    ip_to_add_q.push(ip);
    m_queue_mutex.unlock();
    if(mikrotik_info != nullptr) {
        mikrotik_info->info1 = ip_to_add_s.size();
    }
}

unsigned int GLOBAL::get_ip_to_queue_to_send_mikrotik() {
    unsigned int u = 0;
    m_queue_mutex.lock();
    if(!ip_to_add_q.empty()) {
        u = ip_to_add_q.front();
        ip_to_add_q.pop();
        ip_to_add_s.erase(u);
    }
    m_queue_mutex.unlock();
    if(mikrotik_info != nullptr) {
        mikrotik_info->info1 = ip_to_add_s.size();
    }
    return u;
}



void GLOBAL::test1() {
    printf("test1\n");
    for(int i=0;i<5;i++) need_write_serial_5bytes[i] = 0;
    /*
    need_write_serial_5bytes[1] = 55;
    need_write_serial_5bytes[4] = 'X';
    need_write_serial_5bytes[0] = 'K';
    */
    need_write_serial_5bytes[1] = 55;
    need_write_serial_5bytes[2] = -99;
    need_write_serial_5bytes[4] = 0xac;
    need_write_serial_5bytes[0] = 'M';
}


void GLOBAL::serial_io() {
    unsigned char c;
    int s;
    if(f_serial == -1) {
        
        f_serial = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);
        if (f_serial != -1)
        fcntl(f_serial, F_SETFL, FNDELAY);
    }
    if(f_serial != -1) {
        
        s = read(f_serial, &c, 1);
        if(s != 0) {
            printf("read = %x\n", c);
        }
        if(need_write_serial_5bytes[0] != 0) {
            s = write(f_serial, need_write_serial_5bytes, 5);
            for(int i=0;i<5;i++) need_write_serial_5bytes[i] = 0;
            //printf("write=%d\n", s);
        }
    }
}