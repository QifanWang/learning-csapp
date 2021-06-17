#!/bin/bash
for (( cnt = 01; cnt < 17; cnt++ ))
do
    num=$(printf "%02d" $cnt)
    echo "./sdriver.pl -t trace$num.txt -s ./tsh -a \"-p\""
    ./sdriver.pl -t trace$num.txt -s ./tsh -a "-p"
done
# ./sdriver.pl -t trace02.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace03.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace04.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace05.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace06.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace07.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace08.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace09.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace10.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace11.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace12.txt -s ./tsh -a "-p" 
# ./sdriver.pl -t trace13.txt -s ./tsh -a "-p"
# ./sdriver.pl -t trace14.txt -s ./tsh -a "-p"
# ./sdriver.pl -t trace15.txt -s ./tsh -a "-p"
# ./sdriver.pl -t trace16.txt -s ./tsh -a "-p"