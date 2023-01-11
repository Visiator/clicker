/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __linux__
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#endif

#include "webcam.h"
#include "tools.h"
#include "program.h"
#include "GUI/GUI_Primitives.h"

extern bool GLOBAL_STOP;
extern PROGRAMS programs_;

//TEXTURA tt;

void WEBCAMS::init(std::vector<std::string> *list, SCREEN *src_) {
    

    
    src = src_;
    
#ifdef __linux__
    
    //program = program_;    
    struct v4l2_capability device_params;
    
    std::string s, a, driver, card, bus_info;
    int fd, idx = 0;
    while(idx < 64) {
        s = "/dev/video" + std::to_string(idx);
        if((fd = open(s.c_str(), O_RDONLY)) != -1){
            if (ioctl(fd, VIDIOC_QUERYCAP, &device_params) != -1)
            {
                driver = (char *)device_params.driver;
                card = (char *)device_params.card;
                bus_info = (char *)device_params.bus_info;
            }
            close(fd);
            
            
            
            item.push_back({s, driver, card, bus_info});
            a = s+" | "+driver+" | "+card+" | "+bus_info;
            printf("%s\n", a.c_str());
            list->push_back(a);
        };
        
        idx++;
    };

#endif    
    
    execute_is_run = true;
    execute_thread = new std::thread(&WEBCAMS::execute, this);
    
    return;
}

void WEBCAMS::startCapturing(int fd)
{

#ifdef __linux__
    
    struct v4l2_buffer buf;

    memsetzero((unsigned char *)&buf, sizeof(buf));

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;

    xioctl(fd, VIDIOC_QBUF, &buf);

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    xioctl(fd, VIDIOC_STREAMON, &type);

#endif
    
}

void WEBCAMS::stopCapturing(int fd) {
#ifdef __linux__
    enum v4l2_buf_type type;
    type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl(fd, VIDIOC_STREAMOFF, &type);
#endif
}

void WEBCAMS::freeMMAP() {
#ifdef __linux__
    if(munmap(devbuffer->start, devbuffer->length)==-1)
        return;
    free(devbuffer);
#endif
}

int WEBCAMS::openDevice(std::string dev_name) {
#ifdef __linux__    
    return open(dev_name.c_str(), O_RDWR /* required */ | O_NONBLOCK, 0);   
#elif _WIN32
    // TODO 2023
    return -1;
#else
    return -1;
#endif
}

void WEBCAMS::closeDevice(int& fd){
    if(close(fd)==-1) {
        return;
    };
    fd = -1;
}

void WEBCAMS::execute() {
    
    
#ifdef __linux__
    int fd = openDevice("/dev/video2");
    enumfmtCamera(fd);
    setfmtCamera(fd);
    getFrame(fd, "111");
    
    while(GLOBAL_STOP == false) {
        getFrame(fd, "111");        
        programs_.detect_sprites(src);        
        usleep(100);
    }
    closeDevice(fd);
#endif
    
#ifdef _WIN32
    // TODO 2023
#endif
    
    execute_is_run = false;
}

void WEBCAMS::wait_execute_close() {
     while(execute_is_run == true) {
        usleep(100);
    }
}

int WEBCAMS::xioctl(int fd, int request, void *arg)
{
#ifdef __linux__
       int r;
       r = ioctl (fd, request, arg);
       if(r == -1)
       {
           if (errno == EAGAIN)
               return EAGAIN;
           return -1;
       }
       return r;
#elif _WIN32
       // TODO 2023
       return -1;
#else
       return -1;
#endif
}

void WEBCAMS::getFrame(int fd, std::string file_name)
{
#ifdef __linux__    
    //setFormatcam(fd);
    
    initMMAP(fd);

    startCapturing(fd);

    long int i = 0;

    for (;;)
    {
        if(readFrame(fd, "file_name.jpg"))
           break;
        if(GLOBAL_STOP != false) break; 
        i++;
    }

    stopCapturing(fd);

    freeMMAP();
#endif
}

