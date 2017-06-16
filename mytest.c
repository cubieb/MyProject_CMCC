#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h> /* for close */
#include <linux/wireless.h>

#define RT_PRIV_IOCTL (SIOCIWFIRSTPRIV + 0x01)
#define RTPRIV_IOCTL_SET (SIOCIWFIRSTPRIV + 0x02)
#define RTPRIV_IOCTL_BBP (SIOCIWFIRSTPRIV + 0x03)
#define RTPRIV_IOCTL_MAC (SIOCIWFIRSTPRIV + 0x05)
#define RTPRIV_IOCTL_E2P (SIOCIWFIRSTPRIV + 0x07)
#define RTPRIV_IOCTL_STATISTICS (SIOCIWFIRSTPRIV + 0x09)
#define RTPRIV_IOCTL_ADD_PMKID_CACHE (SIOCIWFIRSTPRIV + 0x0A)
#define RTPRIV_IOCTL_RADIUS_DATA (SIOCIWFIRSTPRIV + 0x0C)
#define RTPRIV_IOCTL_GSITESURVEY (SIOCIWFIRSTPRIV + 0x0D)
#define RTPRIV_IOCTL_ADD_WPA_KEY (SIOCIWFIRSTPRIV + 0x0E)
#define RTPRIV_IOCTL_GET_MAC_TABLE (SIOCIWFIRSTPRIV + 0x0F)
#define OID_GET_SET_TOGGLE 0x8000
#define RT_QUERY_ATE_TXDONE_COUNT 0x0401
#define RT_QUERY_SIGNAL_CONTEXT 0x0402
#define RT_SET_APD_PID (OID_GET_SET_TOGGLE + 0x0405)
#define RT_SET_DEL_MAC_ENTRY (OID_GET_SET_TOGGLE + 0x0406)


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#define MAC_ADDR_LEN 6
#define ETH_LENGTH_OF_ADDRESS 6
#define MAX_LEN_OF_MAC_TABLE 64

typedef struct _COUNTERS
{
	unsigned long TxSuccessTotal;;
	unsigned long TxSuccessWithoutRetry;
	unsigned long TxSuccessWithRetry;
	unsigned long TxFailWithRetry;
	unsigned long RtsSuccess;
	unsigned long RtsFail;
	unsigned long RxSuccess;
	unsigned long RxWithCRC;
	unsigned long RxDropNoBuffer;
	unsigned long RxDuplicateFrame;
	unsigned long FalseCCA;
	unsigned long RssiA;
	unsigned long RssiB;
} COUNTERS;


typedef struct _SITE_SURVEY
{
	unsigned char channel;
	unsigned short rssi;
	unsigned char ssid[33];
	unsigned char bssid[6];
	unsigned char security[9];
} SITE_SURVEY;



typedef union _MACHTTRANSMIT_SETTING {
	struct {
		unsigned short MCS:7; // MCS
		unsigned short BW:1; //channel bandwidth 20MHz or 40 MHz
		unsigned short ShortGI:1;
		unsigned short STBC:2; //SPACE
		unsigned short rsv:3;
		unsigned short MODE:2; // Use definition MODE_xxx.
	} field;

	unsigned short word;

} MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	unsigned char Addr[6];
	unsigned char Aid;
	unsigned char Psm; // 0:PWR_ACTIVE, 1:PWR_SAVE
	unsigned char MimoPs; // 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled
	MACHTTRANSMIT_SETTING TxRate;
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;


typedef struct _RT_802_11_MAC_TABLE {
	unsigned long Num;
	RT_802_11_MAC_ENTRY Entry[MAX_LEN_OF_MAC_TABLE];
} RT_802_11_MAC_TABLE, *PRT_802_11_MAC_TABLE;

// Key mapping keys require a BSSID
typedef struct _NDIS_802_11_KEY
{
	unsigned long Length; // Length of this structure
	unsigned char addr[6];
	unsigned long KeyIndex;
	unsigned long KeyLength; // length of key in bytes
	unsigned char KeyMaterial[32]; // variable length depending on above field
} NDIS_802_11_KEY, *PNDIS_802_11_KEY;


