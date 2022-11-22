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
    file__name = file_name_;
    FILE *f;
    f = fopen(file__name.c_str(), "rb");
    if(f == NULL) {
        printf("open file error\n");
        return;
    }
    unsigned char *b = new unsigned char[500000];
    int sz = fread(b, 1, 500000, f);
    
    load_from_BMP_buffer(b, sz, bitmap, w, h);
    
    fclose(f);
    
    file__name += "_info";
    f = fopen(file__name.c_str(), "rb");
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
            if(my_strcmp(m[0], 100, "mouse_press_target_percent_w")) {
                mouse_press_target_percent_w = atoi(m[1]);
            }
            if(my_strcmp(m[0], 100, "mouse_press_target_percent_h")) {
                mouse_press_target_percent_h = atoi(m[1]);
            }
            if(my_strcmp(m[0], 100, "double_click")) {
                double_click = atoi(m[1]);
            }
            if(my_strcmp(m[0], 100, "nic")) {
                nic = m[1];
            }
            if(my_strcmp(m[0], 100, "nopress")) {
                nopress = atoi(m[1]);
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
    
    delta = (float)(w*h*5)/(float)300;
    if(delta == 0) delta = 1;
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

    detail = "load...";

    clear();
    
    std::string FolderName, fn;
    
    FolderName = "progs/prg" + std::to_string(idx);
    
    DIR *dir;
    int k, sprite_idx;
    char *c;
    dir = opendir(FolderName.c_str());
    struct dirent *entry;
    if(dir == 0) return;
    sprite_idx = 1;
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
            
            sprite.push_back({fn, sprite_idx++});
            //gui->FilesList->item.push_back({gui, ELEMENT_item::ListItem, entry->d_name});
        }
        
    };
    
   closedir(dir);
   
   std::sort(sprite.begin(), sprite.end());
   
   detail = "load text...";
   load_text(FolderName);
   
   detail = "compile...";
   compile();
   
   detail = "lines=" + std::to_string(line.size()) + " sprites=" + std::to_string(sprite.size());
   //run();
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
            ln = rl_trim(ln);
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
    ln = rl_trim(ln);
    if(ln != "") {
        line.push_back({ln});
    }
    delete[] b;
}

std::string PROGRAM::getDetectedSprite() {
   
    std::string r;
    int i;
    i = 0;
    while(i < sprite.size()) {
        if(sprite[i].is_detected) {
            r = "sprite$" + std::to_string(sprite[i].idx);
            return r;
        }
        i++;
    }
    
    return "no";
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
    //item_[0].load();
    
    execute_is_run = true;
    execute_thread = new std::thread(&PROGRAMS::execute, this);
    
    execute_timers_is_run = true;
    execute_timers_thread = new std::thread(&PROGRAMS::execute_timers, this);
    
    
}

void PROGRAMS::execute() {
    Window w = 0;
    while(GLOBAL_STOP == false) {
        
        if(global.WindowListBtnStart) {
            //Grab_Sprites(global.WindowListId);
        } else {
            
        }
        for(auto& p : item_) {
            if(p.need_run) {
                p.need_run = false;
                p.is_run_program = true;
                p.next_step = 0;
                p.init();
            }
            if(p.need_stop) {
                p.need_stop = false;
                p.is_run_program = false;
            }
            if(p.is_run_program) {
                p.execute_next_step();
            };
        }
        usleep(100);
    }
    execute_is_run = false;
}

void PROGRAMS::execute_timers() {
    
    while(GLOBAL_STOP == false) {
        
        for(auto& p : item_) {
            p.timers_increase();
        }
    
        usleep(100);
    }
    execute_is_run = false;
}


