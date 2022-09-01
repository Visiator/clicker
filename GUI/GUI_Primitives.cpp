
#include "GUI_Primitives.h"

void RECTANGLE::paint(SCREEN_BUFFER *screen) {
    if(this == nullptr) return;
    screen->line_h(x, y, w, border_color);
    screen->line_h(x, y+h-1, w, border_color);
    screen->line_v(x, y, h, border_color);
    screen->line_v(x+w-1, y, h, border_color);
    screen->fill(x+1, y+1, w-2, h-2, bg_color);
    
}


