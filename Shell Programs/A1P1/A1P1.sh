#Write a shell script that has 2 user created variables, userv1 and userv2.  
#Ask for the values of the variables from the user and take in any type (real/integer/character) for the 2 variables.
#Try to perform the following operations on the variables: (a) add, (b) multiply and (c) division. 
#Print appropriate error message/s if the operation cannot be performed, otherwise print the result appropriately: 
#like, “the sum of ‘userv1’ and ‘userv2’ is ….”. Execute the shell script as many number of times the user wants. 
#Show all possible combinations of the types of variables and result/s of the corresponding operation/s.


#! /bash/bin

while true; do
	echo "Enter the first number"
	read userv1

	echo "Enter the second number"
	read userv2

	if [ $userv1 -eq $userv1 2>/dev/null ]
	then
		echo "$userv1 is an integer."
		if [ $userv2 -eq $userv2 2>/dev/null ]
		then
			echo "$userv2 is an integer too."
			echo "So the arithmatic operations are: "
			echo "Sum = $(( userv1 + userv2 ))"
			echo "Mul = $(( userv1 * userv2 ))"
			echo "Div = $(( userv1 / userv2 ))"
		else
			echo "$userv2 is not an integer so arithmatic operations are not allowed."	
		fi
	else
		echo "$userv1 is not an integer"
		if [ $userv2 -eq $userv2 2>/dev/null ]
		then
			echo "$userv2 is an integer, so arithmatic operations are not allowed."	
		else
			echo "$userv2 is not an integer."
			echo "So concatination of these two inputs is: $userv1$userv2"	
		fi	
	fi
	
	echo "Continue? [Y/N]" 
	read var
	if  [ "$var" = "N" ] || [ "$var" = "n" ]
	then 
		break	
	fi	
	#echo $ch
done	
	
