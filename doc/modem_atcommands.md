# Instructions to connect to network and set Power Saving Mode (PSM) on modem

## Firmware version

```
AT+QGMR  //check fw version

BC68JAR01A08

OK
```

## Modem configuration

This part only need to be sent once, it is just to be sure that nconfig is set it up properly.

```
AT+CFUN=0

OK
```

Enable scrambling for better power consumption.

```
AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE

OK

AT+NCONFIG=CR_0859_SI_AVOID,TRUE

OK
```

Disable autoconnect.

```
AT+NCONFIG=AUTOCONNECT,FALSE

OK
```
Reboot device in order to use new setting with nconfig

```
REBOOTING

ÃâFø

Boot: Unsigned

Security B.. Verified

Protocol A.. Verified

Apps A...... Verified

REBOOT_CAUSE_APPLICATION_AT

Neul

OK
```

Once device boots set up band.

```
AT+NBAND=20

OK
```

Enable some URC, CEREG, PSM and CSCON if you don't need it you can omit this part.

```
AT+CEREG=2;+NPSMR=1;+CSCON=1

OK

OK

OK
```

## Connect to network

Set up PSM, 1 hour for tau and 10 seconds for idle.

```
AT+CPSMS=1,,,"00100001","00000101"

OK
```

If PSM is not desired, call `AT+CPSMS=0`.

Then, switch on the radio.

```
AT+CFUN=1

+CEREG:0,0000,00000000,9

OK
```

Send APN, here is empty because it is automatically loaded by network.

```
AT+CGDCONT=1,"IP",""

OK
```

Attach to network.

```
AT+COPS=1,2,"21401"

OK

+CEREG:2,0000,00000000,9

+CSCON:1

+CEREG:5,0216,0153803F,9 //here I can see that I am connected to network

AT+NSOCR= DGRAM,17,16666,1 //I created my socket

1

OK
```

## Send some data

Send some data to server.

```
AT+NSOST=1,188.77.165.106,16666,12,48656C6C6F20576F726C6421

1,12

OK
```

## Read messages

Read my reply from server.

```
+NSONMI:1,12

AT+NSORF=1,12

1,188.77.165.106,16666,12,48656C6C6F20576F726C6421,0

OK
```

## Close socket to enter sleep mode

Close the socket.

```
AT+NSOCL=1

OK

+CSCON:0 // start timer (remember 10 seconds that I set it up)

+NPSMR:1 // after 10 seconds my device goes to sleep
```

## Notes

To be able to use PSM, a request has to be made to the telecommunication operator for the sim card in use.
