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

	//char res[1024];

	DBG_MSG("get messge from web:%s\n", input);
	//unencode(input, length, res);
	//web端请求谁在上网
	if(strcmp("wholine", web_get("Client_Info", input, 0)) == 0)
	{
		DBG_MSG("to the here: %d %s",__LINE__, __FILE__);
		getclientlist();
	}
#if 1
	//change the hostname
	if(strcmp("changehost", web_get("changename", input, 0)) == 0)
	{
		char HostName[32];
		char Mac[64];
		strcpy(HostName, web_get("hostname", input, 0));
		strcpy(Mac, web_get("mac", input, 0));

		DBG_MSG("%s  %d %s", HostName, __LINE__, __FILE__);
		DBG_MSG("%s  %d %s", Mac, __LINE__, __FILE__);

		FILE *fp;
		fp = fopen(CLIENT_LSIT, "r+");
		if(fp == NULL)
			return -1;

		change_hostname(HostName, Mac, fp);

		fclose(fp);
	}
#endif 

	//add to the black list
	if(strcmp("black", web_get("blacklist", input, 0)) == 0)
	{
		DBG_MSG("%s  %d  %s", input, __LINE__, __FILE__);
	//	char Mac[18];
	//	if(strcpy(Mac, web_get("mac", input, 0)) != NULL)
	//		addblacklist(Mac);
	}

#if 0

	//delete black list
	if(strcmp("", web_get("", input, 0)) == 0)
	{
		DBG_MSG("%s  %d  %s", input, __LINE__, __FILE__);
	//	char Mac[18];
	//	strcpy(Mac, web_get("", input, 0));
	//	delblacklist(Mac);
	}

#endif 
#if 0
	//show black list
	if(strcpy("show", web_get("showblacklist", input, 0)) == 0)
	{
		DBG_MSG("%s  %d  %s", input, __LINE__, __FILE__);
		showblacklist();
	}
#endif 
	return 0;
}

