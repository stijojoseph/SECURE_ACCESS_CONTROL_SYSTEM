/*SET UP SSID AND PASSWORD in main.h BEFORE EXECUTING the program*/
//for debugging use A2 tx pin to pc
// dont increase JSON_NUM above 3 memory leak happens
#include "main.h"
#include "fatfs.h"
#include "main.h"
#include "fatfs.h"
#include "string.h"
#include "ctype.h"
#include "queue.h"
#include "cJson.h"
int counter=0;  //global variable for internet connection check
int interrupt=0;
int stop=0;
char *d;
char ch[5];
char *datat;
uint8_t datar[30];
char dat2[20],dat3[40];
char  *json_conversion(char h[]);
void queuer(char *data);
void RTCinit();
char queue[810];
int size=0;
FATFS fs;
FATFS *pfs;
FIL fil,f,g,c1,d1,m;                              //global definition for file system handling
FRESULT fres;
DWORD fre_clust;

char buffer[100];
int bufsize (char *buf);                          //fucntion for calculation of datasize for  memset conversion when pointers used
void datas(char *data);                          //function for sending data to esp8266 (USART1TX)
void datas2(char *data);                             //function for debuging ,connect usb to ttl to get the printf statements of the program (USART2TX) A2
int check(char a[50],char b[50]);                  //function for comparing two strings
char* itoa(int value, char* result, int base);     //function for comparing two strings return 1 if same else 0


void currenttime(char *time,int a);              //function for getiing the current time to time pointer, time format HH;MMM;SS

void currentdate(char *date,int b);								//function for getiing the current time to time pointer, time format DD;MM;yy
void id_time_storage(char *sq1);                 //storing incomming name ,along with the current time and date with FAILED status on sd CARD
void sd_send(int h1);                              //function for sending the  data from sd card to esp8266 by updating the status on sd card as "sucess"
int sd_check();                                    //function for checking any data on sd card with "FAILED" status ,return the line number of data with failed status on file "storage1.txt"

void sdcard_status();                                 //function to test the sd card is fine @ the begining functon invoked

void join(char *s,char x[],char p[],char j[],int c);  //function for joining the date or time format to string DD;MM;YY or HH;MM;SS
void main_reciever();                                // TASK FUNCTION    get the data from external device ,comparison with data base,access comtrol


int sd_data_check(char buf[40]);                        //function for checking the remaining data on sd card which is not uploaded to sram
void sd_to_sram();                                          //function to copy names from sd card to sram based on sram size (NAME_SIZE)
int check_sram(char data[20]);                        //function for data comparison on sram
void integer_2_string(int s);                       // convert integer to string for debug output purpose
void sd_check_send();                               //function for sending checking "FAILED STATUS" on storage data and send to esp8266 and update status to sucess
//int sd_web_data();

void send_datas();                                   //TASK FUNCTION to check internet status and send data to esp8266 till no data with "FAILED status
//void sd_writes();
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void const * argument);
void queue_send();
char *get_json_data(void);
void wifi_sender(char *s);
void divide_send(char *s);
void wifi_connection_setup();
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef DateToUpdate = {0};
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;


/*
 * STRUCTURE sd_card is defined for storing the database names stored on SDCARD to SRAM on bootup ,this increases the speed of checking for authenticity
 * @param  NAME_LENGTH max length of names to be updated to SRAM
 * @param NAME_SIZE   maximum names needed to be stored to SRAM
 *
 */


 struct sd_card
{
	char names[NAME_LENGTH];

}sd[NAME_SIZE ];



xTaskHandle *main_recieve=NULL;
xTaskHandle *send_data=NULL;
xSemaphoreHandle *sd_card=NULL;





int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_USART2_UART_Init();
  //MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	NVIC_EnableIRQ(USART1_IRQn);
	USART1->CR1  |= USART_CR1_UE;
	USART1->CR1  |= USART_CR1_RXNEIE;
	USART1->CR1  |= (USART_CR1_RE );  // RX, TX enable.
                       // USART3 enable.


