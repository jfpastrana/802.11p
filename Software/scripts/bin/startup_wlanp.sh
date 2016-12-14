#!/bin/sh

WLAN=wlp2s0
WLAN_OCB=ocb0


ip link set $WLAN down
iw reg set AA

iw dev $WLAN interface add $WLAN_OCB type ocb
iw dev $WLAN_OCB set type ocb
ip link set $WLAN_OCB up

iw dev $WLAN_OCB ocb join 5900 10MHZ
iw dev $WLAN_OCB set bitrates legacy-5 12

