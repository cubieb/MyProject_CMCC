#!/bin/sh

AuthMode="$1"
EncrypType="$2"
Ssid="$3"
WPAPSK="$4"


ifconfig apcli0 down
ifconfig apclio up
iwpriv apcli0 set ApCliEnable=0
iwpriv apcli0 set ApCliAuthMode=$AuthMode
iwpriv apcli0 set ApCliEncrypType=$EncrypType
iwpriv apcli0 set ApCliSsid=$Ssid
iwpriv apcli0 set ApCliWPAPSK=$WPAPSK
iwpriv apcli0 set ApCliSsid=$Ssid
iwpriv apcli0 set ApCliEnable=1