sdcard_status(); //checks the sd is in working condition or not
RTCinit();     // checks the rtc clcok is intialized or not ,if not initalizes it
/*
char *data[40]={"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t","melvin\t12;05;23\t22;12;21\t"
,"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t","melvin\t12;05;23\t22;12;21\t"
,"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t"
,"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t","melvin\t12;05;23\t22;12;21\t"
,"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t"
,"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t","melvin\t12;05;23\t22;12;21\t"
,"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t","melvin\t12;05;23\t22;12;21\t"
,"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t"
,"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t","melvin\t12;05;23\t22;12;21\t"
,"stijojoseph\t12;05;23\t22;12;21\t","arun\t12;05;23\t22;12;21\t","jijo\t12;05;23\t22;12;21\t"};
int i=0;
for(int j=0;j<10;j++)
{
	for(;i<6;i++)
	{queuer(data[i]);

	}
queuer("#");
datas2("\n\n\n");
i=0;

queue_send();
}

/*
//datas(queue);
//json_conversion(queue);

char *sp=json_conversion(queue);
//datas(sp);
int y1=0;
int g=strlen(sp);
//integer_2_string(g);
int m1=20;
char sp21[400];
int k1=0;
int s=(g/m1)+2;
int x;
for(int t=0;t<s;t++)
{
 x=(m1*t);
for( ;y1<x;y1++,k1++)
{
	sp21[k1]=sp[y1];
}
datas2(sp21);
k1=0;
memset(sp21,0,sizeof(sp21));
}
cJSON_free(sp);
}

//CLOCK DEBUG IF U FACE ANY PROBLEM ,UN COMMENT THIS
/*	char time[40];
		char date[40];
	while(1)
	{
		currenttime(time,0);
    currentdate(date,0);

		datas2(time);

		datas2("  -----   ");
	HAL_Delay(1000);
		datas2(date);
		datas2("  -----   ");
	}

*/

wifi_connection_setup();
xTaskCreate(main_reciever,"main_send",recieve,NULL,2,main_recieve);
xTaskCreate(send_datas,"main_send",sends,NULL,2,send_data);


sd_card=xSemaphoreCreateMutex();
vTaskStartScheduler();

// ******* BARE METAL PROGRAM IN COMMENTS DOWN UNCOMMENT IT FOR DEBUGGING IF NEEDED
	//int l=sd_checker();
	//itoa(l,p,10);
	//datas2(p);

/*
	int l=sd_check();

while(l>0)
{ datas2("opened");
	l=sd_check();
	//HAL_Delay(1000);
	sd_send(l);
//l++;
	integer_2_string(l);
}

	  sd_names();
int d=check_sram("Alisa");
integer_2_string(d);
char buff[40]="Caterina";
int h=strlen(buff);
buff[h]='\n';
int u=sd_data_check(buff);
	integer_2_string(u);
	datas2("done");
	*/

	//sdcard_read();
/*	sd_names();
  while (1)
  { char dat2[20],dat3[2];
int j=0;

		while(HAL_UART_Receive(&huart2,datar,1,70)==HAL_OK)

		{
			datat=(char *)datar;
			strcpy(dat3,datat);
			dat2[j]=dat3[0];
			j++;
		}
		if(j!=0)
		{
		datas2("*");
			datas("*");
			//check_internet();

		//	sd_send();
			//strcpy(dat4,dat2);
		//memset(ch, 0, bufsize(ch));
			ch[0]='2';
		}
	else
		if(ch[0]=='0')
		{ datas2("failed");
			datas2(dat2);
			//datas2("hee");
		//id_time_storage(dat2);
		//	sd_send();
			memset(dat2, 0, bufsize(dat2));
ch[0]='2';
		}
		else if(ch[0]=='5')
		{ datas2(dat2);
		//id_time_storage(dat2);
		int d=check_sram(dat2);
integer_2_string(d);
if(d==0)
d=sd_data_check(dat2);
	integer_2_string(d);

			//incomming_send(dat2);
			memset(dat2, 0, bufsize(dat2));
ch[0]='2';

		}
	}
  */

}

