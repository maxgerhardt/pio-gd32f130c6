#include "gd32f1x0.h"
#include "gd32f1x0_libopt.h"
#include "systick.h"
#include <stdio.h>

#define LED1_PIN                         GPIO_PIN_0
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK                    RCU_GPIOA

void init_led() {
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure led GPIO port */
    gpio_mode_set(RCU_GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_PIN);
    gpio_output_options_set(LED1_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,LED1_PIN);
    GPIO_BC(LED1_GPIO_PORT) = LED1_PIN; /* clear port / set to 0*/
}

void toggle_led() {
	GPIO_OCTL(LED1_GPIO_PORT) ^= LED1_PIN;
}

int main(void)
{
    systick_config();
    init_led();

    while(1){
    	toggle_led();
    	delay_1ms(1000);
    }
}
