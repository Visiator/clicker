
#ifndef WEBCAM_H
#define WEBCAM_H

#include <string>
#include <vector>
#include <thread>

#include <linux/videodev2.h>

#include "GUI/GUI_Primitives.h"

#define IMAGE_WIDTH     1920
#define IMAGE_HEIGHT    1080

class PROGRAM;

struct buffer {
        void   *start;
        size_t  length;
};

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
    
    WEBCAM(const WEBCAM& src) {
        this->name  = src.name;
    }
    WEBCAM(WEBCAM&& src) {
        *this = std::move(src);
    }
    WEBCAM(std::string& name, std::string& driver, std::string& card, std::string& bus_info) : name(name)
    {
        
    }

};

class WEBCAMS {
public:
    SCREEN *src = nullptr;
    buffer* devbuffer = nullptr;
    //PROGRAM *program = nullptr;
    
    
    std::vector<WEBCAM> item;
    void init(std::vector<std::string> *list, SCREEN *scr_);
    
    bool execute_is_run = false;
    std::thread* execute_thread = nullptr;
    void execute();
    void wait_execute_close();
    
    void startCapturing(int fd);
    void closeDevice(int& fd);
    void getFrame(int fd, std::string file_name);
    int  readFrame(int fd, std::string file_name);
    int  xioctl(int fd, int request, void *arg);
    void initMMAP(int fd);
    void stopCapturing(int fd);
    void freeMMAP();
    int  openDevice(std::string dev_name);
    void setFormatcam(int fd);
    void enumfmtCamera(int fd);    
    int  setfmtCamera(int fd);
    
    WEBCAMS(){};
    ~WEBCAMS(){};
};

#endif /* WEBCAM_H */

