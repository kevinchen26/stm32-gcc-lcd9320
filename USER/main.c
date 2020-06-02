#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
//#include "lcd.h"
#include <stdio.h>
#include "ili9320.h"

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

//ALIENTEK战舰STM32开发板实验1
//跑马灯实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
 int main(void)
 {		
	char loop[] = "Loop-00000\r\n";
    int16_t i = 0;

	GPIO_InitTypeDef  GPIO_InitStructure;
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
			 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;				 //LED0-->PB.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_2 | GPIO_Pin_3);			 //PB.5 输出高	

  	/* Output a message on Hyperterminal using printf function */
	printf("Printf test.");
  	USART1_printf("USART Printf Example: retarget the C library printf function to the USART\n\r");
	
	//LCD_Init();
	//POINT_COLOR = RED;
	//LCD_ShowString(30, 40, 200, 24, 24, "LCD Display.");

	ili9320_Initializtion();
	ili9320_Clear(Blue);

	ili9320_ShowString(20, 40, 320, 240, 24, "ili9320 LCD Display.");

	while(1)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		delay_ms(500);
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		delay_ms(500);

		i++;
		loop[5] = '0' + i / 10000;
		loop[6] = '0' + (i % 10000) / 1000;
		loop[7] = '0' + (i % 1000) / 100;
		loop[8] = '0' + (i % 100) / 10;
		loop[9] = '0' + i % 10;
		USART1_printf(loop);
		ili9320_ShowString(20, 80, 320, 240, 24, loop);

	}
 }

PUTCHAR_PROTOTYPE
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC)  == RESET)
	{
		/* code */
	}
	return ch;	
}