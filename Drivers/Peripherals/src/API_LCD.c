/*
 * API_LCD.c
 *
 *  Created on: Apr 9, 2022
 *      Author: Lucas
 */

#include "API_LCD.h"

static LCDHandle_t deviceLCD;

void LCD_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

    deviceLCD.SPI_Handle.Instance 				= SPI3;
    deviceLCD.SPI_Handle.Init.Mode 				= SPI_MODE_MASTER;
    deviceLCD.SPI_Handle.Init.Direction 		= SPI_DIRECTION_2LINES;				// Cambiar a 1 linea?
    deviceLCD.SPI_Handle.Init.DataSize 			= SPI_DATASIZE_8BIT;
    deviceLCD.SPI_Handle.Init.CLKPolarity 		= SPI_POLARITY_LOW;
    deviceLCD.SPI_Handle.Init.CLKPhase 			= SPI_PHASE_1EDGE;
    deviceLCD.SPI_Handle.Init.NSS 				= SPI_NSS_HARD_OUTPUT;
    deviceLCD.SPI_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    deviceLCD.SPI_Handle.Init.FirstBit 			= SPI_FIRSTBIT_MSB;
    deviceLCD.SPI_Handle.Init.TIMode 			= SPI_TIMODE_DISABLED;
    deviceLCD.SPI_Handle.Init.CRCCalculation 	= SPI_CRCCALCULATION_DISABLED;
    deviceLCD.SPI_Handle.Init.CRCPolynomial 	= 10;

    if (HAL_SPI_Init(&deviceLCD.SPI_Handle) != HAL_OK)
    {
    	LCD_Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pins : PB15 PB8 PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOB, LCD_PIN_LED, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, LCD_PIN_RST | LCD_PIN_DC, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_RESET);

}

void LCD_Init(void)
{

	HAL_GPIO_WritePin(LCD_PORT_RST, LCD_PIN_RST, GPIO_PIN_SET);			// Habilita el LCD

	HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LCD_PORT_RST, LCD_PIN_RST, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(LCD_PORT_RST, LCD_PIN_RST, GPIO_PIN_SET);

	//SOFTWARE RESET
	LCD_Write_Command(0x01);
	HAL_Delay(100);


	LCD_Write_Command(0xEF);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x80);
	LCD_Write_Data(0x02);
	/*
	LCD_Write_Command(0xCA);
	LCD_Write_Data(0xC3);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x50);
*/
	//POWER CONTROL B
	LCD_Write_Command(0xCF);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0xC1);
	LCD_Write_Data(0x30);

	//POWER ON SEQUENCE CONTROL
	LCD_Write_Command(0xED);
	LCD_Write_Data(0x64);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x12);
	LCD_Write_Data(0x81);

	//DRIVER TIMING CONTROL A
	LCD_Write_Command(0xE8);
	LCD_Write_Data(0x85);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x78);

	//POWER CONTROL A
	LCD_Write_Command(0xCB);
	LCD_Write_Data(0x39);
	LCD_Write_Data(0x2C);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x34);
	LCD_Write_Data(0x02);

	//PUMP RATIO CONTROL
	LCD_Write_Command(0xF7);
	LCD_Write_Data(0x20);

	//DRIVER TIMING CONTROL B
	LCD_Write_Command(0xEA);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);

	//POWER CONTROL,VRH[5:0]
	LCD_Write_Command(0xC0);
	LCD_Write_Data(0x23);

	//POWER CONTROL,SAP[2:0];BT[3:0]
	LCD_Write_Command(0xC1);
	LCD_Write_Data(0x10);

	//VCM CONTROL
	LCD_Write_Command(0xC5);
	LCD_Write_Data(0x3E);
	LCD_Write_Data(0x28);

	//VCM CONTROL 2
	LCD_Write_Command(0xC7);
	LCD_Write_Data(0x86);

	//MEMORY ACCESS CONTROL
	LCD_Write_Command(0x0B);
	LCD_Write_Data(0x48);

	//VERTICAL SCROLL ZERO
	LCD_Write_Command(0x37);
	LCD_Write_Data(0x00);

	//PIXEL FORMAT
	LCD_Write_Command(0x3A);
	LCD_Write_Data(0x55);

	//FRAME RATIO CONTROL, STANDARD RGB COLOR
	LCD_Write_Command(0xB1);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x18);

	//DISPLAY FUNCTION CONTROL
	LCD_Write_Command(0xB6);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x82);
	LCD_Write_Data(0x27);

	//3GAMMA FUNCTION DISABLE
	LCD_Write_Command(0xF2);
	LCD_Write_Data(0x00);

	//GAMMA CURVE SELECTED
	LCD_Write_Command(0x26);
	LCD_Write_Data(0x01);

	//POSITIVE GAMMA CORRECTION
	LCD_Write_Command(0xE0);
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x31);
	LCD_Write_Data(0x2B);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x4E);
	LCD_Write_Data(0xF1);
	LCD_Write_Data(0x37);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x10);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x09);
	LCD_Write_Data(0x00);

	//NEGATIVE GAMMA CORRECTION
	LCD_Write_Command(0xE1);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x14);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x31);
	LCD_Write_Data(0xC1);
	LCD_Write_Data(0x48);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x31);
	LCD_Write_Data(0x36);
	LCD_Write_Data(0x0F);

	//EXIT SLEEP
	LCD_Write_Command(0x11);
	LCD_Write_Data(0x80);
	HAL_Delay(120);

	//TURN ON DISPLAY
	LCD_Write_Command(0x29);
	LCD_Write_Data(0x80);

	LCD_Write_Command(0x00);

	//ORIENTACION
	LCD_Set_Rotation(SCREEN_HORIZONTAL_2);

	LCD_Fill_Screen(RED);

}

