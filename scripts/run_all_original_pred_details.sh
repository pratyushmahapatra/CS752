#!/bin/bash

#for i in `seq 1 9`
#	do	
	i=1
	core=0
	for types in HM_HW HM_SW SM_HW
        do
        for traces in $(ls /afs/cs.wisc.edu/u/s/p/spati/CS752/traces/sets/$types)
                do
		if [ $i -lt 10 ]
		then
			x="0"
		else
			x=""
		fi
#		ssh spati@royal-$x$i "taskset -c $core /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/sim_to_output_pred_details/predictor /afs/cs.wisc.edu/u/s/p/spati/CS752/traces/$traces >> /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/sim_to_output_pred_details/results/result_$traces.csv" &
		ssh spati@royal-$x$i "taskset -c $core /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/Neural/neural_64kb_original/predictor /afs/cs.wisc.edu/u/s/p/spati/CS752/traces/$traces >> /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/Neural/neural_64kb_original/results/result_$traces.csv" &
		core=$((core + 1))
		if [ $core -gt 3 ]
		then	
			core=0
			i=$((i + 1))
			if [ $i -gt 30 ]
			then
				break
			fi
		fi
       		
	done
	
done

