#include "stm32l1xx.h"

/* Function Prototypes */
void RCC_Configuration(void);
void Systick_Config(void);

/* Global data structures */
// data strucutre to hold clock infomation
RCC_ClocksTypeDef RCC_Clocks;
	
// data structure to hold GPIO information
GPIO_InitTypeDef ledInit;

int main() {
	
	// configure clock
	RCC_Configuration();
	
	// configure SysTick
	Systick_Config();
	
	// configure pins 6 and 7 as GPIO output
	ledInit.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
	ledInit.GPIO_Mode = GPIO_Mode_OUT;
	// initialize the peripheral
	GPIO_Init(GPIOB, &ledInit);
	// turn pins 6 and 7 on
	GPIO_SetBits(GPIOB, GPIO_Pin_7 | GPIO_Pin_6);

	// loop forever
	for (;;) {
			// toggle pins 6 and 7
			//GPIO_ToggleBits(GPIOB, GPIO_Pin_7 | GPIO_Pin_6);
			// waste time
			//for (i=0; i<250000; i++);
	}
}

// configure clock
void RCC_Configuration() {
	
	/* reset clock to initial state. HSI ON and used as system clock.
	 * HSE, PLL disabled and all prescalers set to 1.
	 */
	RCC_DeInit();
	
	/* Enable the HSI. HSI frequency = 16000000Hz */
	RCC_HSICmd(ENABLE);
	
	/* Now wait for HSI to stabilize and be ready
	 * We make the application wait in a while loop
	 */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)== RESET)
	{
	}
	
	/* Use the HSI for the system clock source */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	
	/* Disable the HSE as we are not using it in our application */
	RCC_HSEConfig(RCC_HSE_OFF);
	
	/* Now wait for the HSE to stop. 
	 * We pause by waiting in a while loop. In our application,
	 * as no HSE is connected, this statement returns immediately
	 */
	if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
	{
		while(1);
	}
	
	/* Now we enable the clocks to AHB which contains
	 * the GPIOB peripheral to which the on-board LEDs
	 * are connected
	 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
}

// configure SysTick timer
void Systick_Config() {

	/* Get current clock frequencies */
	RCC_GetClocksFreq(&RCC_Clocks);
	
	/* Set the SysTick to interrupt every 1 sec as the 
	 * count is equal to the AHB clock
	 */
	SysTick_Config(RCC_Clocks.HCLK_Frequency);
}

void SysTick_Interrupt_Handler() {
	GPIO_ToggleBits(GPIOB, GPIO_Pin_7 | GPIO_Pin_6);
}

