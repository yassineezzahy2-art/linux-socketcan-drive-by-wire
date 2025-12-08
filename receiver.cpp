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
    std::cout<<"dash is on"<<std::endl;

    struct can_frame frame;
    int byte;
while (true) {
        byte = read(s, &frame, sizeof(struct can_frame));
        if (byte < 0) {
            perror("Read error");
            break;
        }

        if (frame.can_id == 0x123) {
            uint8_t temp = frame.data[2];
            uint16_t rpm = (frame.data[1] << 8) | frame.data[0];
            
            std::cout << "Dashboard: RPM=" << rpm 
                      << " | Temp=" << (int)temp << "C" << std::endl;
        }

        else if (frame.can_id == 0x244) {
            bool is_open = frame.data[0];
            
            if (is_open) {
                std::cout << "Dashboard: [WARNING] DOOR OPEN!" << std::endl;
            } else {
                std::cout << "Dashboard: Door Closed." << std::endl;
            }
        }
    }    close(s);
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
