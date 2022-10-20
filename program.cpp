/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "program.h"
#include "global.h"
#include "tools.h"
#include <dirent.h>
#include <algorithm> 
#include "GUI/GUI.h"

extern bool GLOBAL_STOP;
extern GLOBAL global;
extern GUI gui;

void SPRITE::load_from_bmp(std::string& file_name_) {
    file_name = file_name_;
    FILE *f;
    f = fopen(file_name.c_str(), "rb");
    if(f == NULL) {
        printf("open file error\n");
        return;
    }
    unsigned char *b = new unsigned char[500000];
    int sz = fread(b, 1, 500000, f);
    
    load_from_BMP_buffer(b, sz, bitmap, w, h);
    
    fclose(f);
    
    file_name += "_info";
    f = fopen(file_name.c_str(), "rb");
    if(f == NULL) {
        printf("open file error\n");
        return;
    }
    sz = fread(b, 1, 500000, f);
    fclose(f);

    char m[2][100];
    int i, j, k;
    i = 0;
    m[0][0] = 0;
    m[1][0] = 0;
    j = 0;
    k = 0;
    while(i < sz) {
        if(b[i] == '\n') {
            if(m[0][0] == 'x' && m[0][1] == 0) {
                is_detected_x = atoi(m[1]);
            }
            if(m[0][0] == 'y' && m[0][1] == 0) {
                is_detected_y = atoi(m[1]);
            }
            m[0][0] = 0;
            m[1][0] = 0;
            j = 0;
            k = 0;
        } else {
            if(b[i] != '\r') {
                if(b[i] == ':' && k == 0) {
                    k++;
                    j = 0;
                } else {
                    m[k][j] = b[i];
                    if(j < 100-5) j++;
                    m[k][j] = 0;
                }
            }
        }
        i++;
    }
    
    delete[] b;
    
    delta = w*h/1000*5;
    
}

bool it_is_bmp(char *v) {
    if(v == nullptr) return false;
    int i;
    i = strlen(v);
    if(i > 4) {
        if( v[i-1] == 'p' && 
            v[i-2] == 'm' && 
            v[i-3] == 'b' && 
            v[i-4] == '.' 
                )
        {
            return true;
        }
    }
    return false;
}

void PROGRAM::load() {
    
    std::string FolderName, fn;
    
    FolderName = "progs/prg" + std::to_string(idx);
    
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
             (c[0] == '.' && c[1] == '.' && c[2] == 0)) &&
              entry->d_type != DT_DIR &&
              it_is_bmp(entry->d_name)
            )
          
        {
            fn = FolderName;
            fn += "/";
            fn += entry->d_name;
            
            sprite.push_back({fn});
            //gui->FilesList->item.push_back({gui, ELEMENT_item::ListItem, entry->d_name});
        }
        
    };
    
   closedir(dir);
   
   std::sort(sprite.begin(), sprite.end());
   
   load_text(FolderName);
   
   compile();
   
   run();
}

void PROGRAM::load_text(std::string& FolderName) {
    std::string fn, ln;
    fn = FolderName + "/.txt";
    char *b;
    int sz = 0, i;
    b = new char[100000];
    FILE *f;
    f = fopen(fn.c_str(), "rb");
    if(f != NULL) {
        sz = fread(b, 1, 100000, f);
        fclose(f);
    }
    i = 0;
    int ch;
    while(i < sz) {
        ch = b[i];
        
        
        if(ch == '\n') {
            if(ln != "") {
                line.push_back({ln});
            }
            ln = "";
        } else {
            if(ch != '\r') {
                
                    if(ch == 9) {
                        ln += "        ";
                    } else {
                        ln += ch;
                    }
                
            }
        }
        i++;
    }
    if(ln != "") {
        line.push_back({ln});
    }
    delete[] b;
}

