#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int setup_socket(const char *ifname);
void send_diagnostic(int s,int can_id,const char *message){
int len = strlen(message);
struct can_frame frame;
frame.can_id=can_id;
frame.can_dlc=8;
memset(frame.data,0xAA,8);
std::cout << "--- SENDING: " << message << " (" << len << " bytes) ---" << std::endl;
if (len<7){
    frame.data[0]=len;
    for (int i=1;i<=len;i++){
        frame.data[i]= message[i-1];
    }
    write(s,&frame,sizeof(struct can_frame));
    std::cout << "[SF] Sent Single Frame." << std::endl;
}
else{
frame.data[0]=0x10 | len>>8;
frame.data[1]= len & 0xFF;
for (int i =0;i<6;i++){
    frame.data[i+2]=message[i];
}
write(s, &frame,sizeof(struct can_frame));
        std::cout << "[FF] Sent First Frame. Waiting for Flow Control..." << std::endl;
usleep(10000);
int sent=6;
int nframe=1;
while (sent< len ){
    memset(frame.data, 0xAA, 8);
    frame.data[0]= 0x20 | (nframe & 0x0F);
    for(int i =1;i<8;i++){
        if (sent<len){
        frame.data[i]=message[sent];
        sent++;}
    }
    write(s, &frame, sizeof(struct can_frame));
     std::cout << "[CF] Sent Chunk #" << nframe<< std::endl;
     nframe++;
     if (nframe > 15) nframe = 0;
            
    usleep(5000); 
        
}
    
}
}






int main(){
int s=setup_socket("vcan0");

if (s < 0) {
     std::cerr << "Error setting up socket." << std::endl;
    return 1;
    }

send_diagnostic(s, 0x7E0, "HELLO");
    
 sleep(1); 


send_diagnostic(s, 0x7E0, "WBAM334500F987654");
close(s);
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
