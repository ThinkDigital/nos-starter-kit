
# Script to connect to network and set Power Saving Mode (PSM) on modem

## firmware version
AT+QGMR  //check fw version

BC68JAR01A08

OK

## modem configuration
//This part only need to be sent once, it is just to be sure that nconfig is set it up properly

AT+CFUN=0

OK

AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE //I enable scrambling for better power consumption

OK

AT+NCONFIG=CR_0859_SI_AVOID,TRUE //related with scrambled

OK

AT+NCONFIG=AUTOCONNECT,FALSE //I disable autoconnect

OK

REBOOTING //Reboot device in order to use new setting with nconfig

ÃâFø

Boot: Unsigned

Security B.. Verified

Protocol A.. Verified

Apps A...... Verified

REBOOT_CAUSE_APPLICATION_AT

Neul

OK

//once device boots I set up band

AT+NBAND=20

OK

//I enable some urc, cereg, psm and cscon if you don't need it you can omit this part

AT+CEREG=2;+NPSMR=1;+CSCON=1

OK

OK

OK

## connect to network
//I set up PSM, 1 hour for tau and 10 seconds for idle

AT+CPSMS=1,,,"00100001","00000101" // if PSM is not desired, call AT+CPSMS=0

OK

AT+CFUN=1 //switch on the radio

+CEREG:0,0000,00000000,9

OK

AT+CGDCONT=1,"IP","" //I send my APN, here is empty because it is automatically loaded by network with Vdf

OK

AT+COPS=1,2,"21401" //I do the attach

OK

+CEREG:2,0000,00000000,9

+CSCON:1

+CEREG:5,0216,0153803F,9 //here I can see that I am connected to network

AT+NSOCR= DGRAM,17,16666,1 //I created my socket

1

OK

## send some data
AT+NSOST=1,188.77.165.106,16666,12,48656C6C6F20576F726C6421 //I send some data to server

1,12

OK

## read received messages
+NSONMI:1,12

AT+NSORF=1,12 //I read my reply from server

1,188.77.165.106,16666,12,48656C6C6F20576F726C6421,0

OK

## close socket to enter sleep mode
AT+NSOCL=1 //I close the socket

OK

+CSCON:0 //start timer (remember 10 seconds that I set it up)

+NPSMR:1 //after 10 seconds my device goes to sleep

## Notes
To be able to use PSM, a request has to be made to the telecommunication operator for the sim card in use
​
