#include <iostream>
#include <cstring>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int main() {
    // 1. Create a Socket
    int s;
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Socket Error");
        return 1;
    }

    // 2. Locate the Interface (vcan0)
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    // 3. Bind the Socket
    struct sockaddr_can addr;
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind Error");
        return 1;
    }

    // 4. Prepare the CAN Frame (ID: 0x555, Data: HELLO)
    struct can_frame frame;
    frame.can_id = 0x555;
    frame.can_dlc = 5;
    sprintf((char *)frame.data, "HELLO");

    // 5. Send the Frame
    std::cout << "Sending CAN Frame..." << std::endl;
    if (write(s, &frame, sizeof(frame)) != sizeof(frame)) {
        perror("Write Error");
        return 1;
    }

    std::cout << "Frame Sent! Check your candump terminal." << std::endl;

    close(s);
    return 0;
}

