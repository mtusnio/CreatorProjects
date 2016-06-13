#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/spi/spidev.h>

#include <core/common.h>
#include <core/i2c.h>
#include <click/common.h>
#include <click/joystick.h>
#include <click/led_matrix.h>

#define TOLERANCE 30

#define SIZE 8
#define SNAKE_SIZE 8

#define DIR_LEFT 0
#define DIR_UP 1
#define DIR_RIGHT 2
#define DIR_DOWN 3

typedef struct pos_t
{
	int x;
	int y;
} Pos;

int main()
{
	int i, length;
	int direction = DIR_LEFT;
	Pos * snake[SNAKE_SIZE];

	for(i = 0; i < SNAKE_SIZE; i++)
		snake[i] = 0;

	if(i2c_init() < 0 || i2c_select_bus(MIKROBUS_1) < 0)
		return -1;
	
	if(spi_init(MIKROBUS_2, SPI_MODE_3) < 0)
		return -1;

	if(spi_select_bus(MIKROBUS_2) < 0)
		return -1;

	if(led_matrix_click_enable() < 0)
		return -1;

	snake[0] = malloc(sizeof(Pos));
	snake[1] = malloc(sizeof(Pos));

	snake[0]->x = 1;
	snake[0]->y = 3;
	
	snake[1]->x = 0;
	snake[1]->y = 3;

	length = 2;

	while (1)
	{
		// Controls
		int8_t joyX, joyY;
		if(joystick_click_get_position(&joyX, &joyY) < 0)
		{
			printf("Failed to get joystick position");
			break;
		}

		if(abs(joyX) > TOLERANCE || abs(joyY) > TOLERANCE)
		{
			if(abs(joyX) > abs(joyY) && 
			(direction != DIR_LEFT && direction != DIR_RIGHT))
				direction = joyX > 0 ? DIR_LEFT : DIR_RIGHT;
			else if(abs(joyY) > abs(joyX) &&
			(direction != DIR_UP && direction != DIR_DOWN)) 
				direction = joyY < 0 ? DIR_DOWN : DIR_UP;
		}


		// Logic
		for(i = length - 1; i > 0; i--)
		{
			snake[i]->x = snake[i - 1]->x;
			snake[i]->y = snake[i - 1]->y;	
		}	

		switch(direction)
		{
			case DIR_LEFT:
			snake[0]->x++;
			break;

			case DIR_RIGHT:
			snake[0]->x--;
			break;

			case DIR_UP:
			snake[0]->y++;
			break;

			case DIR_DOWN:
			snake[0]->y--;
			break;
		}

		// Draw
		uint8_t columns[SIZE];

		for(i = 0; i < SIZE; i++)
			columns[i] = 0;
		for(i = 0; i < length; i++)
		{
			int x = snake[i]->x;
			int y = snake[i]->y;

			columns[x] |= (1 << y);
		}

		led_matrix_click_set(columns);
		sleep(1);

		if(snake[0]->x < 0 || snake[0]->x >= SIZE)
			break;

		if(snake[0]->y < 0 || snake[0]->y >= SIZE)
			break;
	}

	for(i = 0; i < length; i++)
		free(snake[i]);
	led_matrix_click_disable();
	i2c_release();
	spi_release();

	return 0;
}
