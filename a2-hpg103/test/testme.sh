#!/bin/bash
echo \*\*\* Making the module...
make
echo \*\*\* Attempting to remove the old module, if it is present...
sudo rmmod -f jiffies
echo \*\*\* Installing the module...
sudo insmod ../jiffies.ko
echo \*\*\* Printing file contents...
if [ ! -e "/proc/jiffies" ]
then
   echo \*\*\* "ERROR: /proc/jiffies does not exist, something has gone wrong..."
else
   cat /proc/jiffies
   echo \*\*\ Success!
fi