// Llena toda la pantalla con un color
void LCD_Fill_Screen(uint16_t Colour)
{
	LCD_Set_Address(0, 0, deviceLCD.LCDSizeX, deviceLCD.LCDSizeY);
	LCD_Draw_Colour_Burst(Colour, (deviceLCD.LCDSizeX)*(deviceLCD.LCDSizeY));
}


void LCD_Set_Address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
	LCD_Write_Command(0x2A);
	LCD_Write_Data(X1>>8);
	LCD_Write_Data(X1);
	LCD_Write_Data(X2>>8);
	LCD_Write_Data(X2);

	LCD_Write_Command(0x2B);
	LCD_Write_Data(Y1>>8);
	LCD_Write_Data(Y1);
	LCD_Write_Data(Y2>>8);
	LCD_Write_Data(Y2);

	LCD_Write_Command(0x2C);
}


void LCD_Draw_Colour_Burst(uint16_t Colour, uint32_t Size)
{

	uint32_t Buffer_Size = 0;
	if((Size*2) < BURST_MAX_SIZE)
	{
		Buffer_Size = Size;
	}
	else
	{
		Buffer_Size = BURST_MAX_SIZE;
	}

	HAL_GPIO_WritePin(LCD_PORT_DC, LCD_PIN_DC, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_RESET);

	unsigned char chifted = 	Colour>>8;;
	unsigned char burst_buffer[Buffer_Size];

	for(uint32_t j = 0; j < Buffer_Size; j+=2)
	{
		burst_buffer[j] = 	chifted;
		burst_buffer[j+1] = Colour;
	}

	uint32_t Sending_Size = Size*2;
	uint32_t Sending_in_Block = Sending_Size/Buffer_Size;
	uint32_t Remainder_from_block = Sending_Size%Buffer_Size;

	if(Sending_in_Block != 0)
	{
		for(uint32_t j = 0; j < (Sending_in_Block); j++)
		{
			HAL_SPI_Transmit(&(deviceLCD.SPI_Handle), (unsigned char *)burst_buffer, Buffer_Size, 10);
		}
	}


	HAL_SPI_Transmit(&(deviceLCD.SPI_Handle), (unsigned char *)burst_buffer, Remainder_from_block, 10);

	HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_SET);
}


void LCD_Set_Rotation(uint8_t Rotation)
{
	uint8_t screen_rotation = Rotation;

	LCD_Write_Command(0x36);
	HAL_Delay(1);

	switch(screen_rotation)
	{
		case SCREEN_VERTICAL_1:
			LCD_Write_Data(0x40|0x08);
			deviceLCD.LCDSizeX = 240;
			deviceLCD.LCDSizeY = 320;
			break;
		case SCREEN_HORIZONTAL_1:
			LCD_Write_Data(0x20|0x08);
			deviceLCD.LCDSizeX = 320;
			deviceLCD.LCDSizeY = 240;
			break;
		case SCREEN_VERTICAL_2:
			LCD_Write_Data(0x80|0x08);
			deviceLCD.LCDSizeX = 240;
			deviceLCD.LCDSizeY = 320;
			break;
		case SCREEN_HORIZONTAL_2:
			LCD_Write_Data(0x40|0x80|0x20|0x08);
			deviceLCD.LCDSizeX = 320;
			deviceLCD.LCDSizeY = 240;
			break;
		default:
			// Sale si la orientacion no es valida
			break;
	}
}


