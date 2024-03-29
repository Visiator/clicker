
#include <cstddef>
#include <dirent.h>
#include <algorithm> 

#ifdef __linux__
#include <termios.h>
#endif

#include "GUI/GUI.h"
#include "GUI/GUI_Element.h"
#include "global.h"
#include "pcap.h"
#include "sniffer.h"
#include "mikrotik.h"
#include "program.h"


extern PCAP pcap;
extern bool GLOBAL_STOP;
extern SNIFFER sniffer;
extern MIKROTIK mikrotik;
extern PROGRAMS programs_;

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
        
        usleep(1);
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

    
#ifdef __linux__
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
              entry->d_type == DT_DIR_ ||
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
   
//  __linux__
#endif

#ifdef _WIN32
   // TODO 2023
#endif
   
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
    gui->PanelGrab->is_visible = false;
    gui->PanelProg->is_visible = false;
    gui->focus = nullptr;
}

void GLOBAL::active_PCAP_tab() {
    gui->PanelIf->is_visible = false;
    gui->PanelPcap->is_visible = true;
    gui->PanelGrab->is_visible = false;
    gui->PanelProg->is_visible = false;
    gui->focus = nullptr;
}

void GLOBAL::active_GRAB_tab() {
    gui->PanelIf->is_visible = false;
    gui->PanelPcap->is_visible = false;
    gui->PanelGrab->is_visible = true;
    gui->PanelProg->is_visible = false;
    gui->focus = gui->View;
}

void GLOBAL::active_PROG_tab() {
    gui->PanelIf->is_visible = false;
    gui->PanelPcap->is_visible = false;
    gui->PanelGrab->is_visible = false;
    gui->PanelProg->is_visible = true;
    gui->focus = gui->View;
}


void GLOBAL::add_ip_to_queue_to_send_mikrotik(unsigned int ip) {
    
    if(is_local_ip(ip)) return;
    
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


bool GLOBAL::test1() {
    //colibrate_mouse();
    
    /*
     2 - Shift
     * 
     11 - H h
     12 - i
     13 - J j
     15 - L l
     */
    
    need_write_serial_5bytes[6] = 0;
        
    need_write_serial_5bytes[1] = 11;
    need_write_serial_5bytes[2] = 8;
    need_write_serial_5bytes[3] = 0x0;
    need_write_serial_5bytes[4] = 0x55;
    need_write_serial_5bytes[0] = 'K';

    uint64_t t;

    t = GetTickCount();      
    while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
        if(t + 2000 < GetTickCount()) return false;
        usleep(1);
    }
    t = GetTickCount();
    while(need_write_serial_5bytes[6] != 0x55 && GLOBAL_STOP == false) {
        if(t+2000 < GetTickCount()) {
            return false;
        };
        usleep(1);
    }
    
    
    return true;
    /*
    int xx = 0, yy = 0, i, j, k=0;
    
    FILE *f;
    f = fopen("/prj/clicker/tt.txt", "rb");
    if(f != NULL)
    {
        i = fgetc(f);
        while(i != EOF) {
            if(i == '\n') {
                k++;
            } else {
                if(i != '\r') {
                    if(i >= '0' && i <= '9') {
                        if(k == 0) {
                            xx *= 10;
                            xx += i-'0';
                        }
                        if(k == 1) {
                            yy *= 10;
                            yy += i-'0';
                        }
                    }
                }
            }
            i = fgetc(f);
        }
        fclose(f);
    }
    
    
    printf("test1 %d %d\n", xx, yy);
    for(int i=0;i<6;i++) need_write_serial_5bytes[i] = 0;

    uint64_t t;
    for(int i=0;i<5;i++)
    {
        need_write_serial_5bytes[6] = 0;
        
        need_write_serial_5bytes[1] = -99;
        need_write_serial_5bytes[2] = -99;
        need_write_serial_5bytes[3] = 0x00;
        need_write_serial_5bytes[4] = i+1;
        need_write_serial_5bytes[0] = 'M';
        
        t = GetTickCount();
        while(need_write_serial_5bytes[0] != 0  && GLOBAL_STOP == false) {
            if(t+2000 < GetTickCount()) return false;
            usleep(1);
        }
        t = GetTickCount();
        while(need_write_serial_5bytes[6] != i+1  && GLOBAL_STOP == false) {
            if(t+2000 < GetTickCount()) return false;
            usleep(1);
        }
        printf("i\n");
    };
    usleep(100);
    while((xx > 0 || yy > 0) && GLOBAL_STOP == false)
    {
        int dx, dy;
        dx = xx; if(dx > 15) dx = 15;
        dy = yy; if(dy > 15) dy = 15;
        
        need_write_serial_5bytes[6] = 0;
        
        need_write_serial_5bytes[1] = dx;
        need_write_serial_5bytes[2] = dy;
        need_write_serial_5bytes[3] = 0x00;
        need_write_serial_5bytes[4] = i+1;
        need_write_serial_5bytes[0] = 'M';
        t = GetTickCount();
        while(need_write_serial_5bytes[0] != 0  && GLOBAL_STOP == false) {
            if(t+2000 < GetTickCount()) return false;
            usleep(1);
        }
        t = GetTickCount();
        while(need_write_serial_5bytes[6] != i+1  && GLOBAL_STOP == false) {
            if(t+2000 < GetTickCount()) return false;
            usleep(1);
        }
        xx -= dx;
        yy -= dy;
        usleep(10);
        //printf("i\n");
    };
    */
}


