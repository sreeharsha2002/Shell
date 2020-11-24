# SHELL:

### N.V.Sree Harsha Reddy 2019101040 CSE

## HOW TO RUN:

1. Open the folder containing files in terminal
2. Run make
3. Run ./a.out
The operating_System_Shell will open up!
The shell takes the directory it is present in as the home of the shell as per requirements.

## BUILT-IN-COMMANDS:

# cd :
It changes the directory. It takes ~ as the folder that contains the executable. It takes relative paths also. Usage cd< path name >.
and if we give cd - then it prints the previous directory and change the current directory to previos directory.

# ls :
It lists files in the format accorning to flags.The flags -al -a -la -l -aaal -laa -lllaa etc. Usage: ls < flags > < files >. Options can come between arguments also. 

# pwd :
It displays the present working directory

#  pinfo :
It executes the pinfo command mentioned in Assignment pdf. Usage: pinfo < pid1 > < pid2 > ...
If no pid is given then it shows the pinfo of shell.

# echo :
It executes echo command. It does not implement "" '' $ etc. But handles whitespaces and tabs as required.

## ASSIGNMENT-2 Bonuses:

# nightswatch :
    BONUS 1 ! : Usage : nightswatch -n < time interval > < command >
    It executes the command every time interval seconds mentioned by the option.
    The commands can be:
    1. interrupt : It shows the CPU interrupts as said in Assignment PDF.
    2. newborn : It shows the latest process pid that was made on the system.
    Press q to exit the loop.

#    history
    BONUS 2 !: Usage : history < num >
    By default it shows max 2 0. But if num is given it shows the last num commands if num commands are available or last available. If num is > 20, It defaults to 20.
    If the last two commands are same then history take them as same and it stores as one command. 
    It also includes the current command that executed like in real terminal. The history is stored in a file called history.txt in the folder that the files exist.

## ASSIGNMENT-3 commands:
# setenv :
Usage -> setenv var [value]
This command sets the value as the value of the variablr var and make it as environment variable. And if it has no arguments or more than two arguments I am printing In valid arguments. And if the value is Null then it sets as null.

# unsetenv:
USAGE -> unsetenv var
This command unsets the value of the var and unsets it from environment variable.And if it has no arguments or more than two arguments I am printing In valid arguments. 

# jobs:
This command will prints the list of jobs that are running in background with their job numbers and their states in background state.

# kjob:
usage-> kjob <jobno> <signalno>
This command will send the signal corresponding to signalno to the jobno specified if the job number is not there then it prints Invalid job number. And I am assigning jobno to pid of the process i.e if the job terminated then the other processes job no will no change this is same as real terminal.If we gives invalid signal to that process then also error prints.

# fg:
usage -> fg <jobno>
This command will make the background process to run as foreground and changes its state to running if the jobno is notthere it prints invalid jobnumber and if the job exited without stopping then this process is removed from the list of jobs. if it stops like when pressed ctrl z then it again goes to background and list will not be updated as jobno is associated with process id.

# bg:
usage -> bg <jobno>
This command will change the state of stopped background process to running in background and for invalid jobno it prints invalid jobno . Here vim will not work as we cannot change the state of vim in background form stopped to running.

# overkill:
This command kills all the background proccesses at once .

# quit:
This command will exit the our shell and we can use ctrl D for exit of shell As we are ignoring the signals ctrl z and c to our shell to not to exit when the signal are passed to our shell no to the program.

## Assignment -3 bonuses:
# cd - :
previous directory this will be handled by gotocd()fn in by making a global variable.

# exit codes:
This indicates the whether the command is exited normally or not and this printing of exit codes will follow the rules in pdf.If intially if there is no command I am printing happy face.

In fg process it checks the exit staus is zero or not if it is not zero then error according to manpage . But some may violate this example diff which has error code as 2 and exit code is 1 if there is difference.

# command chaining with @ and $:
These are logical AND and OR such that the exit codes of the entire chain is the logical AND or the logical OR. These operators bind looser   than ​|​, ​>,​​<​,​ ​>>,​ ​&,​ but tighter than ​;.​  the rules are as followed in the pdf.


## FILES INCLUDED:

# main.c:
It has the main function and the loop that executes the shell. It also has the prompt printing.It also gets the command as input and calls the comm fn which will excecute in comm.c to handle the inputs.it has gotohistory() fn it handle the input or command given to history.txt in the executable folder.

