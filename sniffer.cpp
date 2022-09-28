/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "sniffer.h"
#include <cstdlib>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_ether.h>
#include <linux/socket.h>
#include <linux/ioctl.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ifaddrs.h>  

#include "pcap.h"

extern bool GLOBAL_STOP;
extern PCAP pcap;

int getsock_recv(int index) {
    int sd; // дескриптор сокета
/*
 * При работе с пакетными сокетами для хранения адресной информации
 * сетевого интерфейса вместо структуры sockaddr_in используется структура
 * sockaddr_ll (см. <linux/if_packet.h>)
 */
    struct sockaddr_ll s_ll;

/*
 * Cоздаем пакетный сокет. Т.к. MAC-адреса мы тоже собираемся обрабатывать,
 * параметр type системного вызова socket принимает значение SOCK_RAW
 */
    sd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sd < 0) return -1;

    memset((void *)&s_ll, 0, sizeof(struct sockaddr_ll));

/*
 * Заполним поля адресной структуры s_ll
 */
    s_ll.sll_family = PF_PACKET; // тип сокета
    s_ll.sll_protocol = htons(ETH_P_ALL); // тип принимаемого протокола
    s_ll.sll_ifindex = index; // индекс сетевого интерфейса

/*
 * Привязываем сокет к сетевому интерфейсу. В принципе, делать это не
 * обязательно, если на хосте активен только один сетевой интерфейс.
 * При наличии двух и более сетевых плат пакеты будут приниматься сразу со всех
 * активных интерфейсов, и если нас интересуют пакеты только из одного сегмента
 * сети, целесообразно выполнить привязку сокета к нужному интерфейсу
 */
    if(bind(sd, (struct sockaddr *)&s_ll, sizeof(struct sockaddr_ll)) < 0) {
	close(sd);
        printf("err\n");
	return -1;
    }

    return sd;
}


void SNIFFER::start_sniff(int InterfaceId,const char *InterfaceName ) {

    if(InterfaceId == 0) {
        return;
    }
    
    unsigned char buff[2000];
    int eth0_if;
    
    if((eth0_if = getsock_recv(InterfaceId)) < 0) {
	perror("e) getsock_recv");
	return;
    }
    
    struct ifreq req;
    strcpy(req.ifr_name, InterfaceName);
    ioctl (eth0_if, SIOCGIFMTU, &req);
    int rec, mtu = req.ifr_mtu;

    //mtu = 1500;
    int frame_no = 0;
    FILE *f;
    
    local_stop = false;
    while(GLOBAL_STOP == false && local_stop == false) {
        memset(buff, 0, 2000);
        
        rec = recvfrom(eth0_if, (char *)buff, mtu + 18, 0, NULL, NULL);
        
        pcap.analiz(frame_no++, buff, rec);
        
        printf("%d\n", rec);
        
    }
    
}