void GLOBAL::serial_io() {
#ifdef __linux__
    unsigned char c;
    struct termios tty;
    int s;
    if(f_serial == -1) {
        
        f_serial = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
        if (f_serial == -1) f_serial = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);
        if (f_serial == -1) {
            //printf("serial open error\n");
            usleep(2000);
            return;
        }
        
        if(tcgetattr(f_serial, &tty) != 0) {
            printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
            return;
        }
        
            tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
            tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
            tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
            tty.c_cflag |= CS8; // 8 bits per byte (most common)
            tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
            tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

            tty.c_lflag &= ~ICANON;
            tty.c_lflag &= ~ECHO; // Disable echo
            tty.c_lflag &= ~ECHOE; // Disable erasure
            tty.c_lflag &= ~ECHONL; // Disable new-line echo
            tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
            tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
            tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

            tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
            tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
            // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
            // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

            tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
            tty.c_cc[VMIN] = 0;
        
        cfsetispeed(&tty, B57600);
        cfsetospeed(&tty, B57600);
     
        if (tcsetattr(f_serial, TCSANOW, &tty) != 0) {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }
        
        fcntl(f_serial, F_SETFL, FNDELAY);
    }
    if(f_serial != -1) {
        c = 0;
        s = read(f_serial, &c, 1);
        if(s != 0 && s != -1) {
            printf("read = [0x%02x]\n", c);
            need_write_serial_5bytes[6] = c;
        }
        if(need_write_serial_5bytes[0] != 0) {
            s = write(f_serial, need_write_serial_5bytes, 5);
            for(int i=1;i<5;i++) need_write_serial_5bytes[i] = 0;
            printf("write=%d\n", s);
            need_write_serial_5bytes[0] = 0;
        }
        usleep(1);
    }
#endif
    
#ifdef _WIN32
    // TODO 2023
#endif
    
}

bool GLOBAL::MousePress(int mx_, int my_, int mk, int double_click_, int scr_w, int scr_h) {

    if(my_ <= scr_h/2) {
        if(mx_<= scr_w/2) {
            return mouse_move_from_left_top(mx_, my_, mk, double_click_);
        } else {
            return mouse_move_from_right_top(mx_, my_, mk, double_click_);
        }
    } else {
        return mouse_move_from_right_bottom(mx_, my_, mk, double_click_);
    }
    
    return false;
}

bool GLOBAL::KeyPressIosHome() {
    
    need_write_serial_5bytes[6] = 0;
        
    need_write_serial_5bytes[1] = 11;
    need_write_serial_5bytes[2] = 8;
    need_write_serial_5bytes[3] = 0x0;
    need_write_serial_5bytes[4] = 0xac;
    need_write_serial_5bytes[0] = 'K';

    uint64_t t;

    t = GetTickCount();      
    while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
        if(t + 2000 < GetTickCount()) {
            return false;
        }
        usleep(1);
    }
    t = GetTickCount();
    while(need_write_serial_5bytes[6] != 0xac && GLOBAL_STOP == false) {
        if(t+5000 < GetTickCount()) {
            return false;
        };
        usleep(1);
    }

    usleep(50);
    
    return true;
}

