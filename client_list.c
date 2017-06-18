#include "utils.h"

#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/route.h>

#include <errno.h>

#include <linux/wireless.h>
#include "oid.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "media.h"
#include <errno.h>

#include <string.h>

//record the recent app info
#define CLIENTLIST  "/var/APClient.list"


/*
 * who's in the SoftAP Client. 
 * information of the Client app.
 */

static int find_macaddr(char *macaddr, FILE *stream);

static int change_hostname(char *hostname, char *macaddr);

int GetDhcpClientList(FILE *stream);

int main(int argc, char *argv[])
{
#if 0
	FILE *fp; 
	fp = fopen(CLIENTLIST, "a+");
	if(fp == NULL)
		return -1;

	GetDhcpClientList(fp);

	fclose(fp);
#endif 
#if 1
	char *hostname = "world";
	char *macaddr = "AC:F7:F3:1D:57:6E";
	change_hostname(hostname, macaddr);

#endif 

	return 0;
}

static int change_hostname(char *hostname, char *macaddr)
{
	if(hostname == NULL || macaddr == NULL)
		return -1;

	FILE *fp = fopen(CLIENTLIST, "r+");
	if(fp == NULL)
		return -1;

	char buff[2048];
	char *tmp = macaddr;
	char *file_mac;

	char *myip = NULL;
	char temp[18];
	char *msg_os = NULL;

	while(fgets(buff, 2048, fp) > 0)
	{
		int length = strlen(buff);
		file_mac = web_get("mac", buff, 0);
		if(strcmp(file_mac, tmp) == 0)
		{
			myip = web_get("ip", buff, 2);
			strcpy(temp, myip);

			msg_os = web_get("msg_os", buff, 0);

#if 1
			memset(buff, 0, length);
			fputs(buff, fp);
			fseek(fp, -length, SEEK_CUR);

#endif 
			snprintf(buff, length, "hostname=%s&mac=%s&ip=%s&msg_os=%s", hostname, macaddr, temp, msg_os);
			fseek(fp, -length, SEEK_CUR);
			fwrite(buff,1, length, fp);
			return 0;
		}
	}
	fclose(fp);

	return -1;
}

//找mac地址，找到返回0，找不到返回-1
static int find_macaddr(char *macaddr, FILE *stream)
{
	if( macaddr == NULL || stream == NULL)
		return -1;

	char buff[2048];
	char *tmp = macaddr;
	char *file_mac;

	while(fgets(buff, 2048, stream) > 0)
	{
		file_mac = web_get("mac", buff, 0);
		if(strcmp(file_mac, tmp) == 0)
			return 0;
	}

	return -1;
}

//获取数据并写入到文件东中
int  GetDhcpClientList(FILE *stream)
{

	if(stream == NULL)
		return -1;

	FILE *fp; 
	struct dhcpOfferedAddr {
		unsigned char hostname[16];
		unsigned char mac[16];
		unsigned long ip;
		unsigned long expires;
	} lease;

	int i;
	struct in_addr addr;
	unsigned long expires;
	unsigned d, h, m;
	//char tmpValue[256];

	char hostname[32];
	char mac[18];
	char ip[16];

	do_system("killall -q -USR1 udhcpd");
	fp = fopen("/var/udhcpd.leases", "r");
	if (NULL == fp)
		return;

	while (fread(&lease, 1, sizeof(lease), fp) == sizeof(lease)) {
		if (strlen(lease.hostname) > 0) {
			sprintf(hostname, "%s", lease.hostname);
			convert_string_display(hostname);			
		} else {
			strcpy(hostname, "");
		}

		int j;
		sprintf(&mac[0], "%02X", lease.mac[0]);
		for(i = 1, j= 2; i < 6; i++, j += 3)
			sprintf(&mac[j], ":%02X", lease.mac[i]);

		mac[j] = '\0';

		if(find_macaddr(mac, stream) == 0)
			continue;
		
		addr.s_addr = lease.ip;
		expires = ntohl(lease.expires);

		strcpy(ip, inet_ntoa(addr));

		
#if 0
		d = expires / (24*60*60); expires %= (24*60*60);
		h = expires / (60*60); expires %= (60*60);
		m = expires / 60; expires %= 60;
		if (d) printf("%u days ", d);
		printf("%02u:%02u:%02u\n", h, m, (unsigned)expires);
#endif
		FILE *pp;
		char cmd[128], msg_os[64];
		sprintf(cmd, "fingerprint.sh query %s", inet_ntoa(addr));
		pp = popen(cmd, "r");
		if(pp == NULL) 
			pp = "";
		memset(msg_os, 0, 64);
		fread(msg_os, 64, 1, pp);
		pclose(pp);

		char my_os[64];
		strncpy(my_os, msg_os, strlen(msg_os));

		char msg_info[2048];
		snprintf(msg_info, 2048, "hostname=%s&mac=%s&ip=%s&msg_os=%s", hostname, mac, ip, my_os);

		fputs(msg_info, stream);
		
	}
	fclose(fp);
}

