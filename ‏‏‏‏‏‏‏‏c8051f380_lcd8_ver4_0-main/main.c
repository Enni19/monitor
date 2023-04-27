#include "compiler_defs.h"
#include "C8051F380_defs.h"
#include "initsys.h"
#include "argb2.h"
#include <math.h> 
#define READ_FROM_TEMP 19 
#define READ_FROM_AMPLIFIER  20 

float goertzel_mag(int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE);
int ADC_IN(int num);
int IsAlarmDetected(int numofinstans,int mag);
int IsAlarmDetected2();
int IsAlarmDetected_trial();

void Init_Device(void);
void mp3();
void Start_Working(void);
void MenuScreen();
void Font_test(void);
void Move(void);
void temperature(void);
void Crying(void);

#define NUM_OF_FREQ_TO_CHECK 4 
xdata int  NumInstans[NUM_OF_FREQ_TO_CHECK]; 
struct WaveProperty  
{
	 int freq; 
	 int Th_mag;
	 int Th_NumInstans;
};

//xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{700,100,20},{900,100,20}};
//xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{700,70,8},{800,100,15},{900,60,9}};
xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{700,70,8},{800,100,15},{1000,60,9},{900,60,9}};// frequencies
//xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{353,20,30},{905,20,20}};                 
//xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{800,100,20}};
//xdata struct WaveProperty wave_property_arr[NUM_OF_FREQ_TO_CHECK]={{800,150,20}};
xdata char my_string[20];          
xdata U16 data_in[210];
xdata num=0;



void PUTCHAR1(char tav)
{
  SBUF1=tav;
	while((SCON1&2)==0); //(TI1==0)
	SCON1=SCON1&(~2);    //TI1 = 0;  
}

void PUTCHAR0(char tav)
{
	SBUF0=tav;
	while(TI0==0);
	TI0=0;
}


extern void Init_Device(void);

int i;
code char playtrack1[8]={0x7E,0xFF,0x06,0x0F,0x00,0x01,0x02,0xEF}; 

char playtrack[8];
void play_song(char dir,char song_num)
{
	playtrack[5]=dir;
	playtrack[6]=song_num;
	for (i=0;i<8;i++)
	PUTCHAR1 (playtrack[i]);
}

void main(void)
{
	S16 x=0, y=0,  ButtonNum;
	Init_Device();
	initSYS();

	for (i=0;i<8;i++)
	playtrack[i]=playtrack1[i];	
	
	
	
	
	
	MenuScreen();
	

	while(1) {
		if(!T_IRQ) {
			delay_ms(10);
			x = ReadTouchX();
			y = ReadTouchY();
			LCD_setCursor (25, 100);
			ButtonNum= ButtonTouch(x, y);
			printf("x=%d y=%d  b=%d   ", x, y,ButtonNum);

			if(ButtonNum==0){  
				Start_Working();
				MenuScreen();
			}
			else if(ButtonNum==1){ 
				mp3();
				MenuScreen();
			}
			else if(ButtonNum==2){
				Move();
				MenuScreen();				
			}
			else if(ButtonNum==3) {
				temperature();
				MenuScreen();
			}
			else if(ButtonNum==4) {
				Crying();
				MenuScreen();
			}
		}	
	}
}

void MenuScreen() {
	LCD_fillScreen(BLACK);
	LCD_print2C(0, 10,"BABY MONITOR" , 4,RED, BLACK);   //hebrew		
	LCD_print2C (0 ,50 ,"YUVAL & ENNI",3, WHITE, BLACK);
	LCD_clearButton();	
	LCD_drawButton(0,10,  150,60,60, 10, CYAN,BLACK,"RUN!",2);  
	LCD_drawButton(1,80, 150,60,60, 10, BLUE,YELLOW,"MP3",2);
	LCD_drawButton(2,150, 150,60,60, 10, GREEN,BLACK,"Move",2);
	LCD_drawButton(3,217, 150,66,60, 10, YELLOW,BLACK,"Temperature",1);
	LCD_drawButton(4,290, 150,60,60, 10, PINK,BLACK,"Crying",2);
	LCD_setText2Color(WHITE,BLACK);
}




