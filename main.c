/*Robinson Paniagua
 * Varying Switch Inputs
 * Different Switches control different LED Colors
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"



 int main(void) {

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //Initializes the Clock SysDiv_5, 40MHZ
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Initializes the GPIOPortF
	GPIO_PORTF_LOCK_R = 0x4C4F434B;             //Unlocks GPIO Port F in order to use Pin0 and Pin 4
	GPIO_PORTF_CR_R = 0x1F;                     //Unlocks Pin 0 & 4 in the GPIO Port F
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4 |GPIO_PIN_0 ); //Sets GPIO Pin 0 & 4 as inputs
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //Sets GPIO Pin as Weak Pull Up Resistor, Negative Logic
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //Sets the LED's as Outputs->GPIO Pins 1,2,3
	GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0); //Reads the GPIO Pins 0 & 4, corresponds to Switch 1 and Switch 2


	uint32_t Pin4Value = 0, Pin0Value=0;  //Sets Pin 0 & 4 as variables, initialized to 0

	while(1)
	{
		Pin4Value = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);   //Read Pin 0 and Pin 4 value = Switch 1,2
		Pin0Value = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);
		if((Pin0Value|Pin4Value)==0) //Pipe Operator OR Logic..if both Button are pressed gives 0 then turn on Pin 1
		{
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0x02); //Turn on GPIO Pin 1 Led Red when both buttons pressed, 0x02 gives 1 to bit 2 on the register
			SysCtlDelay(700000);                          //Delay, compensates for mechanical bounce
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0x00); //Turn off LED
		}

     if((!Pin4Value|Pin0Value)==0)   // if Switch 1, pin 0 is pressed then turn on Blue LED -> GPIO_Pin_2
     {
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0x04);        //0x40 -> sets 3rd bit to 1; 3rd bit corresponds to Pin2
			SysCtlDelay(700000);
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0x00);
     }

     if((Pin4Value|!Pin0Value)==0)      //If Switch 2 is pressed then turn on Green Led -> Pin 3
		{
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0x08); //Sets GPIO Pin 3 to 0x08; Pin 3 -> 4th bit 1000
			SysCtlDelay(700000);
			GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0x00);
		}
	}

 }
