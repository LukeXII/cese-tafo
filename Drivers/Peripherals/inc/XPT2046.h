/*
 * XPT2046.h
 *
 *  Created on: May 28, 2022
 *      Author: Lucas
 */

/** 
 * @mainpage Documentación Driver XPT2046
 * @section intro_sec Introducción
 * Este driver implementa las funciones principales para poder usar el chip controlador de pantalla táctil XPT2046 con todas sus configuraciones posibles.
 *
 * @subsection autor Autor
 * Ing. Lucas Constantino <br>
 * Carrera de Especialización en Sistemas Embebidos <br>
 * Protocolos de Comunicación en Sistemas Embebidos <br>
 * Universidad de Buenos Aires - Facultad de Ingeniería <br>
 *
 */

#ifndef XPT2046_INC_XPT2046_H_
#define XPT2046_INC_XPT2046_H_

#include <stdint.h>
#include <stdbool.h>

/*______________________________________________________________
| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
|	S	|   A2	| 	A1	| 	A0	|  MODE |SER/DFR|  PD1	|  PD2	|
*/

/**
 * @brief Margen maximo inferior del eje X
 */
#define MARGEN_MAX_X_INFERIOR		300

/**
 * @brief Margen maximo superior del eje X
 */
#define MARGEN_MAX_X_SUPERIOR		351

/**
 * @brief Margen maximo inferior del eje Y
 */
#define MARGEN_MAX_Y_INFERIOR		464

/**
 * @brief Margen maximo inferior del eje Y
 */
#define MARGEN_MAX_Y_SUPERIOR		191

/**
 * @brief Alto de la panatalla en pixeles
 */
#define SCREEN_PIXELSIZE_X		240

/**
 * @brief Ancho de la panatalla en pixeles
 */
#define SCREEN_PIXELSIZE_Y		320

/**
 * @brief Fondo de escala de 12 bits de resolución
 */
#define MAX_12BIT_RES			4095

/**
 * @brief Fondo de escala de 8 bits de resolución
 */
#define MAX_8BIT_RES			255

/**
 * @brief Parte baja del byte de control para conversión en eje X
 */
#define CTRL_BYTE_HIGH_X	0b1101

/**
 * @brief Parte baja del byte de control para conversión en eje Y
 */
#define CTRL_BYTE_HIGH_Y	0b1001

/**
 * @brief Parte baja del byte de control para conversión en eje Z1
 */
#define CTRL_BYTE_HIGH_Z1	0b1011

/**
 * @brief Parte baja del byte de control para conversión en eje Z2
 */
#define CTRL_BYTE_HIGH_Z2	0b1100

/**
 * @brief Byte de control para conversión de temperatura TEMP0
 */
#define CTRL_BYTE_TEMP0	0b10000100

/**
 * @brief Byte de control para conversión de temperatura TEMP0
 */
#define CTRL_BYTE_TEMP1	0b11110100

/**
 * @brief Tipo de dato para el valor a pedir
 */
typedef enum {COORD_X, COORD_Y, COORD_Z1, COORD_Z2, TEMP0, TEMP1} requestedData_t;

/**
 * @brief Tipo de dato para la resolución del ADC
 */
typedef enum {RES_12BITS = 0, RES_8BITS = 1} adcRes_t;

/**
 * @brief Tipo de dato para el modo de conversión del ADC
 */
typedef enum {MODE_DIFF = 0, MODE_SER = 1} adcMode_t;

/**
 * @brief Tipo de dato para el modo de power down
 */
typedef enum {PD_LOWPOWER = 0b00, PD_ADCON = 0b01, PD_ADCOFF = 0b10, PD_ALLPOWERED = 0b11} powerMode_t;

/**
 * @brief Tipo de dato del handler del controlador <br>
 * Contiene la configuración del controlador y el valor resistivo de Rx-Plate
 *
 * @param res	Resolución del ADC (8 o 12 bits)
 * @param mode	Modo de operación del ADC (serie o diferencial)
 * @param powerMode Modo power down
 * @param RxPlate Valor de la red resistiva del eje X
 */
typedef struct {
	adcRes_t res;
	adcMode_t mode;
	powerMode_t powerMode;
	uint16_t RxPlate;
} XPT2046Handler_t;

// Prototipos para la parte no genérica del drive (funciones de port.c)

/**
 * @brief Configura el periférico SPI mediante el handler
 *
 */
void XPT2046_SPIConfig(void);

/**
 * @brief Habilita la interfaz SPI poniendo el pin CS en nivel bajo
 *
 */
void XPT2046_chipEnable(void);

/**
 * @brief Deshabilita la interfaz SPI poniendo el pin CS en nivel alto
 *
 */
void XPT2046_chipDisable(void);

/**
 * @brief Envía el byte de control al periférico SPI
 *
 */
void XPT2046_startConversion(unsigned char data);

/**
 * @brief Envía 16 clocks por SPI para recibir dos bytes
 *
 */
void XPT2046_getConversionData(uint8_t * dataPtr, uint16_t length);

/**
 * @brief Lee el pin asociado a la señal PENIRQ del controlador
 *
 */
bool XPT2046_readPENIRQ(void);

// Prototipos para la parte genérica del drive (funciones de XPT2046.c)

/**
 * @brief Realiza la consulta de la coordenada
 * 
 * @param[in] coord Valor a consultar al ADC
 * @return Valor de la coordenada consultada. Representa el valor convertido por el ADC del controlador. Rango de valores: 0 a 4095
 */
uint16_t XPT2046_getCoord(requestedData_t coord);

/**
 * @brief Configura el handler con los parámetros especificados
 *
 * @param[in] res Resolución del ADC
 * @param[in] mode Modo de operación del ADC
 * @param[in] pd Modo power down
 * @param[in] rxplate Valor de la red resistiva del eje X
 * @return Valor de la coordenada consultada. El valor representa el valor convertido por el ADC del controlador. Rango de valores: 0 a 4095
 */
void XPT2046_setControllerConfig(adcRes_t res, adcMode_t mode, powerMode_t pd, uint16_t rxplate);

/**
 * @brief Realiza la consulta de la temperatura y computa el valor de acuerdo a la fórmula especificada en las hojas de datos
 *
 * @return Valor de la temperatura del chip
 */
float XPT2046_getTemp(void);

/**
 * @brief Obtiene el valor de la presión ejercida sobre la pantalla.
 *
 * @return Valor de la presión sobre la pantalla
 */
uint16_t XPT2046_getPressure(void);

/**
 * @brief Consulta la señal PENIRQ del controlador, indicando si se está o no tocando la pantalla
 *
 * @return Devuelve true si se este tocando, false en caso contrario
 */
bool XPT2046_isPressed(void);

/**
 * @brief Convierte las coordenadas X e Y obtenidos con la función getCoord en coordenadas de pixeles en la pantalla
 *
 * @return Devuelve las coordenadas X e Y mapeadas a pixeles de la pantalla. El rango de valores es de 0 a 320 para el eje X y de 0 a 240 para el Y
 */
void XPT2046_convertToPixelCoord(uint16_t * xCoord, uint16_t * yCoord);

#endif /* XPT2046_INC_XPT2046_H_ */