void main_reciever()
{
xSemaphoreGive(sd_card);
sd_to_sram();                                        //copies the database names to sram
 while(1)
 {
  if(HAL_UART_Receive(&huart2,datar,1,100)==HAL_OK)   //recieve data from external device if availabe
   {

    int j=0;
	  datat=(char *)datar;
		strcpy(dat3,datat);
		dat2[j]=dat3[0];
		j++;
	  while(HAL_UART_Receive(&huart2,datar,1,100)==HAL_OK)
			{
			datat=(char *)datar;
			strcpy(dat3,datat);
			dat2[j]=dat3[0];
			j++;
		  }
//interrupt=0;
		xSemaphoreTake(sd_card,portMAX_DELAY);
	 datas2(dat2);
	 id_time_storage(dat2);                    //storing data with time date to sd card

   char buff[40];
	 strcpy(buff,dat2);

   int h=strlen(buff);
   buff[h]='\n';
   memset(dat2,0,sizeof(dat2));

   int d=check_sram(buff);                     //checking sram to check the match of name

	 if(d==0)
		{d=sd_data_check(buff);                 //if not found check sd card database


		}
   if(d!=1)
	   datas2("access denied");
    else
     {datas2("access granted");

//HAL_GPIO_WritePin(DOOR_PORT,DOOR_OPEN,GPIO_PIN_SET);  //OPEN THE DOOR ,THEN PUT SOME DELAY AND INVOKE CLOSE THE DOOR

      }
	memset(buff,0,sizeof(buff));
	memset(dat2,0,sizeof(dat2));
  xSemaphoreGive(sd_card);
	 }

	vTaskDelay(1);
 }
 }



 void send_datas()            //TASK FUNCTION to check internet status and send data to esp8266 till no data with "FAILED status
{
	// datas2("hai");
while(1)
{

	//taskYIELD();
	//datas2("*");
	datas("AT+PING=\"www.google.com\"\r\n");
	vTaskDelay(3000);

	if(counter==1)
	{
 xSemaphoreTake(sd_card,portMAX_DELAY);
 sd_check_send();
xSemaphoreGive(sd_card);
		counter=0;
		vTaskDelay(DATA_SEND_DELAY);

 }

}
}
/*
 * wifisender(char *m)  Receives converted JSON data in pointer m, and sends the json data to esp01 with combining all the AT Commands
 * necessary for the esp01 module
 *
 * */



 void wifisender(char *m)
 {

 	char store[]="POST /gym-acs-portal/api/v1.0/devicelogs HTTP/1.1\r\nHost: robatosystems.com\r\nAccept: */*\r\n";

 char mains[]="POST /analog-reading/api/v1.0/logs HTTP/1.1\r\nHost: robatosystems.com\r\nAccept: */*\r\nContent-Length: 187\r\nContent-Type: application/json\r\n\r\n";

 	int o=strlen(mains);
 	char v[2];

 	int s;
 	//char t[2];

 	//datas("   ");
 	char content_length[2];

 	s=strlen(m);
//s=s-14;
 itoa(s,content_length,10);
 //datas(content_length);
 o=o+s+6;
 itoa(o,v,10);
 //	datas(v);

 //datas("AT+PING=\"www.google.com\"\r\n");
 //vTaskDelay(4000);
 datas("AT+CIPSTART=\"TCP\",\"robatosystems.com\",80\r\n");
 vTaskDelay(3000);
 datas("AT+CIPSEND=");
 datas(v);
 datas("\r\n");
 vTaskDelay(2000);
 datas(store);
 datas("Content-Length: ");
 datas(content_length);
 datas("\r\n");
 datas("Content-Type: application/json\r\n\r\n");

 divide_send(m);
 datas("\r\n");

 vTaskDelay(500);
 datas("AT+CIPCLOSE\r\n");
 xSemaphoreGive(sd_card);
 vTaskDelay(2000);
xSemaphoreTake(sd_card,portMAX_DELAY);



 }

 /*
  *  void divide_send(char *m)
  * spliliting the json data for sending through uart terminal if json data size greater than 800  it canno be send in one go,
  * so in this function  the json data is splitted up and sent
  *
  *
  */



 void divide_send(char *m)
 {
 	//datas(m);
 	int y1=0;
 int g=strlen(m);
 	//integer_2_string(g);
 	int m1=20;

 	int k1=0;
 	int s1=(g/m1)+2;
 	int x;

 	for(int t=0;t<s1;t++)
 	{
 		char sp21[100];
 	 x=(m1*t);
 	for( ;y1<x;y1++,k1++)
 	{

 		sp21[k1]=m[y1];
 	}
 	datas(sp21);
    datas2(sp21);
 	k1=0;
 	memset(sp21,0,sizeof(sp21));
 	}
 }

