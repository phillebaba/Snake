#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <unistd.h>

#define SPEED 80 //Delay between each snake movement
#define MAX_LENGTH 25 //Snake max length, which makes the end game
#define START_SIZE 3 //Snake initial size
#define START_X_POS 70 //Snake initial x value
#define START_Y_POS 5 //Snake initial y valiue

#define WIDTH 80 //Game window Width
#define HEIGHT 23 //Game window Height
#define Y_OFFSET 1 //Game window y offset

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define UP_CHAR 'w'
#define DOWN_CHAR 's'
#define LEFT_CHAR 'a'
#define RIGHT_CHAR 'd'


/*-----Structure-----*/
typedef struct coordinate {
	int x;
	int y;
} coordinate;


/*-----Functions-----*/
void goToXY(coordinate cursor);
void checkKeyStroke(int *direction);
void moveSnake(coordinate *snake, int direction, int size);
void rightShiftArray(coordinate *array, int size);
/*-----UI-----*/
void drawSnake(coordinate *snake, int size);
void drawPoint(coordinate *point);
void drawBorder();
/*-----Logic-----*/
bool checkCollision (coordinate *snake, int size);
bool checkPoint(coordinate *snake, coordinate point);


/*-----Main-----*/
int main() {
	
	
	srand(time(0)); //Initiates the psudorandom clock
	
	while (true) {/*Program Loop Start*/
		coordinate snake[MAX_LENGTH]; //Snake coordinate array set to max length size
		coordinate point; //Point coordinate variable 
		
		int snake_size = START_SIZE; //Snake Size
		int snake_direction = LEFT; //Snake Direction
		
		drawBorder(); //Draws game area border
		drawPoint(&point); //Places the first point
	
		int i;
		for (i = 0; i < snake_size; i++) { //Create the initial snake coordinates
			switch (snake_direction) {
				case UP: 
					snake[i].x = START_X_POS;
					snake[i].y = START_Y_POS+i;
					break;
				case DOWN: 
					snake[i].x = START_X_POS;
					snake[i].y = START_Y_POS-i;
					break;
				case LEFT: 
					snake[i].x = START_X_POS+i;
					snake[i].y = START_Y_POS;
					break;
				case RIGHT: 
					snake[i].x = START_X_POS-i;
					snake[i].y = START_Y_POS;
					break;
			}
		}
		
		while (true) {/*Game Loop Start*/
			drawSnake(snake, snake_size); //Places snake on the screen
			moveSnake(snake, snake_direction, snake_size); //Moves the snake in direction
			checkKeyStroke(&snake_direction); //Checks for keystrok
			
			if (checkPoint(snake, point) == true) {
				snake_size++;
				drawPoint(&point);
			}
			
			if (checkCollision(snake, snake_size) == true) {
				break;
			}
			
			usleep(100000); //delay
		}/*Game Loop End*/
		
	}/*Program Loop End*/
	return 0;
}


/*-----Definitions-----*/
void goToXY(coordinate cursor) {
  printf("\033[%i;%iH", cursor.y, cursor.x);
}
void checkKeyStroke (int *direction) {
	if (kbhit()) {	//Check if there is any input
		char input = getchar();
		switch (input) {
			case UP_CHAR:
				*direction = *direction != UP ? UP : DOWN;
				break;
			case DOWN_CHAR:
				*direction = *direction != DOWN ? DOWN : UP;
				break;
			case LEFT_CHAR:
				*direction = *direction != RIGHT ? LEFT : RIGHT;
				break;
			case RIGHT_CHAR:
				*direction = *direction != LEFT ? RIGHT : LEFT;
				break;
		}
	}
}
void moveSnake(coordinate *snake, int direction, int size) {
	rightShiftArray(snake, size); //Moves every index in array step to right leaving index 0 and 1 the same 
	switch (direction) {
		case UP:
			snake[0].y--;
			break;
		case DOWN:
			snake[0].y++;
			break;
		case LEFT:
			snake[0].x--;
			break;
		case RIGHT:
			snake[0].x++;
			break;
	}
}
void rightShiftArray(coordinate *array, int size) {
	int i;
	for (i = size; i > 0; i--) {
		array[i] = array[i-1];
	}
}
void drawSnake(coordinate *snake, int size) {
	if (snake[size].x != 0 && snake[size].y != 0) {
		goToXY(snake[size]);
		putchar('\0');
		goToXY(snake[0]);
		putchar('5');
	}
}
void drawPoint(coordinate *point) {
	point->x = rand() % (WIDTH-2+1);
	point->y = rand() % (HEIGHT-Y_OFFSET-1+Y_OFFSET+1);
	
	goToXY(*point);
	putchar('2');
}
void drawBorder() {
	//printf("Score: 0");
	
	int i;
	for (i = (WIDTH*Y_OFFSET)+1; i <= WIDTH * HEIGHT; i++) {
		coordinate current;
		current.x = (i%WIDTH) > 0 ? (i%WIDTH)-1 : (WIDTH)-1;
		current.y = (i%WIDTH) > 0 ? i/WIDTH : (i/WIDTH)-1;

		if (current.y == Y_OFFSET || current.y == HEIGHT-1) {
			goToXY(current);
			putchar('-');
		}
		
		else if(current.x == 0 || current.x == WIDTH-1) {
			goToXY(current);
			putchar('|');
		}
	}
}
bool checkCollision(coordinate *snake, int size) {
	int i;
	for (i = 1; i < size; i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) { //True when the front of the snake = another place on the snake
			return true; 
		}
	}
	
	if (snake[0].x == 0 || snake[0].x == WIDTH-1 || snake[0].y == Y_OFFSET || snake[0].y == HEIGHT-1) { //True when x is on border or y is on the border
		return true;
	} else {
		return false;
	} //Will return false in the end when no point is in the ohter
}
bool checkPoint(coordinate *snake, coordinate point) {
	if (point.x == snake[0].x && point.y == snake[0].y) { //True when the placed point = the front of the snake 
		return true;
	} else {
		return false;
	}
}


