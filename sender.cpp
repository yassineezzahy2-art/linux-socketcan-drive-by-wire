#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h>   
int setup_socket(const char *ifname);

void set_nonblocking_mode(bool enable);
int kbhit();

int main(){
    int s=setup_socket("vcan0");
    if (s<0) return 1;
    set_nonblocking_mode(true);
    struct can_frame frame;
    memset(frame.data,0,8);
    frame.can_id=0x123;
    frame.can_dlc=8;
    double rpm =0;
    std::cout << "--- REAL-TIME ENGINE STARTED ---" << std::endl;
    std::cout << "Engine is idling. Press 'w' to Rev, 's' to Brake, 'q' to Quit." << std::endl;
    while(true){
        if(kbhit()){
            char c=getchar();
            if (c=='w') rpm +=150;
            if (c=='s') rpm -=150;
            if (c=='q') break; 
        }
        if (rpm>0) rpm-=20;
        if (rpm<0) rpm=0;
        if (rpm>5000) rpm=5000;
        uint16_t send_rpm =rpm;
        frame.data[0]= send_rpm & 0xFF;
        frame.data[1] = (send_rpm>>8) & 0xFF;
        frame.data[2] = 90; // Temp
         write(s,&frame,sizeof(struct can_frame));
         std::cout << "RPM: " << send_rpm << "    \r" << std::flush;

        usleep(100000);


    }
set_nonblocking_mode(false);
    close(s);
    std::cout << "\nEngine Stopped." << std::endl;
    return 0;
}
int setup_socket(const char *ifname) {
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    struct ifreq ifr;
    std::strcpy(ifr.ifr_name, ifname);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) return -1;
    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) return -1;
    return s;
}


void set_nonblocking_mode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); 
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    }
}


int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