void sdcard_status()
{
	if(f_mount(&fs, "", 0) != FR_OK)
	{
		datas("sd card is not opened");
	}
  else
	{


		datas2("started");
	  if((f_open(&f,FILE_NAME,FA_WRITE |FA_OPEN_ALWAYS 		)== FR_OK))
	   {
      datas2("sd card test success.....");
	    f_close(&f);
    //datas2("closed ");
     }
		else
			datas2("sd card failed");
	}


}




void USART1_IRQHandler()    //RECIEVING THE INTERNET CONNECTIVITY STATUS FROM USART1
{

if(HAL_UART_Receive(&huart1,datar,1,10)==HAL_OK)
		{

			datat=(char *)datar;
			strcpy(ch,datat);

			if(ch[0]=='E')
			{
				counter=0;}
			else if(ch[0]=='K'||(ch[1]=='K'))
			{
				counter=1;

			}
		}

}



void sd_check_send()              //function for sending checking "FAILED STATUS" on storage data and send to esp8266 and update status to sucess
{

	int l=sd_check();
	datas("AT+PING=\"www.google.com\"\r\n");
	vTaskDelay(2000);
while(l>0&&counter==1)
{

	l=sd_check();

	sd_send(l);
	integer_2_string(l);
	datas("AT+PING=\"www.google.com\"\r\n");
		vTaskDelay(2000);
}


}
void queuer(char *data)
{
	//integer_2_string(stop);
	int i=strlen(data);




int h=size+i;
for(int p=0;size<h;size++,p++)
queue[size]=data[p];
stop++;

}
/*  FOR STIMULATING AND TESTING JSON DATA YOU CAN UNCOMMENT THE BELOW
char *get_json_data(void)
{

    const char *const memberName[] = {"stijo joseph", "arun kumar", "krishnan", "thomas jacob","stijo joseph", "arun kumar", "krishnan", "thomas jacob"
		,"stijo joseph", "arun kumar", "krishnan", "thomas jacob","stijo joseph", "arun kumar", "krishnan", "thomas jacob"
		,"stijo joseph", "arun kumar", "krishnan", "thomas jacob","stijo joseph", "arun kumar", "krishnan", "thomas jacob",
		"stijo joseph", "arun kumar", "krishnan", "thomas jacob","stijo joseph", "arun kumar", "krishnan", "thomas jacob"
		,	"stijo joseph", "arun kumar", "krishnan", "thomas jacob","stijo joseph", "arun kumar", "krishnan", "thomas jacob"};
    const char *const datetime[] = {"2021-01-23 13:00:00", "2021-01-23 13:02:05", "2021-01-23 16:00:00", "2021-01-23 16:10:00"
		,"2021-01-23 13:00:00", "2021-01-23 13:02:05", "2021-01-23 16:00:00", "2021-01-23 16:10:00","2021-01-23 13:00:00", "2021-01-23 13:02:05", "2021-01-23 16:00:00", "2021-01-23 16:10:00"
		,"2021-01-23 13:00:00", "2021-01-23 13:02:05", "2021-01-23 16:00:00", "2021-01-23 16:10:00",
		"2021-01-23 13:00:00", "2021-01-23 13:02:05", "2021-01-23 16:00:00", "2021-01-23 16:10:00",
		"2021-01-23 13:00:00", "2021-01-23 13:02:05", "2021-01-23 16:00:00", "2021-01-23 16:10:00"
		,"2021-01-23 13:00:00", "2021-01-23 13:02:05", "2021-01-23 16:00:00", "2021-01-23 16:10:00"};
    char *string = NULL;
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        goto end;
    }

    cJSON_AddStringToObject(root, "did", "00000001");
    cJSON_AddStringToObject(root, "gid", "00000001");

    cJSON *records = cJSON_CreateArray();
    if (records == NULL)
    {
        goto end;
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        cJSON *record = cJSON_CreateObject();
        if (record == NULL)
        {
            goto end;
        }
   cJSON_AddStringToObject(record, "uid",memberName[i]);
        cJSON_AddStringToObject(record, "atp","1");
         cJSON_AddStringToObject(record, "as", "VALID");
     cJSON_AddStringToObject(record,"qr","ABC");
				cJSON_AddStringToObject(record,"dt",datetime[i]);
        cJSON_AddItemToArray(records, record);
    }
    cJSON_AddItemToObject(root, "rc", records);

    string = cJSON_Print(root);
    if (string == NULL)
    {
        datas("Handle error");
    }

end:
    cJSON_Delete(root);
  //  datas2(string);
    return string;
} */
char  *json_conversion(char h[])
{

 char *string = NULL;
    cJSON *name = NULL;
    cJSON *resolutions = NULL;
    cJSON *resolution = NULL;
    cJSON *width = NULL;
    cJSON *height = NULL;
    size_t index = 0;

    cJSON *monitor = cJSON_CreateObject();


    name = cJSON_CreateString("00000001");

    /* after creation was successful, immediately add it to the monitor,
     * thereby transferring ownership of the pointer to it */
    cJSON_AddItemToObject(monitor, "did", name);
     cJSON_AddStringToObject(monitor, "gid","00000001");
    resolutions = cJSON_CreateArray();

    cJSON_AddItemToObject(monitor, "rc", resolutions);
	 int h1=0;


char person[100];
char times[100];
char dates[100];

	int count=0;
int f1=0;
int c2=0;
int per1=0;
int tim1=0;
int date1=0;
//int y=;
//Serial.print("entered");
while((f1<strlen(h)))                                  //iterate the loop till the end of h[c] ,till the data recieved finishes
{                                                                  // our stm32 msg format $stijojoseph\t19;34;21\t1;5;21\tarun kumar\t12;14;51\t1;5;21\t# likewise,here for eg i have shown only two datas

if(h[f1]=='\t')                                                     //increments count on reaching \t
count++;
if((count==0)&&(h[f1]!='\t'))                                     //till count becomes 1 the character gets added to String person ,till h[f1] meets its first count it will be storing the ID
{
person[per1]=h[f1];
per1++;

}

if((count==1)&&(h[f1]!='\t'))                                       //if count==1 means next data is time ,till next '\t' the character getrs stored on to String time
{
times[tim1]=h[f1];
  tim1++;
  //Serial.print(h[f1]);
}
if((count==2)&&(h[f1]!='\t'))// similary date get stored to String date
{
dates[date1]=h[f1];
   //Serial.print(h[f1]);
date1++;
}
if(count==3)  //if count==3 means a person's id,time,date has been stored to STRING person,timesdates  variables
{
count=0;
//datas(person);
//datas(times);
//===datas(dates);
per1=0;
tim1=0;
date1=0;
char datetime[100];
strcpy(datetime,times);
int count3=0;
int timesize=strlen(times);

int totalsize=strlen(dates)+timesize;
int count4=0;
datetime[timesize++]=' ';
for(;timesize<totalsize;timesize++,count4++)
{


	datetime[timesize]=dates[count4];

}
        resolution = cJSON_CreateObject();


       cJSON_AddItemToArray(resolutions, resolution);
       cJSON_AddStringToObject(resolution, "uid",person);
      cJSON_AddStringToObject(resolution, "atp","1");
      cJSON_AddStringToObject(resolution, "as", "VALID");
      cJSON_AddStringToObject(resolution,"qr","ABC");
      cJSON_AddStringToObject(resolution,"dt",datetime);
memset(person,0,sizeof(person));
memset(times,0,sizeof(times));
memset(dates,0,sizeof(dates));
memset(datetime,0,sizeof(datetime));
datas("\n");
}

f1++;

}





  char *json =cJSON_Print(monitor);

  cJSON_Delete(monitor);
memset(queue,0,sizeof(queue));
//datas2(json);
return json;
}


