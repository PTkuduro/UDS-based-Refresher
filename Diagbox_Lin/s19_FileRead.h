#ifndef __S19_FILEREAD__H__
#define __S19_FILEREAD__H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define S19_LENGTH 32
struct  List_s19
{
	uint8_t number;
	uint32_t address;
	uint8_t p_data[S19_LENGTH];
	struct List_s19* next;
};
/*Function Declarations*/
int S19Str2Int(char buf);
struct List_s19* S19_FileRead(FILE* fp);
struct List_s19* Format_Trans_s19(const char *s19_data);
int s19_data_byte(struct List_s19* PtrL);

#endif