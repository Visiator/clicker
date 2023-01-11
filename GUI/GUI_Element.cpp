
#include <algorithm>
#include "../pcap.h"
#include "GUI_Element.h"
#include "GUI_Primitives.h"
#include "../program.h"

#include "GUI.h"

#include "../tools.h"

extern TEXTURA tt;
extern PROGRAMS programs_;

PROGRAM *ELEMENT::program_() {
    return &programs_.item_[program_idx];
}

ELEMENT* ELEMENTS::add(GUI *gui, unsigned int parent_id, ELEMENT::Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color, std::string caption) {
    
    
    item.push_back({gui, generate_new_id(), parent_id, type, x, y, w, h, bg_color, border_color, caption});
    return &item.back();
};

void ELEMENT::get_parent_xy(int *px, int *py) {
    *px += getx();
    *py += gety();
    if(parent != nullptr) parent->get_parent_xy(px, py);
    
}

unsigned int grn(unsigned int v) {
    unsigned int u;
    u = (v & 0x00ff00);
    if(u > 0x7f00) {
        u = u - 0x3f00;
    } else {
        u = u + 0x3f00;
    }
    return u;
}

void ELEMENT::paint_green_rec(SCREEN_BUFFER *screen, int px, int py) {
  
    unsigned int *q;
    int ww, hh;
    ww = green_rec.w;
    if(px + green_rec.x + ww > w) {
        ww = w - px - green_rec.x;
        if(ww < 0) ww = 0;
    }
    hh = green_rec.h;
    if(green_rec.y + hh > h) {
        hh = h - green_rec.y;
        if(hh < 0) hh = 0;
    }
    for(int j = 0; j < hh; j++ ) {
        q = screen->buffer + (py+j+green_rec.y)*screen->w + px+green_rec.x;
        for(int i = 0; i < ww; i++) {
            *q++ = grn(*q);
        }
    };
    
}

void generate_file_name(const char *dir, char *file_name) {
    
    struct tm       *tm;
    char buf[100];
    buf[0] = 0;
    
#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv,nullptr);   
    if((tm = localtime(&tv.tv_sec)) != NULL)
    {
        strftime(buf, 100, "%Y-%m-%d_%H-%M-%S", tm);
    };
#endif
    
#ifdef _WIN32
    time_t tt;
    time(&tt);
    if((tm = localtime(&tt)) != NULL)
    {
        strftime(buf, 100, "%Y-%m-%d_%H-%M-%S", tm);
    };
#endif
    
    sprintf(file_name, "%s/%s.bmp", dir, buf);
    
    //return (tv.tv_sec*1000+tv.tv_usec/1000);
}

void ELEMENT::paint_green_rec_x2(SCREEN_BUFFER *screen, int px, int py) {

    if(grab_screen == nullptr || grab_screen->buffer == nullptr || grab_screen->w == 0) return;
    unsigned int *q, *v;
    int ww, hh;
    ww = green_rec.w;
    if(ww > w) ww = w;
    
    hh = green_rec.h;
    if(hh > h) hh = h;
    
    if(h + hh > grab_screen->h) {
        //hh = grab_screen->h - h;
    }
    
    unsigned int *bf, e=0;
    
    if(need_save_bmp) {
        bf = new unsigned int[ww*hh];
    }
    
    for(int j = 0; j < hh; j++ ) {
        q = screen->buffer + ((h+py-hh-1)+j)*screen->w + (w+px)-ww-1;
        v = grab_screen->buffer+(j+green_rec.y)*grab_screen->w+green_rec.x;
        for(int i = 0; i < ww; i++) {
            if(need_save_bmp) {
                bf[e++] = *v;
            }
            *q++ = *v++;//grn(*q);
        }
    };
    if(need_save_bmp) {
        FILE *f;
        char file_name[100], file_name_info[100];
        generate_file_name("progs/prg0", file_name);
        f = fopen(file_name, "wb");
        if(f != NULL) {
            save_textura_to_BMP_file_(f, bf, ww, hh);
            fclose(f);
            strcpy(file_name_info, file_name);
            strcat(file_name_info, "_info");
            f = fopen(file_name_info, "wb");
            if(f != NULL) {
                save_textura_info(f, green_rec.x, green_rec.y, (char *)"");
                fclose(f);
            };
            PROGRAM *program = program_();
            int z = program->get_sprite_max_id() + 1;
            program->sprite.push_back({file_name, z});
            
        } else {
            printf("open file error\n");
        }
        need_save_bmp = false;
        delete[] bf;
        printf("save bmp\n");
    };

}

