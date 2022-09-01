
#ifndef WEBCAM_H
#define WEBCAM_H

#include <string>
#include <vector>

#include <linux/videodev2.h>

class WEBCAM {
public:
    std::string name, driver, card, bus_info;
    
    WEBCAM& operator=(WEBCAM&& src) {
        if (this != &src) {
            this->name  = src.name;
        };
        return *this;        
    }
    WEBCAM& operator=(const WEBCAM& src) {
        if (this != &src) {
            this->name  = std::move(src.name);
        };
        return *this;
    }
    WEBCAM(std::string& s);
    WEBCAM(const WEBCAM& src) {
        this->name  = src.name;
    }
    WEBCAM(WEBCAM&& src) {
        *this = std::move(src);
    }
};

class WEBCAMS {
public:
    std::vector<WEBCAM> webcam;
    void init();
    
    WEBCAMS(){};
    ~WEBCAMS(){};
};

#endif /* WEBCAM_H */