int WEBCAMS::readFrame(int fd, std::string file_name)
{
#ifdef __linux__
    struct v4l2_buffer buf;

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    buf.length = 0;

    if (xioctl(fd, VIDIOC_DQBUF, &buf) == EAGAIN) {
            return 0;
    };
    buffer *temp = devbuffer;

    /*FILE *out_file = fopen(file_name.c_str(),"w");
    fwrite(temp->start,temp->length,1,out_file);
    fclose(out_file);*/

    int i=0, j=0;
    
    int r, g, b;
    int r1, g1, b1;
    int r2, g2, b2;
    int r3, g3, b3;
    int r4, g4, b4;
    float y1, y2, u, v;
    
    //if(buf.length == 614400) {
    //    src->set_size(640, 480);
    //if(buf.length == 691200) {
    if(buf.length == 4147200) {
        src->set_size(IMAGE_WIDTH/2, IMAGE_HEIGHT/2); // 960 * 540 = 518400
        
        src->grab_begin++;
        
        unsigned int *q;
        unsigned char *w;
        w = (unsigned char *)temp->start;
        q = src->buffer;
        
        int x, xx, y, yy;
        y = 0;
        j = 0;
        while(y < IMAGE_HEIGHT/2) {
            
            x = 0;
            while(x < IMAGE_WIDTH/2) {

                if(x < 331 || x > 628) {
                    src->buffer[j++] = 0xff;
                    i += 4;
                } else {
                ////////////////////////////////////////////////////////////////
                    y1 = w[i];
                    u =  w[i+1];
                    y2 = w[i+2];
                    v =  w[i+3];

                    r1 = y1 + 1.402 * (v-128);
                    g1 = y1 - 0.714 * (v-128) - 0.344 * (u-128);
                    b1 = y1 + 1.772 * (u-128);

                    if(r1 < 0) r1 = 0;            if(r1 > 255) r1 = 255;
                    if(g1 < 0) g1 = 0;            if(g1 > 255) g1 = 255;
                    if(b1 < 0) b1 = 0;            if(b1 > 255) b1 = 255;

                    r2 = y2 + 1.402 * (v-128);
                    g2 = y2 - 0.714 * (v-128) - 0.344 * (u-128);
                    b2 = y2 + 1.772 * (u-128);

                    if(r2 < 0) r2 = 0;            if(r2 > 255) r2 = 255;
                    if(g2 < 0) g2 = 0;            if(g2 > 255) g2 = 255;
                    if(b2 < 0) b2 = 0;            if(b2 > 255) b2 = 255;

                    ////////////////////////////////////////////////////////////

                    y1 = w[i + IMAGE_WIDTH*2];
                    u =  w[i+1 + IMAGE_WIDTH*2];
                    y2 = w[i+2 + IMAGE_WIDTH*2];
                    v =  w[i+3 + IMAGE_WIDTH*2];

                    r3 = y1 + 1.402 * (v-128);
                    g3 = y1 - 0.714 * (v-128) - 0.344 * (u-128);
                    b3 = y1 + 1.772 * (u-128);

                    if(r3 < 0) r3 = 0;            if(r3 > 255) r3 = 255;
                    if(g3 < 0) g3 = 0;            if(g3 > 255) g3 = 255;
                    if(b3 < 0) b3 = 0;            if(b3 > 255) b3 = 255;

                    r4 = y2 + 1.402 * (v-128);
                    g4 = y2 - 0.714 * (v-128) - 0.344 * (u-128);
                    b4 = y2 + 1.772 * (u-128);

                    if(r4 < 0) r4 = 0;            if(r4 > 255) r4 = 255;
                    if(g4 < 0) g4 = 0;            if(g4 > 255) g4 = 255;
                    if(b4 < 0) b4 = 0;            if(b4 > 255) b4 = 255;

                    
                    ////////////////////////////////////////////////////////////
                    
                    r = (r1 + r2 + r3 + r4)/4;
                    g = (g1 + g2 + g3 + g4)/4;
                    b = (b1 + b2 + b3 + b4)/4;
                    
                    g = g << 8;
                    r = r << 16;

                    src->buffer[j-331] = r | g | b;
                    j++;
                    i += 4;
                ////////////////////////////////////////////////////////////////
                };    
                
                x += 1;
            }
            i += IMAGE_WIDTH * 2;
            y += 1;
        }
        i += 2;
        /***
        //while(i<614400) {
        while(i<4147200) {
            y1 = w[i];
            u =  w[i+1];
            y2 = w[i+2];
            v =  w[i+3];
            
            r = y1 + 1.402 * (v-128);
            g = y1 - 0.714 * (v-128) - 0.344 * (u-128);
            b = y1 + 1.772 * (u-128);

            if(r < 0) r = 0;            if(r > 255) r = 255;
            if(g < 0) g = 0;            if(g > 255) g = 255;
            if(b < 0) b = 0;            if(b > 255) b = 255;
            
            g = g << 8;
            r = r << 16;
            
            src->buffer[j++] = r | g | b;
            
            r = y2 + 1.402 * (v-128);
            g = y2 - 0.714 * (v-128) - 0.344 * (u-128);
            b = y2 + 1.772 * (u-128);
            
            if(r < 0) r = 0;            if(r > 255) r = 255;
            if(g < 0) g = 0;            if(g > 255) g = 255;
            if(b < 0) b = 0;            if(b > 255) b = 255;
            
            g = g << 8;
            r = r << 16;
            
            src->buffer[j++] = r | g | b;
            i += 4;
        }
        ***/
    }
    src->grab_end++;
#endif    
    return 1;
}