void ELEMENT::SpriteList_cursor_scroll(int key) {
    if(key == 34) {
        if(SpriteList_cursor > 0) SpriteList_cursor--;
    }
    if(key == 35) {
        if(SpriteList_cursor + 1 < program_()->sprite.size()) {
            SpriteList_cursor++;
        }
    }
}

void ELEMENT::paint(SCREEN_BUFFER *screen) {
    if(is_visible == false) return;
    ELEMENT *q;
    q = parent;
    while(q != nullptr) {
        if(q->is_visible == false) return;
        q = q->parent;
    }
    int dx = 0, dy = 0, i, x, y;
    int px = getx(), py = gety(), ww;
    if(parent != nullptr) parent->get_parent_xy(&px, &py);
    
    PROGRAM *program = program_();
    
    if(type == Type::Print) {
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);
        
        screen->fill(px, py, getw(), geth(), bg_color);

        screen->rectangle(px, py, getw(), geth(), border_color);
        if(program != nullptr) {
            x = 5;
            y = 5;
            
            for(auto const& p : program->print_out) {
                screen->fonts.print(px + x, py + y, "arial", 12, p.c_str(), 0xffffff);
                
                y += 20;
            }
        }
        return;
    }
    
    if(type == Type::ProgramText) {
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);
        
        screen->fill(px, py, getw(), geth(), bg_color);

        screen->rectangle(px, py, getw(), geth(), border_color);
        if(program != nullptr) {
            x = 5;
            y = 5;
            /*for(auto const& ln : program->line) {
                screen->fonts.print(px + x, py + y, "arial", 12, ln.info.c_str(), 0xffffff);
                screen->fonts.print(px + x+350, py + y, "arial", 12, ln.raw.c_str(), 0xcccccc);
                y += 20;
            }*/
            i = 0;
            while(i < program->line.size() && y + 40 < h) {
                screen->fonts.print(px + x, py + y, "arial", 12, program->line[i].info.c_str(), 0xffffff);
                screen->fonts.print(px + x+350, py + y, "arial", 12, program->line[i].raw.c_str(), 0xcccccc);
                y += 20;
                i++;
            }
        }
        screen->fonts.print(px + 5, py + h - 30, "arial", 12, program->detail.c_str(), 0xcccccc);
        
        char ss[100];
        sprintf(ss, "%d", program->sprite_detected_idx);
        screen->fonts.print(px + w - 40, py + h - 30, "arial", 12, ss, 0xcccccc);
        
        
        return;
    }
    
    if(type == Type::SpriteList) {
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);
        
        screen->fill(px, py, getw(), geth(), bg_color);

        screen->rectangle(px, py, getw(), geth(), border_color);
        
        i = 0;
        x = 5;
        y = 5;
        int max_h = 0;
        while(i < program->sprite.size()) {
        
            if(px + x + program->sprite[i].w + 5 > w) {
                x = 5;
                y += max_h + 5;
            } 
            
            if(max_h < program->sprite[i].h) {
                max_h = program->sprite[i].h;
            }
            
            screen->fill(px + x, py + y, program->sprite[i].w, program->sprite[i].h, program->sprite[i].bitmap);
            
            unsigned int clr;
            clr = border_color;
            if(program->sprite[i].is_detected) {
                clr = 0xff00ff;
                //screen->rectangle(px+program->sprite[i].is_detected_x, py+program->sprite[i].is_detected_y, program->sprite[i].w, program->sprite[i].h, clr);
            } else {
                clr = bg_color;
            }
            
            if(i == SpriteList_cursor) {
                clr = 0xaa0000;
                char vv[500];
                sprintf(vv, "nic:%s dc:%d nc:%d time:%d file:%s ", program->sprite[i].nic.c_str(), program->sprite[i].double_click, program->sprite[i].nopress
                        , program->sprite[i].get_detected_time() 
                        , program->sprite[i].file__name.c_str()
                        );
                screen->fonts.print(10 , 860, "arial", 12, vv, 0x0000ff);
        
            }
            
            screen->rectangle(px + x-1, py + y-1, program->sprite[i].w+2, program->sprite[i].h+2, clr);
            screen->rectangle(px + x-2, py + y-2, program->sprite[i].w+4, program->sprite[i].h+4, clr);
            
            x += program->sprite[i].w+5;
            i++;
        }
        
        return;
    }
    
    if(type == Type::MikrotikInfo) {
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);
        
        screen->fill(px, py, getw(), geth(), bg_color);
        screen->rectangle(px, py, getw(), geth(), border_color);
        
        screen->fonts.print(px , py - screen->fonts.text_height("arial",12), "arial",12,caption.c_str(), 0xff);
        
        std::string s;
        
        s = " " + std::to_string(info1) + " : " + std::to_string(info2);
        
        screen->fonts.print(px , py, "arial", 12, s.c_str(), 0xff);
        
        return;
    }
    
    if(type == Type::View) {
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);
        
        
        if(is_pressed) {
            screen->fill(px, py, getw(), geth(), bg_color_pressed);
        } else {
            screen->fill(px, py, getw(), geth(), bg_color);
            
        }
        screen->rectangle(px, py, getw(), geth(), border_color);
        ww = screen->fonts.text_width("arial",12,caption);
        
        screen->fonts.print(px + getw()/2 - ww/2, py + geth()/2 - screen->fonts.text_height("arial",12)/2, "arial",12,caption.c_str(), 0xff);
        
        if(grab_screen != nullptr) {
            grab_screen->lock();
            unsigned int *q, ww, hh, *v;
            
            v = grab_screen->buffer;
            
            ww = grab_screen->w < w ? grab_screen->w : w;
            hh = grab_screen->h < h ? grab_screen->h : h;
            for(int j=0;j<hh;j++) {
                q = screen->buffer + (py+j)*screen->w + px;
                v = grab_screen->buffer + j*grab_screen->w;
                for(int i=0;i<ww;i++) {
                    *q++ = *v++;

                }
            };
            grab_screen->unlock();            
        }
        
        paint_green_rec(screen, px, py);
        
        paint_green_rec_x2(screen, px, py);
        
        i = 0;
        while(i < program->sprite.size()) {

            if(program->sprite[i].is_detected) {
                screen->rectangle(px+program->sprite[i].is_detected_x, py+program->sprite[i].is_detected_y, program->sprite[i].w, program->sprite[i].h, 0xff00ff);
            }
            i++;
        }
        return;
    }
    
    if(type == Type::Button) {
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);
        
        
        if(is_pressed) {
            screen->fill(px, py, getw(), geth(), bg_color_pressed);
        } else {
            screen->fill(px, py, getw(), geth(), bg_color);
            
        }
        screen->rectangle(px, py, getw(), geth(), border_color);
        ww = screen->fonts.text_width("arial",12,caption);
        
        screen->fonts.print(px + getw()/2 - ww/2, py + geth()/2 - screen->fonts.text_height("arial",12)/2, "arial",12,caption.c_str(), 0xff);
        
        return;
    }
    
    if(type == Type::PCAPinfo) {
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);
        
        screen->fill(px, py, getw(), geth(), bg_color);
        screen->rectangle(px, py, getw(), geth(), border_color);
        
        screen->fonts.print(px , py - screen->fonts.text_height("arial",12), "arial",12,caption.c_str(), 0xff);
        
        char cc[500];
        cc[0] = 0;
        if(pcap != nullptr) {
            sprintf(cc, "pg = %d", (int)pcap->counter._decode_page_6);
            screen->fonts.print(px+5 , py, "arial",12,cc, 0xff);
            sprintf(cc, "tcp = %d / %d", (int)pcap->counter._ipv4_tcp_ingress, (int)pcap->counter._ipv4_tcp_egress);
            screen->fonts.print(px+5 , py+18, "arial",12,cc, 0xff);
            sprintf(cc, "udp = %d / %d", (int)pcap->counter._ipv4_udp_ingress, (int)pcap->counter._ipv4_udp_egress);
            screen->fonts.print(px+5 , py+18*2, "arial",12,cc, 0xff);

            sprintf(cc, "sess = %d", (int)pcap->sessions.items.size() );
            screen->fonts.print(px+5 , py+18*3, "arial",12,cc, 0xff);

        };
        
        
        
        //RECTANGLE::paint(screen);
        
        return;
    }
    
    if(type == Type::List) {
        
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);

        
        screen->fill(px, py, getw(), geth(), bg_color);
        screen->rectangle(px, py, getw(), geth(), border_color);
        
        screen->fonts.print(px , py - screen->fonts.text_height("arial",12), "arial",12,caption.c_str(), 0xff);
        
        dy = 5;
        dx = 5;
        for(auto& s : item) {
            if(dy + 5 + screen->fonts.text_height("arial", 12) > geth()) break;
            //screen->fonts.print(px + dx, py + dy, "arial", 12, s.text, 0);
            s.set_xywh(px + dx, py + dy, getw() - dx*2, screen->fonts.text_height("arial", 12)+1);
            s.paint(screen);
            dy += screen->fonts.text_height("arial", 12)+0;
        }
        
        return; 
    }
    
    if(type == Type::Memo) {
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);

        
        screen->fill(px, py, getw(), geth(), bg_color);
        screen->rectangle(px, py, getw(), geth(), border_color);
        
        
        
        //dx = (w - screen->fonts.text_width("arial",12,caption))/2;
        //dy = (h - screen->fonts.text_height("arial",12))/2;
        
        screen->fonts.print(px , py - screen->fonts.text_height("arial",12), "arial",12,caption.c_str(), 0xff);
        
        
        dy = 5;
        dx = 5;
        for(auto& s : item) {
            screen->fonts.print(px + dx, py + dy, "arial", 12, s.text.c_str(), 0);
            dy += screen->fonts.text_height("arial", 12);
        }
        
        return;
    }
    if(type == Type::Panel) {
        px = getx(), py = gety();
        if(parent != nullptr) parent->get_parent_xy(&px, &py);
        
        screen->fill(px, py, getw(), geth(), bg_color);
        screen->rectangle(px, py, getw(), geth(), border_color);
        
        /*
        RECTANGLE::paint(screen);
        int dx = 0, dy = 0;
        dx = (getw() - screen->fonts.text_width("arial",12,caption))/2;
        dy = (geth() - screen->fonts.text_height("arial",12))/2;
        screen->fonts.print(px + dx, py + dy, "arial",12,caption.c_str(), 0xff);
        */
        
        return;
    }
    RECTANGLE::paint(screen);
}

