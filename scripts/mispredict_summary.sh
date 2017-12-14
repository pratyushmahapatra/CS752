for num_proc in 2 6 10
do
echo "Trace,    Mispred_Per_Kilo_Cond_Br" >> summary_4950_new_$num_proc.csv
for types in HM_HW HM_SW SM_HW
	do
	for traces in $(ls /afs/cs.wisc.edu/u/s/p/spati/CS752/traces/sets/$types)
		do
			mispredict=0
			total=0
			count=0
			average_mispredict=0
				for files in $(ls /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/Neural/neural_with_interleave/results/interval_4950_Iter1/result_${num_proc}* )
					do
					if  grep -q "$traces" $files 
					then
						line=`grep "$traces" $files`
						mispredict=`echo $line | awk -F ',' '{print $4}'`
        	                                total=$(echo "scale=2;( $total + $mispredict )" | bc)
	                                        count=$(( count + 1 ))
					else 
						continue
					fi
					done
				for files in $(ls /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/Neural/neural_with_interleave/results/interval_4950_Iter2/result_${num_proc}* )
                                        do
                                        if  grep -q "$traces" $files
                                        then
                                                line=`grep "$traces" $files`
                                                mispredict=`echo $line | awk -F ',' '{print $4}'`
                                                total=$(echo "scale=2;( $total + $mispredict )" | bc)
                                                count=$(( count + 1 ))
                                        else
                                                continue
                                        fi
				done
				for files in $(ls /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/Neural/neural_with_interleave/results/interval_4950_Iter3/result_${num_proc}* )
                                        do
                                        if  grep -q "$traces" $files
                                        then
                                                line=`grep "$traces" $files`
                                                mispredict=`echo $line | awk -F ',' '{print $4}'`
                                                total=$(echo "scale=2;( $total + $mispredict )" | bc)
                                                count=$(( count + 1 ))
                                        else
                                                continue
                                        fi
                                        done
				for files in $(ls /afs/cs.wisc.edu/u/s/p/spati/CS752/cbp2016.eval/Neural/neural_with_interleave/results/interval_4950_Iter4/result_${num_proc}* )
                                        do
                                        if  grep -q "$traces" $files
                                        then
                                                line=`grep "$traces" $files`
                                                mispredict=`echo $line | awk -F ',' '{print $4}'`
                                                total=$(echo "scale=2;( $total + $mispredict )" | bc)
                                                count=$(( count + 1 ))
                                        else
                                                continue
                                        fi
                                        done
			average_mispredict=$(echo "scale=2;($total / $count)"| bc)
			echo "$traces,    $average_mispredict" >> summary_4950_new_$num_proc.csv
		done

	done
done