void Start_Working(void) {                      //  Start Working
	while(1){
		temperature();
		//Move();
		//Crying();
		//LCD_println("ABCDE");
	}
}



void Font_test(void) {
	int x=1234;
	LCD_fillScreen(BLACK);
	LCD_print2C(20,30,"abcd",1,WHITE,BLACK);
	LCD_print2C(80,30,"xyz",2,WHITE,BLACK);
	LCD_printC(20,60,"3.95' ILI9488 320X480",3,YELLOW);
	LCD_printC(20,120,"Test 2017-11-23",3,GREEN);
	LCD_println("");
	LCD_setTextSize(2);
	LCD_setText1Color(WHITE);
	LCD_println("ABCDabcd1234");
	LCD_println("~!@#$%^&*()_+{}:<>?/|-+.");
	LCD_setTextSize(2);
	LCD_setText1Color(YELLOW);
	LCD_printHebln("אבגדהוזחטיכלמנסעפצקרשת");
	LCD_println("ABCDE");
	printf("x=%d  \n",x);
	printf("abcdABCD1234",x);
	while(T_IRQ);
}



//printf("Enter the maximum time in seconds: ");  /??



sbit senMove=P1^0;                         //   MOVE  CODE

void Move(void) {
	int cnt=0, numSec=0;
	numSec=15000; // ?
	
while(1)
{
	cnt++;
	delay_ms(1000);
	if ((senMove=='0') && (cnt==15)){
	//if (cnt==15){
		break;
	}
	if((senMove=='1') && (cnt==15)){
		break;
	}
}
  if (senMove=='1')
	{
		printf("\ndetect the movement! \n");
		SetTarget(1);		
		printf("**0528606455");    // Yuval
	  delay_ms(1000);
    printf("##detect the movement !");
	  delay_ms(1000);
	  printf("$$");
		delay_ms(1000);
		printf("&&");
		delay_ms(1000);
		printf("**0546224328");  // Enni
		delay_ms(1000);
		printf("##detect the movement !");
		delay_ms(1000);
		printf("$$");
		delay_ms(1000);
		printf("&&");
		delay_ms(1000);
		SetTarget(0);                       
		MenuScreen();
		cnt=0;
			}
	Crying();
		}
	//while(T_IRQ);
   
	






// P2.0 num=0,P2.1 num=1,P2.2 num=2,P2.3 num=3,P2.5 num=4,P2.6 num=5,P3.0 num=6,P3.1 num=7,P3.4 num=8,P3.5 num=9,           
// P3.7 num=10,P4.0 num=11,P4.3 num=12,P4.4 num=13,P4.5 num=14,P4.6 num=15
// P0.3 num=17, P0.4 num=18,P1.1 num=19, P1.2 num=20, P1.0 num=21, P1.3 num=22, P1.6 num=23, P1.7 num=24,
//  P2.4 num=25, P2.7 num=26, P3.2 num=27, P3.3 num=28, P3.6 num=29, P4.1 num=32, P4.2 num=33, P4.7 num=34
// Temp Sensor num=30, VDD   num=31

int ADC_IN(int num)
{
	AMX0N     = 0x1F;// negtive input= GND
	AMX0P=num;
	delay_us(1);
	AD0BUSY=1; // start conversion 
	while(!AD0INT); // wait end convesion 
	AD0INT = 0;   // clear ADC0 conversion complete flag 
	return (ADC0);
}


		float CalcTemp(int digVal)
	{
		float temp, analog_val;
		//analog_val=((float)digVal*100);
		// 2->  analog_val=((float)digVal*3.3/1024)*0.01;
		analog_val=((float)digVal/1024)*3.3;
		// 2->  temp=analog_val;
		temp=analog_val/0.01;
		//temp=analog_val;
		return temp;
	}

