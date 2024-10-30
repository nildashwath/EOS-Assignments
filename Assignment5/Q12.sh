#!/bin/bash

echo "Enter the filename:"
read filename

if [ ! -f $filename ]
then

echo "$filename is not a file."

exit 1

fi
		echo "$filename was last modified on $(stat -c %x $filename)"