bool GLOBAL::KeyPressRaw(int k1, int k2) {

    need_write_serial_5bytes[6] = 0;
        
    need_write_serial_5bytes[1] = k1;
    need_write_serial_5bytes[2] = k2;
    need_write_serial_5bytes[3] = 0x0;
    need_write_serial_5bytes[4] = 0xac;
    need_write_serial_5bytes[0] = 'K';

    uint64_t t;

    t = GetTickCount();      
    while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
        if(t + 2000 < GetTickCount()) return false;
        usleep(1);
    }
    t = GetTickCount();
    while(need_write_serial_5bytes[6] != 0xac && GLOBAL_STOP == false) {
        if(t+2000 < GetTickCount()) {
            return false;
        };
        usleep(1);
    }


    usleep(50);
    
    return true;
}

bool GLOBAL::MouseMove(int mx_, int my_, int scr_w, int scr_h) {

    if(my_ <= scr_h/2) {
        if(mx_<= scr_w/2) {
            return mouse_move_from_left_top(mx_, my_, 0, 0);
        } else {
            return mouse_move_from_right_top(mx_, my_, 0, 0);
        }
    } else {
        return mouse_move_from_right_bottom(mx_, my_, 0, 0);
    }
    
    return false;
}

bool GLOBAL::mouse_to_left_top() {
    uint64_t t;
    for(int i=0;i<10;i++) 
    {
        
        need_write_serial_5bytes[6] = 0;
        
        need_write_serial_5bytes[1] = -90;
        need_write_serial_5bytes[2] = -90;
        need_write_serial_5bytes[3] = 0x0;
        need_write_serial_5bytes[4] = 0xac;
        need_write_serial_5bytes[0] = 'M';

        t = GetTickCount();      
        while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
            if(t + 2000 < GetTickCount()) return false;
            usleep(1);
        }
        t = GetTickCount();
        while(need_write_serial_5bytes[6] != 0xac && GLOBAL_STOP == false) {
            if(t+2000 < GetTickCount()) {
                return false;
            };
            usleep(1);
        }


        usleep(50);
    }
    
    return true;
}

bool GLOBAL::mouse_to_right_top() {
    uint64_t t;
    for(int i=0;i<10;i++) 
    {
        need_write_serial_5bytes[1] = 90;
        need_write_serial_5bytes[2] = -90;
        need_write_serial_5bytes[3] = 0x0;
        need_write_serial_5bytes[4] = 0xac;
        need_write_serial_5bytes[0] = 'M';

        t = GetTickCount();      
        while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
            if(t + 2000 < GetTickCount()) return false;
            usleep(1);
        }
        t = GetTickCount();
        while(need_write_serial_5bytes[6] != 0xac && GLOBAL_STOP == false) {
            if(t+2000 < GetTickCount()) return false;
            usleep(1);
        }


        usleep(50);
    }
    
    return true;
}

bool GLOBAL::mouse_to_left_bottom() {
    uint64_t t;
    for(int i=0;i<10;i++) 
    {
        need_write_serial_5bytes[1] = -90;
        need_write_serial_5bytes[2] = 90;
        need_write_serial_5bytes[3] = 0x0;
        need_write_serial_5bytes[4] = 0xac;
        need_write_serial_5bytes[0] = 'M';

        t = GetTickCount();      
        while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
            if(t + 2000 < GetTickCount()) return false;
            usleep(1);
        }
        t = GetTickCount();
        while(need_write_serial_5bytes[6] != 0xac && GLOBAL_STOP == false) {
            if(t+2000 < GetTickCount()) return false;
            usleep(1);
        }


        usleep(50);
    }
    
    return true;
}

