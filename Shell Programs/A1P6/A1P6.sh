# Extend the shell script written in question (5) to perform the following task: 
# User is asked to enter another word. (i) The first word (entered in question (5)) 
# will have to be matched with the contents of the file and replaced by the second word, 
# if a match occurs, (ii) Ignore replacing Partial matches, but show that partial matches do exist. 

#!/bin/bash
file=$1
v=0
 
if [ $# -ne 1 ]
then
	exit 1
fi
if [ ! -f $file ]
then
	echo "File not found"
	exit 2
fi
 
echo "Enter the word to be replaced"
read wordd 
echo " "
echo "Enter the replaced word"
read wordd2
echo " "
occur=$(grep  -c $wordd $file)
y=0
if [ $occur -ne $y ]
then
    #grep -v "^\s*$"
    echo "Lines containing the word '$wordd':"
    echo "$(grep  -n $wordd $file)"
    echo " "
    echo "Total number of lines in which '$wordd' is : $(grep  -o -w  $wordd $file| wc -l)"
    echo " "
    no_of_lines=$(wc -l $file)
    #echo $no_of_lines
    A="$(cut -d' ' -f1 <<<"$no_of_lines")"
    #echo $A
    l=1
    echo "Partial occurance of $wordd is in: "
    echo " "
    while read line
    do
        for word in $line; do
            if [ "${word/$wordd2}" != "$word" ] ; then
                echo "'$wordd2' was found in '$word' in Line number $l"
        fi    
        done
        l=$(( $l+1 )) 
    done < $file

    l=1       

    while read line
    do
        #name=$line
        #echo "Text read from file - $name"
        count=0
        for word in $line; do
            #echo "word = '$word'"
            if [ "$wordd" == "$word" ]        
            then
                 #$wordd=$wordd2
                 #${line/wordd/wordd2}
                 sed -i 's/\<'"$wordd"'\>/'"$wordd2"'/g' $file
                 #echo $wordd2
                 count=$(( $count+1 ))   
            fi        
        done
        #echo "Ocuurance of '$wordd' in Line||$l is : $count"
        l=$(( $l+1 ))
    done < $file
    echo " " 
    echo "After replacemnet,the content of the file is :"  
    echo " "
    while read line
    do
        name=$line
        echo "$name"
    done < $file    
    #echo " "
    #echo "Lines that doesn't contain the word '$wordd' is: $(grep  -v $wordd $file)"
else
    echo "The word '$wordd' is not present in the file"
fi    

#$echo  "$(grep  -v $wordd $file)"

