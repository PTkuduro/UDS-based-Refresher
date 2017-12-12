#include <stdio.h>
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
#include "Hex_FileRead.h"
#include "s19_FileRead.h"
#include "socket_can.h"
#include "UDS.h"
#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib")
#include <sys/time.h>
#include "timer.h"

/*************************ISOTP Thread*******************************/
pthread_mutex_t Send_Lock;  //Lock 
pthread_mutex_t Read_Lock;   //Lock 
pthread_cond_t Is_Req_Send;  //Conditional Variables
pthread_cond_t Is_Req_Read; 
int Send_Flag=0;//Ensure only send once
int Read_Flag=0;//Ensure only receive once
void *ISOTP(void)
{	
	/*Variables*/
	extern struct can_frame* pf_SF;
	extern struct can_frame* pf_CF;
	extern int Send_State;
	extern uint8_t Frame_Number;

	while (1)
	{   
		pthread_mutex_lock(&Send_Lock);
		if(Send_Flag==0)
		{
			pthread_cond_wait(&Is_Req_Send, &Send_Lock);
			if (Send_State == 0)
			{
				Sending(req_SF, pf_SF, 0);
				Send_Flag=1;
			}
			else if (Send_State != 0)
			{
				Sending(req_CF, pf_CF, Frame_Number);
				Send_Flag=1;
			}
			pthread_mutex_unlock(&Send_Lock);
		}
		else
		{
			pthread_cond_broadcast(&Is_Req_Send);
			pthread_mutex_unlock(&Send_Lock);
		}
	}
}
void *ISOTP_Rece(void)
{	
	/*Variables*/
	extern struct can_frame* pf_Receive;
	extern int Receive_State;
	extern uint8_t SERVICE;
	while (1)
	{
		pthread_mutex_lock(&Read_Lock);
		if(Read_Flag==0)
		{
			pthread_cond_wait(&Is_Req_Read, &Read_Lock);
			if (Receive_State== 0)
			{
				Receiving(indi_SF, pf_Receive, SERVICE);
				Read_Flag=1;
			}
			else if (Receive_State != 0)
			{
				Receiving(indi_FC, pf_Receive, SERVICE);
				Read_Flag=1;
			}
			pthread_mutex_unlock(&Read_Lock);
		}
		else
		{
			pthread_cond_broadcast(&Is_Req_Read);
			pthread_mutex_unlock(&Read_Lock);
		}
		
	}
}
/*************************ISOTP Thread*******************************/