# bonus.c:
It has gotobonus() function this funtion takes care about the bonus3 section which has @ and $ we will take care about given conditions and this will depend upon exit codes of each command (implemented bonus2 also)for this I implemented global varaible error and track the exit codes of commands and this exit codes will used in this file. here the commands seperated by @ and $ and passed to piping()function.

# piping.c:
It has piping() function which has the the parameters passed form gotobonus() funtion and I  seperated the command using '|' and passing each command after seperated by pipe to gotocomm()fn  and the input and out put of pipes will take care by pipe fn and other dups etc., and I am passing the input and output filediscriptees to the gotocomm() fn.

# comm.c:
It will take the in and out given from piping() and seperate the command passed to this fn with redirection symbols and storing the appropriate things as in and out for this command as mentioned in pdf and send these parameters to fncalls()fn in that I am changing the input and output of that command as we seperated using redirections or passed from the piping()fn and In this fn we have the fuction calls to execute commands like :
1.acd():handle input regarding cd command
2.als(): handle input regarding ls command
3.apwd():hadle the input regarding pwd command
4.anight():handle the input regarding nightswatch command;
5.gotofgbg():handle the input to run on backgroung or foreground.
6.gotoecho():handle the input regarding echo command;
7.gotopinfo():handle the input regarding pinfo command;
8.retrievehistory():retrieves the history from history.txt file.
9.gotosetenv():handles the input regarding setenv command.
10.gotounsetenv():handle the input regarding unsetenv command.
11.gotojobs():it will print the jobs.
12.gotokill():handle the input regarding kill command.
13.gotocomfg():handle the command fg
14.gotocombg():handle the command bg
15.gotooverkill():kills all bg processes.

# substring.c:
It has the funtions like:
1.substring() to make a substring from a string.
2.acd(): it parses the passed command with spaces and tabs. and pass them to gotocd()
3.gotocd() : handle input regarding cd command regarging to the passing parameters.
4.apwd():it will parse the command with spaces and tabs and pass to gotpwd().
5.gotopwd():handle the input regarding pwd command
6.gotoecho():handle the echo command.
4.max():returns max fn.

# prompt.c:
It will handle how to print the prompt with ~ representing the folder where we are executing and other things.

# ls.c:
It has funtions like:
1.perm():returns the permissions of the file.
2.als():it parses the command according to spaces and tabs. and setting flags and pass them to gotols().
3.gotols(): handle input regarding ls command and prints the output according to flags.

# fgbg.c
it has funtions like:
1.gotofgbg():handle the input to run on backgroung or foreground. as parameters passed from comm.c seperates the process which has to run in fg and bg to coreesponding fns.
2.gotofg():after seperation of process the processes which has to be run on foreground will to this function and execute appropriately.
3.gotobg():after seperation of process the processes which has to be run on background will to this function and execute appropriately. and in this function the signal fn will be called when there is signal interrupt. and we have to print which has exited so there is handler fn which outputs tp stderr for which process ha exited according to signal.
4.myhand():it is handler function of signal fn to handle how to print the exited ones.
5.gotojobs():it will prints the all backgrounds process.
6.gotokill():it will handle the kill command.
7.gotocomfg():here it will make the background process to fg and handles the ctrl z signal to add the process to bg.
8.gotocombg():here it will change the state of stopped to running process in bg.
9.gotooverkill():here it will send the kill signals to all bg processes.

# nigtw.c :
it has funtions like:
1.anight():it parses the command passed with spaces and tabs , And pass to gotonight() fn.
2.gotonight():handle the input regarding nightswatch command and parameters passed from comm.c

# history.c :
it has functions like:
1.gotohistory():It will handle the commands given to history.txt file; in this we create the history.txt file to store permanently that means if we go out our shell and came back it has to have the previous commands also as real history command does.
2.retrivehistory():it will retrieve  20 commands from history.txt file.

# pinfo.c:
It has functions like :
1.gotopinfo():it will output the process information for given process or it will hive the process info of the our shell.
2.gotosetenv():This fn will set the variable with given value or null according to command.
3.gotounsetenv():This fn will unsets the variable from environment variable according to command.   

# headers.h:
it has all the list of inbuilt headers that we use for this programs.

All the other headers are should be include to use their corresponding .c files.

makefile has all my .c files to compile at once.

After the backgroung process is terminated I am printing it has printed normally or abnormally so after printing that press ENTER to make the prompt visible if you donot give the ENTER also it takes input the prompt will come after pressing ENTER because of stderr printing.