void temperature(void) {                              //  TEMPERATURE  CODE
  int val;
	while(1)
	{
		float temp;
		val=ADC_IN(READ_FROM_TEMP);
		temp=CalcTemp(val);
		LCD_fillScreen(RED);
		LCD_setCursor (10, 0);
		printf("the temperature in the room is:\n %f \n", temp);
		if (temp>29){
			printf("the temperature is too high !");
		  SetTarget(1);		// before sending to BT switch printf to uart
		 //printf("**0522547704");
			printf("**0546224328");     // Enni
			delay_ms(1000);
			printf("##the temperature is too high !");
			delay_ms(1000);
			printf("$$");
			delay_ms(1000);
			printf("&&");
			delay_ms(1000);
			printf("**0528606455");    // Yuval
			delay_ms(1000);
			printf("##the temperature is too high !");
			delay_ms(1000);
			printf("$$");
			delay_ms(1000);
			printf("&&");
			delay_ms(1000);
		  SetTarget(0);
      MenuScreen();
		}
		if (temp<25){
			printf("the temperature is too low !");
		  SetTarget(1);	
			printf("**0546224328");          // Enni
			delay_ms(1000);
			printf("##the temperature is too low !");
			delay_ms(1000);
			printf("$$");
			delay_ms(1000);
			printf("&&");
			delay_ms(1000);    
			printf("**0528606455");      // Yuval
			delay_ms(1000);
			printf("##the temperature is too low !");
			delay_ms(1000);
			printf("$$");
			delay_ms(1000);
			printf("&&");
			delay_ms(1000);        
		  SetTarget(0);
      MenuScreen();
		}
		Move();
       //MenuScreen();	       
	  //while(T_IRQ);
   }
}




int IsAlarmDetected2()
{
	int mag_temp,detect;
	int j,i;
	int k=0;
	num=0;
	LCD_fillScreen(BLACK);
	for(j=0;j<NUM_OF_FREQ_TO_CHECK;j++)
	{
		NumInstans[j] = 0;	
	}
  for(j=0;j<600/NUM_OF_FREQ_TO_CHECK;j++)
	{		
	  for(i=0;i<205;i++)   
		 { 
				data_in[i]=2*ADC_IN(20);	//P1.2 num=20
				delay_us(112);//dt sample
			  
	   }	

    for(i=0;i<NUM_OF_FREQ_TO_CHECK;i++)
		 {
			 mag_temp=(int)(goertzel_mag(205,wave_property_arr[i].freq,8000));
			 k++;	
	    //sprintf(my_string,"m=%3d f=%3d i=%d",mag_temp,wave_property_arr[i].freq,NumInstans[i]);
if ((k%20==0) || (k%20==1))
{
       
			LCD_setCursor (10, 100);
			if (i==0) 
			{  
				LCD_setCursor (10, 0);
				printf("m=%3d f=%3d i=%d",mag_temp,wave_property_arr[i].freq,NumInstans[i]);
			}

			if (i==1) 
			{  
				LCD_setCursor (10,100);
				printf("m=%3d f=%3d i=%d",mag_temp,wave_property_arr[i].freq,NumInstans[i]);
			}

			if (i==2) 
			{  
				LCD_setCursor (10, 200);
				printf("m=%3d f=%3d i=%d",mag_temp,wave_property_arr[i].freq,NumInstans[i]);
			}
			
			if (i==3) 
			{  
				LCD_setCursor (10, 200);
				printf("m=%3d f=%3d i=%d",mag_temp,wave_property_arr[i].freq,NumInstans[i]);
			}
		
} 			  
			 
		//	 if (i==1) LCD_print2C(20,100,my_string,2,RED, BLACK);
		//	 if (i==2) LCD_print2C(20,200,my_string,2,RED, BLACK);
			 		 
//		   if (i==0) lcd_string_line(1,0,my_string);
//			 if (i==1) lcd_string_line(2,0,my_string);
//			 if (i==2) lcd_string_line(1,0,my_string);
//       wait_ms(50); 			 
			 if (mag_temp > wave_property_arr[i].Th_mag)
			 {
				 NumInstans[i]++;
			 }
		 }
 		 
	 }
	 
	 detect=1;
	 for(i=0;i<NUM_OF_FREQ_TO_CHECK;i++)
	 {
		 if (NumInstans[i] < wave_property_arr[i].Th_NumInstans)
		 {
        detect = 0 ;
        break;
		 }			 
	 }
	 return(detect);
}