void sd_send(int h1)     //function for sending the  data from sd card to esp8266 by updating the status on sd card as "success"
{
char buffer[40];
char sender[40];

	int coun=0;

if(h1==-5||stop>JSON_NUM)
{
//datas("#");
//queuer("#");
if(stop!=0)
{queue_send();
//queuer('#');
stop=0;
}
}else
{
 while(coun<3)
 {
	 if(h1==-5||stop>JSON_NUM)
	 {
	 //datas("#");
	 //queuer("#");
	 if(stop!=0)
	 { queue_send();
	// queuer("#");
	 stop=0;
	// queuer("#");
	 }
	 }else
 if(f_open(&f, "storage1.txt", FA_OPEN_ALWAYS |FA_READ|FA_WRITE) == FR_OK)
 { //datas("opened storage \n");
  	 }
 int o=1;
 int h=0;
 int x;
  if((coun==1)&&(h1==1))

{

	//datas2(sender);
	queuer(sender);
  //datas("$");
	//datas(sender);
f_puts(sender,&f);
f_puts("sucess\t\n",&f);


	memset(sender, 0, bufsize(sender));


}
  while(f_gets(buffer, sizeof(buffer), &f))
  {



		if((coun==0)&&(o==h1))
		{
			 x=strlen(buffer)-8;
   h=0;
   for(;h<x;h++)
		sender[h]=buffer[h];


		}
		int s=h1-1;
		if((coun==1)&&(o==s)&&(h1!=1))
			{

//	datas(sender);
	//datas2(sender);

	queuer(sender);
f_puts(sender,&f);
f_puts("sucess\t\n",&f);

	memset(sender, 0, bufsize(sender));

}

		o++;
	 memset(buffer, 0, bufsize(buffer));

	}


	coun++;
	f_close(&f);
}



}


}


