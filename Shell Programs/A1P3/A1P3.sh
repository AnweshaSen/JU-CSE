#Write a shell script that counts the number of disk blocks occupied by a file 
#(the file name is passed as an argument and the file may be assumed to be in the current directory).

#!/bin/bash
if [ $# -gt 0 ]
then
	echo "Filename: $1"
	echo "Block Count : $(stat --printf "%b" $1)"
else
	echo "No filename specified"
fi