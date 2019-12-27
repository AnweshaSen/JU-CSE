#Write a shell script that takes 4 file names (C programs) as command line arguments 
#and prints the frequency of the occurrences of the following 3 strings “printf”, “scanf”, “int” in each file. 
#The output (in tabular format) should clearly denote the frequency of the occurrences of each string in each file.

#!/bin/bash
file1=$1
file2=$2
file3=$3
file4=$4
# v=0
 
if [ $# -lt 4 ]
then
    echo "Four filenames are needed.You have entered less......"
	exit 1
fi
if [ ! -f $file1 ]
then
	echo "File 1 not found.Enter a valid filename......."
    exit 2
fi    
if [ ! -f $file2 ]
then 
    echo "File 2 not found.Enter a valid filename......."
    exit 2
fi    
if [ ! -f $file3 ]
then 
    echo "File 3 not found.Enter a valid filename......."
    exit 2
fi    
if [ ! -f $file4 ]
then 
    echo "File 4 not found.Enter a valid filename......."
    exit 2
fi
                
#echo $file1 $file2 $file3 $file4 

echo " "

echo "-----------------------------------------------------------------------"
echo "  File        ||                   Frequency of words                  "
echo "              ||-------------------------------------------------------"
echo "  Name        ||           printf            scanf            int"
echo "-----------------------------------------------------------------------"
echo " "

occur1=$(grep  -c "printf" $file1)
#echo $occur1

occur2=$(grep  -c "scanf" $file1)
#echo $occur2

occur3=$(grep  -c "int" $file1)
#echo $occur3


echo " $file1                    $occur1                $occur2               $occur3"
echo "------------------------------------------------------------------------"

occur1=$(grep  -c "printf" $file2)
#echo $occur1

occur2=$(grep  -c "scanf" $file2)
#echo $occur2

occur3=$(grep  -c "int" $file2)

echo " $file2                    $occur1                $occur2               $occur3"
echo "------------------------------------------------------------------------"

occur1=$(grep  -c "printf" $file3)
#echo $occur1

occur2=$(grep  -c "scanf" $file3)
#echo $occur2

occur3=$(grep  -c "int" $file3)

echo " $file3                    $occur1                $occur2               $occur3"
echo "------------------------------------------------------------------------"

occur1=$(grep  -c "printf" $file4)
#echo $occur1

occur2=$(grep  -c "scanf" $file4)
#echo $occur2

occur3=$(grep  -c "int" $file4)

echo " $file4                    $occur1                $occur2               $occur3"
echo "------------------------------------------------------------------------"

echo " "