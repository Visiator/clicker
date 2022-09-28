
#include "GUI_Primitives.h"

void RECTANGLE::paint(SCREEN_BUFFER *screen) {
    if(this == nullptr) return;
    screen->line_h(getx(), gety(), getw(), border_color);
    screen->line_h(getx(), gety()+geth()-1, getw(), border_color);
    screen->line_v(getx(), gety(), geth(), border_color);
    screen->line_v(getx()+getw()-1, gety(), geth(), border_color);
    screen->fill(getx()+1, gety()+1, getw()-2, geth()-2, bg_color);
    
}


