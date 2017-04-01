for((i=0;i<2;i++))
do
	if (( $i == 0 )) ; then
	    sudo sysctl -w net.ipv4.tcp_sack=0
	elif (($i == 1)) ; then
	    sudo sysctl -w net.ipv4.tcp_sack=1
	fi
    
    for ((j=0;j<2;j++))
    do
		if (( $j == 0 )) ; then
			sudo sysctl -w net.ipv4.tcp_congestion_control="reno"
		elif (($j == 1)) ; then
			sudo sysctl -w net.ipv4.tcp_congestion_control="cubic"
		fi

		for ((k=0;k<2;k++))
    	do
			sudo sysctl -w net.core.rmem_max=16777216
			sudo sysctl -w net.core.wmem_max=16777216
			if (( $k == 0 )) ; then
    			sudo sysctl -w net.ipv4.tcp_rmem="4096 87380 16777216"
				sudo sysctl -w net.ipv4.tcp_wmem="4096 16384 16777216"
			elif (($k == 1)) ; then
				sudo sysctl -w net.ipv4.tcp_rmem="4096 1310720 16777216"
				sudo sysctl -w net.ipv4.tcp_wmem="4096 262144 16777216"
			fi

			for ((l=0;l<2;l++))
    		do
    			if (( $l== 0 )) ; then
    				sudo tc qdisc add dev eth0 root netem delay 2ms
				elif (($l == 1)) ; then
					sudo tc qdisc add dev eth0 root netem delay 50ms
				fi

				for ((m=0;m<2;m++))
    			do
    				if (( $m == 0 )) ; then
    					sudo tc qdisc change dev eth0 root netem loss 0.3%
					elif (($m == 1)) ; then
						sudo tc qdisc change dev eth0 root netem loss 5%
					fi
					wget http://$1/$2 -O $2
	   			done
    		done
    	done
    done	
done