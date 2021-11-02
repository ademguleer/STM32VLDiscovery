#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

void GPIO_Config(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//leds
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//button
	
	
	//led
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	//button
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_6;
	GPIO_Init(GPIOC,&GPIOInitStructure);
}

void EXTI_Config(){
	EXTI_InitTypeDef	EXTIInitStructure;
	NVIC_InitTypeDef	NVICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
	
	//EXTERNAL INTERRUPT
	EXTIInitStructure.EXTI_Line = GPIO_PinSource2| GPIO_PinSource6;
	EXTIInitStructure.EXTI_LineCmd = ENABLE;
	EXTIInitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTIInitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	
	EXTI_Init(&EXTIInitStructure);

	//NVIC INTERRUPT PART1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVICInitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVICInitStructure.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_Init(&NVICInitStructure);
	
	//NVIC INTERRUPT PART2
	NVICInitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVICInitStructure.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_Init(&NVICInitStructure);
}
void delay(uint32_t time){
	while(time--);

}
void EXTI2_IRQHandler(){
	
	if(EXTI_GetITStatus(EXTI_Line2)!= RESET){
		GPIO_SetBits(GPIOB,GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2);
		delay(3600000);
	
	}
	EXTI_ClearITPendingBit(EXTI_Line2);

}

void EXTI9_5_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line6)!= RESET){
		GPIO_SetBits(GPIOB,GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2);
		delay(3600000);
	
	}
	EXTI_ClearITPendingBit(EXTI_Line6);

}	
	
int main(){
	GPIO_Config();
	EXTI_Config();
	
	while(1){
		GPIO_SetBits(GPIOB,GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2);
		delay(3600000);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2);
		delay(3600000);
	
	}

}