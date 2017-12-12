/*
 * socket_can.h
 *
 *  Created on: 2017-11-14
 *      Author: raymond
 */

#ifndef SOCKET_CAN_H_
#define SOCKET_CAN_H_
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdio.h>
#include <string.h>

#define BS 8
#define FF_ID 0x74A
#define FC_ID 0X7CA

int s;  //Socket
struct sockaddr_can addr;
struct ifreq ifr;
extern uint8_t GLOBAL_SN;


/*CallBack function used by ISOTP*/
/*transmit the frame TP received to APP laye��state for Pos and Neg*/
typedef int(*ISOTP_indi)(struct can_frame* pf, char service);
/*Confirm state for Success and Failed*/
typedef int(*ISOTP_req)(struct can_frame* pf);



int Socket_Init(int s);
void Frame_Init_FF(struct can_frame* frame,uint32_t byte_send);
void Frame_Init_CF(struct can_frame* frame,int frame_count);
int Filter(unsigned int ID,int s);


int  indi_SF(struct can_frame* pf, char service);
int  indi_FC(struct can_frame* pf, char service);



int req_SF(struct can_frame* pf);
int req_CF(struct can_frame* pf);


#endif
