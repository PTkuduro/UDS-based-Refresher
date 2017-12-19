#include <stdio.h>
#include "UDS.h"
void getFuncPointer(char* sfuncname)  
{  
	if (strcmp(sfuncname, "") == 0)  
	{  
		return &func1;  
	}
	else if (strcmp(sfuncname, "") == 0) {  
		return &func2;  
	}  
	return NULL;                                                                                                    
}  