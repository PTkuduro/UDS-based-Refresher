
#ifndef SA_ALGORITH_H_
#define SA_ALGORITH_H_
#include <stdint.h>
#include <linux/can.h>
#include <linux/can/raw.h>




uint32_t MASK=0xF5F58181;
uint32_t Key=0;
void Seed2Key(struct can_frame* pos_resp, struct can_frame* key_frame){

	long int seed=0;
	key_frame->can_dlc=8;
	key_frame->can_id=FF_ID;
	key_frame->data[0]=0x06;
	key_frame->data[1]=0X27;
	key_frame->data[2]=pos_resp->data[2]+1;

	seed=((long int)pos_resp->data[3]<<24)+((long int)pos_resp->data[4]<<16)+((long int)pos_resp->data[5]<<8)+((long int)pos_resp->data[6]);

	if(seed!=0)
	{
		for(int i=0;i<35;i++)
		{
			if(seed&0x80000000)
			{
				seed=seed<<1;
				seed=seed^MASK;
			}
			else
			{
				seed=seed<<1;
			}
		}
	}
	Key=seed;
	key_frame->data[3]=(Key&0xFF000000)>>24;
	key_frame->data[4]=(Key&0x00FF0000)>>16;
	key_frame->data[5]=(Key&0x0000FF00)>>8;
	key_frame->data[6]=Key&0x000000FF;
}

#endif /* SA_ALGORITH_H_ */
