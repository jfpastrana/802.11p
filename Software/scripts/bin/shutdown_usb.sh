#!/bin/sh

PATH_USB=/home/wlanp/scripts/tmp/USB_JAVP
PATH_CAPTURES=/home/wlanp/scripts/tmp/USB_JAVP/captures/
DEV_USB=/dev/sdb1
USB=0951:16a2
WLAN_OCB=wlp2s0
MOUNTED=$(df -l | grep -i /dev/sdb1 | cut -d " " -f 15)
STAMP=$(date +"%F-%H-%M-%S")
ID_USB=$(lsusb | grep -i "Kingston Technology" | cut -d " " -f 6)


if [ "$USB" = "$ID_USB" ] ; then
	echo "Conected"
	if [ "$MOUNTED" = "$PATH_USB" ] ; then
		echo "Mounted"
		umount /dev/sdb1
	else
		echo "No mounted"
	fi 
else
	echo "NO ENCONTRADO"
	sleep 10
fi
