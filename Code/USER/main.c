#include "main.h"
#include "bmp.h"
/************************************************  

************************************************/   

#define PHVAL_MAX  1000 //PHֵ����
/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;

extern gizwitsProtocol_t gizwitsProtocol;
//WIFI����״̬
//wifi_sta 0: �Ͽ�
//         1: ������
u8 wifi_sta=0;

//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
    usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}
_bool OledDisplayTimeFlag = FALSE;
_bool OledDisplayParamFlag = FALSE;

_bool Task500MsEventFlag = TRUE;
 int main(void)
 {	 
	u8 wifi_con=0;//��¼wifi����״̬ 1:���� 0:�Ͽ�
	u8 temperature;  	    
	u8 humidity;    
	u8 key_val = 0;
	delay_init();	    	 //��ʱ������ʼ��	
	//KEY_Init();	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Periph_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 
	BUZEER_DISABLE;
	uart_init(115200);
	Gizwits_Init();         //Э���ʼ��
	OLED_Init();			//��ʼ��OLED  
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
//	OLED_ShowCHinese(18 * 5, 0, 5);
//	OLED_ShowCHinese(18 * 6, 0, 6); 
//	OLED_ShowCHinese(18 * 7, 0, 7);
//	gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ 
//	wifi_sta=0;//��־wifi�ѶϿ�
	while(1)
	{
		switch(key_val)
		{
			case 0:
			if(OledDisplayTimeFlag == TRUE)
			{	
				OledDisplayTimeFlag = FALSE;
				if(gizwitsProtocol.TimeNTP.year %100 /10  == 7)//����Ƿ���յ�����ʱ��
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
				OledDisplayTimeFlag = TRUE;
				break;
			}
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
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ
			currentDataPoint.valuerssi = gizwitsProtocol.wifiStatusData.rssi;
			gizwitsGetNTP();
		}
							    

	
		if(wifi_con!=wifi_sta)
		 {
			 wifi_con=wifi_sta;
			 wifi_con?printf("connect"):printf("close");
		 }
		

		gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��

		
		
		
		
		
//		key = KEY_Scan(0);
//		if(key==KEY1_PRES)//KEY1����
//		{
//			printf("WIFI����AirLink����ģʽ\r\n");
//			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
//		}			
//		if(key == KEY1_PRES)//KEY_UP����
//		{  
//			printf("WIFI��λ����������������\r\n");
//			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ 
//			wifi_sta=0;//��־wifi�ѶϿ�
//		}

	}
}
