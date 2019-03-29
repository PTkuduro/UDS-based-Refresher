#include "UDS.h"
#include <memory.h>
#include "SA_algorithm.h"
extern struct can_frame* pf_SF;
extern struct can_frame* pf_Receive;
extern struct can_frame* pf_CF;
extern uint8_t SERVICE;
extern uint8_t Frame_Number;
extern pthread_mutex_t Send_Lock;  //Lock
extern pthread_mutex_t Read_Lock;   //Lock
extern pthread_cond_t Is_Req_Send;  //Conditional Variables
extern pthread_cond_t Is_Req_Read;
extern int Send_Flag;//Ensure only send once
extern int Read_Flag;//Ensure only receive once
void Send_Service_Session_ctrl(uint8_t sub_para)
{	memset(pf_SF,0,sizeof(struct can_frame));
	pf_SF->can_dlc = 8;
	pf_SF->can_id = FF_ID;
	pf_SF->data[0] = 0x02;
	pf_SF->data[1] = SERVICE = UDS_SESSION_CTRL_CODE;
	pf_SF->data[2] = sub_para;
}
void Send_Service_Security_access(uint8_t sub_para)
{	memset(pf_SF,0,sizeof(struct can_frame));
	pf_SF->can_dlc=8;
	pf_SF->can_id=FF_ID;
	pf_SF->data[0]=0x02;
	pf_SF->data[1]=UDS_SECRUTIRY_ACCESS_CODE;
	pf_SF->data[2]=sub_para;
	write(s,pf_SF,sizeof(*pf_SF));
	do {read(s,pf_Receive,sizeof(*pf_Receive));}
		while((pf_Receive->data[3])==0x78&&(pf_Receive->data[1]==0x7F));
	memset(pf_SF,0,sizeof(struct can_frame));
	Seed2Key(pf_Receive,pf_SF);
;
}
void Send_Service_Communication_ctrl(uint8_t sub_para, uint8_t commu_type)
{	memset(pf_SF,0,sizeof(struct can_frame));
	pf_SF->can_dlc = 8 ;
	pf_SF->can_id = FF_ID ;
	pf_SF->data[0] = 0x03 ;
	pf_SF->data[1] = SERVICE =UDS_COMMU_CTRL_CODE;
	pf_SF->data[2] = sub_para ;
	pf_SF->data[3] = commu_type;
}
void Send_Service_Read_fprint(uint8_t DID_1, uint8_t DID_2) {
	memset(pf_SF,0,sizeof(struct can_frame));
	pf_SF->can_dlc = 8 ;
	pf_SF->can_id = FF_ID ;
	pf_SF->data[0] = 0X03 ;
	pf_SF->data[1] = SERVICE=UDS_READ_FPRINT_CODE;
	pf_SF->data[2] = DID_1 ;
	pf_SF->data[3] = DID_2 ;
}
void Send_Service_Write_fprint() {

	pf_SF->can_dlc = 8 ;
	pf_SF->can_id = FF_ID ;
	pf_SF->data[0] = SERVICE=UDS_WRITE_FPRINT_CODE;
}
void Send_Service_Routine_ctrl(uint8_t mode, uint8_t ID_H, uint8_t ID_L)
{
	memset(pf_SF,0,sizeof(struct can_frame));
	pf_SF->can_dlc = 8 ;
	pf_SF->can_id = FF_ID ;
	pf_SF->data[0] = 0x04 ;
	pf_SF->data[1] = SERVICE=UDS_ROUTINE_CTRL_CODE;
	pf_SF->data[2] = mode ;
	pf_SF->data[3] = ID_H ;
	pf_SF->data[4] = ID_L ;
}

void Send_Service_Reset_ctrl(uint8_t reset_mode)

{	memset(pf_SF,0,sizeof(struct can_frame));
	pf_SF->can_dlc = 8 ;
	pf_SF->can_id = FF_ID ;
	pf_SF->data[0] = 0x02 ;
	pf_SF->data[1] = SERVICE=UDS_RESET_CODE;
}
void Send_Service_DTC_ctrl(uint8_t mode) {
	memset(pf_SF,0,sizeof(struct can_frame));
	pf_SF->can_dlc = 8 ;
	pf_SF->can_id = FF_ID ;
	pf_SF->data[0] = 0x02 ;
	pf_SF->data[1] = SERVICE=UDS_DTC_CONF_CODE;
	pf_SF->data[2] = mode ;
}
void Send_Service_Data_req(uint32_t address, uint32_t size)

