#Write a shell script that accepts a file name as an input. 
#The program then asks for a string of characters (that is, any word) to be provided by the user. 
#The file will be searched to find whether it contains the given word. (i) If the file contains the given word, 
#the program will display the total number of occurrences of the word.
# (ii) The program is also required to display the line number/s in which the word has occurred 
# as well the frequency of the word in that line. (Note: the word may occur multiple times in a given line). 
# (iii) If the file does not contain the word, an appropriate message will be displayed.

#!/bin/bash
file=$1
v=0

if [ $# -lt 1 ]
then
	exit 1
fi
if [ ! -f $file ]
then
	echo "$file is not a file"
	exit 2
fi
 
echo "Enter the word to be searched"
read wordd 
echo " "
occur=$(grep  -c $wordd $file)
y=0
if [ $occur -ne $y ]
then
    echo "Lines containing the word :"
    echo "$(grep  -n -w $wordd $file)"
    echo " "
    echo "Total occurance of '$wordd' is : $(grep  -c $wordd $file)"
    echo " "
    l=1
    while read line
    do
        count=0
        for word in $line; do
            if [ "$wordd" == "$word" ]        
            then
                count=$(( $count+1 ))
            fi        
        done
        echo "Ocuurance of '$wordd' in Line||$l is : $count"
        l=$(( $l+1 ))
    done < $file 
    echo " "
    echo "Lines that doesn't contain the word '$wordd' is: $(grep  -v $wordd $file)"
else
    echo "The word is not present in the file"
fi    

