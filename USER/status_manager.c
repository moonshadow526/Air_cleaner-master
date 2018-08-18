#include "status_manager.h"
static int status_flag = 0;

void set_status_flag(u8 status)
{
	status_flag = status;
}

int get_status_flag(void)
{
	return status_flag;
}