{	extern uint8_t GLOBAL_SN;
	memset(pf_SF,0,sizeof(struct can_frame));
	Frame_Init_FF(pf_SF,11);
	pf_SF->data[2]=UDS_REQUEST_DOWN_CODE;
	pf_SF->data[3]=0x00;
	pf_SF->data[4]=0x44;
	pf_SF->data[5]=(address&0xFF000000)>>24;
	pf_SF->data[6]=(address&0x00FF0000)>>16;
	pf_SF->data[7]=(address&0x0000FF00)>>8;
	GLOBAL_SN=0x21;
	Frame_Init_CF(pf_CF,1);
	pf_CF->data[1]=address&0x000000FF;
	pf_CF->data[2]=(size&0xFF000000)>>24;
	pf_CF->data[3]=(size&0x00FF0000)>>16;
	pf_CF->data[4]=(size&0x0000FF00)>>8;
	pf_CF->data[5]=size&0x000000FF;
	write(s,pf_SF,sizeof(*pf_SF));
	do {read(s,pf_Receive,sizeof(*pf_Receive));}
		while(pf_Receive->data[0]!=0x30);


}
void Send_Service_Data_exit(uint8_t CRC_mode)
{	memset(pf_SF,0,sizeof(struct can_frame));
	pf_SF->can_dlc = 8 ;
	pf_SF->can_id = FF_ID ;
	pf_SF->data[0] = 0x02 ;
	pf_SF->data[1] = SERVICE=UDS_TRANS_EXIT_CODE ;
	pf_SF->data[2] = CRC_mode ;
}
void Send_Service_Erase_ctrl(uint32_t address, uint32_t size)
{ 	memset(pf_SF,0,sizeof(struct can_frame));
	Frame_Init_FF(pf_SF,13) ;
	pf_SF->data[2] = UDS_ROUTINE_CTRL_CODE ;
	pf_SF->data[3] = 0x01 ;
	pf_SF->data[4] = 0xFF ;
	pf_SF->data[5] = 0x00 ;
	pf_SF->data[6] = 0x44 ;
	pf_SF->data[7] = (address & 0xFF000000) >> 24 ;
	GLOBAL_SN= 0x21 ; //needs to de changed
	Frame_Init_CF(pf_CF, 1) ;
	pf_CF->data[1] = (address & 0x00FF0000) >> 16 ;
	pf_CF->data[2] = (address & 0x0000FF00) >> 8 ;
	pf_CF->data[3] = address & 0x000000FF ;
	pf_CF->data[4] = (size & 0xFF000000) >> 24 ;
	pf_CF->data[5] = (size & 0x00FF0000) >> 16 ;
	pf_CF->data[6] = (size & 0x0000FF00) >> 8 ;
	pf_CF->data[7] = size & 0x000000FF ;
	write(s,pf_SF,sizeof(*pf_SF));
	do {read(s,pf_Receive,sizeof(*pf_Receive));}
		while(pf_Receive->data[0]!=0x30);
}

void Send_Service_Data_send_s19(uint32_t Counter, const uint8_t* pd, uint32_t BLOCK_DATA_LENTH)

