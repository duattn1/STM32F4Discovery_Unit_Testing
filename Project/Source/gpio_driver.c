/** @file gpio_driver.c
 *  @brief Function implementation for GPIO driver.
 *
 *  This is the source file for the definition of GPIO driver.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V1.0
 */ 
 
/*******************************************************************************
 * 1. Included Files
 ******************************************************************************/
#include "gpio_driver.h"

/*******************************************************************************
 * 2. Object-like Macros
 ******************************************************************************/


/*******************************************************************************
 * 3. Function-like Macros
 ******************************************************************************/


/*******************************************************************************
 * 4. Typedefs: Enumerations, Structures, Pointers, Others
 ******************************************************************************/


/*******************************************************************************
 * 5. Global, Static and Extern Variables
 ******************************************************************************/


/*******************************************************************************
 * 6. Function Definitions
 ******************************************************************************/
void GPIO_Enable(Enum_GPIO_Port_Typedef port) {	
	assert(IS_GPIO_PORT(port));
	
	Enum_RCC_AHB1ENR_Typedef peripheral;	
	switch(port){
		case GPIO_PortGPIOA:
			peripheral = RCC_AHB1ENR_GPIOAEnable;
			break;
		case GPIO_PortGPIOB:
			peripheral = RCC_AHB1ENR_GPIOBEnable;
			break;
		case GPIO_PortGPIOC:
			peripheral = RCC_AHB1ENR_GPIOCEnable;
			break;
		case GPIO_PortGPIOD:
			peripheral = RCC_AHB1ENR_GPIODEnable;
			break;
		case GPIO_PortGPIOE:
			peripheral = RCC_AHB1ENR_GPIOEEnable;
			break;
		case GPIO_PortGPIOF:
			peripheral = RCC_AHB1ENR_GPIOFEnable;
			break;
		case GPIO_PortGPIOG:
			peripheral = RCC_AHB1ENR_GPIOGEnable;
			break;
		case GPIO_PortGPIOH:
			peripheral = RCC_AHB1ENR_GPIOHEnable;
			break;
		case GPIO_PortGPIOI:
			peripheral = RCC_AHB1ENR_GPIOIEnable;
			break;
	};
	
	Struct_RCC_ClockEnableConfig config;
	config.AHB1Config = peripheral;
	RCC_EnableClock(config);
}
 
void GPIO_Init(GPIO_TypeDef* gpioX, Struct_GPIO_InitConfig config) {
	// Assert parameters
	assert(IS_GPIO_PORT_BASE_ADDRESS(gpioX));
	assert(IS_GPIO_PIN(config.pin));
	assert(IS_GPIO_MODE(config.mode));
	assert(IS_GPIO_OUTPUT_TYPE(config.ouputType));
	assert(IS_GPIO_OUTPUT_SPEED(config.outputSpeed));
	assert(IS_GPIO_PULLUP_PULLDOWN(config.pupd));
	// Assert preconditions
	assert(IS_GPIO_PERIPHERAL_CLOCK_ENALBED(gpioX));
	
	// Select GPIO pin mode
	gpioX->MODER &= ~(0x03 << config.pin*2);
	gpioX->MODER |= config.mode << config.pin*2;
	
	// Do config when GPIO pin mode is output OR alternate function
	// Reference: Table 35. Port bit configuration table
	if(GPIOx_MODER_Output == config.mode || GPIOx_MODER_AlternateFunction == config.mode) {
		gpioX->OTYPER &= ~(0x1 << config.pin);
		gpioX->OTYPER |= config.ouputType << config.pin;
		
		gpioX->OSPEEDR &= ~(0x03 << config.pin*2);
		gpioX->OSPEEDR |= config.outputSpeed << (config.pin*2);
	} else {
		// Do nothing
	}
	
	// Select pull-up/pull-down resister connection
	gpioX->PUPDR &= ~(0x03 << config.pin*2);
	gpioX->PUPDR |= config.pupd << (config.pin*2);
}

void GPIO_ConfigPinAlternateFunction(
	GPIO_TypeDef* gpioX, 
	Enum_GPIO_Pin_Typedef pin, 
	Enum_GPIOx_AFR_Typedef altFunction)
{
	// Assert parameters
	assert(IS_GPIO_PORT_BASE_ADDRESS(gpioX));
	assert(IS_GPIO_PIN(pin));
	assert(IS_GPIO_PIN_ALTERNATE_FUNCTION(altFunction));
	// Assert preconditions
	assert(IS_GPIO_PERIPHERAL_CLOCK_ENALBED(gpioX));
	
	if(pin < GPIO_Pin8){
		gpioX->AFR[0] &= ~(0x0F << 4*pin);
		gpioX->AFR[0] |= altFunction << 4*pin;
	} else {
		gpioX->AFR[1] &= ~(0x0F << 4*(pin - GPIO_Pin8));
		gpioX->AFR[1] |= altFunction << 4*(pin - GPIO_Pin8);
	}
}

void GPIO_SetPin(GPIO_TypeDef* gpioX, Enum_GPIO_Pin_Typedef pin){
	// BRn pins are located in 16 LSB bits of BSRR
	gpioX->BSRR |= 0x01 << pin;
}

void GPIO_ResetPin(GPIO_TypeDef* gpioX, Enum_GPIO_Pin_Typedef pin){
	// BSn pins are located in 16 MSB bits of BSRR
	gpioX->BSRR |= 0x01 << (pin + 16);
}

/** End of File ***************************************************************/