void LCD_Draw_Hollow_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour)
{
	int x = Radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (Radius << 1);

    while (x >= y)
    {
    	LCD_Draw_Pixel(X + x, Y + y, Colour);
        LCD_Draw_Pixel(X + y, Y + x, Colour);
        LCD_Draw_Pixel(X - y, Y + x, Colour);
        LCD_Draw_Pixel(X - x, Y + y, Colour);
        LCD_Draw_Pixel(X - x, Y - y, Colour);
        LCD_Draw_Pixel(X - y, Y - x, Colour);
        LCD_Draw_Pixel(X + y, Y - x, Colour);
        LCD_Draw_Pixel(X + x, Y - y, Colour);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += (-Radius << 1) + dx;
        }
    }
}


void LCD_Draw_Filled_Circle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Colour)
{

	int x = Radius;
    int y = 0;
    int xChange = 1 - (Radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = X - x; i <= X + x; i++)
        {
            LCD_Draw_Pixel(i, Y + y,Colour);
            LCD_Draw_Pixel(i, Y - y,Colour);
        }
        for (int i = X - y; i <= X + y; i++)
        {
            LCD_Draw_Pixel(i, Y + x,Colour);
            LCD_Draw_Pixel(i, Y - x,Colour);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }

}


void LCD_Draw_Hollow_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour)
{
	uint16_t X_length = 0;
	uint16_t Y_length = 0;
	uint8_t	Negative_X = 0;
	uint8_t Negative_Y = 0;
	float Calc_Negative = 0;

	Calc_Negative = X1 - X0;
	if(Calc_Negative < 0) Negative_X = 1;
	Calc_Negative = 0;

	Calc_Negative = Y1 - Y0;
	if(Calc_Negative < 0) Negative_Y = 1;

	if(!Negative_X)
		X_length = X1 - X0;
	else
		X_length = X0 - X1;

	LCD_Draw_Horizontal_Line(X0, Y0, X_length, Colour);
	LCD_Draw_Horizontal_Line(X0, Y1, X_length, Colour);

	if(!Negative_Y)
		Y_length = Y1 - Y0;
	else
		Y_length = Y0 - Y1;
	LCD_Draw_Vertical_Line(X0, Y0, Y_length, Colour);
	LCD_Draw_Vertical_Line(X1, Y0, Y_length, Colour);

	if((X_length > 0)||(Y_length > 0))
		LCD_Draw_Pixel(X1, Y1, Colour);

}


void LCD_Draw_Filled_Rectangle_Coord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Colour)
{
	uint16_t X_length = 0;
	uint16_t Y_length = 0;
	uint8_t	Negative_X = 0;
	uint8_t Negative_Y = 0;
	int32_t Calc_Negative = 0;

	uint16_t X0_true = 0;
	uint16_t Y0_true = 0;

	Calc_Negative = X1 - X0;

	if(Calc_Negative < 0)
		Negative_X = 1;

	Calc_Negative = 0;

	Calc_Negative = Y1 - Y0;

	if(Calc_Negative < 0)
		Negative_Y = 1;

	if(!Negative_X)
	{
		X_length = X1 - X0;
		X0_true = X0;
	}
	else
	{
		X_length = X0 - X1;
		X0_true = X1;
	}

	if(!Negative_Y)
	{
		Y_length = Y1 - Y0;
		Y0_true = Y0;
	}
	else
	{
		Y_length = Y0 - Y1;
		Y0_true = Y1;
	}

	LCD_Draw_Rectangle(X0_true, Y0_true, X_length, Y_length, Colour);
}


void LCD_Draw_Char(char Character, uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour)
{
	uint8_t function_char;
    uint8_t	i,j;
	char temp[CHAR_WIDTH];

	function_char = Character;

    if (function_char < ' ')
    	Character = 0;
    else
    {
    	function_char -= 32;
    }

	for(uint8_t k = 0; k<CHAR_WIDTH; k++)
			temp[k] = font[function_char][k];

	LCD_Draw_Rectangle(X, Y, CHAR_WIDTH*Size, CHAR_HEIGHT*Size, Background_Colour);

    for (j=0; j<CHAR_WIDTH; j++) {
        for (i=0; i<CHAR_HEIGHT; i++) {
            if (temp[j] & (1<<i)) {
				if(Size == 1)
					LCD_Draw_Pixel(X+j, Y+i, Colour);
				else
					LCD_Draw_Rectangle(X+(j*Size), Y+(i*Size), Size, Size, Colour);
            }
        }
    }
}


