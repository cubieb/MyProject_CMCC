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

void GetDhcpClientList(void);

int main(int argc, char *argv[])
{

	GetDhcpClientList();

	return 0;
}

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

	fp = fopen("/var/udhcpd.leases", "r");
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

		printf("not code mac address is: %04X\n", lease.mac);

		printf("mac is:%02X", lease.mac[0]);
		for (i = 1; i < 6; i++)
			printf(":%02X", lease.mac[i]);
		printf("\n");

		addr.s_addr = lease.ip;
		expires = ntohl(lease.expires);
		printf("IP is:%s\n", inet_ntoa(addr));
		d = expires / (24*60*60); expires %= (24*60*60);
		h = expires / (60*60); expires %= (60*60);
		m = expires / 60; expires %= 60;
		if (d) printf("%u days ", d);
		printf("%02u:%02u:%02u\n", h, m, (unsigned)expires);

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

	}
	fclose(fp);
}

