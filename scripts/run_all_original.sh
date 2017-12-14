#!/bin/bash

#for i in `seq 1 9`
#	do	
	i=1
	core=0
	for k in 2 6 10
	do
#	for dir in $(ls /u/s/p/spati/CS752/scripts/simulation_*)
	for tracelist in $(ls /u/s/p/spati/CS752/scripts/simulation_iter4_$k)
		do
		if [ $i -lt 10 ]
		then
			x="0"
		else
			x=""
		fi
#		ssh spati@royal-$x$i "taskset -c $core /u/s/p/spati/CS752/cbp2016.eval/sim_with_64kb_tage/predictor /afs/cs.wisc.edu/u/s/p/spati/CS752/traces/$traces >> /u/s/p/spati/CS752/cbp2016.eval/sim_with_64kb_tage/results/result_$traces.csv" & 
#		ssh spati@royal-$x$i "taskset -c $core /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/sim_to_output_pred_details/predictor /afs/cs.wisc.edu/u/s/p/spati/CS752/traces/$traces >> /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/sim_to_output_pred_details/results/result_$traces.csv" &
#		ssh spati@royal-$x$i "taskset -c $core /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/sim_multiple_interleave/predictor $k /u/s/p/spati/CS752/scripts/simulation_iter4_$k/$tracelist >> /u/s/p/spati/CS752/cbp2016.eval/sim_multiple_interleave/results/interval_4950_Iter4/result_${k}_${tracelist}.csv" &
#		ssh spati@royal-$x$i "taskset -c $core /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/Neural/neural_with_interleave/predictor $k /u/s/p/spati/CS752/scripts/simulation_iter4_$k/$tracelist >> /u/s/p/spati/CS752/cbp2016.eval/Neural/neural_with_interleave/results/interval_99950_Iter4/result_${k}_${tracelist}.csv" &
		core=$((core + 1))
		if [ $core -gt 3 ]
		then	
			core=0
			i=$((i + 1))
			if [ $i -eq 14 ]
                        then
                                i=15
                        fi
			if [ $i -gt 30 ]
			then
				break
			fi
		fi
       		
	done
	
done

