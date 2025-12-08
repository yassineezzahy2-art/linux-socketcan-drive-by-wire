#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h> // Needed for sleep()



int setup_socket(const char *ifname);
int main(){

    int s= setup_socket("vcan0");
    std::cout<<"engine  on"<<std::endl;

    struct can_frame frame;
    memset(frame.data,0,8);
    frame.can_id=0x123;
    frame.can_dlc=8;
    uint16_t rpm=0;
    char key ;
    while(true){
        if(rpm>5000){
            rpm=0;
        }
        if(rpm<0){
            rpm=0;
        }
        std::cout<<"current rpm is"<<std::endl;
        std::cin >> key;
       

        if(key=='a'){
            // SAFETY CHECK: Stop adding if we are at 5000
            if (rpm <= 4500) { 
                rpm = rpm + 500;
            } else {
                std::cout << " [MAX SPEED REACHED]" << std::endl;
            }
        }
        if(key=='b'){
            // SAFETY CHECK: Only brake if we have enough speed!
            if (rpm >= 500) { 
                rpm = rpm - 500;
            } else {
                rpm = 0; // Just stay at 0
                std::cout << " [IDLE - CANNOT BRAKE MORE]" << std::endl;
            }
        }
        frame.data[2]=90;
        frame.data[0]=rpm & 0xff;
        frame.data [1]=(rpm>>8)& 0xff;
write(s,&frame,sizeof(struct can_frame));
std::cout << std::endl;
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
