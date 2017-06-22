#include "mtk_operate.h"


int main(int argc, char *argv[])
{
	char input[MAX_MSG_SIZ];
	int length;
	length = get_message_from_web(input);
	if(length <=0)
		DBG_MSG("get message from web it empty, in %s", __FILE__);

	char rout[32];
	strcpy(rout, web_get("router_set", input, 0));
	
	//router setting 
	if(strcmp(rout, "router") == 0)
	{
		//channel ssid security passwd
		char channel[4];
		char ssid[186];
		char security[23];
		char passwd[64];

		char encrypetype[10];
		char authmode[23];


		strcpy(channel, web_get("Channel", input, 0));
		strcpy(ssid, web_get("ssid", input, 0));
		strcpy(security, web_get("security", input, 0));
		strcpy(passwd, web_get("passwd", input, 0));

#if 0
		iwpriv ra0 set AuthMode=WPAPSKWPA2PSK
		iwpriv ra0 set EncrypType=TKIPAES
		iwpriv ra0 set SSID=MySsid
		iwpriv ra0 set WpaMixPairCipher=WPA_TKIP_WPA2_AES
		iwpriv ra0 set WPAPSK=MyPassword
		iwpriv ra0 set SSID=MySsid
#endif 

		get_nth_value(0, security, '/', authmode, strlen(security));
		get_nth_value(1, security, '/', encrypetype, strlen(security));
		if(strcmp(security, "WAP1PSKWPA2PSK/TKIPAES") ==0)
		{
			nvram_bufset(RT2860_NVRAM, "ApCliEnable", "1");
			nvram_bufset(RT2860_NVRAM, "ApCliAuthMode", "WPAPSKWPA2PSK");
			nvram_bufset(RT2860_NVRAM, "ApCliEncrypType", "TKIPAES");
			nvram_bufset(RT2860_NVRAM, "ApCliSsid", ssid);
			nvram_bufset(RT2860_NVRAM, "WpaMixPairCipher", "WPA_TKIP_WPA2_AES");
			nvram_bufset(RT2860_NVRAM, "ApCliWPAPSK", passwd);

			//扩展路由的加密方式，频道和类型跟主路由一致
			nvram_bufset(RT2860_NVRAM, "Channel", Channel);
			nvram_bufset(RT2860_NVRAM, "AuthMode", "WPA_TKIP_WPA2_AES");
			nvram_bufset(RT2860_NVRAM, "EncrypType", "TKIPAES");

			//nvram_bufset(RT2860_NVRAM, "SSID1", );
			//nvram_bufset(RT2860_NVRAM, "WPAPSK1", );
			nvram_bufset(RT2860_NVRAM, "ManagePasswd", );
			nvram_commit(RT2860_NVRAM);

		}
		else
		{
			nvram_bufset(RT2860_NVRAM, "ApCliEnable", "1");
			nvram_bufset(RT2860_NVRAM, "ApCliAuthMode", authmode);
			nvram_bufset(RT2860_NVRAM, "ApCliEncrypType", encrypetype);
			nvram_bufset(RT2860_NVRAM, "ApCliSsid", ssid);
			nvram_bufset(RT2860_NVRAM, "ApCliWPAPSK", passwd);

			//扩展路由的加密方式，频道和类型跟主路由一致
			nvram_bufset(RT2860_NVRAM, "Channel", Channel);
			nvram_bufset(RT2860_NVRAM, "AuthMode", authmode);
			nvram_bufset(RT2860_NVRAM, "EncrypType", encrypetype);

			//nvram_bufset(RT2860_NVRAM, "SSID1", );
			//nvram_bufset(RT2860_NVRAM, "WPAPSK1", );
			//nvram_bufset(RT2860_NVRAM, "ManagePasswd", );
			nvram_commit(RT2860_NVRAM);

		}


	}

	//WIFI setting
	if(strcmp(wifi, "wifi") == 0)
	{
#if 0
		//ssid(需扩展路由的) passwd managepasswd 
		char ssid1[186];
		char passwd[64];
		char managepasswd[64];

		strcpy(ssid1, web_get("ssid1", input, 0));
		strcpy(passwd, web_get("passwd", input, 0));
		strcpy(managepasswd, web_get("managepasswd", input, 0));

		nvram_bufset(RT2860_NVRAM, "SSID1", ssid1);
		nvram_bufset(RT2860_NVRAM, "WPAPSK1", passwd);
		nvram_bufset(RT2860_NVRAM, "ManagePasswd", managepasswd);
		nvram_commit(RT2860_NVRAM);

#endif 



#if 0
		//select TxPower mode(扩展路由的信号强度,默认100) 100   75    50 三种选择
		char txpower;
		strcpy(txpower, web_get("txpoer", input, 0));
		nvram_bufset(RT2860_NVRAM, "TxPower", txpower);
		nvram_commit(RT2860_NVRAM);

#endif 

	}
}
