((num=0));
if [ ! -d "$1" ];
	then mkdir $1;
	echo "$1 created";
else
	echo "$1 exists";
fi

for Ki in 1 4
do
	for Km in 1 1.5
    do
		for Kn in 0.5 1
    	do
			for Kf in 0.1 0.3
    		do
    			for Ps in 0.99 0.9999
    			do
    				((num++));
					java cw -i $Ki -m $Km -n $Kn -f $Kf -s $Ps -T 1000 	-o "$1/$1$num $Ki $Km $Kn $Kf $Ps"
				done
    		done
    	done
    done	
done

python graph.py $1;