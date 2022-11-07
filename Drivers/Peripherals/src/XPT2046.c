/*
 * XPT2046.c
 *
 *  Created on: May 28, 2022
 *      Author: Lucas
 */

#include "XPT2046.h"

static unsigned char CTRL_BYTE_LOW = 0b0000;
static XPT2046Handler_t tsHandler;

static uint16_t XPT2046_convertReceivedData(uint16_t firstByte, uint16_t secondByte);
static bool isControllerConfigured = false;

void XPT2046_setControllerConfig(adcRes_t res, adcMode_t mode, powerMode_t pd, uint16_t rxplate)
{
	tsHandler.res = res;
	tsHandler.mode = mode;
	tsHandler.powerMode = pd;
	tsHandler.RxPlate = rxplate;

	CTRL_BYTE_LOW = (res << 3) | (mode << 2) | pd;

	isControllerConfigured = true;
}

uint16_t XPT2046_getCoord(requestedData_t coord)
{
	uint8_t receivedData[3];
	unsigned char controlByte;
	uint16_t coordData;

	receivedData[2] = '\0';

	if(!isControllerConfigured)						// Si el controlador no está configurado, aborta la operación
		return -1;

	XPT2046_chipEnable();

	// Arma el byte de control segun el dato a consultar
	switch(coord)
	{
		case COORD_X:
			controlByte = (CTRL_BYTE_HIGH_X << 4) | CTRL_BYTE_LOW;
			break;

		case COORD_Y:
			controlByte = (CTRL_BYTE_HIGH_Y << 4) | CTRL_BYTE_LOW;
			break;

		case COORD_Z1:
			controlByte = (CTRL_BYTE_HIGH_Z1 << 4) | CTRL_BYTE_LOW;
			break;

		case COORD_Z2:
			controlByte = (CTRL_BYTE_HIGH_Z2 << 4) | CTRL_BYTE_LOW;
			break;

		case TEMP0:
			controlByte = CTRL_BYTE_TEMP0;
			break;

		case TEMP1:
			controlByte = CTRL_BYTE_TEMP1;
			break;
	}

	// Envia el byte de control y luego recibe los dos bytes de la respuesta
	XPT2046_startConversion(controlByte);
	XPT2046_getConversionData(receivedData, 16);
	XPT2046_chipDisable();

	// Pone los bits de la respuesta en el orden correcto
	coordData = XPT2046_convertReceivedData(receivedData[0], receivedData[1]);

	return coordData;
}

uint16_t XPT2046_getPressure(void)
{
	uint16_t coordX, coordZ1, coordZ2;

	if(!isControllerConfigured)
		return -1;

	// Obtiene las coordenadas X, Z1 y Z2 del controlador para poder calcular la presión
	coordX = XPT2046_getCoord(COORD_X);
	coordZ1 = XPT2046_getCoord(COORD_Z1);
	coordZ2 = XPT2046_getCoord(COORD_Z2);

	return tsHandler.RxPlate*coordX*(coordZ2/coordZ1 - 1)/MAX_12BIT_RES;
}

float XPT2046_getTemp(void)
{
	uint16_t temp0, temp1;

	if(!isControllerConfigured)
		return -1;

	// Obtiene los valores de TEMP0 y TEMP1 del controlador
	temp0 = XPT2046_getCoord(TEMP0);
	temp1 = XPT2046_getCoord(TEMP1);

	return 2.573*((float)temp1 - (float)temp0)*2500/4095;
}

bool XPT2046_isPressed(void)
{
	return XPT2046_readPENIRQ();
}

void XPT2046_convertToPixelCoord(uint16_t * xCoord, uint16_t * yCoord)
{
	*xCoord = (*xCoord - MARGEN_MAX_X_INFERIOR)*SCREEN_PIXELSIZE_X/(MAX_12BIT_RES - MARGEN_MAX_X_INFERIOR - MARGEN_MAX_X_SUPERIOR);
	*yCoord = (*yCoord - MARGEN_MAX_Y_INFERIOR)*SCREEN_PIXELSIZE_Y/(MAX_12BIT_RES - MARGEN_MAX_Y_INFERIOR - MARGEN_MAX_Y_SUPERIOR);
}

static uint16_t XPT2046_convertReceivedData(uint16_t firstByte, uint16_t secondByte)
{
	uint16_t coord;

	coord = (firstByte << 5) | (secondByte >> 3);

	return coord;
}
