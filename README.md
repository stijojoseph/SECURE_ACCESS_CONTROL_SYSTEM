# SECURE_ACCESS_CONTROL_SYSTEM# SECURE_ACCESS_CONTROL_SYSTEM
PROJECT2                                                
                                                                                                                                         by   stijo joseph 
                                                                 EEE 3rd year,NIT AP
SECURE ACCESS CONTROL AND DATALOGGER   SYSTEM


INTRODUCTION
In order to implement a safe environments for employees, visitors,customers  in a designated area
To keep track who comes and goes,protect against unwanted visitors and to create a safe breach environment a secure access system is essential
HARDWARES USED
1.stm32f103c8t6 (bluepill)
2.Esp8266-01
3. Sd card jack
4. Sd card
SOFTWARES USED
1.STM32 CUBEIDE               

      PROJECT BASIC IDEA
•	On boot up the stm32 stores the database names from sd card to Sram(based on SRAM capacity).The incoming data on reception is stored on sd card with date and time
•	And check with database on sram and if not found also checks on sdcard  ,if the id matches with database the person is given access ,by setting Gpio pin to high state,else acess is denied .
•	Within an interval of 2 min ,stm32 checks internet connectivity status by sending AT command
(AT+PING=www.google.com)
if it returns OK ,STM32 converts the name ,date,time into the JSON format as per the API demand,and then required AT commands for sending JSON data and the bulk JSON data is sent along with it, and update the status on sdcard as success for sent datas else status as failed,
•	The Esp8266 upon reception of bulk data uploads it to the server 







DETAILED DESCRIPTION
•	The project is executed using FREERTOS,two threads are used
1.Recieve the data-if data available receives
                                   Stores on sd card
                                  Checks on database
                                 Allows or deny access  

2.Internet check and Data send-Checks the internet,converts to JSON FORMAT and send Data to Esp8266
                                                   



                                                




CIRCUIT DIAGRAM 




FOR BETTER VIEWING GDRIVE LINK OF CIRCUIT DIAGRAM
https://drive.google.com/file/d/1iHn1296J9a794OrEf6iI836TUdi0DIwN/view?usp=sharing


The database of this system is maintained as two parts
1.SRAM
2.SDCARD
ON bootup  it takes 8 seconds to connect to the wifi network ,the SSID and PASSWORD stored on main.h 
If the database id size takes more space than the SRAM of microcontroller then database check of the remaining data will also be done on the SD card otherwise only SRAM will be checked on data arrival for entry access.
The database is intially  stored on “test.txt” file on sdcard where names are written line by line .
(Status of the sd card can be found through Uart2tx  PA2 out enabled for debug)
 
•	On boot up an Sd card test will be done to make sure  Sd card is working fine and is in good working condition Then sdcard to sram database copy starts which will take a few milli seconds to finish off.Then starts the thread ,checks incomming data.And 2 mins interval check internet and data sending


•	The incoming data containing the name/id of the person is fed into the stm32 from an external device through serial communication(UART Protocol).From USART2RX pin(A3) the stm32 recieves the incoming id and is stored along with its entry time and date on the SD card file “storage1.txt” .And the id is checked first on SRAM database and if not found checks on Sd Card Database on “test.txt” ,if the id exists GPIO pin is enabled and hence access is granted else access denied
•	On 2mins interval AT COMMAND  for internet connectivity  check is send to through USART1TX pin(A9) ,esp8266 recieve “*” from its RX pin  and the byte received by ep8266,then it pings @ www.google.com
•	If it receives OK it sends through its TX pin  to Stm32 USART2RX(A10) denoting internet is fine data can be sent. If bad http code received back esp8266 send ‘timeout’ denotes bad network connection.Based on internet status .The data traken from SD card that is id,time date is converted to its prescribed json format as per the API demand and sent to esp01 
•	
•	JSON FORMAT FOR BULK API
•	{
•	"did":"1",
•	"gid":"00000001",
•	"rc":[
•	    	{
•			"uid":"00000001",
•			"atp":"I",
•			"as":"V",
•			"qr":"ABC",
•			"dt":"2021-02-06 16:13:54"
•		},
•		{
•			"uid":"00000002",
•			"atp":"I",
•			"as":"V",
•			"qr":"ABC",
•			"dt":"2021-02-06 16:13:54"
•		}
•	     ]
•	}

