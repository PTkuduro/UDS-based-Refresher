#ifndef UDS_H_
#define UDS_H_
#include "socket_can.h"
#include <stdio.h>
#include "SA_algorithm.h"
#include <pthread.h>
/*SERVICE CODE*/
#define UDS_SESSION_CTRL_CODE 0x10
#define UDS_SECRUTIRY_ACCESS_CODE 0X27
#define UDS_COMMU_CTRL_CODE 0x28
#define UDS_TESTERPRESENT_CODE 0x3E
#define UDS_LINK_CTRL_CODE 0x87
#define UDS_DTC_CONF_CODE 0x85
#define UDS_RESET_CODE 0x11
#define UDS_READ_FPRINT_CODE 0x22
#define UDS_WRITE_FPRINT_CODE 0x2E
#define UDS_ROUTINE_CTRL_CODE 0x31
#define UDS_REQUEST_DOWN_CODE 0x34
#define UDS_TRANS_DATA_CODE 0x36
#define UDS_TRANS_EXIT_CODE 0x37


/*Variables in UDS and main*/
extern uint32_t MAX_BLOCK_LENTH;
extern uint32_t MAX_BLOCK_DATA_LENTH;
extern uint32_t GLOBAL_ADDRESS;
extern uint32_t GLOBAL_DATA_BYTE_ALL;
extern uint8_t SERVICE;
extern uint16_t GLOBAL_BLOCK_COUNT;
extern uint32_t GLOBAL_DATA_COUNT;
/*Global Variables*/
extern uint8_t  GLOBAL_SN;
/*Application*/
void Send_Service_Session_ctrl(uint8_t sub_para);
void Send_Service_Security_access(uint8_t sub_para);
void Send_Service_Communication_ctrl(uint8_t sub_para,uint8_t commu_type);
void Send_Service_Tester_ctrl(uint8_t sub_para);
void Send_Service_Link_ctrl(uint8_t sub_para,uint8_t Baudrate);
void Send_Service_Reset_ctrl(uint8_t reset_mode);
void Send_Service_Read_fprint(uint8_t DID_1, uint8_t DID_2);
void Send_Service_Write_fprint();
void Send_Service_Routine_ctrl(uint8_t mode,uint8_t ID_H,uint8_t ID_L);
void Send_Service_Erase_ctrl(uint32_t Address, uint32_t size);
void Send_Service_DTC_ctrl(uint8_t mode);
void Send_Service_Data_req(uint32_t Address, uint32_t size);
void Send_Service_Data_exit(uint8_t CRC_mode);
void Send_Service_Data_send_s19(uint32_t counter,const uint8_t* p_data,uint32_t BLOCK_LENTH);
int Parse_Rsp_Frame(const struct can_frame *rsp_frame);

/*Transmisson*/
int Receiving(ISOTP_indi Callback, struct can_frame* pf_receive, char service);
int Sending(ISOTP_req Callback, struct can_frame* pf,int frame_number);
int Send_Receive();

#endif
