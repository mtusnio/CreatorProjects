#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/spi/spidev.h>
#include <math.h>

#include <core/common.h>
#include <core/i2c.h>
#include <click/common.h>
#include <click/joystick.h>
#include <click/led_matrix.h>

#define GAME_SPEED 480000000

#define BLINKS 3
#define BLINK_SPEED 550000000

#define TOLERANCE 25

#define SIZE 8
#define HALF_SIZE 4

#define SNAKE_SIZE 16

#define DIR_LEFT 0
#define DIR_UP 1
#define DIR_RIGHT 2
#define DIR_DOWN 3

typedef struct pos_t
{
	int x;
	int y;
} Pos;

Pos get_random_fruit_position(Pos ** snake, int snake_length)
{
	// The naive way of doing it - just randomize until you find a
	// position not occupied by the snake
	while(1)
	{
		int i, x, y, valid;

		valid = 1;
		x = rand() % SIZE;
		y = rand() % SIZE;

		for(i = 0; i < snake_length; i++)
		{
			if(snake[i]->x == x &&
				snake[i]->y == y)
			{
				valid = 0;
				break;
			}
		}

		if(valid)
		{
			Pos ret;
			ret.x = x;
			ret.y = y;
			return ret;
		}
	}
}

int main()
{
	int i, length, score, direction, extend;
	Pos * snake[SNAKE_SIZE];
	Pos fruit;
	uint8_t clear_columns[SIZE];
	for(i = 0; i < SIZE; i++)
		clear_columns[i] = 0;

	srand(time(0));

	if(i2c_init() < 0)
		return -1;

    i2c_select_bus(MIKROBUS_1);

	if(spi_init(MIKROBUS_2, SPI_MODE_3) < 0)
		return -1;

	if(spi_select_bus(MIKROBUS_2) < 0)
		return -1;

	if(led_matrix_click_enable() < 0)
		return -1;

	// Initialize game
	for(i = 0; i < SNAKE_SIZE; i++)
		snake[i] = 0;

	snake[0] = malloc(sizeof(Pos));
	snake[1] = malloc(sizeof(Pos));
	snake[2] = malloc(sizeof(Pos));

	snake[0]->x = 2;
	snake[0]->y = 3;

	snake[1]->x = 1;
	snake[1]->y = 3;

	snake[2]->x = 0;
	snake[2]->y = 3;

	direction = DIR_LEFT;
	length = 3;
	score = 0;
	extend = 0;

	fruit = get_random_fruit_position(snake, length);
	while (1)
	{
		// Controls
		int collided;
		int8_t joyX, joyY;
		struct timespec t;

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
		if(extend)
		{
			snake[length] = malloc(sizeof(Pos));
			snake[length]->x = snake[length - 1]->x;
			snake[length]->y = snake[length - 1]->y;
		}

		for(i = length - 1; i > 0; i--)
		{
			snake[i]->x = snake[i - 1]->x;
			snake[i]->y = snake[i - 1]->y;
		}

		if(extend)
		{
			extend = 0;
			length++;
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

		if(fruit.x == snake[0]->x &&
			fruit.y == snake[0]->y)
		{
			score++;
			fruit = get_random_fruit_position(snake, length);
			if(length < SNAKE_SIZE)
				extend = 1;
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

		columns[fruit.x] |= (1 << fruit.y);

		led_matrix_click_set(columns);

		t.tv_sec = 0;
		t.tv_nsec = GAME_SPEED/sqrt(score > 0 ? score : 1);
		nanosleep(&t, 0);

		// Collision detection
		if(snake[0]->x < 0 || snake[0]->x >= SIZE)
			collided = 1;

		if(snake[0]->y < 0 || snake[0]->y >= SIZE)
			collided = 1;

		for(i = 1; i < length; i++)
		{
			if(snake[i]->x == snake[0]->x &&
				snake[i]->y == snake[0]->y)
			{
				collided = 1;
				break;
			}
		}

		if(collided)
		{
			t.tv_sec = 0;
			t.tv_nsec = BLINK_SPEED;
			nanosleep(&t, 0);

			// Don't blink the fruit
			clear_columns[fruit.x] |= (1 << fruit.y);
			for(i = 0; i < BLINKS; i++)
			{
				led_matrix_click_set(clear_columns);
				nanosleep(&t, 0);
				led_matrix_click_set(columns);
				nanosleep(&t, 0);
			}

			led_matrix_click_display_number(score);
			sleep(5);
			break;
		}

	}

	printf("Score: %i\n", score);

	for(i = 0; i < length; i++)
		free(snake[i]);
	led_matrix_click_disable();
	i2c_release();
	spi_release();

	return 0;
}