void PROGRAMS::wait_execute_close() {
    while(execute_is_run == true) {
        usleep(10);
    }
    while(execute_timers_is_run == true) {
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

/*
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
*/
void PROGRAM::detect_sprites(SCREEN *src) {
    if(src == nullptr || src->buffer == nullptr || src->w == 0 || src->h == 0) return;
    bool is_mouse_pointer_detect = false;
    int min_x=9999, min_y=9999, max_x=0, max_y=0;
    unsigned int *q, c1, c2, c3, a, aa = 0;
    /*
    for(int yy=0;yy<540;yy++) {
        q = src->buffer + yy*src->w;
        a = 0;
        for(int xx=0;xx<300;xx++) {
            c1 = (*q & 0xff0000)>>16;
            c2 = (*q & 0xff00)>>8;
            c3 = (*q & 0xff);
            //if(c1 == 0xff && c2 >= 0x15 && c2 <= 0x1e && c3 >= 0x20 && c3 <= 0x2f) {
            if(c1 >= 0xF0 && c1 <= 0xFF && c2 >= 0x90 && c2 <= 0x9f && c3 == 0) {
                    a++;
                    *q++ = 0xffff00;
                    if(xx < min_x) min_x = xx;
                    if(xx > max_x) max_x = xx;
                    if(yy < min_y) min_y = yy;
                    if(yy > max_y) max_y = yy;
            } else {
                //*q++ = 0xff0000;
                q++;
            }
        }
        //if(a > 15) break;
        aa += a;
        
    }

    if(aa > 10) {
        //printf("ccccccc\n");
    }
    
    if(aa >= 103 && aa <= 150) {
        if(max_x - min_x >= 18 &&
           max_x - min_x <= 22 &&
           max_y - min_y >= 18 &&
           max_y - min_y <= 22)
        {
        
            src->mouse_pointer_detect_x = min_x + (max_x - min_x)/2;
            src->mouse_pointer_detect_y = min_y + (max_y - min_y)/2;
            src->mouse_pointer_detect = GetTickCount();
            is_mouse_pointer_detect = true;
            //printf("111 %d %d\n", max_x - min_x, max_y - min_y);
        }
    } else {
        if(aa >= 29 && aa <= 36) {
            if(min_x >= 0 && min_x <= 1 &&
               max_x >= 9 && max_x <= 11 && 
               min_y >= 0 && min_y <= 1 &&
               max_y >= 9 && max_y <= 11)
            {
                //printf("333\n");
                src->mouse_pointer_detect_x = min_x;
                src->mouse_pointer_detect_y = min_y;
                src->mouse_pointer_detect = GetTickCount();
                is_mouse_pointer_detect = true;
            } else {
             
                if(min_x >= 286 && min_x <= 291 &&
                   max_x >= 294 && max_x <= 300 && 
                   min_y >= 0 && min_y <= 1 &&
                   max_y >= 9 && max_y <= 11)
                {
                    //printf("444\n");
                    src->mouse_pointer_detect_x = max_x;
                    src->mouse_pointer_detect_y = min_y;
                    src->mouse_pointer_detect = GetTickCount();
                    is_mouse_pointer_detect = true;
                } else {
                    if( min_x >= 0 && min_x <= 1 &&
                        max_x >= 9 && max_x <= 11 && 
                        min_y >= 528 && min_y <= 531 &&
                        max_y >= 538 && max_y <= 540)
                    {
                        
                        src->mouse_pointer_detect_x = min_x;
                        src->mouse_pointer_detect_y = max_y;
                        src->mouse_pointer_detect = GetTickCount();
                        is_mouse_pointer_detect = true;
                    } else {
                        if( min_x >= 286 && min_x <= 290 &&
                            max_x >= 294 && max_x <= 300 && 
                            min_y >= 528 && min_y <= 531 &&
                            max_y >= 538 && max_y <= 540)
                        {
                            //printf("ssss\n");    
                            src->mouse_pointer_detect_x = max_x;
                            src->mouse_pointer_detect_y = max_y;
                            src->mouse_pointer_detect = GetTickCount();
                            is_mouse_pointer_detect = true;
                        }
                    }
                }
            
            }
        }
    }
    */
    if(is_mouse_pointer_detect) {
        printf("MOUSE detect %d %d\n", src->mouse_pointer_detect_x, src->mouse_pointer_detect_y);
    } else {
        printf("MOUSE not detect\n");
    }
    
    //sprite[2].detect_sprite(src);
    int x = 0;
    for(auto& s : this->sprite) {
        if(s.detect_sprite(src)) {
            x = s.idx;
        }
    }
    sprite_detected_idx = x;
    src->detect_sprite_end++;
}

bool eq_byte_soft(unsigned int p1, unsigned int p2) {
    unsigned int r1, r2, g1, g2, b1, b2, delta = 5;
    r1 = p1 & 0xff;
    r2 = p2 & 0xff;
    
    g1 = (p1 & 0xff00)>>8;
    g2 = (p2 & 0xff00)>>8;
    
    b1 = (p1 & 0xff0000)>>16;
    b2 = (p2 & 0xff0000)>>16;
    
    if(r1 < r2) {
        if(r1 + delta < r2) {
            return false;
        }
    } else {
        if(r2 + delta < r1) {
            return false;
        }
    }
    
    if(g1 < g2) {
        if(g1 + delta < g2) {
            return false;
        }
    } else {
        if(g2 + delta < g1) {
            return false;
        }
    }
    
    if(b1 < b2) {
        if(b1 + delta < b2) {
            return false;
        }
    } else {
        if(b2 + delta < b1) {
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
    if(p2 < delta || p2 == 0) {
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
    return is_detected;
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
    print_out_add(s);
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
        next_step = line[n].next_idx;
        return;
    }
    if(line[n].cmd == CMD::Print) {
        exec_print(line[n].s2, line[n].s3, line[n].s4);
        next_step = line[n].next_idx;
        return;
    }
    if(line[n].cmd == CMD::Label) {
        next_step = line[n].next_idx;
        return;
    }
    if(line[n].cmd == CMD::If) {
        bool b;
        b = calc_boolean(line[n].s2 + line[n].s3 + line[n].s4);
        if(b == true) {
            next_step = line[n].next_idx;
        } else {
            next_step = line[n].else_idx;
        }
        return;
    }
    if(line[n].cmd == CMD::Endif) {
        next_step = line[n].next_idx;
        return;
    }
    if(line[n].cmd == CMD::Else) {
        next_step = line[n].next_idx;
        return;
    }
    if(line[n].cmd == CMD::Stop) {
        next_step = line[n].next_idx;
        stop();
        return;
    }
    if(line[n].cmd == CMD::Goto) {
        next_step = line[n].next_idx;
        return;
    }
    if(line[n].cmd == CMD::Comment) {
        next_step = line[n].next_idx;
        return;
    }
    if(line[n].cmd == CMD::MousePress) {
        exec_MousePress(line[n].s2, line[n].s3, 1);
        next_step = line[n].next_idx;
        return;
    }
    if(line[n].cmd == CMD::MouseMove) {
        exec_MouseMove(line[n].s2, line[n].s3);
        next_step = line[n].next_idx;
        return;
    }
    if(line[n].cmd == CMD::KeyPressRaw) {
        exec_KeyPressRaw(line[n].s2, line[n].s3);
        next_step = line[n].next_idx;
        return;
    }
    int rr;
    rr = 11;
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

std::string PROGRAM::get_timer(std::string name) {
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
        return "?";
    }
    
    return std::to_string(ttimer[idx]/10);    
}

std::string PROGRAM::calc_value(std::string v1, std::string v2, std::string v3) {
    
    if(v2 == "" && v3 == "") {
        return calc_value(v1);
    }
    
    std::string s1, s3;
    s1 = calc_value(v1);
    s3 = calc_value(v3);
    if(v2 == "+") {
        if(it_is_integer(s1) && it_is_integer(s3)) {
            return std::to_string( my_atoi(s1.c_str()) + my_atoi(s3.c_str()) );
        }
        return s1+s3;
    }
    
    return v1 + v2 + v3;
}

std::string PROGRAM::calc_value(std::string s) {

    s = rl_trim(s);

    if(s == "getDetectedSprite()") {
        
        return getDetectedSprite();
    }
    
    /**/
    
    std::string s1, s2, s3, v1, v3;
    
    /*if(s.length()>1) {
        if(s[0] == '\"') {
            if(s[s.length()-1] == '\"') {
                s1 = s.substr(1, s.length()-2);
                return s1;
            }
        }
    }*/
    
    
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
    
    if(s2 == "" && s3 == "") {
        if(it_is_timer(s1)) {
            return get_timer(s1);
        }
        if(it_is_var(s1)) {
            std::string v;
            v = get_var(s1);
            if(it_is_sprite(v)) {
                return get_sprite_value(v);
            }
            return v;
        }
        return s1;
    }
    
    if(s2 == "+") {
        int r;
        v1 = calc_value(s1);
        v3 = calc_value(s3);
        
        if(it_is_integer(v1) && it_is_integer(v3)) {        
            r = my_atoi(v1.c_str()) + my_atoi(v3.c_str());
            return std::to_string(r);
        } else {            
            wtf("?");
        }
    }
    
    return s;
}

bool PROGRAM::it_is_sprite_name(std::string name) {
    int i = 0;
    while(i < sprite.size()) {
        if(sprite[i].nic == name) return true;
        i++;
    }
    return false;
}

bool PROGRAM::it_is_var(std::string name) {
    auto it = vars.find(name);
    if(it == vars.end()) {
        return false;        
    }    
    return true;
}
    
std::string PROGRAM::get_var(std::string name) {
    auto it = vars.find(name);
    if(it == vars.end()) {
        return "?";
    }    
    return it->second;
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

void PROGRAM::print_out_lock() {
    //print_out_mutex.lock();
}

void PROGRAM::print_out_unlock() {
    //print_out_mutex.unlock();
}

void PROGRAM::print_out_add(std::string s) {
    
    if(s == "") return;
    
    std::string tt;
    
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    struct tm       *tm;
    char buf[100];
    buf[0] = 0;
    if((tm = localtime(&tv.tv_sec)) != NULL)
    {
        strftime(buf, 100, "[%H:%M:%S] ", tm);
    };
    tt += buf;
    tt += s;
    if(print_out.size() < 20) {
        print_out.push_back(tt);
    } else {
        for(int i=0;i<20-1;i++) {
            print_out[i] = print_out[i+1];
        }
        print_out[19] = tt;
    }
}

void PROGRAM::exec_MousePress(std::string v1, std::string v2, int mk) {
    std::string x1, x2;
    if(v1.length() > 0 && v2.length() > 0) {
        
        x1 = calc_value(v1);
        x2 = calc_value(v2);
        if(it_is_integer(x1) && it_is_integer(x2)) {
            global.MousePress(my_atoi(x1.c_str()), my_atoi(x2.c_str()), mk, 0, 300, 540);
            return;
        }
    }
       
    if(v1.length() > 0 && v2.length() == 0 && it_is_var(v1)) {
        x1 = get_var(v1);
        if(it_is_sprite(x1)) {
            int x = -1, y = -1, double_click = -1;
            if(get_XY_from_sprite_by_idx(x1, x, y, double_click)) {
                global.MousePress(x, y, mk, double_click, 300, 540);
                return;
            }
        }        
    }
    
    if(v1.length() > 0 && v2.length() == 0 && it_is_sprite_name(v1)) {
        printf("ssssss\n");
        
        int x = -1, y = -1, double_click = -1;
        if(get_XY_from_sprite_by_name(v1, x, y, double_click)) {
            global.MousePress(x, y, mk, double_click, 300, 540);
            return;
        }
        
        /*x1 = get_var(v1);
        if(it_is_sprite(x1)) {
            int x = -1, y = -1, double_click = -1;
            if(get_XY_from_sprite(x1, x, y, double_click)) {
                global.MousePress(x, y, mk, double_click, 300, 540);
                return;
            }
        }*/        
    }
    
}

void PROGRAM::exec_KeyPressRaw(std::string v1, std::string v2) {
    std::string x1, x2;
    if(v1.length() > 0 && v2.length() > 0) {
        
        x1 = calc_value(v1);
        x2 = calc_value(v2);
        if(it_is_integer(x1) && it_is_integer(x2)) {
            global.KeyPressRaw(my_atoi(x1.c_str()), my_atoi(x2.c_str()));
            return;
        }
    }
}

void PROGRAM::exec_MouseMove(std::string v1, std::string v2) {
    std::string x1, x2;
    if(v1.length() > 0 && v2.length() > 0) {
        
        x1 = calc_value(v1);
        x2 = calc_value(v2);
        if(it_is_integer(x1) && it_is_integer(x2)) {
            global.MouseMove(my_atoi(x1.c_str()), my_atoi(x2.c_str()), 300, 540);
            return;
        }
    }
       
    if(v1.length() > 0 && v2.length() == 0 && it_is_var(v1)) {
        x1 = get_var(v1);
        if(it_is_sprite(x1)) {
            int x = -1, y = -1, double_click = -1;
            if(get_XY_from_sprite_by_idx(x1, x, y, double_click)) {
                global.MouseMove(x, y, 300, 540);
                return;
            }
        }        
    }
    
    if(v1.length() > 0 && v2.length() == 0 && it_is_sprite_name(v1)) {
        printf("ssssss\n");
        
        int x = -1, y = -1, double_click = -1;
        if(get_XY_from_sprite_by_name(v1, x, y, double_click)) {
            global.MouseMove(x, y, 300, 540);
            return;
        }
        
        /*x1 = get_var(v1);
        if(it_is_sprite(x1)) {
            int x = -1, y = -1, double_click = -1;
            if(get_XY_from_sprite(x1, x, y, double_click)) {
                global.MousePress(x, y, mk, double_click, 300, 540);
                return;
            }
        }*/        
    }
    
}

void PROGRAM::exec_print(std::string v1, std::string v2, std::string v3) {
    std::string s;
    s = calc_value(v1, v2, v3);
    print_out_add(s);
}

bool PROGRAM::get_XY_from_sprite_by_name(std::string s,int &x, int &y, int &double_click) {
    x = -1;
    y = -1;
    double_click = -1;

    int i = 0;
    while(i < sprite.size()) {
        if(sprite[i].nic == s) {
            if(sprite[i].nopress == 1) return true;
            x = sprite[i].is_detected_x + ((float)sprite[i].w/(float)100 * (float)sprite[i].mouse_press_target_percent_w);
            y = sprite[i].is_detected_y + ((float)sprite[i].h/(float)100 * (float)sprite[i].mouse_press_target_percent_h);
            double_click = sprite[i].double_click;
            
            return true;
        }
        i++;
    }
    
    return false;
}

bool PROGRAM::get_XY_from_sprite_by_idx(std::string s,int &x, int &y, int &double_click) {
    x = -1;
    y = -1;
    double_click = -1;
    if(s.length() >= 8) {
        if(s.substr(0, 7) == "sprite$") {
            SPRITE *ss = nullptr;
            std::string n;
            n = s.substr(7, 255);
            if(it_is_integer(n)) {
                int nn;
                nn = my_atoi(n.c_str());
                for(int i=0;i<sprite.size();i++) {
                    if(sprite[i].idx == nn) {
                        ss = &sprite[i];
                    }
                }
            } else {
                for(int i=0;i<sprite.size();i++) {
                    if(sprite[i].nic == n) {
                        ss = &sprite[i];
                    }
                }
            }
            
            if(ss != nullptr) {
                if(ss->nopress == 1) return true;
                x = ss->is_detected_x + ((float)ss->w/(float)100 * (float)ss->mouse_press_target_percent_w);
                y = ss->is_detected_y + ((float)ss->h/(float)100 * (float)ss->mouse_press_target_percent_h);
                double_click = ss->double_click;
                return true;
            };
        }
    }
    
    return false;
}

std::string PROGRAM::get_sprite_value(std::string s) {
    
    std::string v;

    if(s.length() >= 8) {
        if(s.substr(0, 7) == "sprite$") {
            SPRITE *ss = nullptr;
            std::string n;
            n = s.substr(7, 255);
            if(it_is_integer(n)) {
                int nn;
                nn = my_atoi(n.c_str());
                for(int i=0;i<sprite.size();i++) {
                    if(sprite[i].idx == nn) {
                        ss = &sprite[i];
                    }
                }
            } else {
                for(int i=0;i<sprite.size();i++) {
                    if(sprite[i].nic == n) {
                        ss = &sprite[i];
                    }
                }
            }
            
            if(ss == nullptr) {
                v = "null";
            } else {            
                v = ss->print();
            };
        }
    }
    
    
    return v;
}

bool PROGRAM::it_is_sprite(std::string s) {
    if(s.length() >= 8) {
        if(s.substr(0, 7) == "sprite$") {    
            return true;
        }
    }
    return false;
}

bool PROGRAM::it_is_integer(std::string v) {
    if(v.length() == 0) return false;
    int i = 0;
    if(v[i] == '-') i++;
    while(i < v.length()) {
        if(v[i]<'0' || v[i]>'9') return false;
        i++;
    }
    return true;
}

void split_by_znak(std::string s, std::string &l, std::string &z, std::string &r) {
    l = "";
    z = "";
    r = "";
    int i;
    i = 0;
    while(i < s.length()) {
        if(i+1<s.length()) {
            if(s[i] == '=' && s[i+1] == '=') {
                z = "==";
                l = s.substr(0, i);
                r = s.substr(i+1, s.length());
                return;
            }
            if(s[i] == '=' || 
               s[i] == '<' ||
               s[i] == '>' 
              ) 
            {
                z = s[i];
                l = s.substr(0, i);
                r = s.substr(i+1, s.length());
                return;
            }
        }
        i++;
    }
    l = s;
}

void split_function_name_and_param(std::string s, std::string &fn, std::string &p1) {
    
    fn = "";
    p1 = "";
    
    int i1, i2;
    i1 = s.find('(');
    if(i1 > 1) {
        i2 = s.find(')');
        if(i2 > i1) {
            fn = rl_trim(s.substr(0, i1));
            p1 = rl_trim(s.substr(i1+1, i2-i1-1));
        }
    }
    
}

bool it_is_function(std::string s) {
    
    std::string fn, p1;
    
    split_function_name_and_param(s, fn, p1);
    if(fn != "") return true;
    return false;
}

std::string PROGRAM::calc_value_function_sprite_by_nic_is_detected(std::string p) {
    int i = 0;
    while(i<sprite.size()) {
        if(sprite[i].nic == p) {
            if(sprite[i].is_detected) {
                return "true";
            } else {
                return "false";
            }
        }
        i++;
    }
    return "";
}

std::string PROGRAM::calc_value_function(std::string e) {
    std::string fn, p1;
    
    split_function_name_and_param(e, fn, p1);
    
    if(fn == "sprite_by_nic_is_detected") {
        return calc_value_function_sprite_by_nic_is_detected(p1);
    }
    
    return "";
}

bool PROGRAM::calc_boolean(std::string s) {
 
    std::string val1, val3;
    std::string l, z, r;
    split_by_znak(s, l, z, r);
    
    if(z == "" && it_is_function(l)) {
        val1 = calc_value_function(l);
        if(val1 == "true") {
            return true;
        }
        return false;
    }
    
    if(z == ">" ||
       z == "<" ||
       z == "=" 
      )
    {
        val1 = calc_value(l);
        val3 = calc_value(r);
    
        if(it_is_integer(val1) && it_is_integer(val3)) {
            int i1, i3;
            i1 = my_atoi(val1.c_str());
            i3 = my_atoi(val3.c_str());

            if(z == "<") return i1 < i3;
            if(z == ">") return i1 > i3;
            if(z == "==") return i1 == i3;
            if(z == "=") return i1 == i3;
            if(z == "!=") return i1 != i3;
            if(z == "<=") return i1 <= i3;
            if(z == ">=") return i1 >= i3;

            return false;
        }
        
    }
    
    /*
    
    
    val1 = calc_value(v1);
    val3 = calc_value(v3);
    
    
    */
    return false;
}

void PROGRAM::timers_increase() {
    for(int i=0;i<10;i++) {
        ttimer[i]++;
    }
}

int PROGRAM::get_sprite_max_id() {
    int x = 0;
    for(auto& s : sprite) {
        if(s.idx > x) x = s.idx;
    }
    return x;
}