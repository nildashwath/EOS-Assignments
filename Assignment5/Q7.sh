#!/bin/bash

echo "Enter a number"
read number

if  [ $number -lt 0 ]
then
	echo " $number is negative."

  elif [ $number -gt 0 ]
  then	
  	echo  "$number is Positive."

 else
 	echo "$number is niether positive or negative!!!"
fi
