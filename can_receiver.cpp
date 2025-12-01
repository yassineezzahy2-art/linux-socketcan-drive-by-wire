#include <iostream>
#include <cstring>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int main() {
    // 1. Create Socket
    int s;
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Socket Error");
        return 1;
    }

    // 2. Locate Interface
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    // 3. Bind Socket
    struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind Error");
        return 1;
    }

    std::cout << "Listening on vcan0..." << std::endl;

    // 4. The Infinite Loop (The "Engine" always runs)
    struct can_frame frame;
    while (true) {
        // This function BLOCKS (waits) until a message arrives
        int nbytes = read(s, &frame, sizeof(struct can_frame));

        if (nbytes < 0) {
            perror("Read Error");
            break;
        }

        // 5. Print the Message
        std::cout << "Received CAN Frame!" << std::endl;
        std::cout << "ID: 0x" << std::hex << frame.can_id << std::dec << std::endl;
        std::cout << "Data: " << std::string((char*)frame.data) << std::endl;
        std::cout << "-----------------------" << std::endl;
    }

    close(s);
    return 0;
}
