#include "socket_can.h"

int Socket_Init(int s)
{
	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	strcpy(ifr.ifr_name, "can0");
	ioctl(s, SIOCGIFINDEX, &ifr);
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	bind(s, (struct sockaddr *) &addr, sizeof(addr));
	return s;
}
void Frame_Init_FF(struct can_frame* frame,uint32_t byte_send)
{
		frame->can_id=FF_ID;
		frame->can_dlc=8;
		frame->data[0]=0xf&(byte_send>>8);
		frame->data[0]|=0x10;
		frame->data[1]=0xff&byte_send;
}
void Frame_Init_CF(struct can_frame* Frame,int Frame_Count){
	for(int i=0;i<Frame_Count;i++)
	{
		Frame[i].can_id = FF_ID;
		Frame[i].data[0] = GLOBAL_SN;
		Frame[i].can_dlc = 8;
		GLOBAL_SN++;
		if (GLOBAL_SN > 0x2F)
			GLOBAL_SN=0x21;
	}
}
int Filter(unsigned int ID, int s)
{
	struct can_filter Rx_filter;

	Rx_filter.can_id = ID;
	Rx_filter.can_mask = CAN_SFF_MASK;
	if (ID == 0)
	{
		setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
		return 0;
	}
	else
	{
		setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &Rx_filter, sizeof(Rx_filter));
		return -1;
	}
}
int indi_FC(struct can_frame* pf, char service)
{	
	int bytes;
	bytes = read(s, pf, sizeof(*pf));
	while ((pf->data[0] != 0x30))
	{
		bytes = read(s, pf, sizeof(*pf)); 
	}
	if (bytes)
		return 1;
	else
		return -1;
}

int indi_SF(struct can_frame* pf, char service)
{
	int bytes;
	bytes = read(s, pf, sizeof(*pf));
	while((pf->data[3])==0x78&&(pf->data[1]==0x7F))
		bytes = read(s, pf, sizeof(*pf));
	if (bytes)
		return 1;
	else 
		return -1;
		
}
int req_SF(struct can_frame* pf)
{	int byte = 0;
	byte=write(s, pf, sizeof(*pf));
	if ((pf->data[0] & 0xF0) == 0x00&&byte>0)
		return 1;
	else 
		return -1;
}	
int req_CF(struct can_frame* pf)
{
	int byte = 0;
	byte = write(s, pf, sizeof(*pf));
	if ((pf->data[0]&0xF0) == 0x20&&byte>0)
		return 1;
	else 
		return -1;
}
int req_FF(struct can_frame* pf)
{
	int byte = 0;
	byte = write(s, pf, sizeof(*pf));
	if ((pf->data[0] & 0xF0) == 0x10&&byte>0)
		return 1;
	else 
		return -1;
	
}



