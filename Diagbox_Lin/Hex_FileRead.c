#include "Hex_FileRead.h"

/*Define Functions*/
/*BEGIN_FUNCTION_HDR*
**************************************************************************
*Function Name:data_byte
*Description:Count data_byte in the Hex file
*Inputs:File pointer
*Outputs:Number of data bytes
*Limitations
**************************************************************************
END_FUNCTION_HDR*/
int hex_data_byte(struct List_hex* PtrL){
	int data_byte = 0;
	while(PtrL->next != NULL)
	{
		data_byte += PtrL->number;
		PtrL = PtrL->next;
	}
	return data_byte;
}
/*BEGIN_FUNCTION_HDR*
**************************************************************************
*Function Name:Hex_FileRead
*Description:Read .hex file and store data
*Inputs:File name 
*Outputs:Pointer to data list.REMERMBER the head Node is NULL!
*Limitations
**************************************************************************
END_FUNCTION_HDR*/
struct List_hex* Hex_FileRead(FILE* fp){
	char *buffer = malloc(sizeof(char) * 100);
	if (fp == NULL)
		printf("Open file error.\n");
	struct List_hex *PtrL, *tmp;
	PtrL = (struct List_hex*)calloc(1,sizeof(struct List_hex));
	PtrL->number = 0;
	tmp = PtrL;
	while (fscanf(fp, "%s", buffer) != EOF)
	{
		struct  List_hex* s;
		s = Format_Trans_hex(buffer);
		tmp->next = s;
		tmp = s;
	}
	return PtrL;
}

/*BEGIN_FUNCTION_HDR*
**************************************************************************
*Function Name:Hex_FileRead
*Description:Store the data of .hex in a list, dropped out the CRC
*Inputs:File name
*Outputs:Success of Failed and a list of data
*Limitations
**************************************************************************
END_FUNCTION_HDR*/
struct List_hex* Format_Trans_hex(const char *hex_data)
{	
	int data_length=strlen(hex_data),data_index = 0;
	struct List_hex* s;
	s = (struct List_hex*)calloc(1,sizeof(struct List_hex));
	s->next = NULL;
	while (data_index < data_length - 2)//Drop out CRC
	{
		if (data_index == 0)
		{
			data_index++;
			continue;
		}

		if (data_index == 1)//byte of data lenth
		{
			s->number = s->number | HexStr2Int(hex_data[data_index++]);
			s->number = s->number << 4 | HexStr2Int(hex_data[data_index++]);
			continue;
		}
		if (data_index == 3)//byte of address
		{
			s->address = s->address | HexStr2Int(hex_data[data_index++]);
			s->address = s->address << 4 | HexStr2Int(hex_data[data_index++]);
			s->address = s->address << 4 | HexStr2Int(hex_data[data_index++]);
			s->address = s->address << 4 | HexStr2Int(hex_data[data_index++]);
			continue;
		}
		if (data_index == 7)//byte of data type
		{
			s->type = s->type | HexStr2Int(hex_data[data_index++]);
			s->type = ((s->type << 4) | HexStr2Int(hex_data[data_index++]));
			continue;
		}
		else
		{
			for (int i = 0; i < s->number; i++)//
			{
				s->p_data[i] = s->p_data[i] | HexStr2Int(hex_data[data_index++]);
				s->p_data[i] = (s->p_data[i] << 4) | HexStr2Int(hex_data[data_index++]);
			}
		}

	}
	return s;
}

int HexStr2Int(char buf)
{
	int result = 0;
	int tmp;

	if (buf >= 'A'&& buf <= 'Z')
		tmp = buf - 'A' + 10;
	else if (buf >= 'a'&& buf <= 'z')
		tmp = buf - 'a' + 10;
	else
		tmp = buf - '0';
	result *= 16;
	result += tmp;

	return result;
}
