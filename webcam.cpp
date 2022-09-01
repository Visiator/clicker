/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#include "webcam.h"
WEBCAM::WEBCAM(std::string& s) {
    name = s;
    int fd;
    struct v4l2_capability device_params;
    if((fd = open(s.c_str(), O_RDONLY)) != -1){
        if (ioctl(fd, VIDIOC_QUERYCAP, &device_params) != -1)
        {
            driver = (char *)device_params.driver;
            card = (char *)device_params.card;
            bus_info = (char *)device_params.bus_info;
        }
        close(fd);
    }
};
void WEBCAMS::init() {
    
    std::string s;
    int fd, idx = 0;
    while(idx < 64) {
        s = "/dev/video" + std::to_string(idx);
        if((fd = open(s.c_str(), O_RDONLY)) != -1){
            close(fd);
            webcam.push_back(s);
        };
        
        idx++;
    };
    /*
    if(ioctl(fd, VIDIOCGCAP, &video_cap) == -1)
        perror("cam_info: Can't get capabilities");
    else {
        printf("Name:\t\t '%s'\n", video_cap.name);
        printf("Minimum size:\t%d x %d\n", video_cap.minwidth, video_cap.minheight);
        printf("Maximum size:\t%d x %d\n", video_cap.maxwidth, video_cap.maxheight);
    }

    if(ioctl(fd, VIDIOCGWIN, &video_win) == -1)
        perror("cam_info: Can't get window information");
    else
        printf("Current size:\t%d x %d\n", video_win.width, video_win.height);

    if(ioctl(fd, VIDIOCGPICT, &video_pic) == -1)
        perror("cam_info: Can't get picture information");
    else
        printf("Current depth:\t%d\n", video_pic.depth);
*/
    
    return;
}