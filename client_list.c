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


//record the recent app info
#define CLIENTLIST  "/var/APClient.list"


/*
 * who's in the SoftAP Client. 
 * information of the Client app.
 */

typedef struct app_info
{
	char hostname[32];
	char mac[32];
	//unsigned long time; 	// connect time
	//char signal[9];  		// the app signal value
	char msg_os[64]; 		// operation system message.
} app_info_t;

void GetDhcpClientList(void);
//int changehostname(char *hostname, char *mac);
//int changehostname(void);

int main(int argc, char *argv[])
{

	GetDhcpClientList();
//	char hostname[16] = "myphone";
//	char mymac[32] = "AC:F7:F3:1D:57:6E";

//	changehostname(hostname, mymac);

	return 0;
}

#if 1
void GetDhcpClientList(void)
{
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
	char tmpValue[256];

	do_system("killall -q -USR1 udhcpd");
	fp = fopen("/var/udhcpd.leases", "rw");
	
	int j = 0;

	if (NULL == fp)
		return;
	while (fread(&lease, 1, sizeof(lease), fp) == sizeof(lease)) {
		if (strlen(lease.hostname) > 0) {
			sprintf(tmpValue, "%s", lease.hostname);
			convert_string_display(tmpValue);			
			printf("hostname is:%-16s\n", tmpValue);
		} else {
			printf("hostname is: \n");
		}
		if(strcmp(lease.hostname, "MI2-xiaomishouj") == 0)
			strcpy(lease.hostname, "helloworld");
		printf("mac is:%02X", lease.mac[0]);
		for (i = 1; i < 6; i++)
			printf(":%02X", lease.mac[i]);
		printf("\n");

		addr.s_addr = lease.ip;
		expires = ntohl(lease.expires);
		printf("IP is:%s\n", inet_ntoa(addr));
#if 1
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
		printf("Platfrom OS is: %s\n", msg_os);

		if(strcmp(lease.hostname, "MI2-xiaomishouj") == 0)
		{
			strcpy(lease.hostname, "helloworld");
			fwrite(&lease, 1, sizeof(lease), fp);
		}
	}
	fclose(fp);
}
#endif 

#if 0
int changehostname(char *hostname, char *mymac)
{
	FILE *fp;
	//int flags = 0;
	struct dhcpOfferedAddr {
		unsigned char hostname[16];
		unsigned char mac[16];
		unsigned long ip;
		unsigned long expires;
	} lease;
	char getmac[32];
	int i, j;

	do_system("killall -q -USR1 udhcpd");
	fp = fopen("/var/udhcpd.leases", "rw");
	if (NULL == fp)
		return;
	while (fread(&lease, 1, sizeof(lease), fp) == sizeof(lease)) {

#if 0
		printf("mac is:%02X", lease.mac[0]);
		for (i = 1; i < 6; i++)
			printf(":%02X", lease.mac[i]);
		printf("\n");

#endif 

		sprintf(&getmac[0], "%02X", lease.mac[0]);
		for(i = 1, j = 2; i < 6; i++, j+=3)
			sprintf(&getmac[j], ":%02X", lease.mac[1]);


#if 1
	//	printf("mymac: %s\n", mymac);
		printf("getmac: %s\n", getmac);
#if 0
		if(strncmp(mymac, getmac, 17) == 0)
		{
			strncpy(lease.hostname, hostname, sizeof(lease.hostname) -1);
			lease.hostname[15] = '\0';
			flags = 1;
			fwrite(&lease, 1, sizeof(lease), fp);
		}

#endif 

#endif 
	}

	//if(flags == 0)
	//	printf("cat not find the mac address!\n");

	fclose(fp);
	
	return 0;
}
#endif 