void ELEMENTS::recalc_parent_id() {

    for(auto& a : item) {
        for(auto& b : item) {
            if(a.parent_id == b.id) {
                a.parent = &b;
                
            }
        }    
    }
    
}

ELEMENT* ELEMENTS::get_element_by_id(unsigned int element_id) {
    int i;
    i = 0;
    while(i < item.size()) {
        if(item[i].id == element_id) {
            return &item[i];
        }
        i++;
    }
    return nullptr;
}

void ELEMENT_item::get_parent_xy(int *px, int *py) {
    
}

void ELEMENT_item::paint(SCREEN_BUFFER *screen) {
    //int dx = 0, dy = 0;
    int px = 0, py = 0;
    //if(parent == nullptr) return;
    
    if(parent != nullptr) parent->get_parent_xy(&px, &py);
    
    if(type == Type::WindowList) {
        if(is_pressed) {
            screen->fill(px + getx(), py + gety(), getw(), geth(), bg_color);
        } else {
            screen->fill(px + getx(), py + gety(), getw(), geth(), bg_color_pressed);
        }
        //screen->rectangle(px, py, w, h, border_color);
        
        screen->line_h(px + getx(), py + gety(), getw(), border_color);
        
        screen->fonts.print_w(px+5+getx() , py + gety(), getw() - 10, "arial",12,text.c_str(), 0xff);
        

        
        return; 
    }
    
    if(type == Type::ListItem) {
        //px = 0, py = 0;
        //if(parent != nullptr) parent->get_parent_xy(&px, &py);

        if(is_pressed) {
            screen->fill(px + getx(), py + gety(), getw(), geth(), bg_color);
        } else {
            screen->fill(px + getx(), py + gety(), getw(), geth(), bg_color_pressed);
        }
        //screen->rectangle(px, py, w, h, border_color);
        
        screen->line_h(px + getx(), py + gety(), getw(), border_color);
        
        screen->fonts.print_w(px+5+getx() , py + gety(), getw() - 10, "arial",12,text.c_str(), 0xff);
        

        
        return; 
    }
    
    RECTANGLE::paint(screen);
   
}