Github source code of the project- https://github.com/stijojoseph/Secure-Control-Access-Sytem-Stm32f103c8t6-bluepill-.git

Measures needed to be taken
•	Make sure connections are correct
•	Connect Usb to ttl RX to  stm32  USART2tx(PA2),So that output of the running  program will be  visible on terminal 
•	Format sd card if it fails
•	Store the database file name as test.txt
 
 
       INSTRUCTIONS
1.Dont forget to edit the SSID and Password
In the main.h ,Before connecting ESP01 to stm32 make sure it is supporting AT commands baudarate perfectly 115200,there are case Esp01 throws garbage values as response this is due to inadequate power supply ,so make sure esp01 is working fine 
For checking it connect esp01 to usb ttl @ 115200 baudrate ,RX to TX,TX to RX,
EN pin to 3.3v,then 3.3v and GND

2. Connect Usb to ttl RX to  stm32  USART2tx(PA2) baudarate 9600,So that output of the running  program will be  visible on terminal 
3. To view the status of communication between stm32 and esp01 tap the uart signal on PA9TX uart terminal to PC through Usb to ttl
Debugging steps
1.connect usb to ttl RX to stm32 PA2 TX
2.if the sd card holder is connected correctly 
And sdcard is in working condition
The output on the serial terminal will be
“sd card test success.....2opened storage done”
3.if the output is “startedsd card failed2”
Then check the sdcard holder connection or format the sd card or make sure sd card holder is in working condition with Arduino ide sample sdcardinfo program,make sure jumpers wires are in working condition


4.If your ouput is “sd card test success.....2”
Then you have sucessfullly initialized sdcard
If this initailzation fails the remaining program may not get executed

5.Now you can send data to stm32 via UART
For testing u can use Bluetooth module or stimulate from Arduino
6.Example:
On sending a name “stijo” via uart to stm32 PA3RX terminal,if the data is received
Its prints out 
“stijo access denied”
Access is denied because name is not regestred on database
If it was  registered it shows
“access granted”
And makes gpio a pin high u can define the gpio pin in main.h
Then if the internet connectivity is ok
It printout the converted json data
{
                                                        "did":  "00000001",
                                                                               "gid":   "00000001",
                        "rc":   [{
                                                        "uid":  "stijo",
                                                                               "atp":   "1",
                                "as":   "VALID",
                                                                        "qr":  "ABC",
                        "dt":   "0;4;20 1;3;2"
                                                        }]
                                                          }-
If this much executes on the debug screen your data has been successfully published to the webserver
DEBUGGING 2
To make the printout of STM32 to esp01 communication connect usb to TTL as a tapping to esp01 TX terminal which is conncted to stm32 RX,baudrate @115200
ON reset
The printout statments
 








AT+CWMODE=3
OK
WIFI GOT IP
AT+CWJAP_DEF="stijo","stijojoseph"
WIFI DISCONNECT
WIFI CONNECTED
WIFI GOT IP
AT+PING="www.google.com"
busy p...
OK
AT+PING="www.google.com"
+89
OK

If you got similar to this as output then you have successfully made the connection of stm32 wit esp01
Else
1.Check the jumpers
2.check the power supply of esp01
3.make sure data is coming from stm32
4.sepreatly check esp01 with AT commands
After connecting with USB to ttl
AT
Output- OK
AT+CWMODE=3 //(1 is "Station" only (wifi client), 3 is mixed mode "Station and Access-Point", both should work)
Output -ok
AT+CWJAP_DEF="ssid","password"
Output- 
WIFI DISCONNECT
WIFI CONNECTED
WIFI GOT IP




After sending two datas ouput will be

AT+CIPSTART="TCP","robatosystems.com",80
CONNECT

OK
AT+CIPSEND=396

OK
>
Recv 396 bytes


busy s...

SEND OK

+IPD,469:HTTP/1.1 200 OK
Connection: Keep-Alive
X-Powered-By: PHP/7.2.34
Content-Type: application/json
Access-Control-Allow-Origin: *
Access-Control-Allow-Methods: POST
Access-Control-Allow-Headers: Access-Control-Allow-Headers,Content-Type,Access-Control-Allow-Methods,Authorization,X-Requested-With
Content-Length: 91
Date: Sun, 14 Mar 2021 18:50:18 GMT
Server: LiteSpeed

{"status":"success","message":"log inserted"}{"status":"success","message":"log inserted"} AT+CIPCLOSE
CLOSED
 
