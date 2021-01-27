/*! @file : main.c
 * @author  Miguel Gnecco
 * @version 1.0.0
 * @date    16/01/2021
 * @brief   Archivo principal
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"


#include "sdk_hal_gpio.h"
#include "sdk_hal_uart0.h"
#include "sdk_hal_i2c0.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MMA851_I2C_DEVICE_ADDRESS	0x1D

#define MMA8451_WHO_AM_I_MEMORY_ADDRESS		0x0D

#define MMA8451_OUT_X_MSB   0x01
#define MMA8451_OUT_X_LSB  0x02
#define MMA8451_OUT_Y_MSB   0x03
#define MMA8451_OUT_Y_LSB  0x04
#define MMA8451_OUT_Z_MSB   0x05
#define MMA8451_OUT_Z_LSB  0x06



/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/


/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
int main(void) {
	status_t status;
	uint8_t nuevo_byte_uart;
	uint16_t nuevo_dato_i2c;
	uint16_t MSB;
    uint16_t LSB;
    uint16_t datocompleto;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200);	//115200bps
    (void)i2c0MasterInit(100000);	//100kbps

    PRINTF("Usar teclado para controlar LEDs\r\n");
    PRINTF("r-R led ROJO\r\n");
    PRINTF("v-V led VERDE\r\n");
    PRINTF("a-A led AZUL\r\n");
    PRINTF("M buscar acelerometro\r\n");
    PRINTF("x-X registro salida de X (MSB|LSB)\r\n");
    PRINTF("y-Y Registro salida de Y (MSB|LSB)\r\n");
    PRINTF("z-Z Registro salida de Z (MSB|LSB)\r\n");


    while(1) {
    	if(uart0CuantosDatosHayEnBuffer()>0){
    		status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);
    		if(status==kStatus_Success){
    			printf("dato:%c\r\n",nuevo_byte_uart);
    			switch (nuevo_byte_uart) {
				case 'a':
				case 'A':
					gpioPutToggle(KPTB10);
					break;

				case 'v':
					gpioPutHigh(KPTB7);
					break;
				case 'V':
					gpioPutLow(KPTB7);
					break;

				case 'r':
					gpioPutValue(KPTB6,1);
					break;
				case 'R':
					gpioPutValue(KPTB6,0);
					break;

				case 'M':
					i2c0MasterReadByte(&nuevo_dato_i2c, MMA851_I2C_DEVICE_ADDRESS, MMA8451_WHO_AM_I_MEMORY_ADDRESS);

					if(nuevo_dato_i2c==0x1A);
						printf("MMA8451 encontrado!!\r\n");

					break;

    			case 'x':
    			case 'X':
    			i2c0MasterReadByte(&MSB, MMA851_I2C_DEVICE_ADDRESS,MMA8451_OUT_X_MSB);

    			i2c0MasterReadByte(&LSB, MMA851_I2C_DEVICE_ADDRESS,MMA8451_OUT_X_LSB);

    				datocompleto=(MSB<<6)|(LSB>>2);

					printf("el dato MSB de X es: %d \r\n ", MSB);
					printf("el dato LSB de X es: %d \r\n ", LSB);
    			    printf("el dato completo es: %d \r\n ", datocompleto);

    						break;

    			case 'y':
    			case 'Y':

    			i2c0MasterReadByte(&MSB, MMA851_I2C_DEVICE_ADDRESS,MMA8451_OUT_Y_MSB);

    			i2c0MasterReadByte(&LSB, MMA851_I2C_DEVICE_ADDRESS,MMA8451_OUT_Y_LSB);

    				datocompleto=(MSB<<6)|(LSB>>2);

					printf("el dato MSB de Y es: %d \r\n ", MSB);
					printf("el dato LSB de Y es: %d \r\n ", LSB);
    			    printf("el dato completo es: %d \r\n ", datocompleto);

    						break;
    			case 'z':
    			case 'Z':

    			i2c0MasterReadByte(&MSB, MMA851_I2C_DEVICE_ADDRESS,MMA8451_OUT_Z_MSB);

    			i2c0MasterReadByte(&LSB, MMA851_I2C_DEVICE_ADDRESS,MMA8451_OUT_Z_LSB);

    				datocompleto=(MSB<<6)|(LSB>>2);

					printf("el dato MSB de Z es: %d \r\n ", MSB);
					printf("el dato LSB de Z es: %d \r\n ", LSB);
    			    printf("el dato completo es: %d \r\n ", datocompleto);

    						break;
				}
    		}else{
    			printf("error\r\n");
    		}
    	}
    }

    return 0 ;
}

