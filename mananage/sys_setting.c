#include "mtk_operate.h" 
#include "nvram.h"

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

	if(strcmp("reboot", webget("reboot_sys", input, 0)) == 0)
	{
		do_system("reboot");
	}
	if(strcmp("recover", webget("recover_sys", input, 0)) == 0)
	{
		recover_factory_setting();
	}
	if(strcmp("update", webget("update_sys", input, 0)) == 0)
	{
		//还没有做,在考虑如何升级固件
	}

}
