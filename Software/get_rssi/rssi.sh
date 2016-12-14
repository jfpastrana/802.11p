#/bin/bash

MAC_SSID1=1e:38:0f:b0:cd:61
MAC_SSID2=4a:aa:c4:2c:c5:3e
MAC_SSID3=ae:27:bb:b2:d5:4a
MAC_SSID4=6e:ad:63:80:fb:e6

power=0
p_total=0
counter=0
is_number='^-?[0-9]+([.][0-9]+)?$'

for((i=0; i<500; i++))
do
        power=$(iw dev ibss0 scan | grep -A5 $MAC_SSID4 | grep -i signal | tr ' ' '\t' | cut -f 3 | tr -d '\t')
        echo $power

        if [[ $power =~ $is_number ]] ; then
                p_total=$(echo $p_total - $power | bc)
                let counter=$counter+1
        else
                echo expression evaluated as false
        fi

done

