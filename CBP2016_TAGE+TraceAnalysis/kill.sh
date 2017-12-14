while read pid
do
kill -9 $pid
done < pid.txt