{	extern int Send_State;
	extern int Receive_State; 
	Send_State = 0;//Send SF
	Receive_State = 1;//Rece FC
	int CF_Loops = 0;
	if (((BLOCK_DATA_LENTH - 4) % 56 == 0))
		CF_Loops = ((BLOCK_DATA_LENTH - 4) / 56) - 1;
	else
		CF_Loops = ((BLOCK_DATA_LENTH - 4) / 56);
	extern struct can_frame* pf_SF;
	extern struct can_frame* pf_Receive;
	memset(pf_SF, 0, sizeof(struct can_frame));
	Frame_Init_FF(pf_SF, BLOCK_DATA_LENTH+2);
	pf_SF->data[2] = UDS_TRANS_DATA_CODE;
	pf_SF->data[3] = Counter;
	for (int i = 4; i < 8; i++)
	{
		pf_SF->data[i] = pd[GLOBAL_DATA_COUNT++];
	}
	Send_Receive();
	int i = 0;
	Send_State = 1;//Send CF
	Frame_Number=8;
	GLOBAL_SN = 0x21;
	while (i < CF_Loops)
	{
		pf_CF = (struct can_frame*)calloc(BS, sizeof(struct can_frame));
		Frame_Init_CF(pf_CF, BS);
		for (int k = 0; k < BS; k++)
		{
			for (int j = 1; j < 8; j++)
			{
				pf_CF[k].data[j] = pd[GLOBAL_DATA_COUNT++];
			}
		}
		Send_Receive();
		i++;
		free(pf_CF);

	}

	int Remain_Frame = 0;
	if (i == CF_Loops)
	{
		Receive_State = 0;//Rece SF
		if (((BLOCK_DATA_LENTH - i * 56 - 4) % 7) == 0)
		{
			Remain_Frame = ((BLOCK_DATA_LENTH - i * 56 - 4) / 7);
			Frame_Number=Remain_Frame;//Change the Frame
			pf_CF = (struct can_frame*)calloc(Remain_Frame, sizeof(struct can_frame));
			Frame_Init_CF(pf_CF, Remain_Frame);
			for (int Frame_count = 0; Frame_count < Remain_Frame; Frame_count++)
			{
				for (int data_count = 1; data_count < 8; data_count++)
				{
					pf_CF[Frame_count].data[data_count] = pd[GLOBAL_DATA_COUNT++];
				}
			}
			Send_Receive();
			free(pf_CF);
		}
		else
		{
			int remain_ID = 0;
			remain_ID = 1 + ((BLOCK_DATA_LENTH - i * 56 - 4) % 7);
			int k = 0;
			Remain_Frame = ((BLOCK_DATA_LENTH - i * 56 - 4) / 7) + 1;
			Frame_Number=Remain_Frame;
			pf_CF = (struct can_frame*)calloc(Remain_Frame, sizeof(struct can_frame));
			Frame_Init_CF(pf_CF, Remain_Frame);
			while (k < (Remain_Frame - 1))
			{
				for (int data_count = 1; data_count < 8; data_count++)
				{
					pf_CF[k].data[data_count] = pd[GLOBAL_DATA_COUNT++];
				}
				k++;
			}
			if (k == (Remain_Frame - 1))
			{
				for (int j = 1; j < remain_ID; j++)
				{
					pf_CF[k].data[j] = pd[GLOBAL_DATA_COUNT++];
				}
			}
			Send_Receive();
			free(pf_CF);
		}

	}


}


int Receiving(ISOTP_indi Callback, struct can_frame* pf,char service)
{
	int rsp = 0;
	rsp=Callback(pf, service);
	return rsp;
}
int Sending(ISOTP_req Callback, struct can_frame* pf, int frame_number)
{	int state;
	if (frame_number == 0)
	{
		state = Callback(pf);
		return state;
	}
	else
	{
		for (int i = 0; i < frame_number; i++)
		{
			state = Callback(pf);
			pf++;

		}
		return state;
	}
}
int Parse_Rsp_Frame(const struct can_frame *rsp_frame)
{
	if (rsp_frame->data[1] == 0x7F)
		return -1;
	else if (rsp_frame->data[1] == SERVICE + 0x40)
		return 1;
	else
		return -1;
 }
int Send_Receive(){
	extern struct can_frame* pf_Receive;
	int rece_parse=0;
	while(1)
	{
		pthread_mutex_lock(&Send_Lock);
		pthread_cond_broadcast(&Is_Req_Send);
		pthread_mutex_unlock(&Send_Lock);
		if(Send_Flag==1)
			break;
	}
	while(1)
	{
		pthread_mutex_lock(&Read_Lock);
		pthread_cond_broadcast(&Is_Req_Read);
		pthread_mutex_unlock(&Read_Lock);
		if(Read_Flag==1)
			break;
	}
	rece_parse=Parse_Rsp_Frame(pf_Receive);
	Send_Flag = 0;
	Read_Flag = 0;
	return rece_parse;
}

