#**********************************************************************************************
#Name of the module : makefile
#Date of creation 	: 21/5/2020
#Author of module 	: Shubhangi Joshi
#Description of module:
#This modeule contains the commands for compiling all the files together. Objects files for all
#the cpp files are generated. Also the executable file name is changed.

#Revision/Modification History:
#Added		makefile		Shubhangi Joshi		21/5/2020

#************************************************************************************************


tstel: Tstel.o main.o database.o prepaid.o postpaid.o charging.o Interception_Barred.o
	g++ Tstel.o main.o database.o prepaid.o postpaid.o charging.o Interception_Barred.o -o tstel

main.o: main.cpp
	g++ -c main.cpp

database.o: database.cpp
	g++ -c database.cpp

prepaid.o: prepaid.cpp
	g++ -c prepaid.cpp

postpaid.o: postpaid.cpp
	g++ -c postpaid.cpp

charging.o: charging.cpp
	g++ -c charging.cpp

Tstel.o: Tstel.cpp
	g++ -c Tstel.cpp

Iterception_Barred.o: Interception_Barred.cpp
	g++ -c Interception_Barred.cpp

clean:
	rm -rf *o tstel
