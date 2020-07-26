#include <chrono>
#include <iostream>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <linux/can.h>
#include <linux/can/raw.h>
// int main(int argc, char *argv[])
// {
//    std::cout << "Hello CMake!" << std::endl;
struct can_frame rxframe;

int rxCounter = 0;
int trErrorCounter = 0;
void rxThread(int s) {

  for (int i = 0;; i++) {
    int nbytes;
    struct can_frame rxframe;
    nbytes = read(s, &rxframe, sizeof(struct can_frame));
    if (nbytes < 0) {
      perror("Read");
      break;
    }
    printf("0x%03X [%d] ", rxframe.can_id, rxframe.can_dlc);
    for (i = 0; i < rxframe.can_dlc; i++)
      printf("%02X ", rxframe.data[i]);
    printf("\r\n");
    rxCounter++;
    printf("the %d  rx\r\n", rxCounter);
  }
}

int main(void) {
  int s;
  int nbytes;
  struct sockaddr_can addr;
  struct can_frame frame;
  struct ifreq ifr;

  const char *ifname = "can0";

  if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
    perror("Error while opening socket");
    return -1;
  }

  strcpy(ifr.ifr_name, ifname);
  ioctl(s, SIOCGIFINDEX, &ifr);

  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;

  printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

  if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Error in socket bind");
    return -2;
  }

  std::thread canRx(rxThread, s);

  for (int i = 0; i < 50000; i++) {
    frame.can_id = i;
    frame.can_dlc = 8;
    for (int j = 0; j < 8; j++) {
      frame.data[j] = 0xff;
    }
    frame.data[0] = (i & 0xff000000) >> 24;
    frame.data[1] = (i & 0x00ff0000) >> 16;
    frame.data[2] = (i & 0x0000ff00) >> 8;
    frame.data[3] = (i & 0x000000ff);

    nbytes = write(s, &frame, sizeof(struct can_frame));
    printf("Wrote %d bytes\n", nbytes);
    if (nbytes == -1) {
      printf("send error\n");
      trErrorCounter++;
      //   return 0;
    }
    std::this_thread::sleep_for(
        std::chrono::nanoseconds(500000)); // 10 6600 9 7200   8  7700
  }
  printf("send over\n");
  printf(" trErrorCounter is  %d  \r\n", trErrorCounter);
  printf("the final number rx is  %d  \r\n", rxCounter);
  return 0;
}