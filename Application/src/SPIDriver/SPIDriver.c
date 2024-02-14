/*
 * SPIDriver.c
 *
 * Created: 2023-12-01 7:26:11 PM
 *  Author: vicga
 */ 
#include "SPIDriver.h"
#include "SerialConsole/SerialConsole.h"

SemaphoreHandle_t SPIMutexHandle;		

void configure_spi_master(void)
{
	
	struct system_pinmux_config config_pinmux;
	system_pinmux_get_config_defaults(&config_pinmux);
	config_pinmux.mux_position = (1<<4); //Enable SERCOM-ALT
	system_pinmux_pin_set_config(PIN_PB02D_SERCOM5_PAD0, &config_pinmux);
	
	char bufferPrint[64];
	struct spi_config config_spi_master;
	struct spi_slave_inst_config slave_dev_config;
	
	
	/* Configure and initialize software device instance of peripheral
	slave */
	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = PINMUX_UNUSED;
	
	/* Configure, initialize and enable SERCOM SPI module */
	spi_get_config_defaults(&config_spi_master);
	config_spi_master.mux_setting = SPI_SIGNAL_MUX_SETTING_A;
	/* Configure pad 0 for data in */
	config_spi_master.pinmux_pad0 = PINMUX_PB02D_SERCOM5_PAD0;
	/* Configure pad 1 as unused */
	config_spi_master.pinmux_pad1 = PINMUX_UNUSED;
	/* Configure pad 2 as unused */
	config_spi_master.pinmux_pad2 = PINMUX_UNUSED;
	/* Configure pad 3 as unused */
	config_spi_master.pinmux_pad3 = PINMUX_UNUSED;
	
	spi_init(&spi_master_instance, SERCOM5, &config_spi_master);
	spi_attach_slave(&slave, &slave_dev_config);

	spi_init(&spi_master_instance, SERCOM5, &config_spi_master);
	
	if(STATUS_OK == spi_set_baudrate(&spi_master_instance, 4250000)){
 		SerialConsoleWriteString("BAUD RATE SET SUCCESSFULLY\r\n");
	} else {
 		SerialConsoleWriteString("BAUD RATE NOT SET!\r\n");
 	}
	 
	config_spi_master.transfer_mode = SPI_TRANSFER_MODE_0;
	
// 	struct port_config config_port_pin;
// 	port_get_config_defaults(&config_port_pin);
// 	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
// 	port_pin_set_config(PIN_PB02D_SERCOM5_PAD0, &config_port_pin);
// 	port_pin_set_output_level(PIN_PB02D_SERCOM5_PAD0, true);
	
	spi_enable(&spi_master_instance);
	
}

void configure_spi_master_callbacks(void)
{
	spi_register_callback(&spi_master_instance, callback_spi_master,
	SPI_CALLBACK_BUFFER_TRANSCEIVED);
	spi_enable_callback(&spi_master_instance,
	SPI_CALLBACK_BUFFER_TRANSCEIVED);
	
}

static void callback_spi_master( struct spi_module *const module)
{
	transrev_complete_spi_master = true;
}

int32_t SPIInitializeDriver(void)
{
	int32_t error = STATUS_OK;
	
	configure_spi_master();
	configure_spi_master_callbacks();
	SPIMutexHandle = xSemaphoreCreateMutex();
	
	return error;
}

int32_t SPIFreeMutex(void){
	
	int32_t error = ERROR_NONE;
	
	//students to fill out. Check what the function has to return
	if(xSemaphoreGive(SPIMutexHandle) != pdTRUE){
		error = ERROR_NOT_INITIALIZED;
	}
	
	return error;
}

int32_t SPIGetMutex(TickType_t waitTime){
	
	int32_t error = ERROR_NONE;
	//students to fill out. Check what the function has to return
	if(xSemaphoreTake(SPIMutexHandle, waitTime) != pdTRUE){
		error = ERROR_NOT_READY;
	}
	return error;
}
