#include "main.h"
#include "bmp.h"
/************************************************  

************************************************/   

#define PHVAL_MAX  1000 //PH值报警
/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;

extern gizwitsProtocol_t gizwitsProtocol;
extern u8 display_status;
extern u8 Water_Temp_status;
//WIFI连接状态
//wifi_sta 0: 断开
//         1: 已连接
u8 wifi_sta=0;

//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
    usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}
_bool OledDisplayTimeFlag = FALSE;
_bool OledDisplayParamFlag = FALSE;

_bool Task500MsEventFlag = TRUE;
 int main(void)
 {	 
	u8 wifi_con=0;//记录wifi连接状态 1:连接 0:断开
	u8 temperature;  	    
	u8 humidity;    
	delay_init();	    	 //延时函数初始化	
	//KEY_Init();	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	Periph_Init();		  		//初始化与LED连接的硬件接口 
	 Key_Io_Init();
	BUZEER_DISABLE;
	uart_init(115200);
	Gizwits_Init();         //协议初始化
	OLED_Init();			//初始化OLED  
	DHT11_Init();
	Adc_Init();
	OLED_Clear(); 
	OLED_DrawBMP(0,0,128,1,BMP1);
	OLED_ShowCHinese(18 * 0, 2, 0);
	OLED_ShowCHinese(18 * 1, 2, 1);
	OLED_ShowCHinese(18 * 2, 2, 2);
	OLED_ShowCHinese(18 * 3, 2, 3);
	OLED_ShowCHinese(18 * 4, 2, 4);
	OLED_ShowCHinese(18 * 5, 2, 5);
	OLED_ShowCHinese(18 * 6, 2, 6);
	OLED_ShowCHinese(18 * 0, 4, 7);
	OLED_ShowCHinese(18 * 1, 4, 8);
	OLED_ShowCHinese(18 * 2, 4, 9);
	OLED_ShowCHinese(18 * 3, 4, 10);
	OLED_ShowCHinese(18 * 4, 4, 11);
	OLED_ShowCHinese(18 * 5, 4, 12);
	OLED_ShowCHinese(18 * 6, 4, 13);
	OLED_ShowChar(18 * 4, 6, '-', 16);
	OLED_ShowCHinese(18 * 5, 6, 14);
	OLED_ShowCHinese(18 * 6, 6, 15);
	FuncReg(&OledDisplayTimeFlag, 5000, TRUE);
	while(1)
	{
		if(KEY_3 != 0)
		{
			switch(display_status)
			{
				case 0:
				if(OledDisplayTimeFlag == TRUE)
				{	
					OledDisplayTimeFlag = FALSE;
					if(gizwitsProtocol.TimeNTP.year %100 /10  == 7)//检测是否接收到网络时间
						FuncReg(&OledDisplayTimeFlag, 500, TRUE);
					else
					{
						OLED_Clear_static(1, 2, 7);
						FuncReg(&OledDisplayTimeFlag, 10000, TRUE);
						OLED_Show_32_32(16*2-8,2,gizwitsProtocol.TimeNTP.hour / 10 + '0');
						OLED_Show_32_32(16*3-8,2,gizwitsProtocol.TimeNTP.hour % 10 + '0');
						OLED_Show_32_32(16*4-8,2,':');
						OLED_Show_32_32(16*5-8,2,gizwitsProtocol.TimeNTP.minute / 10 + '0');
						OLED_Show_32_32(16*6-8,2,gizwitsProtocol.TimeNTP.minute % 10 + '0');
						OLED_ShowChar(32,	  6, gizwitsProtocol.TimeNTP.year %100 /10 + '0',16);
						OLED_ShowChar(32+8*1, 6, gizwitsProtocol.TimeNTP.year % 10 + '0',16);
						OLED_ShowChar(32+8*2, 6, '-',16);
						OLED_ShowChar(32+8*3, 6, gizwitsProtocol.TimeNTP.month / 10 + '0',16);
						OLED_ShowChar(32+8*4, 6, gizwitsProtocol.TimeNTP.month % 10 + '0',16);
						OLED_ShowChar(32+8*5, 6, '-',16);
						OLED_ShowChar(32+8*6, 6, gizwitsProtocol.TimeNTP.day / 10 + '0',16);
						OLED_ShowChar(32+8*7, 6, gizwitsProtocol.TimeNTP.day % 10 + '0',16);
					}
				}
				OledDisplayParamFlag = TRUE;
				break;
				case 1:
				{
					if(OledDisplayParamFlag == TRUE)
					{
						OledDisplayParamFlag = FALSE;
						OLED_Clear_static(2, 2, 7);
						FuncReg(&OledDisplayParamFlag, 200, TRUE);
						OLED_ShowCHinese(18 * 0, 2, 16);
						OLED_ShowCHinese(18 * 1, 2, 17);
						OLED_ShowChar(18 * 2, 2, ':',16);
						OLED_ShowCHinese(18 * 6, 2, 21);
						OLED_ShowNum(18 * 3, 2, temperature, 3, 16);
						
						OLED_ShowCHinese(18 * 0, 4, 18);
						OLED_ShowCHinese(18 * 1, 4, 19);
						OLED_ShowChar(18 * 2, 4, ':',16);
						OLED_ShowChar(18 * 6, 4, '%',16);
						OLED_ShowNum(18 * 3, 4, humidity, 3, 16);
						
						OLED_ShowChar(18 * 0, 6, 'P',16);
						OLED_ShowChar(18 * 1, 6, 'H',16);
						OLED_ShowChar(18 * 2, 6, ':',16);
						OLED_ShowNum(18 * 3, 6, ADC_Val[0], 4, 16);
						
					}
					break;
				}
				case 2:
				{
					OledDisplayTimeFlag = TRUE;
					OLED_Clear_static(3, 2, 7);
					OLED_ShowCHinese(18 * 0, 2, 22);
					OLED_ShowCHinese(18 * 1, 2, 23);
					OLED_ShowCHinese(18 * 2, 2, 24);
					OLED_ShowCHinese(18 * 3, 2, 25);
					OLED_ShowChar(18 * 4, 2, ':',16);
					switch(Water_Temp_status)
					{
						case 0:OLED_ShowNum(18 * 5, 4, 25, 2, 16);break;
						case 1:OLED_ShowNum(18 * 5, 4, 50, 2, 16);break;
						case 2:OLED_ShowNum(18 * 5, 4, 75, 2, 16);break;
						default :break;
					}
					OLED_ShowCHinese(18 * 6, 4, 21);
					
				}	
					break;			
			}
		}
		else
		{
			OLED_Clear_static(4, 2, 7);
			OledDisplayTimeFlag = TRUE;
			OledDisplayParamFlag = TRUE;
			OLED_ShowCHinese(18 * 0, 2, 26);
			OLED_ShowCHinese(18 * 1, 2, 27);
			OLED_ShowCHinese(18 * 2, 2, 28);
			OLED_ShowCHinese(18 * 3, 2, 29);
			OLED_ShowCHinese(18 * 1, 4, 30);
			OLED_ShowCHinese(18 * 2, 4, 31);
			OLED_ShowCHinese(18 * 3, 4, 32);
			OLED_ShowCHinese(18 * 4, 4, 33);
			OLED_ShowCHinese(18 * 5, 4, 34);
			OLED_ShowChar(18 * 6, 4, '.',16);
			OLED_ShowChar(18 * 7 - 8, 4, '.',16);
		}
		
		if(Task500MsEventFlag == TRUE)
		{
			Task500MsEventFlag = FALSE;
			FuncReg(&Task500MsEventFlag, 500, TRUE);
			currentDataPoint.valueHUM = humidity;
			currentDataPoint.valuePH = ADC_Val[0];
			if(ADC_Val[0] > PHVAL_MAX){
				BUZEER_ENABLE;
				currentDataPoint.valuePH_ERR = 1;
			}
			else{
				BUZEER_DISABLE;
				currentDataPoint.valuePH_ERR = 0;
			}
			currentDataPoint.valueTEMP = temperature;
			DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值
			currentDataPoint.valuerssi = gizwitsProtocol.wifiStatusData.rssi;
			gizwitsGetNTP();
		}
							    

	
		if(wifi_con!=wifi_sta)
		 {
			 wifi_con=wifi_sta;
			 wifi_con?printf("connect"):printf("close");
		 }
		

		gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理

		
		
		
		
		
//		key = KEY_Scan(0);
//		if(key==KEY1_PRES)//KEY1按键
//		{
//			printf("WIFI进入AirLink连接模式\r\n");
//			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
//		}			
//		if(key == KEY1_PRES)//KEY_UP按键
//		{  
//			printf("WIFI复位，请重新配置连接\r\n");
//			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位 
//			wifi_sta=0;//标志wifi已断开
//		}

	}
}
