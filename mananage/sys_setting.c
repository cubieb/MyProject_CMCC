#include "mtk_operate.h" 
#include "nvram.h"

#include <string.h>

/*
 * 主要是实现系统的进本设置
 * 重启，恢复出厂设置，固件升级三个功能
 */

int main(int argc, char *argv[])
{
	char input[MAX_MSG_SIZ];
	int length;

	length = get_message_from_web(input);
	if(length <= 0)
		DBG_MSG("get the message is empty or failed!");

	char *temp;
	temp = webget("reboot_sys", input, 0);
	if(strcmp("reboot", temp) == 0)
	{
		do_system("reboot");
	}

	temp = webget("recover_sys", input, 0);
	if(strcmp("recover", temp) == 0)
	{
		recover_factory_setting();
	}

	temp = webget("update_sys", input, 0);
	if(strcmp("update", temp) == 0)
	{
		//还没有做,在考虑如何升级固件
	}

}
