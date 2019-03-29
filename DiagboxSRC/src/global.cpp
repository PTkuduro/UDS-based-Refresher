#include <cstdint>
struct can_frame *pf_SF;
struct can_frame *pf_CF;
struct can_frame *pf_Receive;
uint8_t GLOBAL_SN = 0X20;
uint32_t GLOBAL_ADDRESS = 0;
uint32_t GLOBAL_DATA_BYTE_ALL = 0;
uint16_t GLOBAL_BLOCK_COUNT = 0x00;
uint32_t GLOBAL_DATA_COUNT = 0;
uint32_t MAX_BLOCK_LENTH = 0;
uint32_t MAX_BLOCK_DATA_LENTH = 0;
int Send_State = 0;    //1 for request sending
int Receive_State = 0; //1 for received
uint8_t Frame_Number = 0;
uint8_t SERVICE = 0x00;