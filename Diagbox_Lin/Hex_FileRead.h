#ifndef __HEX_FILEREAD__H__
#define __HEX_FILEREAD__H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEX_LENGTH 16
struct  List_hex
{
	uint8_t number;
	uint16_t address;
	uint8_t type;
	uint8_t p_data[HEX_LENGTH];
	struct List_hex* next;
};
/*Function Declarations*/
int HexStr2Int(char buf);
struct List_hex* Hex_FileRead(FILE* fp);
struct List_hex* Format_Trans_hex(const char *hex_data);
int hex_data_byte(struct List_hex* PtrL);



#endif