void ELEMENTS::KeyPressEvent(unsigned int key) {
    for(auto& a : item) {
        a.KeyPressEvent(key);
    }
}

void ELEMENTS::MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y ) {
    for(auto& a : item) {
        a.MouseButtonEvent(MouseButtonType, mouse_x, mouse_y);
    }
}

void ELEMENT::KeyPressEvent(unsigned int key) {
    if(type == Type::View) {
        gui->set_ui_action( UI_Action_Type::KeyEvent, id, "", key );
    }
}

void ELEMENT::MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y ) {
    
    static unsigned long last_MouseButtonEvent = 0;
    
    int parent_x = 0, parent_y = 0;
    
    if(is_visible == false) return;
    if(is_passive == true) return;
    if(type == Type::Button) {
        int xx = 0, yy = 0;
        if(parent != nullptr) parent->get_parent_xy(&xx, &yy);
        if(its_me(mouse_x - xx, mouse_y - yy)) {
            if(is_pressed == false) {
                is_pressed = true;
                gui->set_ui_action( UI_Action_Type::Select , id, "", 0);
            } else {
                is_pressed = false;
                gui->set_ui_action( UI_Action_Type::Deselect , id, "", 0);
            }
            
        }
        return;
    }
    if(type == Type::List) {
        bool r = false;
        for(auto& a : item) {           
            if(a.MouseButtonEvent(MouseButtonType, mouse_x, mouse_y)) {
                if(a.is_pressed == false) {
                    for(auto& b : item) {
                        b.is_pressed = false;
                    }
                    a.is_pressed = true;
                    last_MouseButtonEvent = GetTickCount();
                    gui->set_ui_action( UI_Action_Type::Select, id, a.text, a.window_id );
                } else {
                    if(GetTickCount() - last_MouseButtonEvent < 700) {
                        gui->set_ui_action( UI_Action_Type::DoubleClick, id, a.text, a.window_id);    
                    }
                    last_MouseButtonEvent = GetTickCount();
                }
            }
        }
        return;
    }
    
}

bool ELEMENT_item::MouseButtonEvent(MOUSE_BUTTON_TYPE MouseButtonType, int mouse_x,int mouse_y ) {
    if(type == Type::ListItem) {
        if(its_me(mouse_x, mouse_y)) {
            return true;
        }
    }
    return false;
}

unsigned int decode_id(std::string& s) {
    unsigned int r, p;
    std::string a;
    r = 0;
    p = s.find(":");
    if(p > 0) {
        r = std::stoi(s.substr(0, p));
    }
    return r;
}

void ELEMENT::add_to_window_list(unsigned int window_id, int wx, int wy, int ww, int wh) {
    
    std::string str;
    str = std::to_string(window_id) + ":" + std::to_string(wx)+":"+ std::to_string(wy)+":"+ std::to_string(ww)+":"+ std::to_string(wh);

    for(auto& a : item) {
        if(a.window_id == window_id) {
            if(a.text != str) {
                a.text = str;
            }
            return;
        }
    }
    
    item.push_back({gui, ELEMENT_item::Type::ListItem, str});
    item.back().window_id = window_id;

}

void ELEMENT::set_need_save_bmp() {
    need_save_bmp = true;
}