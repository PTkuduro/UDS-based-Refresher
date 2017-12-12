#include "timer.h"
struct timespec Get_Time_Limits()
{	struct timespec Outtime;
	gettimeofday(&Now, NULL);
	Outtime.tv_nsec = (Now.tv_usec+25*1000) * 1000;
	return Outtime;
}