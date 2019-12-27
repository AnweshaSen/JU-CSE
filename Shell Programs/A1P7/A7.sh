#Develop a Linux shell BCSEIII-2019, that will display a prompt, accept user commands and execute.
#The overview of the functions of BCSEIII-2019 is as follows: 
#     • At startup, the shell will display the prompt string: BCSE!!, and ready to accept command.
#     • A command line has the following syntax: command [argument/s]
#     • When a valid command is entered by the user, the shell will execute the command. Commands  may run in the foreground as well as in the background. 
#     • The shell will return an appropriate error message when the command is invalid, or when there are problems with either the arguments or the execution of the command.
# The commands to be executed by  BCSEIII-2019  are given below:
#     (i) dirnew directoryname  <action: creates a new directory>
#     (ii) editfile [filename] 	([] denotes that the argument is optional)
# <action : the file will be opened with the vi editor if filename is given otherwise
# 	       the vi editor for a new file will be opened.>
#     (iii) info [filename]	([] denotes that the argument is optional)
# action: displays essential information about the file specified, it must list the following:
#     • full path of the file, size of the file, last modification date, name of the creator
#     (iv) exitnewshell <action: this will quit the shell>


#!/bin/bash
flag=10
i=0

echo "--------------My Shell---------------"
echo "||          BCSE - III,2019          ||"
echo "||          ANWESHA SEN              ||"
echo "--------------------------------------"
echo " "

while [ $flag -ne $i ]
do
    echo "> "
    read command
    if [ "$command" == "exitnewshell" ]
    then 
        flag=0
        exit 1
    fi
    j=0
    for word in $command
    do    
        if [ "editfile" == "$word" ]
        then
            flag=1
            break
        elif [ "info" == "$word" ]
        then 
            flag=2
            break
        elif [ "dirnew" == "$word" ]
        then 
            flag=3
            break
        else
            echo "Invalid command.Please enter a valid command........"
            flag=4
            break
        fi
        echo "$flag"
    done
    if [ $flag -eq 1 ]
    then
        #echo "In if"
        for word in $command
        do
            echo "$word"
            if [ "editfile" != "$word" ]
            then 
                flag=5
                filename=$word
                break
            fi    
        done
        if [ $flag -eq 5 ]
        then
            vi $filename
        else
            vi
        fi        
    fi            
    if [ $flag -eq 2 ]
    then
        for word in $command
            do
                echo "$word"
                if [ "info" != "$word" ]
                then 
                    flag=6
                    filename=$word
                    break
                fi    
            done
            if [ $flag -eq 6 ]
            then
                echo "Filepath: $PWD"/$filename
                stat --format="File size = %s" $filename
                stat --format="Last Modification Date & Time = %y" $filename
                stat --format="Owner = %U" $filename

            else
                echo "Filename is missing.Enter an existing filename to validate this command."
            fi        
    fi
    if [ $flag -eq 3 ]
    then
        for word in $command
            do
                echo "$word"
                if [ "dirnew" != "$word" ]
                then 
                    flag=7
                    filename=$word
                    break
                fi    
            done
            if [ $flag -eq 7 ]
            then
                mkdir $filename
            else
                echo "Directory name is missing.Enter a name to make a new directory."
            fi        
    fi
#echo "$flag"                        
done            