int sd_check() //function for checking any data on sd card with "FAILED" status ,return the line number of data with failed status on file "storage1.txt"
{
char buffer[40];
char sender[40]="FAILED";
 char sender1[40];

if(f_open(&c1, "storage1.txt", FA_OPEN_ALWAYS |FA_READ|FA_WRITE) == FR_OK)
 {
  int o=0;
while(f_gets(buffer, sizeof(buffer), &c1))
  {

  int x=strlen(buffer)-8;
  int r=(strlen(buffer)-2);
	int coun=0;
	int	h=0;
   for(;x<r;h++,x++)
		if(sender[h]==buffer[x])
		{

			coun++;

		}
		h=0;


		o++;
		if(coun>5)
		{	 x=strlen(buffer)-8;
		 int p1=0;
   for(;p1<x;h++,p1++)
		sender1[p1]=buffer[h];

memset(sender1, 0, bufsize(sender1));
		//	datas(sender1);
			f_close(&c1);
			return o;

		}



	memset(sender1, 0, bufsize(sender1));
	memset(buffer, 0, bufsize(buffer));
}

	f_close(&c1);


}

f_unlink ((TCHAR const*)"STORAGE1.TXT");
f_close(&c1);
return -5;

}
void wifi_connection_setup()
{
datas("AT+CWMODE=3\r\n");
HAL_Delay(1500);
datas("AT+CWJAP_DEF=");
datas(SSID);
datas(",");
datas(PASSWORD);
datas("\r\n");
HAL_Delay(8000);

}