bool GLOBAL::mouse_to_right_bottom() {
    uint64_t t;

    need_write_serial_5bytes[6] = 0;
    need_write_serial_5bytes[1] = -90;
    need_write_serial_5bytes[2] = -90;
    need_write_serial_5bytes[3] = 0x0;
    need_write_serial_5bytes[4] = 0xac;
    need_write_serial_5bytes[0] = 'M';

    usleep(1000);

    for(int i=0;i<10;i++) 
    {
        need_write_serial_5bytes[6] = 0;
        need_write_serial_5bytes[1] = 90;
        need_write_serial_5bytes[2] = 90;
        need_write_serial_5bytes[3] = 0x0;
        need_write_serial_5bytes[4] = 0xac;
        need_write_serial_5bytes[0] = 'M';

        t = GetTickCount();      
        while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
            if(t + 2000 < GetTickCount()) return false;
            usleep(1);
        }
        t = GetTickCount();
        while(need_write_serial_5bytes[6] != 0xac && GLOBAL_STOP == false) {
            if(t+2000 < GetTickCount()) return false;
            usleep(1);
        }


        usleep(50);
    }
    
    return true;
}


bool GLOBAL::mouse_move_from_right_top(int x, int y, int k, int double_click) {
    int mx, my, dx, dy;
    float kx = 1.4;
    float ky = 1.41;
    mx = (300 - x);
    my = y;
    mx = (float)mx * kx;
    my = (float)my * ky;
    
    
    
    printf("== %d %d\n", mx, my);
    
    if( mouse_to_right_top() == false ) {
        printf("mouse_to_right_top fail\n");
    }
 
    uint64_t t;
    int i = 1;
    while(mx > 0 || my > 0)
    {
        if(mx > 15) dx = 15; else dx = mx;
        if(my > 15) dy = 15; else dy = my;
        
        printf("%d move - %d %d\n", i++, dx, dy);
        
        need_write_serial_5bytes[6] = 0;
        
        need_write_serial_5bytes[1] = dx*-1;
        need_write_serial_5bytes[2] = dy;
        need_write_serial_5bytes[3] = 0x0;
        need_write_serial_5bytes[4] = 0xac;
        need_write_serial_5bytes[0] = 'M';

        t = GetTickCount();      
        while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
            if(t + 5000 < GetTickCount()) return false;
            usleep(1);
        }
        t = GetTickCount();
        while(need_write_serial_5bytes[6] != 0xac && GLOBAL_STOP == false) {
            if(t+5000 < GetTickCount()) return false;
            usleep(1);
        }

        mx -= dx;
        my -= dy;

        usleep(50);
    }
    
    if(k == 1) {
        usleep(500);
        mouse_press();
        usleep(500);
        if(double_click == 1) {
            mouse_press();
            usleep(500);
        }
        mouse_to_right_top();
    }
    
    return true;
    
}

bool GLOBAL::mouse_move_from_left_top(int x, int y, int k, int double_click) {
    
    /*need_write_serial_5bytes[1] = 5;
    need_write_serial_5bytes[2] = 5;
    need_write_serial_5bytes[3] = 0x0;
    need_write_serial_5bytes[4] = 0xac;
    need_write_serial_5bytes[0] = 'M';
    
    usleep(50);
    
    need_write_serial_5bytes[1] = -5;
    need_write_serial_5bytes[2] = -5;
    need_write_serial_5bytes[3] = 0x0;
    need_write_serial_5bytes[4] = 0xac;
    need_write_serial_5bytes[0] = 'M';
    
    usleep(50);*/
    
    int mx, my, dx, dy;
    float kx = 1.4;
    float ky = 1.41;
    mx = (float)x * kx;
    my = (float)y * ky;
    
    
    
    printf("== %d %d\n", mx, my);
    
    if( mouse_to_left_top() == false ) {
        printf("mouse_to_left_top fail\n");
    }
    
    uint64_t t;
    int i = 1;
    while(mx > 0 || my > 0)
    {
        if(mx > 15) dx = 15; else dx = mx;
        if(my > 15) dy = 15; else dy = my;
        
        printf("%d move - %d %d\n", i++, dx, dy);
        
        need_write_serial_5bytes[6] = 0;
        
        need_write_serial_5bytes[1] = dx;
        need_write_serial_5bytes[2] = dy;
        need_write_serial_5bytes[3] = 0x0;
        need_write_serial_5bytes[4] = 0xac;
        need_write_serial_5bytes[0] = 'M';

        t = GetTickCount();      
        while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
            if(t + 5000 < GetTickCount()) return false;
            usleep(1);
        }
        t = GetTickCount();
        while(need_write_serial_5bytes[6] != 0xac && GLOBAL_STOP == false) {
            if(t+5000 < GetTickCount()) return false;
            usleep(1);
        }

        mx -= dx;
        my -= dy;

        usleep(50);
    }
    
    if(k == 1) {
        usleep(500);
        mouse_press();
        usleep(500);
        if(double_click == 1) {
            mouse_press();
            usleep(500);
        }
        mouse_to_left_top();
    }
   
    return true;
}

