#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>

int setup_socket(const char *ifname);
int main(){
int s=setup_socket("vcan0");
if (s<0) return 1;

struct can_frame frame;

int rpm =0;
int temp =0;
std::cout << "Waiting for data..." << std::endl;
while(true){
    int nb = read(s,&frame,sizeof(struct can_frame));
    if (nb<0) break;
    if (frame.can_id=0x123){
        rpm= frame.data[1]<<8 | frame.data[0];
        temp =frame.data[2];
    
    }
    std::cout << "\033[2J\033[H"; 
        
        std::cout << "=========================" << std::endl;
        std::cout << "   VIRTUAL COCKPIT v2.0  " << std::endl;

        std::cout << "=========================" << std::endl;
std::cout << "RPM:  " << rpm << "\t";
        int bars = rpm / 500;
        std::cout << "[";
        for(int i=0; i<10; i++) std::cout << (i < bars ? "#" : " ");
        std::cout << "]" << std::endl;

        std::cout << "TEMP: " << temp << " C" << std::endl;
        std::cout << "-------------------------" << std::endl;
}
close(s);
return 0;


}
int setup_socket(const char *ifname) {
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    struct ifreq ifr;
    std::strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);
    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    return s;
}