int main()
{
	
/*Read Hex File, configure data_byte and data_list*/
	
	printf("0 for .hex file,1 for .s19 file\n");
	uint8_t ch;
	scanf("%d",&ch);
	FILE *fp=NULL;
	struct List_hex* PtrL_hex;
	struct List_s19* PtrL_s19;
	struct List_s19* tmp;
	if(ch==0)
	{
		fp=fopen("project.hex","r");
		PtrL_hex=Hex_FileRead(fp);
		GLOBAL_DATA_BYTE_ALL=hex_data_byte(PtrL_hex);
	}
	else if (ch==1)
	{
		fp=fopen("project.s19","r");
		PtrL_s19=S19_FileRead(fp);
		tmp=PtrL_s19;
		tmp=tmp->next;
		GLOBAL_ADDRESS=tmp->address;
		GLOBAL_DATA_BYTE_ALL=s19_data_byte(PtrL_s19);
	}
	fclose(fp);
	uint8_t* pd=calloc(GLOBAL_DATA_BYTE_ALL,sizeof(uint8_t));
	int k = 0;
	while (tmp->next != NULL)
	{
		for (int i = 0; i < tmp->number; i++)
		{
			*(pd+k) = tmp->p_data[i];
			k++;
		}
		tmp = tmp->next;
	}
/*Read Hex OR S19 File, configure data_byte and data_list*/


/*Initializing*/
	extern int Send_State;
	extern int Receive_State; 
	extern uint8_t Frame_Number;
	extern struct can_frame* pf_SF;
	extern struct can_frame* pf_CF;
	extern struct can_frame* pf_Receive;
	extern uint8_t SERVICE;
	extern uint32_t MAX_BLOCK_LENTH;
	pf_SF = (struct can_frame*)calloc(1, sizeof(struct can_frame));
	pf_CF= (struct can_frame*)calloc(1, sizeof(struct can_frame));
	pf_Receive = (struct can_frame*)calloc(1, sizeof(struct can_frame));
	s=Socket_Init(s);
	Filter(FC_ID,s);
	int ret=0;
	pthread_mutex_init(&Send_Lock, NULL);
	pthread_mutex_init(&Read_Lock, NULL);
	pthread_cond_init(&Is_Req_Send, NULL);
	pthread_cond_init(&Is_Req_Read, NULL);
	pthread_t PID;
	ret=pthread_create(&PID,NULL,(void*)ISOTP,NULL);
	pthread_t PID_Rece;
	ret = pthread_create(&PID_Rece,NULL,(void*)ISOTP_Rece,NULL);
	if(ret)
	{
		printf("create pthread error!\n");
		return -1;
	}
/*Initializing*/


/*Pre Programming*/
	Send_Service_Session_ctrl(0x03);
	if(Send_Receive())
		printf("Session 03 Success!\n");
	Send_Flag=0;
	Read_Flag=0;

	Send_Service_Communication_ctrl(0x03,0x01);
	if(Send_Receive())
		printf("Communication Success!\n");
	Send_Flag=0;
	Read_Flag=0;

	Send_Service_DTC_ctrl(0x02);
	if(Send_Receive())
		printf("DTC Success!\n");
	Send_Flag=0;
	Read_Flag=0;
	
/*Pre Programming*/

/*Programming*/
	Send_Service_Session_ctrl(0x02);
	if(Send_Receive())
		printf("Session 02 Success!\n");
	Send_Flag=0;
	Read_Flag=0;

	
	Send_Service_Security_access(0x05);
	if(Send_Receive())
			printf("Security Access!\n");
	Send_Flag=0;
	Read_Flag=0;
	
	Send_Service_Read_fprint(0XAF,0XFC);
	if(Send_Receive())
		printf("Read print! the flash times is %d\n",pf_Receive->data[5]);
	Send_Flag=0;
	Read_Flag=0;

	
	Send_Service_Routine_ctrl(0x01,0xDF,0xFF);
	if(Send_Receive())
		printf("Checking Integrity Succeed!,the state is %d\n",pf_Receive->data[5]);
	Send_Flag=0;
	Read_Flag=0;

	Send_State=1;
	Frame_Number=1;
	Send_Service_Erase_ctrl(GLOBAL_ADDRESS, GLOBAL_DATA_BYTE_ALL);
	if(Send_Receive())
		printf("Erase succeed!\n");
	Send_Flag=0;
	Read_Flag=0;

	
	Send_Service_Data_req(GLOBAL_ADDRESS,GLOBAL_DATA_BYTE_ALL);

	if(Send_Receive())
		printf("Data-req succeed!\n");
	Send_Flag=0;
	Read_Flag=0;

	MAX_BLOCK_LENTH |= pf_Receive->data[3];
	MAX_BLOCK_LENTH = MAX_BLOCK_LENTH << 8;
	MAX_BLOCK_LENTH |= pf_Receive->data[4];
	MAX_BLOCK_DATA_LENTH = MAX_BLOCK_LENTH - 2;
	






	/*if (GLOBAL_DATA_BYTE_ALL % MAX_BLOCK_DATA_LENTH == 0)
		GLOBAL_BLOCK_COUNT = (GLOBAL_DATA_BYTE_ALL /MAX_BLOCK_DATA_LENTH) - 1;
	else
		GLOBAL_BLOCK_COUNT = (GLOBAL_DATA_BYTE_ALL / MAX_BLOCK_DATA_LENTH);
	int download_times = 0;
	while (download_times < GLOBAL_BLOCK_COUNT)
	{
		Send_Service_Data_send_s19(download_times + 1, pd, MAX_BLOCK_DATA_LENTH);
		do{read(s, pf_Receive, sizeof(*pf_Receive)); } while (pf_Receive->data[1] != 0x76);
		download_times++;
	}
	uint32_t REMAIN_BLOCK_DATA_LENTH = (GLOBAL_DATA_BYTE_ALL - MAX_BLOCK_DATA_LENTH*download_times);
	if (download_times == GLOBAL_BLOCK_COUNT)
	{
		Send_Service_Data_send_s19(download_times + 1, pd, REMAIN_BLOCK_DATA_LENTH);
		do{read(s, pf_Receive, sizeof(*pf_Receive)); } while (pf_Receive->data[1] != 0x76);
	}*/






	Send_State=0;
	Frame_Number=0;
	Send_Service_Data_exit(0x00);
	if(Send_Receive())
		printf("Download succeed!");
	Send_Flag=0;
	Read_Flag=0;

/*Programming*/

/*Past Programming*/
	Send_Service_Routine_ctrl(0x01,0xDF,0xFF);
	if(Send_Receive())
		printf("Checking Integrity Succeed!,the state is %d\n",pf_Receive->data[5]);
	Send_Flag=0;
	Read_Flag=0;

	
	Send_Service_Routine_ctrl(0x01,0xFF,0x01);
	if(Send_Receive())
		printf("Checking Dependencies Succeed!,the state is %d\n",pf_Receive->data[5]);
	Send_Flag=0;
	Read_Flag=0;

	
	Send_Service_Reset_ctrl(0x01);
	if(Send_Receive())
		printf("Reset once");
	Send_Flag=0;
	Read_Flag=0;
	
	Send_Service_Session_ctrl(0x01);
	if(Send_Receive())
		printf("Finished!");
	Send_Flag=0;
	Read_Flag=0;

	
    close(s);
	pthread_cancel(PID);
	pthread_cancel(PID_Rece);
    return 0;
}


