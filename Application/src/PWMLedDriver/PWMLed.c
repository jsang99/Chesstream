/*
 * PWMLed.c
 *
 * Created: 2023-12-04 2:59:00 PM
 *  Author: vicga
 */ 

// #define CONF_PWM_MODULE LED_0_PWM4CTRL_MODULE
// #define CONF_PWM_CHANNEL LED_0_PWM4CTRL_CHANNEL
// #define CONF_PWM_OUTPUT LED_0_PWM4CTRL_OUTPUT
// #define CONF_PWM_OUT_PIN LED_0_PWM4CTRL_PIN
// #define CONF_PWM_OUT_MUX LED_0_PWM4CTRL_MUX
// 
// #include "PWMLedDriver/PWMLed.h"
// 
// static void configure_tcc(void)
// {
// 	struct tcc_config config_tcc;
// 	tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
// 	config_tcc.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV256;
// 	config_tcc.counter.period = 0xFFFF;
// 	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
// 	config_tcc.compare.match[CONF_PWM_CHANNEL] = (0xFFFF / 4);
// 	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT] = true;
// 	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT]        = CONF_PWM_OUT_PIN;
// 	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT]    = CONF_PWM_OUT_MUX;
// 	tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
// 	tcc_enable(&tcc_instance);
// }
