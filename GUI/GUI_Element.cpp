
#include "GUI_Element.h"
#include "GUI_Primitives.h"

ELEMENT* ELEMENTS::add(ELEMENT* parent, ELEMENT::Type type, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color, std::string caption) {
    
    ELEMENT e(parent, type, x, y, w, h, bg_color, border_color, caption);
    item.push_back(std::move(e));
    return &item.back();
};

void ELEMENT::paint(SCREEN_BUFFER *screen) {
    if(type == Type::Memo) {
        RECTANGLE::paint(screen);
        int dx = 0, dy = 0;
        dx = (w - screen->fonts.text_width("arial",12,caption))/2;
        dy = (h - screen->fonts.text_height("arial",12))/2;
        screen->fonts.print(x + dx, y + dy, "arial",12,caption, 0xff);
        return;
    }
    RECTANGLE::paint(screen);
}