void LCD_Draw_Text(const char* Text, uint16_t X, uint16_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour)
{
    while (*Text)
    {
        LCD_Draw_Char(*Text++, X, Y, Colour, Size, Background_Colour);
        X += CHAR_WIDTH*Size;
    }
}

void LCD_Draw_Rectangle(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height, uint16_t Colour)
{
	if((X >=deviceLCD.LCDSizeX) || (Y >= deviceLCD.LCDSizeY))
		return;

	if((X+Width-1) >= deviceLCD.LCDSizeX)
	{
		Width = deviceLCD.LCDSizeX-X;
	}
	if((Y+Height-1) >= deviceLCD.LCDSizeY)
	{
		Height = deviceLCD.LCDSizeY-Y;
	}

	LCD_Set_Address(X, Y, X+Width-1, Y+Height-1);
	LCD_Draw_Colour_Burst(Colour, Height*Width);
}


void LCD_Draw_Horizontal_Line(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Colour)
{
	if((X >= deviceLCD.LCDSizeX) || (Y >= deviceLCD.LCDSizeY))
		return;

	if((X+Width-1) >= deviceLCD.LCDSizeX)
		Width = deviceLCD.LCDSizeX-X;

	LCD_Set_Address(X, Y, X+Width-1, Y);
	LCD_Draw_Colour_Burst(Colour, Width);
}


void LCD_Draw_Vertical_Line(uint16_t X, uint16_t Y, uint16_t Height, uint16_t Colour)
{
	if((X >= deviceLCD.LCDSizeX) || (Y >= deviceLCD.LCDSizeY))
		return;

	if((Y+Height-1)>=deviceLCD.LCDSizeY)
		Height = deviceLCD.LCDSizeY-Y;

	LCD_Set_Address(X, Y, X, Y+Height-1);
	LCD_Draw_Colour_Burst(Colour, Height);
}

// Dibuja un pixel en la posicion indicada
void LCD_Draw_Pixel(uint16_t X, uint16_t Y, uint16_t Colour)
{
	if((X < deviceLCD.LCDSizeX) && (Y < deviceLCD.LCDSizeY))			//Verifica que la posicion este dentro de los limites de la pantalla
	{
		unsigned char tempBuffer[4];

		LCD_Write_Command(0x2A);

		tempBuffer[0] = X>>8;
		tempBuffer[1] = X;
		tempBuffer[2] = (X+1)>>8;
		tempBuffer[3] = X+1;

		HAL_GPIO_WritePin(LCD_PORT_DC, LCD_PIN_DC, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&(deviceLCD.SPI_Handle), tempBuffer, 4, 1);
		HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_SET);

		LCD_Write_Command(0x2B);

		tempBuffer[0] = Y>>8;
		tempBuffer[1] = Y;
		tempBuffer[2] = (Y+1)>>8;
		tempBuffer[3] = Y+1;

		HAL_GPIO_WritePin(LCD_PORT_DC, LCD_PIN_DC, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&(deviceLCD.SPI_Handle), tempBuffer, 4, 1);
		HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_SET);

		LCD_Write_Command(0x2C);

		tempBuffer[0] = Colour>>8;
		tempBuffer[1] = Colour;

		HAL_GPIO_WritePin(LCD_PORT_DC, LCD_PIN_DC, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&(deviceLCD.SPI_Handle), tempBuffer, 2, 1);
		HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_SET);
	}

}

// Descomentar las lineas que togglean el CS si se utiliza el NSS en modo SPI_NSS_SOFT
/* Envia un comando (char) al LCD */
void LCD_Write_Command(uint8_t Command)
{
//	HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_RESET);		// Pone la señal CS (chip select) del SPI en bajo antes de mandar el comando. La vuelve a poner en alto cuando termina.
	HAL_GPIO_WritePin(LCD_PORT_DC, LCD_PIN_DC, GPIO_PIN_RESET);
	LCD_SPI_Send(Command);
//	HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_SET);
}

/* Envia un dato (char) al LCD */
void LCD_Write_Data(uint8_t Data)
{
	HAL_GPIO_WritePin(LCD_PORT_DC, LCD_PIN_DC, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_RESET);
	LCD_SPI_Send(Data);
//	HAL_GPIO_WritePin(LCD_PORT_CS, LCD_PIN_CS, GPIO_PIN_SET);
}

/* Reliza la transmision al LCD mediante el SPI */
void LCD_SPI_Send(unsigned char SPI_Data)
{
	HAL_SPI_Transmit(&(deviceLCD.SPI_Handle), &SPI_Data, 1, 1);
}

void LCD_Error_Handler(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}


