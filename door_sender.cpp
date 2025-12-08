#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h> 

int setup_socket(const char *ifname);

int main() {
    int s = setup_socket("vcan0");
    if (s < 0) return 1;

    struct can_frame frame;
    memset(frame.data, 0, 8);
    
    // DIFFERENT ID! 
    // 0x123 is the Engine. 
    // 0x244 is the Door Controller.
    frame.can_id = 0x244; 
    frame.can_dlc = 1; 

    char key;
    bool door_is_open = false;

    std::cout << "--- DOOR CONTROLLER ECU ---" << std::endl;
    std::cout << "Press 'o' to Open, 'c' to Close." << std::endl;

    while (true) {
        std::cout << "Door Status: " << (door_is_open ? "OPEN" : "CLOSED") 
                  << " | Command: ";
        
        
        std::cin >> key;

        if (key == 'o') door_is_open = true;
        if (key == 'c') door_is_open = false;

        // PACKING
        frame.data[0] = door_is_open ? 1 : 0;

        // SENDING
        write(s, &frame, sizeof(struct can_frame));
        std::cout << "Sent Door Status!" << std::endl;
    }

    close(s);
    return 0;
}

// THE BUREAUCRACY
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
