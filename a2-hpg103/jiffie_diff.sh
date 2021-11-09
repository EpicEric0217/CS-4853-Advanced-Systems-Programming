#!/bin/bash
SLEEP_TIME="$1"
echo "Sleep time will be $1 seconds..."
jiff1=$(cat /proc/jiffies)
echo jiffies1=$jiff1 
sleep ${SLEEP_TIME}
jiff2=$(cat /proc/jiffies)
echo jiffies2=$jiff2
diff=$((jiff2-jiff1))
echo "diff=$diff"
HZ=$((diff/SLEEP_TIME))
echo "HZ=$HZ"