int PROGRAM::get_else_idx(int v) {

    int i, lvl = 0;
    i = v+1;
    while(i < line.size()) {
        if(line[i].cmd == CMD::If) {
            lvl++;
        } else if(line[i].cmd == CMD::Else && lvl == 0) {
            return line[i].idx;
        } else if(line[i].cmd == CMD::Endif && lvl == 0) {
            return line[i].idx;
        } else if(line[i].cmd == CMD::Endif && lvl > 0){
            lvl--;
        }
        i++;
    }
    
    return -1;
}

int PROGRAM::get_label_idx(std::string s) {
    std::string ss;
    ss = ":" + s;
    int i;
    i = 0;
    while(i < line.size()) {
        
        if(line[i].s1 == ss) return line[i].idx;
        i++;
    }
    
    return -1;
}

void PROGRAM::compile() {
    int i = 0;
    while(i < line.size()) {
        line[i].idx = i+1;
        
        i++;
    }
    std::string s, w;
    
    i = 0;
    while(i < line.size()) {
    
        line[i].s1 = "";
        line[i].s2 = "";
        line[i].s3 = "";
        line[i].s4 = "";
        
        s = line[i].raw;
        s = rl_trim(s);
        s = remove_double_space(s);
        if(s != "") {
            line[i].s1 = get_word(s);
            s = del_word(s);
        }
        if(s != "") {
            line[i].s2 = get_word(s);
            s = del_word(s);
        }
        if(s != "") {
            line[i].s3 = get_word(s);
            s = del_word(s);
        }
        if(s != "") {
            line[i].s4 = s;
        }
        line[i].decode_CMD();
        line[i].next_idx = line[i].idx + 1;
        
        
        i++;
    }
    
    i = 0;
    while(i < line.size()) {
    
        if(line[i].cmd == CMD::Stop) {
            line[i].next_idx = -1;
        }
        
        if(line[i].cmd == CMD::Goto) {
            line[i].next_idx = get_label_idx(line[i].s2);
        }
        if(line[i].cmd == CMD::If) {
            line[i].else_idx = get_else_idx(i);
        }
        if(line[i].cmd == CMD::Else) {
            line[i].else_idx = get_else_idx(i);
        }
        i++;
    }
    
    i = 0;
    while(i < line.size()) {
        s = std::to_string(line[i].idx);
        while(s.length() < 3) s = "0" + s;
        if(line[i].cmd == CMD::Undef) {
            line[i].info = s + " - ??? " + line[i].raw ;
        } else {
            if(line[i].cmd == CMD::If || line[i].cmd == CMD::Else) {
                line[i].info = s + " - (" + std::to_string(line[i].next_idx) + ","+std::to_string(line[i].else_idx)+") " + line[i].s1 + "|" + line[i].s2 + "|" + line[i].s3 + "|" + line[i].s4;
            } else {
                line[i].info = s + " - (" + std::to_string(line[i].next_idx) + ") " + line[i].s1 + "|" + line[i].s2 + "|" + line[i].s3 + "|" + line[i].s4;
            }
        };
        
        i++;
    }
}

void PROGRAMS::init() {
    item_.emplace_back(PROGRAM(0));
    item_[0].load();
    execute_is_run = true;
    execute_thread = new std::thread(&PROGRAMS::execute, this);
}

void PROGRAMS::execute() {
    Window w = 0;
    while(GLOBAL_STOP == false) {
        
        if(global.WindowListBtnStart) {
            Grab_Sprites(global.WindowListId);
        } else {
            
        }
        for(auto& p : item_) {
            if(p.need_run) {
                p.need_run = false;
                p.is_run = true;
                p.next_step = 0;
                p.init();
            }
            if(p.need_stop) {
                p.need_stop = false;
                p.is_run = false;
            }
            if(p.is_run) {
                p.execute_next_step();
            };
        }
        usleep(100);
    }
    execute_is_run = false;
}

void PROGRAMS::wait_execute_close() {
    while(execute_is_run == true) {
        usleep(10);
    }
}