float goertzel_mag(int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE)
{

	int     k,i;
	float analog;
	float   floatnumSamples;
	float   omega,sine,cosine,coeff,q0,q1,q2,magnitude,real,imag;
	float   scalingFactor = numSamples / 2.0;

	floatnumSamples = (float) numSamples;
	k = (int) (0.5 + ((floatnumSamples * TARGET_FREQUENCY) / SAMPLING_RATE));
	omega = (2.0 * 3.14 * k) / floatnumSamples;
	sine = sin(omega);
	cosine = cos(omega);
	coeff = 2.0 * cosine;
	q0=0;
	q1=0;
	q2=0;

	for(i=0; i<numSamples; i++)
	{
		analog=data_in[i];
		q0 = coeff * q1 - q2 + analog;
		q2 = q1;
		q1 = q0;
	}

	// calculate the real and imaginary results
	// scaling appropriately
	real = (q1 - q2 * cosine) / scalingFactor;
	imag = (q2 * sine) / scalingFactor;

	magnitude = sqrt(real*real + imag*imag);

	return magnitude;
}




void Crying(void) {                              	//   Crying  CODE
		int status;
   int cnt=0, numSec=15000;	  
	LCD_fillScreen(RED);
	delay_ms(100);
	//LCD_print2C(0 ,50 ,"voice",3, WHITE, BLACK);
	while(1)
	{
		status = IsAlarmDetected2();
		cnt++;
		delay_ms(1000);
		if ((status==0) && (cnt==15))
		//if (cnt==15)
			break;
		if (status==1)
			break;
	}
    if (status==1)
		{			
			printf("\nThe baby is crying !\n");  //print the message
			SetTarget(1);	
			printf("**0546224328");  //send SMS to Enni
			delay_ms(1000);
			printf("##The baby is crying !");
			delay_ms(1000);
			printf("$$");
			delay_ms(1000);
			printf("&&");
			delay_ms(1000);
			printf("**0528606455");  //send SMS to Yuval
			delay_ms(1000);
			printf("##The baby is crying !");
			delay_ms(1000);
			printf("$$");
			delay_ms(1000);
			printf("&&");
			delay_ms(1000);
			SetTarget(0);
			MenuScreen();
			play_song(1,1);	   //start playing the lullaby
			LCD_setCursor (10,140);
			delay_ms(360000); // 6 minutes
			play_song(1,2);	
			LCD_setCursor (10,140);
			delay_ms(360000);
			play_song(1,3);	
			LCD_setCursor (10,140);
			delay_ms(360000);
			play_song(1,4);	
			LCD_setCursor (10,140);
			delay_ms(360000);
			play_song(1,5);
			LCD_setCursor (10,140);
			delay_ms(360000);
		}
		cnt=0;	
}





//int j=1;
void mp3()                          //   MP3   CODE  
{
	LCD_fillScreen(BLACK);
	LCD_setCursor (10,100);
	printf("press touch to play\n");
	while(T_IRQ);
	
	play_song(1,1);	
	LCD_setCursor (10,140);
	delay_ms(360000);  // 6 minutes
	printf("press to start playing the lullaby \n");
	play_song(1,2);	
  LCD_setCursor (10,140);
	delay_ms(360000);
	printf("press to start playing the lullaby \n");
	play_song(1,3);	
	LCD_setCursor (10,140);
	delay_ms(360000);
	printf("press to start playing the lullaby \n");
	play_song(1,4);	
	LCD_setCursor (10,140);
	delay_ms(360000);
	printf("press to start playing the lullaby \n");
	play_song(1,5);
	LCD_setCursor (10,140);
	delay_ms(360000);
	printf("press to start playing the lullaby \n");
	
	while(T_IRQ);
	MenuScreen();
}


 
