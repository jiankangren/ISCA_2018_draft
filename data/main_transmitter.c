#include <REG51.H>
#include <stdio.h>
#include "I2C.h"
#include "Delay.h"
#include "LCD.h"
#include "KXTJ9.h"
#include "tmp100.h"
#include "NVRF.h"

#define LCD_display // comment this line to disable LCD display
#define LCD_display_temp // comment this line to disable LCD display

void main()
{   
	int	acc_x,acc_y,acc_z;
	int tmp;
	unsigned char i;	
				
	#ifdef LCD_display
		Ht1621_Init();				// LCD display initialization
	#endif
	#ifdef LCD_display_temp
		 LCD_show(0xAAAA);
		 DelayMS(1000);
  	#endif
	
	while(1)
	{	
		for(i=0;i<PNum;i++)
		{
		 payload[i]=0xAAAA;
		}
		
		// initialization of NVRF module
	  	nvrf_config();        // NVRF module
		
		#ifdef LCD_display_temp
			LCD_show(0xAABB);
			DelayMS(1000);
    		#endif
		
		
		// initialization of Temp Sensor
		tmpInit();           //initialization of tmp100 		
		tmpSetRes(3);        //Set resolution of tmp100: 12bits
		
		#ifdef LCD_display_temp
		 	LCD_show(0xCCCC);
		 	DelayMS(1000);
   	 	#endif
		
		
		// data read from Temp Sensor
		tmpGetTmpCont(&tmp);
		payload[0]=(uchar)((tmp&0xFF00)>>8);
		payload[1]=(uchar)(tmp&0x00FF);
		
		#ifdef LCD_display_temp
		 	LCD_show(tmp);
		 	DelayMS(2000);
    		#endif
		
		
		// initialization of ACM Sensor				
		KXTJ9_Init();				      // ACM Sensor			
		KXTJ9_Set_Resolution(1);	// high res
		KXTJ9_Set_Range(2);			  // -2g -- +2g	
		
		#ifdef LCD_display_temp
			LCD_show(0xEEEE);
			DelayMS(1000);
		#endif
		
		// data read from ACM Sensor
		for(i=1;i<=ANum;i++)
		{
				KXTJ9_Get_Acceleration(&acc_x, &acc_y, &acc_z);//wait(1,100);
				payload[6*i-4]=(uchar)((acc_x&0xFF00)>>8);
				payload[6*i-3]=(uchar)(acc_x&0x00FF);
				payload[6*i-2]=(uchar)((acc_y&0xFF00)>>8);
				payload[6*i-1]=(uchar)(acc_y&0x00FF);
				payload[6*i]=(uchar)((acc_z&0xFF00)>>8);
				payload[6*i+1]=(uchar)(acc_z&0x00FF);
		}
		
		#ifdef LCD_display_temp
			LCD_show(0xFFFF);
			DelayMS(1000);
		#endif
		
		// data transmission via RF module			
		//P3=0xf7;
		drill_buffer();
		DelayMS(100);
		nvrf_start(PNum,0);
		DelayMS(200);
		
    		#ifdef LCD_display
			LCD_show(0x1111);
			DelayMS(1000);
			LCD_show(0xd111);
			DelayMS(1000);
			LCD_show(0xdd11);
			DelayMS(1000);
			LCD_show(0xddd1);
			DelayMS(1000);
			LCD_show(0xdddd);
			DelayMS(1000);
		#endif
	}	
}	