unsigned int avg(unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4) {
    unsigned int r, g, b;
    r = ((v1 & 0xff) + (v2 & 0xff) + (v3 & 0xff) + (v4 & 0xff)) / 4;
    g = (((v1 & 0xff00) + (v2 & 0xff00) + (v3 & 0xff00) + (v4 & 0xff00))>>8) / 4;
    b = (((v1 & 0xff0000) + (v2 & 0xff0000) + (v3 & 0xff0000) + (v4 & 0xff0000))>>16) / 4;

    return r | (g<<8) | (b<<16);
}


void PROGRAMS::Grab_Sprites(Window w) {

    if(w == 0) {
        return;
    }
    XWindowAttributes window_attr;
    Display *d = XOpenDisplay(getenv("DISPLAY"));

    XImage      *imageS;

    //d = gui.get_display();
    
    if(!XGetWindowAttributes(d , w, &window_attr)) {
        printf("XGetWindowAttributes(0 false\n");
        return;
    }
    
    
    if(window_attr.width/2 != grab_screen_buffer.w || window_attr.height/2 != grab_screen_buffer.h) {
        //g_mutex.lock();
        grab_screen_buffer.set_size(window_attr.width/2, window_attr.height/2);
        //g_mutex.unlock();
    };

    imageS = XGetImage(d, w, 0, 0, window_attr.width, window_attr.height, AllPlanes, ZPixmap);

    if(imageS == nullptr) {
        w = 0;
    } else {

        unsigned int *qq, *q;
        qq = (unsigned int *)imageS->data;
        q = grab_screen_buffer.buffer;
        int k = 0;
        for(int j=0;j<window_attr.height/2;j++) {

            for(int i=0;i<window_attr.width/2;i++) {
                *q++ = avg( *qq, *(qq+1) , *(qq+window_attr.width), *(qq+window_attr.width+1));
                qq+=2;
            }
            qq += window_attr.width;
            //q += window_attr.width/2;
        }

        XFree(imageS);
    };
    
    XCloseDisplay(d);
}

void PROGRAM::detect_sprites(SCREEN *src) {
    if(src == nullptr) return;
    
    //sprite[2].detect_sprite(src);
    
    for(auto& s : this->sprite) {
        s.detect_sprite(src);
    }
}

bool eq_byte_soft(unsigned int p1, unsigned int p2) {
    unsigned int r1, r2, g1, g2, b1, b2;
    r1 = p1 & 0xff;
    r2 = p2 & 0xff;
    
    g1 = (p1 & 0xff00)>>8;
    g2 = (p2 & 0xff00)>>8;
    
    b1 = (p1 & 0xff0000)>>16;
    b2 = (p2 & 0xff0000)>>16;
    
    if(r1 < r2) {
        if(r1 + 25 < r2) {
            return false;
        }
    } else {
        if(r2 + 25 < r1) {
            return false;
        }
    }
    
    if(g1 < g2) {
        if(g1 + 25 < g2) {
            return false;
        }
    } else {
        if(g2 + 25 < g1) {
            return false;
        }
    }
    
    if(b1 < b2) {
        if(b1 + 25 < b2) {
            return false;
        }
    } else {
        if(b2 + 25 < b1) {
            return false;
        }
    }
    
    return true;
}

int max_p1 = 0;
bool SPRITE::eq(SCREEN *src, int p_x, int p_y) {
    if(src->buffer == nullptr ||
       src->w == 0 || 
       src->h == 0
      ) 
    {
        return false;
    }
    int x, y, i, p1 = 0, p2 = 0;
    unsigned int *q;
    
    i = 0;
    y = 0;
    while(y < h) {
        
        q = src->buffer + ((p_y + y)*src->w + p_x);
        
        x = 0;
        while(x < w) {
            if(bitmap[i] == *q) {
                p1++;
            } else {
                if( eq_byte_soft(bitmap[i], *q) ) {
                    p1++;
                } else {
                    p2++;
                    if(p2 > delta) {
                        return false;
                    }
                }
            }
            i++;
            q++;
            x++;
        }
        y++;
    }
    if(p2 < delta) {
        is_detected = true;
        is_detected_x = p_x;
        is_detected_y = p_y;
        return true;
    }
    if(p1 > max_p1) {
        max_p1 = p1;
        
    }
    return false;
}

