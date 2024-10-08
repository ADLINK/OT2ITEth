/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#include "Arduino.h"
#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include "hri_mclk.h"
#include "hal_gpio.h"
#include "hal_init.h"
#include "atmel_start_pins.h"
//#include <hal_init.h>

/*struct usart_sync_descriptor TARGET_IO;*/

struct mac_async_descriptor COMMUNICATION_IO;
/*
void TARGET_IO_PORT_init(void)
{

	gpio_set_pin_function(PB25, PINMUX_PB25D_SERCOM2_PAD0);

	gpio_set_pin_function(PB24, PINMUX_PB24D_SERCOM2_PAD1);
}

void TARGET_IO_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM2_bit(MCLK);
}

void TARGET_IO_init(void)
{
	TARGET_IO_CLOCK_init();
	usart_sync_init(&TARGET_IO, SERCOM2, (void *)NULL);
	TARGET_IO_PORT_init();
}*/


void COMMUNICATION_IO_PORT_init(void)
{
	gpio_set_pin_function(PC11, PINMUX_PC11L_GMAC_GMDC);

	gpio_set_pin_function(PC12, PINMUX_PC12L_GMAC_GMDIO);

	gpio_set_pin_function(PA13, PINMUX_PA13L_GMAC_GRX0);

	gpio_set_pin_function(PA12, PINMUX_PA12L_GMAC_GRX1);

	gpio_set_pin_function(PC20, PINMUX_PC20L_GMAC_GRXDV);

	gpio_set_pin_function(PA15, PINMUX_PA15L_GMAC_GRXER);

	gpio_set_pin_function(PA18, PINMUX_PA18L_GMAC_GTX0);

	gpio_set_pin_function(PA19, PINMUX_PA19L_GMAC_GTX1);

	gpio_set_pin_function(PA14, PINMUX_PA14L_GMAC_GTXCK);

	gpio_set_pin_function(PA17, PINMUX_PA17L_GMAC_GTXEN);
}

void COMMUNICATION_IO_CLOCK_init(void)
{
	hri_mclk_set_AHBMASK_GMAC_bit(MCLK);
	hri_mclk_set_APBCMASK_GMAC_bit(MCLK);
}

void COMMUNICATION_IO_init(void)
{
	COMMUNICATION_IO_CLOCK_init();
	mac_async_init(&COMMUNICATION_IO, GMAC);
	COMMUNICATION_IO_PORT_init();
}

void COMMUNICATION_IO_example(void)
{
	mac_async_enable(&COMMUNICATION_IO);
	mac_async_write(&COMMUNICATION_IO, (uint8_t *)"Hello World!", 12);
}

void led_10_100_on(void)
{
	gpio_set_pin_level(LED_0, false);
}

void led_10_100_off(void)
{
	gpio_set_pin_level(LED_0, true);
}

void system_init(void)
{
	//init_mcu();

	// GPIO on PC18

	gpio_set_pin_level(LED_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED_0, GPIO_PIN_FUNCTION_OFF);

	gpio_set_pin_level(PC21, true);
	gpio_set_pin_direction(PC21, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PC21, GPIO_PIN_FUNCTION_OFF);

/*
	TARGET_IO_init();
*/

	COMMUNICATION_IO_init();
	COMMUNICATION_IO_example();
}
