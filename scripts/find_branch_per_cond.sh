#for line in $(cat results.csv)
count=0
while read line
	do
	cond_br=`echo $line | awk -F ':' '{print $5}'`
	mispredicts=`echo $line | awk -F ':' '{print $6}'`
        trace=`echo $line | awk -F ':' '{print $1}'`
	NUM_INSTRUCTIONS=`echo $line | awk -F ':' '{print $2}'`
	NUM_BR=`echo $line | awk -F ':' '{print $3}'`
 	NUM_UNCOND_BR=`echo $line | awk -F ':' '{print $4}'`
	NUM_CONDITIONAL_BR=`echo $line | awk -F ':' '{print $5}'`
	NUM_MISPREDICTIONS=`echo $line | awk -F ':' '{print $6}'`
	MISPRED_PER_1K_INST=`echo $line | awk -F ':' '{print $7}'`
	NUM_UNIQUE_BR=`echo $line | awk -F ':' '{print $8}'`
	if [ $count -lt 1 ]
	then
	count=$((count + 1))
	echo " TRACE, NUM_INSTRUCTIONS, NUM_BR, NUM_UNCOND_BR, NUM_CONDITIONAL_BR, NUM_MISPREDICTIONS, MISPRED_PER_1K_INST, NUM_UNIQUE_BR, MISPRED_PK_COND_BR" >> results_final.csv
	continue
	fi
	mispredicts="$(echo $mispredicts)"
	cond_br="$(echo $cond_br)"
	mispred_per_kilo_cond_br=$(echo "scale=2;(($mispredicts * 1000) / $cond_br )"| bc);
	echo "$trace, $NUM_INSTRUCTIONS, $NUM_BR, $NUM_UNCOND_BR, $NUM_CONDITIONAL_BR, $NUM_MISPREDICTIONS, $MISPRED_PER_1K_INST, $NUM_UNIQUE_BR, $mispred_per_kilo_cond_br" >> results_final.csv
#done
#	mispred_per_kilo_cond_br=$((($mispredicts * 1000) / $cond_br ));
#	if [[ $NUM_UNIQUE_BR -gt 10000 && $mispred_per_kilo_cond_br -gt 50 ]] 
#	then
#	`cp $trace /afs/cs.wisc.edu/u/s/p/spati/scripts/HwHm`
#	elif [[ $NUM_UNIQUE_BR -gt 10000 && $mispred_per_kilo_cond_br -le 50 ]]
#	then
#		if [ $mispred_per_kilo_cond_br -gt 10 ]
#			then
#			`cp $trace /afs/cs.wisc.edu/u/s/p/spati/scripts/HwSm`
# 			fi
#	
#	elif [[ $NUM_UNIQUE_BR -le 10000 && $mispred_per_kilo_cond_br -gt 50 ]]
#	then
#	`cp $trace /afs/cs.wisc.edu/u/s/p/spati/scripts/SwHm`
#	fi

done < results.csv