void WEBCAMS::setFormatcam(int fd){
#ifdef __linux__
    struct v4l2_format fmt;
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = IMAGE_WIDTH;
    fmt.fmt.pix.height      = IMAGE_HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV422P;

    if(-1==xioctl(fd, VIDIOC_S_FMT, &fmt)) {
        //errno_exit("VIDIOC_S_FMT");
    };
#endif
}

void WEBCAMS::initMMAP(int fd)
{
#ifdef __linux__    
    struct v4l2_requestbuffers req;

    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    xioctl(fd, VIDIOC_REQBUFS, &req);

    devbuffer = (buffer*) calloc(req.count, sizeof(*devbuffer));

    
    
    struct v4l2_buffer buf;

    memsetzero((unsigned char *)&buf, sizeof(buf));

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = 0;

    xioctl(fd, VIDIOC_QUERYBUF, &buf);

    devbuffer->length = buf.length;
    devbuffer->start =
               mmap(NULL,
                    buf.length,
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED,
                    fd,
                    buf.m.offset);

    if (devbuffer->start == MAP_FAILED)
        return;
#endif   
    
    
}

void WEBCAMS::enumfmtCamera(int fd)
{
#ifdef __linux__
    static struct v4l2_fmtdesc fmtdesc;
    int ret;
    int i;
    memsetzero((unsigned char *)&fmtdesc, sizeof(fmtdesc));
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("-------------VIDIOC_ENUM_FMT--------------\n");
    while((ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc)) != -1)
    {
            printf("index:%d   \npixelformat:%c%c%c%c  \ndescription:%s\n",fmtdesc.index, fmtdesc.pixelformat&0xff,(fmtdesc.pixelformat>>8)&0xff,(fmtdesc.pixelformat>>16)&0xff,
            (fmtdesc.pixelformat>>24)&0xff,fmtdesc.description);
            fmtdesc.index++;
    }
#endif
}

int WEBCAMS::setfmtCamera(int fd)
{
#ifdef __linux__    
    int ret;
    struct v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = IMAGE_WIDTH;
    format.fmt.pix.height = IMAGE_HEIGHT;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;  // 设置为yuyv格式数据
    format.fmt.pix.field = V4L2_FIELD_INTERLACED;
    ret = ioctl(fd, VIDIOC_S_FMT, &format);
    if(ret < 0){
            printf("VIDIOC_S_FMT fail\n");
            return -1;
    }
    memsetzero((unsigned char *)&format, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(fd, VIDIOC_G_FMT, &format);
    if(ret < 0)
    {
            printf("VIDIOC_G_FMT fail\n");
            return -1;
    }
    printf("-----------------VIDIOC_G_FMT----------------------\n");
    printf("width:%d   \nheight:%d   \ntype:%x   pixelformat:%c%c%c%c\n",format.fmt.pix.width,format.fmt.pix.height,
            format.type,format.fmt.pix.pixelformat&0xff,(format.fmt.pix.pixelformat>>8)&0xff,(format.fmt.pix.pixelformat>>16)&0xff,
            (format.fmt.pix.pixelformat>>24)&0xff);
#endif    
    return 0;
}

