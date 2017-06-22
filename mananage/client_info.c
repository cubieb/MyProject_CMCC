#include "adm_info.h"

#include "mtk_operate.h"

/*
 * who's in the SoftAP Client. 
 * information of the Client app.
 */


int main(int argc, char *argv[])
{

#if 1  //获取设备的信息存储到本地
	FILE *fp; 
	fp = fopen(CLIENT_LSIT, "a+");
	if(fp == NULL)
		return -1;
	getdevicelist(fp);
	fclose(fp);
#endif 

	char input[MAX_MSG_SIZ];
	int length;

	length = get_message_from_web(input);

	//for web client 
	web_debug_header();

	//web端请求谁在上网
	if(strcmp("wholine", web_get("Client_Info", input, 0)) == 0)
	{
		getclientlist();
	}

	//change the hostname
	if(strcmp("changehost", web_get("changename", input, 0)) == 0)
	{
		char HostName[32];
		char Mac[64];
		strcpy(HostName, web_get("HostName", input, 0));
		strcpy(Mac, web_get("Mac", input, 0));

		FILE *fp;
		fp = fopen(CLIENT_LSIT, "r+");
		if(fp == NULL)
			return -1;

		change_hostname(HostName, Mac, fp);

		fclose(fp);
	}

	//add to the black list
	if(strcmp("black", web_get("blacklist", input, 0)) == 0)
	{
		char Mac[18];
		strcpy(Mac, web_get("Mac", input, 0));
		addblacklist(Mac);
	}

	//delete black list
	if(strcmp("", web_get("", input, 0)) == 0)
	{
		char Mac[18];
		strcpy(Mac, web_get("", input, 0));
		delblacklist(Mac);
	}

	//show black list
	if(strcpy("", web_get("", input, 0)) == 0)
	{
		showblacklist();
	}

	return 0;
}

