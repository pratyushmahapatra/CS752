for traces in $(ls SHORT_MOBILE*); do
#	out=`echo $traces | sed 's/.*.//'`
	filename="${traces%.*}.txt"
#	echo $out
	string=`/u/s/p/spati/CS752/cbp2016.eval/trace_analysis/cbp2016.eval/sim/predictor $traces $filename` 
	echo "$traces $string" >> short_mobile_result.txt 
done
