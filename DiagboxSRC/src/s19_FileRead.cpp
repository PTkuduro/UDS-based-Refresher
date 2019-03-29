
#include "s19_FileRead.h"
#include <cstring>
int s19_data_byte(struct List_s19* PtrL){
	static int data_byte = 0;
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
struct List_s19* S19_FileRead(FILE* fp){
	char *buffer = (char*)calloc(100, sizeof(char));
	if (fp == NULL)
		printf("Open file error.\n");
	struct List_s19 *PtrL, *tmp;
	PtrL = (struct List_s19*)calloc(1, sizeof(struct List_s19));
	PtrL->number = 0;
	tmp = PtrL;	
	while (fscanf(fp, "%s", buffer)!=EOF)
	{
		struct List_s19 *s;
		s = Format_Trans_s19(buffer);
		tmp->next = s;
		tmp = s;
	}
	tmp = PtrL;//delete first line
	tmp = tmp->next;
	PtrL->next = tmp->next;
	tmp->next = NULL;
	free(tmp);
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
struct List_s19* Format_Trans_s19(const char *s19_data)
{
	int data_length=strlen(s19_data),data_index = 0;
	struct List_s19* s;
	s = (struct List_s19*)calloc(1,sizeof(struct List_s19));
	s->next = NULL;
	if (data_length < 70)
		return s;
	while (data_index < data_length - 2)//Drop out CRC
	{
		if (data_index == 0)
		{
			data_index++;
			continue;
		}

		if (data_index == 1)
		{
			data_index++;
			continue;
		}
		if (data_index == 2)//byte of number
		{
			s->number = s->number | S19Str2Int(s19_data[data_index++]);
			s->number = s->number << 4 | S19Str2Int(s19_data[data_index++]);
			s->number -= 5;
			continue;
		}
		if (data_index == 4)//byte of address
		{
			s->address = s->address | S19Str2Int(s19_data[data_index++]);
			s->address = ((s->address << 4) | S19Str2Int(s19_data[data_index++]));
			s->address = ((s->address << 4) | S19Str2Int(s19_data[data_index++]));
			s->address = ((s->address << 4) | S19Str2Int(s19_data[data_index++]));
			s->address = ((s->address << 4) | S19Str2Int(s19_data[data_index++]));
			s->address = ((s->address << 4) | S19Str2Int(s19_data[data_index++]));
			s->address = ((s->address << 4) | S19Str2Int(s19_data[data_index++]));
			s->address = ((s->address << 4) | S19Str2Int(s19_data[data_index++]));
			continue;
		}
		else
		{
			for (int i = 0; i < s->number; i++)//
			{
				s->p_data[i] = s->p_data[i] | S19Str2Int(s19_data[data_index++]);
				s->p_data[i] = (s->p_data[i] << 4) | S19Str2Int(s19_data[data_index++]);
			}
		}

	}
	return s;
}

int S19Str2Int(char buf)
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