typedef struct _RT_SIGNAL_STRUC {
	unsigned short Sequence;
	unsigned char MacAddr[MAC_ADDR_LEN];
	unsigned char CurrAPAddr[MAC_ADDR_LEN];
	unsigned char Sig;
} RT_SIGNAL_STRUC, *PRT_SIGNAL_STRUC;

//---------------------------------------------------------
COUNTERS counter;
SITE_SURVEY SiteSurvey[100];

//user spaces buffer data;
char data[4096];



int main( int argc, char ** argv )
{
	char name[25];
	int socket_id;
	struct iwreq wrq;
	int ret;

	// open socket based on address family: AF_NET ----------------------------
	socket_id = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_id < 0)
	{
		printf("\nrtuser::error::Open socket error!\n\n");
		return -1;
	}
	// set interface name as "ra0" --------------------------------------------
	sprintf(name, "ra0");
	memset(data, 0x00, 255);

#if 0 // have test is ok
	//
	//example of iwconfig ioctl function ==========================================
	//
	// get wireless name ------------------------------------------------------
	strcpy(wrq.ifr_name, name);
	wrq.u.data.length = 255;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, SIOCGIWNAME, &wrq);
	if(ret != 0)
	{
		printf("\nrtuser::error::get wireless name\n\n");
		goto rtuser_exit;
	}
	printf("\nrtuser[%s]:%s\n", name, wrq.u.name);

#endif 


#if 0 //have test is ok
	//get statistics, remove "stat" string ------------------------------------
	memset(data, 0x00, 2048);
	strcpy(data, "");
	strcpy(wrq.ifr_name, name);
	wrq.u.data.length = 0;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, RTPRIV_IOCTL_STATISTICS, &wrq);
	if(ret != 0)
	{
		printf("\nrtuser::error::get statistics\n\n");
		goto rtuser_exit;
	}

	printf("\n========= Get AP Statistics ==========\n");
	{
		int i;
		char *sp = wrq.u.data.pointer;
		unsigned long *cp = (unsigned long *)&counter;
		for (i = 0 ; i < 13 ; i++)
		{
			sp = strstr(sp, "= ");
			sp = sp+2;
			sscanf(sp, "%ul", (unsigned int *)&cp[i]);
		}
		printf("Tx success = %u\n", (unsigned int)counter.TxSuccessTotal);
		printf("Tx success without retry = %u\n", (unsigned int)counter.TxSuccessWithoutRetry);
		printf("Tx success after retry = %u\n", (unsigned int)counter.TxSuccessWithRetry);
		printf("Tx fail to Rcv ACK after retry = %u\n", (unsigned int)counter.TxFailWithRetry);
		printf("RTS Success Rcv CTS = %u\n", (unsigned int)counter.RtsSuccess);
		printf("RTS Fail Rcv CTS = %u\n", (unsigned int)counter.RtsFail);
		printf("Rx success = %u\n", (unsigned int)counter.RxSuccess);
		printf("Rx with CRC = %u\n", (unsigned int)counter.RxWithCRC);
		printf("Rx drop due to out of resource= %u\n", (unsigned int)counter.RxDropNoBuffer);
		printf("Rx duplicate frame = %u\n", (unsigned int)counter.RxDuplicateFrame);
		printf("False CCA (one second) = %u\n", (unsigned int)counter.FalseCCA);
		printf("RSSI-A = %d\n", ( signed int)counter.RssiA);
		printf("RSSI-B (if available) = %d\n", ( signed int)counter.RssiB);
	}

#endif 


