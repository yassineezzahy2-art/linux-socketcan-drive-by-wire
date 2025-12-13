#include <iostream>
#include <cstring>
#include <vector>       
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <iomanip>      
std::vector<uint8_t> rx_buffer; 


int expected_length = 0; 


bool receiving_multi_frame = false;


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

int main() {
    
    int s = setup_socket("vcan0");
    std::cout << "--- ISO-TP RECEIVER LISTENING ---" << std::endl;

    
    struct can_frame frame;
    
    
    while(true) {
    
 

        int nbytes = read(s, &frame, sizeof(struct can_frame));

        
        if (nbytes < 0) continue;
        
        uint8_t type = frame.data[0] & 0xF0; 

        
        if (type == 0x00) {
            
            int len = frame.data[0] & 0x0F; 
            
            std::cout << "[SF] Received: ";
           
            for (int i=1; i <= len; i++) {
                std::cout << (char)frame.data[i];
            }
            std::cout << std::endl;
        }

        
        else if (type == 0x10) {
            
            expected_length = ((frame.data[0] & 0x0F) << 8) | frame.data[1];
            
            std::cout << "[FF] Start Multi-Frame. Size: " << expected_length << std::endl;
            
       
            rx_buffer.clear();
            receiving_multi_frame = true;

           
            for (int i=2; i<8; i++) {
                rx_buffer.push_back(frame.data[i]);
            }
        }

        else if (type == 0x20) {
           
            if (!receiving_multi_frame) continue; 
          
            for (int i=1; i<8; i++) {
                
                if (rx_buffer.size() < expected_length) {
                   
                    rx_buffer.push_back(frame.data[i]);
                }
            }

           
            if (rx_buffer.size() >= expected_length) {
                std::cout << "[COMPLETE] Message: ";
                
                
                for (uint8_t byte : rx_buffer) {
                    std::cout << (char)byte;
                }
                std::cout << std::endl;
                
                
                receiving_multi_frame = false;
                expected_length = 0;
            }
        }
    }
    return 0;
}