void sd_to_sram()               //function to copy names from sd card to sram based on sram size (NAME_SIZE)
{
	 int i=0;
if(f_open(&d1,FILE_NAME, FA_OPEN_ALWAYS |FA_READ) == FR_OK)
 { datas2("opened storage ");
 }
while((f_gets(buffer, sizeof(buffer), &d1))&&(i<NAME_SIZE))
  {

	if(i<NAME_SIZE)
	{
	strcpy(sd[i].names,buffer);

		i++;
	}

	}
	f_close(&d1);

datas2("done");




}
int check_sram(char names[20])   //function for data comparison on sram
{

	char nam[NAME_LENGTH]; //NAME LENGHT USER CAN DEFINE IN MAIN.H
	strcpy(nam,names);


	int i=0;
	while(i<NAME_SIZE)
	{
		if( check(sd[i].names,nam)==1)
		{

		return 1;
		}
			i++;
	}

return 0;
}
void integer_2_string(int s)
{
char p[2];
itoa(s,p,10);
datas2(p);



}


int sd_data_check(char buffer4[40])  //function for checking the remaining data on sd card which is not uploaded to sram
{
char buffers[NAME_LENGTH];
	int i=0;
	int g=0;
	int f=strlen(buffer4);
	i=0;
	strcpy(buffers,buffer4);
datas2(buffers);
	char buffer1[40];
		if((f_open(&m,FILE_NAME,FA_READ |FA_OPEN_ALWAYS	)== FR_OK))
	{

		  while((f_gets(buffer1, sizeof(buffer1), &m)))
  {
		//datas2(buffer1);
		if(i>=NAME_SIZE)
		{


	if(check(buffer1,buffers)==1)
	{
	//datas2(buffers);
		memset(buffers,0,strlen(buffers));
		datas2("sdcard success");
		f_close(&m);
	return 1;
	}
			}
	memset(buffer1, 0, sizeof(buffer1));
	i++;
	}

	memset(buffers,0,strlen(buffers));
f_close(&m);
return 0;
	}
	f_close(&m);
}



void datas(char *data)                             //function for sending data to esp8266 (USART1TX) PA9
{

uint16_t p=strlen(data);

HAL_UART_Transmit(&huart1,(uint8_t *)data,p,1000);




}
void datas2(char *data) //function for debuging ,connect usb to ttl to get the printf statements of the program (USART2TX) A2
{

uint16_t p=strlen(data);

HAL_UART_Transmit(&huart2,(uint8_t *)data,p,1000);




}




int bufsize (char *buf)   //fucntion for calculation of datasize for  memset conversion when pointers used
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}
int check(char a[50],char b[50]) //function for comparing two strings return 1 if same else 0
{


	int p=strlen(a);
	int s=strlen(b);
if(p==s)
{
int i=0;
	char j[50],k[50];
for(i=0;i<strlen(a);i++)  //converting to lower case for comparison
{

j[i]=tolower(a[i]);

k[i]=tolower(b[i]);

}
i=0;
while(i<strlen(a))
{

if(j[i]!=k[i])
{

return 0;

}


i++;
}

if(i==strlen(a))
{

	return 1;

}
else
return -1;
}

}
void join(char *m,char x[],char p[],char j[],int b) //function for joining the date or time format to string DD;MM;YY or HH;MM;SS
{
int i=0;
int k=0;
memset(m, 0, bufsize(m));
while(k<strlen(x))
{
m[i]=x[k];
i++;
k++;

}
k=0;
m[i]=';';
i++;
while(k<strlen(p))
{
if(isdigit(p[k]))
{
	m[i]=p[k];
i++;
k++;
}
}

k=0;
m[i]=';';
i++;

while(k<strlen(j))
{
if(isdigit(j[k]))
{m[i]=j[k];
i++;
k++;
}
}
i=0;
k=0;

int z=strlen(m);


}
void currenttime(char *time,int b)
{

	if(HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN)==HAL_OK)
	{

		char p[40];
		char q[40];
		char s[40];
		char r[100];
		uint8_t hr[3];
		hr[0]=sTime.Hours;
		hr[1]=sTime.Minutes;
		hr[2]=sTime.Seconds;
	 itoa(hr[0],p,10);
//		p[2]=';';

	itoa(hr[1],q,10);
  //  q[2]=';';

		itoa(hr[2],s,10);



join(r,p,q,s,b);
//char dater[20];
int h=0;
int o=0;
while(o<strlen(r))
{
if(isdigit(r[o])||r[o]==';')
{
time[h]=r[o];
	h++;
}

o++;

	}

}
}
void currentdate(char *date,int b)   //function for getiing the current time to time pointer, time format DD;MM;yy
{
	if(HAL_RTC_GetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BIN)==HAL_OK)
		{	char a[40];
		char q[50];
		char v[50];
		char u[50];

			uint8_t hr[3];
		hr[0]=DateToUpdate.Date;
		hr[1]=DateToUpdate.Month;
		hr[2]=DateToUpdate.Year;
	 itoa(hr[0],a,10);


	itoa(hr[1],q,10);


		d=u;
			itoa(hr[2],v,10);


join(u,a,q,v,b);

			int h=0;
int o=0;
while(o<strlen(u))
{
if(isdigit(u[o])||u[o]==';')
{date[h]=u[o];
	h++;
}

o++;

	}


		}
}