#if 1
	//get AP's mac table, remove "get_mac_table" string -----------------------
	memset(data, 0x00, 2048);
	strcpy(data, "");
	strcpy(wrq.ifr_name, name);
	wrq.u.data.length = 2048;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, RTPRIV_IOCTL_GET_MAC_TABLE, &wrq);
	if(ret != 0)
	{
		printf("\nrtuser::error::get mac table\n\n");
		goto rtuser_exit;
	}
	printf("\n========== Get Associated MAC Table ==========");
	{
		RT_802_11_MAC_TABLE *mp;
		int i;
		mp = (RT_802_11_MAC_TABLE *)wrq.u.data.pointer;
		printf("\n%-4s%-20s%-4s%-10s%-10s%-10s\n",
				"AID", "MAC_Address", "PSM", "LastTime", "RxByte", "TxByte");


		for(i = 0 ; i < mp->Num ; i++)
		{
			printf("%-4d", mp->Entry[i].Aid);
			printf("%02X:%02X:%02X:%02X:%02X:%02X ",
					mp->Entry[i].Addr[0], mp->Entry[i].Addr[1],
					mp->Entry[i].Addr[2], mp->Entry[i].Addr[3],
					mp->Entry[i].Addr[4], mp->Entry[i].Addr[5]);
			printf("%-4d", mp->Entry[i].Psm);
			printf("%-10u", (unsigned int)mp->Entry[i].HSCounter.LastDataPacketTime);
			printf("%-10u", (unsigned int)mp->Entry[i].HSCounter.TotalRxByteCount);
			printf("%-10u", (unsigned int)mp->Entry[i].HSCounter.TotalTxByteCount);
			printf("\n");

		}
		printf("\n");
	}

#endif 

