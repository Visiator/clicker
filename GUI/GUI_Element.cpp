
#include "GUI_Element.h"
#include "GUI_Primitives.h"

extern TEXTURA tt;

ELEMENT* ELEMENTS::add(unsigned int parent_id, ELEMENT::Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color, std::string caption) {
    
    
    item.push_back({generate_new_id(), parent_id, type, x, y, w, h, bg_color, border_color, caption});
    return &item.back();
};

void ELEMENT::get_parent_xy(int *px, int *py) {
    *px += x;
    *py += y;
    if(parent != nullptr) parent->get_parent_xy(px, py);
}

void ELEMENT::paint(SCREEN_BUFFER *screen) {
    
    int px = x, py = y;
    if(parent != nullptr) parent->get_parent_xy(&px, &py);
    
    if(type == Type::Memo) {
        px = x, py = y;
        if(parent != nullptr) parent->get_parent_xy(&px, &py);

        
        screen->fill(px, py, w, h, bg_color);
        screen->rectangle(px, py, w, h, border_color);
        
        /*
        int dx = 0, dy = 0;
        dx = (w - screen->fonts.text_width("arial",12,caption))/2;
        dy = (h - screen->fonts.text_height("arial",12))/2;
        screen->fonts.print(px + dx, py + dy, "arial",12,caption, 0xff);
        */
        int dx, dy;
        dy = 5;
        dx = 5;
        for(auto& s : string_list) {
            screen->fonts.print(px + dx, py + dy, "arial", 12, s, 0);
            dy += screen->fonts.text_height("arial", 12);
        }
        
        return;
    }
    if(type == Type::Panel) {
        RECTANGLE::paint(screen);
        int dx = 0, dy = 0;
        dx = (w - screen->fonts.text_width("arial",12,caption))/2;
        dy = (h - screen->fonts.text_height("arial",12))/2;
        screen->fonts.print(px + dx, py + dy, "arial",12,caption, 0xff);
        if(tt.w == 640) {
            int j=0;
            for(int y = 0; y < 480; y++ ) {
                for(int x = 0; x < 640; x++ ) {
                    screen->buffer[y* screen->w + x] = tt.buf[j++];
                }   
            }
        }
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