bool SPRITE::detect_sprite(SCREEN *src) {
    
    
    if(eq(src, is_detected_x, is_detected_y)) {
        is_detected = true;
        //is_detected_x = is_detected_x;
        //is_detected_y = is_detected_y;
    } else {
        is_detected = false;
    }
    
    /*int x, y, xx, yy;
    
    y = 0;
    yy = src->h - h;
    while( y < yy ) {
    
        x = 0;
        xx = src->w - w;
        while(x < xx) {
            
            if(eq(src, x, y)) {
                
                is_detected = true;
                is_detected_x = x;
                is_detected_y = y;
                return true;
            }
            x++;
        }
        y++;
    };
    
    if(is_detected == true) {
        is_detected = false;
        is_detected_x = -1;
        is_detected_y = -1;
        return false;
    }*/
}

void PROGRAM::print(char *s) {
    
}

void PROGRAM::execute_next_step() {
    if(next_step == 0) next_step = 1;
    if(next_step > line.size()) {
        print((char *)"next_step >= line.size() - STOP");
        stop();
        return;
    }
    int n;
    n = next_step - 1;
    if(line[n].idx != next_step) {
        print((char *)"line[n].idx != next_step - STOP");
        stop();
        return;
    }
    if(line[n].cmd == CMD::Set) {
        exec_set(line[n].s2, line[n].s3, line[n].s4);
        next_step++;
        return;
    }
}

bool PROGRAM::it_is_timer(std::string name) {
    if(name == "timer0" ||
       name == "timer1" ||
       name == "timer2" ||
       name == "timer3" ||
       name == "timer4" ||
       name == "timer5" ||
       name == "timer6" ||
       name == "timer7" ||
       name == "timer8" ||
       name == "timer9"
      )
    {
        return true;
    }
    return false;
}

void PROGRAM::set_timer(std::string name, std::string val) {
    int idx = -1;
    if(name == "timer0") idx = 0;
    if(name == "timer1") idx = 1;
    if(name == "timer2") idx = 2;
    if(name == "timer3") idx = 3;
    if(name == "timer4") idx = 4;
    if(name == "timer5") idx = 5;
    if(name == "timer6") idx = 6;
    if(name == "timer7") idx = 7;
    if(name == "timer8") idx = 8;
    if(name == "timer9") idx = 9;

    if(idx == -1) {
        print((char *)"set_timer() error idx==-1");
        return;
    }
    
    ttimer[idx] = my_atoi(val.c_str());
        
}

std::string PROGRAM::calc_value(std::string s) {
    
    std::string s1, s2, s3;
    if(s != "") {
        s1 = get_word(s);
        s = del_word(s);
    }
    if(s != "") {
        s2 = get_word(s);
        s = del_word(s);
    }
    if(s != "") {
        s3 = get_word(s);
        s = del_word(s);
    }
    
    if(s2 == "+") {
        int r;
        r = my_atoi(s1.c_str()) + my_atoi(s3.c_str());
        return std::to_string(r);
    }
    
    return s;
}


void PROGRAM::exec_set(std::string v1, std::string v2, std::string v3) {
    
    if(v2 != "=") {
        print((char *)"v2 != '='");
        stop();
        return;
    }
    
    if(it_is_timer(v1)) {
        set_timer(v1, v3);
        return;
    }
    
    auto it = vars.find(v1);
    if(it == vars.end()) {
        vars.insert(std::make_pair(v1, ""));
        it = vars.find(v1);
        if(it == vars.end()) {
            return;
        }        
    }
    
    it->second = calc_value(v3);
}