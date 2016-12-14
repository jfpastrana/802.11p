#!/bin/sh

WLAN=wlp2s0
WLAN_OCB=ocb0

ip link set $WLAN down
ip link set $WLAN_OCB down
iw reg set AA

