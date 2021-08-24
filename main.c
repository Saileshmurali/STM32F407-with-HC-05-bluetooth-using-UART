#include "stm32f4xx.h"
void start_timer(void) {
  RCC->APB1ENR |= (1UL <<0);  	//Enable TIM2 clock
	TIM2->DIER|=(0x0041UL);//Enable the interrupt
	TIM2->CR1 |= (0x0084UL);    		//Set the mode to Count up
	TIM2->PSC = 8000-1;					//Set the Pre-scalar
	TIM2->ARR = 1000; 						//Set period (Auto reload) to 1000
	TIM2->SR &= ~(0x0001UL);				//Clear Update interrupt flag
	TIM2->CR1 |= 1UL;//Start the timer
	TIM2->EGR|=1UL;//Reinitialise counter with auto reload value
}
void gpio_config(void)
{
	RCC->AHB1ENR |=(1UL<<3);//Enable clock for port D
	GPIOD->MODER &= ~(0xFFFUL<<12*2);
	GPIOD->MODER |= (0x555UL<<12*2);//Set PD12-PD15 as output mode
}
void UART_Config()
{
	RCC->AHB1ENR|=(1UL<<2);//Enable clock for Port C
	RCC->APB1ENR|=(1UL<<19);//Enable clock for UART4
	GPIOC->MODER|=(0xA<<20);//Set PC10 and PC11 to alternate function
	GPIOC->PUPDR|=(1UL<<22);//Set RX pin of UART ie PC11 as pull up
	GPIOC->AFR[1]|=(8UL<<12);//set PC11 to UART4_RX
	GPIOC->AFR[1]|=(8UL<<8);//set PC10 to UART4_TX
	UART4->BRR|=(104UL<<4)|(3UL<<0);//Set baud rate to 9600
	UART4->CR1&=~(1UL<<12);//Set word length to 8bits
	UART4->CR1|=(1UL<<3);//Enable transmit
	UART4->CR2&=~(3UL<<12);//1 stop bit
	UART4->CR1|=(1UL<<13);//enable UART
}
int j=0;
void UART_Transmit(int i)
{
	//while(UART4->SR==0);//wait till transmission is complete
	if(i<10)
	{
		UART4->DR=i+48;
		while (!(UART4->SR & (1<<6)));//wait till transmission is complete
	}
	else
	{
		int n=i;
		int t=n%10;
		t+=48;
		n/=10;
		n+=48;
		UART4->DR=n;
		while (!(UART4->SR & (1<<6)));//wait till transmission is complete
		UART4->DR=t;
		while (!(UART4->SR & (1<<6)));//wait till transmission is complete
		if(i==99)
			j=0;
	}
}
void TIM2_IRQHandler(void) {
  // Handle a timer 'update' interrupt event
	GPIOD->ODR ^= (0x1UL<<12);
	UART_Transmit(j++);
	TIM2->SR &= ~(0x0001UL);
  }
int main()
{
	gpio_config();
	UART_Config();
	start_timer();
	NVIC_EnableIRQ(TIM2_IRQn);//Enable the timer 2 global interrupt
	while(1)
	{
	}
}