bool GLOBAL::mouse_move_from_right_bottom(int x, int y, int k, int double_click) {
    
    need_write_serial_5bytes[1] = 5;
    need_write_serial_5bytes[2] = 5;
    need_write_serial_5bytes[3] = 0x0;
    need_write_serial_5bytes[4] = 0xac;
    need_write_serial_5bytes[0] = 'M';
    
    usleep(50);
    
    need_write_serial_5bytes[1] = -5;
    need_write_serial_5bytes[2] = -5;
    need_write_serial_5bytes[3] = 0x0;
    need_write_serial_5bytes[4] = 0xac;
    need_write_serial_5bytes[0] = 'M';
    
    usleep(50);
    
    int mx, my, dx, dy;
    float kx = 1.4;
    float ky = 1.41;
    mx = (300 - x);
    my = (540 - y);
    mx = (float)mx * kx;
    my = (float)my * ky;
    
    
    
    printf("== %d %d\n", mx, my);
    
    if( mouse_to_right_bottom() == false ) {
        printf("mouse_to_left_top fail\n");
    }
    
    uint64_t t;
    int i = 1;
    while(mx > 0 || my > 0)
    {
        if(mx > 15) dx = 15; else dx = mx;
        if(my > 15) dy = 15; else dy = my;
        
        printf("%d move - %d %d\n", i++, dx, dy);
        
        need_write_serial_5bytes[6] = 0;
        
        need_write_serial_5bytes[1] = dx*-1;
        need_write_serial_5bytes[2] = dy*-1;
        need_write_serial_5bytes[3] = 0x0;
        need_write_serial_5bytes[4] = 0xac;
        need_write_serial_5bytes[0] = 'M';

        t = GetTickCount();      
        while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {           
            if(t + 5000 < GetTickCount()) return false;
            usleep(1);
        }
        t = GetTickCount();
        while(need_write_serial_5bytes[6] != 0xac && GLOBAL_STOP == false) {
            if(t+5000 < GetTickCount()) return false;
            usleep(1);
        }

        mx -= dx;
        my -= dy;

        usleep(50);
    }
    
    if(k == 1) {
        usleep(500);
        mouse_press();
        usleep(500);
        if(double_click == 1) {
            mouse_press();
            usleep(500);
        }
        mouse_to_right_bottom();
    }
    
    return true;
}

void GLOBAL::mouse_press() {
    
    uint64_t t;
    
    need_write_serial_5bytes[6] = 0;
        
    need_write_serial_5bytes[1] = 1;
    need_write_serial_5bytes[2] = 1;
    need_write_serial_5bytes[3] = 0x01;
    need_write_serial_5bytes[4] = 0xad;
    need_write_serial_5bytes[0] = 'M';

    t = GetTickCount();
    while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {
        if(t+2000 < GetTickCount()) return;
        usleep(1);
    }
    t = GetTickCount();
    while(need_write_serial_5bytes[6] != 0xad && GLOBAL_STOP == false) {
        if(t+2000 < GetTickCount()) return;
        usleep(1);
    }
}

void GLOBAL::mouse_unpress() {
    
    uint64_t t;
    
    need_write_serial_5bytes[6] = 0;
        
    need_write_serial_5bytes[1] = 0;
    need_write_serial_5bytes[2] = 0;
    need_write_serial_5bytes[3] = 0x00;
    need_write_serial_5bytes[4] = 0xad;
    need_write_serial_5bytes[0] = 'M';

    t = GetTickCount();
    while(need_write_serial_5bytes[0] != 0 && GLOBAL_STOP == false) {
        if(t+2000 < GetTickCount()) return;
        usleep(1);
    }
    t = GetTickCount();
    while(need_write_serial_5bytes[6] != 0xad && GLOBAL_STOP == false) {
        if(t+2000 < GetTickCount()) return;
        usleep(1);
    }
}

    