#if 0
	//set: raw data
	// RTPRIV_IOCTL_RADIUS_DATA
	// RTPRIV_IOCTL_ADD_WPA_KEY
	// RTPRIV_IOCTL_ADD_PMKID_CACHE
	//set RADIUS Data ---------------------------------------------------------
	printf("\nrtuser::set radius data\n\n");
	memset(data, 0x55, 100);
	strcpy(wrq.ifr_name, name);
	wrq.u.data.length = 100;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, RTPRIV_IOCTL_RADIUS_DATA, &wrq);
	if(ret != 0)


	{
		printf("\nrtuser::error::set radius data\n\n");
		goto rtuser_exit;
	}

	//add WPA Key -------------------------------------------------------------
	printf("\nrtuser::add wpa key\n\n");
	{
		NDIS_802_11_KEY *vp;
		memset(data, 0, sizeof(NDIS_802_11_KEY));
		vp = (NDIS_802_11_KEY *)&data;
		vp->Length = sizeof(NDIS_802_11_KEY);
		memset(vp->addr, 0x11, 6);
		vp->KeyIndex = 2;
		vp->KeyLength = 32;
		memset(vp->KeyMaterial, 0xAA, 32);
		strcpy(wrq.ifr_name, name);
		wrq.u.data.length = sizeof(NDIS_802_11_KEY);
		wrq.u.data.pointer = data;
		wrq.u.data.flags = 0;
		ret = ioctl(socket_id, RTPRIV_IOCTL_ADD_WPA_KEY, &wrq);
		if(ret != 0)
		{
			printf("\nrtuser::error::add wpa key\n\n");
			goto rtuser_exit;
		}
	}

	//add PMKID_CACHE ---------------------------------------------------------
	printf("\nrtuser::add PMKID_CACHE\n\n");
	{
		NDIS_802_11_KEY *vp;
		memset(data, 0, sizeof(NDIS_802_11_KEY));
		vp = (NDIS_802_11_KEY *)&data;
		vp->Length = sizeof(NDIS_802_11_KEY);
		memset(vp->addr, 0x11, 6);
		vp->KeyIndex = 2;

		vp->KeyLength = 32;
		memset(vp->KeyMaterial, 0xBB, 32);
		strcpy(wrq.ifr_name, name);
		wrq.u.data.length = sizeof(NDIS_802_11_KEY);
		wrq.u.data.pointer = data;
		wrq.u.data.flags = 0;
		ret = ioctl(socket_id, RTPRIV_IOCTL_ADD_PMKID_CACHE, &wrq);
		if(ret != 0)
		{
			printf("\nrtuser::error::add PMKID_CACHE\n\n");
			goto rtuser_exit;
		}
	}

	//set: raw data
	// RT_SET_APD_PID
	// RT_SET_DEL_MAC_ENTRY
	//set APD_PID -------------------------------------------------------------
	printf("\nrtuser::set APD_PID\n\n");
	memset(data, 0, 4);
	data[0] = 12;
	strcpy(wrq.ifr_name, name);
	wrq.u.data.length = 4;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = RT_SET_APD_PID;
	ret = ioctl(socket_id, RT_PRIV_IOCTL, &wrq);
	if(ret != 0)
	{
		printf("\nrtuser::error::set APD_PID\n\n");
		goto rtuser_exit;
	}

	//set DEL_MAC_ENTRY -------------------------------------------------------
	printf("\nrtuser::set DEL_MAC_ENTRY\n\n");
	memset(data, 0xdd, 6);
	strcpy(wrq.ifr_name, name);
	wrq.u.data.length = 6;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = RT_SET_DEL_MAC_ENTRY;
	ret = ioctl(socket_id, RT_PRIV_IOCTL, &wrq);
	if(ret != 0)
	{
		printf("\nrtuser::error::set DEL_MAC_ENTRY\n\n");
		goto rtuser_exit;
	}

	//get: raw data
	// RT_QUERY_ATE_TXDONE_COUNT
	// RT_QUERY_SIGNAL_CONTEXT
	//get ATE_TXDONE_COUNT ----------------------------------------------------
	printf("\nrtuser::get ATE_TXDONE_COUNT\n\n");
	memset(data, 0, 4);
	strcpy(wrq.ifr_name, name);
	wrq.u.data.length = 4;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = RT_QUERY_ATE_TXDONE_COUNT;
	ret = ioctl(socket_id, RT_PRIV_IOCTL, &wrq);


	if(ret != 0)
	{
		printf("\nrtuser::error::get ATE_TXDONE_COUNT\n\n");
		goto rtuser_exit;
	}
	printf("\nATE_TXDONE_COUNT:: %08lx\n\n", (unsigned long)*wrq.u.data.pointer);
	//get SIGNAL_CONTEXT ------------------------------------------------------
	printf("\nrtuser::get SIGNAL_CONTEXT\n\n");
	{
		RT_SIGNAL_STRUC *sp;
		memset(data, 0, sizeof(RT_SIGNAL_STRUC));
		strcpy(wrq.ifr_name, name);
		wrq.u.data.length = sizeof(RT_SIGNAL_STRUC);
		wrq.u.data.pointer = data;
		wrq.u.data.flags = RT_QUERY_SIGNAL_CONTEXT;
		ret = ioctl(socket_id, RT_PRIV_IOCTL, &wrq);
		if(ret != 0)
		{
			printf("\nrtuser::error::get SIGNAL_CONTEXT\n\n");
			goto rtuser_exit;
		}
		sp = (RT_SIGNAL_STRUC *)wrq.u.data.pointer;
		printf("\n===== SIGNAL_CONTEXT =====\n\n");
		printf("Sequence = 0x%04x\n", sp->Sequence);
		printf("Mac.Addr = %02x:%02x:%02x:%02x:%02x:%02x\n",
				sp->MacAddr[0], sp->MacAddr[1],
				sp->MacAddr[2], sp->MacAddr[3],
				sp->MacAddr[4], sp->MacAddr[5]);
		printf("CurrAP.Addr = %02x:%02x:%02x:%02x:%02x:%02x\n",
				sp->CurrAPAddr[0], sp->CurrAPAddr[1],
				sp->CurrAPAddr[2], sp->CurrAPAddr[3],
				sp->CurrAPAddr[4], sp->CurrAPAddr[5]);
		printf("Sig = %d\n\n", sp->Sig);
	}

	//SSID, remove "set" string -----------------------------------------------
	memset(data, 0x00, 255);
	strcpy(data, "SSID=rtuser");
	strcpy(wrq.ifr_name, name);
	wrq.u.data.length = strlen(data)+1;
	wrq.u.data.pointer = data;
	wrq.u.data.flags = 0;
	ret = ioctl(socket_id, RTPRIV_IOCTL_SET, &wrq);
	if(ret != 0)
	{
		printf("\nrtuser::error::set SSID\n\n");
		goto rtuser_exit;
	}
#endif 
rtuser_exit:
	if (socket_id >= 0)
		close(socket_id);
	if(ret)
		return ret;
	else
		return 0;
}

