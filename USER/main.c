#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
//#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include "ili9320.h"

// #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)		//KEIL C

int _write(int fd, char *ptr, int len)
{
    int i = 0;

    /*
     * write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
    * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2)
    {
        return -1;
    }

    while (*ptr && (i < len))
    {
        //usart_send_blocking(USART1, *ptr);
		USART_SendData(USART1, *ptr);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC)  == RESET)
		{
			/* code */
		}
        // if (*ptr == '\n')
        // {
        //     usart_send_blocking(USART1, '\r');
        // }
        i++;
        ptr++;
    }
    return i;
}
// https://blog.csdn.net/zhengyangliu123/java/article/details/54966402

//ALIENTEK战舰STM32开发板实验1
//跑马灯实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
 int main(void)
 {		
	char loop[] = "000.00 \0";
    uint16_t i = 65500;
	uint16_t sec = 0, nCount = 10;
	__IO  u32 TimingDelay; 

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

	ili9320_ShowString(10, 10, 320, 240, 24, "ili9320 LCD DISPLAY 16x24.");
	ili9320_ShowString(10, 80, 320, 240, 8, "ili9320 LCD DISPLAY 8 x 16.");
	ili9320_ShowString(20, 120, 320, 240, 24, "Loop-");
	while(1)
	{
		//delay_ms(10);
		nCount = 10;
		//sec = i / 100;
		if (sec == i / 100)
		{
			//GPIO_SetBits(GPIOA, GPIO_Pin_2);
			//GPIO_ResetBits(GPIOA, GPIO_Pin_3);
			GPIOA->BSRR = GPIO_Pin_2;
			GPIOA->BRR = GPIO_Pin_3;
		}
		else
		{
			sec = i / 100;
			//GPIO_SetBits(GPIOA,GPIO_Pin_3);
			//GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIOA->BSRR = GPIO_Pin_3;
			GPIOA->BRR = GPIO_Pin_2;

			/*methord1*/
			printf("Enter the delay(ms) constant for blink : ");
			printf("printf seconde line.");
			fflush(stdout);
			/*methord2*/
			printf("Error: expected a delay > 0\r\n");

			printf("Loop-%d\r\n", i);
		}

		i++;
		loop[0] = '0' + (i / 10000);
		loop[1] = '0' + ((i % 10000) / 1000);
		loop[2] = '0' + ((i % 1000) / 100);
		loop[4] = '0' + ((i % 100) / 10);
		loop[5] = '0' + (i % 10);
		//USART1_printf(loop);
		//USART1_printf("\r\n");

		if (i > 10000)
		{
			ili9320_ShowString(100, 120, 320, 240, 8, loop);
		}
		else if (i > 1000)
		{
			ili9320_ShowString(108, 120, 320, 240, 8, (loop + 1));
		}
		else
		{
			ili9320_ShowString(116, 120, 320, 240, 8, (loop + 2));
		}
	}
 }

// PUTCHAR_PROTOTYPE
// {
// 	USART_SendData(USART1, (uint8_t) ch);
// 	while (USART_GetFlagStatus(USART1, USART_FLAG_TC)  == RESET)
// 	{
// 		/* code */
// 	}
// 	return ch;	
// }