char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void queue_send()
{
char *sp;
	//char *sp=(char *)malloc(500*sizeof(char));
sp=json_conversion(queue);
	//datas(sp);
wifisender(sp);
	memset(queue,0,sizeof(queue));
	cJSON_free(sp);
	size=0;
	stop=0;
}
void id_time_storage(char d[])  //storing incomming name ,along with the current time and date with FAILED status on sd CARD
{
	char buffer1[30];
strcpy(buffer1,d);
FRESULT res;
UINT numread;
DWORD size;
int e=0;
while((f_open(&g,"storage1.txt",FA_WRITE |FA_OPEN_ALWAYS	)== FR_OK)&&(e<9))
	{
   FRESULT res;
   UINT numread;
   DWORD size;
	 char time1[40];
	 char date1[40];
	 currenttime(time1,0);
   currentdate(date1,0);
	 if(e==2)
	 {strcpy(buffer1,time1);

		}
		if(e==4)
			{
		  strcpy(buffer1,date1);
	    }

		if(e==8)
		 strcpy(buffer1,"\n");
	  if(e==6)
		  strcpy(buffer1,"FAILED");
	   if(e%2==1)
		 strcpy(buffer1,"\t");
   size = (&g)->fsize;

    res = f_lseek(&g,size);
   f_puts(buffer1, &g);
  //datas2(buffer1);

	memset(buffer1, 0, bufsize(buffer1));

	 e++;
		f_close(&g);

 }

 f_close(&g);

}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}
static void MX_RTC_Init(void)
{



		__HAL_RCC_BKP_CLK_ENABLE();


  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = HOUR;
  sTime.Minutes = MIN;
  sTime.Seconds = SECOND;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = MONTH;
  DateToUpdate.Date =DAY;
  DateToUpdate.Year = YEAR;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */
HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR6,(uint32_t )DateToUpdate.Date);
HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR7,(uint32_t )DateToUpdate.Month);
HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR8,(uint32_t )DateToUpdate.Year);

}

void RTCinit()
{

		__HAL_RCC_PWR_CLK_ENABLE();

	HAL_PWR_EnableBkUpAccess();

		uint32_t s;



	 hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

		__HAL_RCC_BKP_CLK_ENABLE();








	s=(uint32_t )HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR5);
	if(s!=TIMERESET)
	{


		HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR5,(uint32_t )TIMERESET);
	datas2("intilized");
MX_RTC_Init();

	}
else
{
	  DateToUpdate.Date =HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR6);
  DateToUpdate.Month = HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR7);
  DateToUpdate.Year = HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR8);

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }




}



//__HAL_RCC_BKP_CLK_DISABLE();
	integer_2_string(s);

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate =115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : SD_CS_Pin */
  GPIO_InitStruct.Pin = SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(DOOR_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(DOOR_PORT,DOOR_OPEN,GPIO_PIN_RESET);

GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(DOOR_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(DOOR_PORT,DOOR_CLOSE,GPIO_PIN_RESET);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
