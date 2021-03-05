#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ddcPrint.h>

long WIDTH;
long HEIGHT;

void map_print(int** map);
char touching(int** map, int x, int y);
void right(int** map, int x, int y);
void left(int** map, int x, int y);
void up(int** map, int x, int y);
void down(int** map, int x, int y);
void set(int** map, int x, int y);

void map_draw_spot(int** map, int x, int y)
{
	cursor_move_to(y*2, x);
	ddPrintf("\x1b[38;2;0;0;255m██");
}

void map_print(int** map)
{
	cursor_clear();
	cursor_home();
	for (int i = 0; i <HEIGHT; i++)
	{
		for (int j = 0; j <WIDTH; j++)
		{
			if (map[i][j] == 3) printf("\x1b[38;2;255;0;0m██");
			else if (map[i][j] == 2) printf("\x1b[38;2;0;0;255m██");
			else if (map[i][j] == 1) printf("  ");
			else if (map[i][j] == 0) printf("\x1b[38;2;255;255;0m██");
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}

char touching(int** map, int x, int y)
{
	if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) return 1;
	int total = 0;
	if (map[x+1][y]   || x+1 == HEIGHT) total++;
	if (map[x-1][y]   || x-1 == -1) total++;
	if (map[x][y+1]   || y+1 == WIDTH) total++;
	if (map[x][y-1]   || y-1 == -1) total++;
	return total > 1;
}

void right(int** map, int x, int y)
{ if (!touching(map, x+1, y)) set(map, x+1, y); }
void left(int** map, int x, int y)
{ if (!touching(map, x-1, y)) set(map, x-1, y); }
void up(int** map, int x, int y)
{ if (!touching(map, x, y-1)) set(map, x, y-1); }
void down(int** map, int x, int y)
{ if (!touching(map, x, y+1)) set(map, x, y+1); }

void shuffle(void (*directions[4])(int** map, int x, int y))
{
	long n = 4;
	for (int i = 0; i < n; i++) 
	{
		size_t j = rand() % 4;
		void (*tmp)(int** map, int x, int y) = directions[j];
		directions[j] = directions[i];
		directions[i] = tmp;
	}
}

void set(int** map, int x, int y)
{
	void (*directions[4])(int** map, int x, int y) = {
		right, left, up, down,
	};
	shuffle(directions);
	map[x][y] = 1;
	for (int i = 0; i < 4; i++)
	{
		(*directions[i])(map, x, y);
	}
	for (int i = 0; i < 4; i++)
	{
		(*directions[i])(map, x, y);
	}
}

char solve(int** map, int x, int y)
{
	map_draw_spot(map, x, y);
	usleep(10000);
	if (map[x][y] == 3) return 1;
	map[x][y] = 2;
	if (map[x+1][y] == 1 || map[x+1][y] == 3)
		if (solve(map, x+1, y)) return 1;
	if (map[x-1][y] == 1 || map[x-1][y] == 3)
		if (solve(map, x-1, y)) return 1;
	if (map[x][y+1] == 1 || map[x][y+1] == 3)
		if (solve(map, x, y+1)) return 1;
	if (map[x][y-1] == 1 || map[x][y-1] == 3)
		if (solve(map, x, y-1)) return 1;
	return 0;
}

int main(void)
{
	srand(time(0));
	cursor_clear();
	cursor_home();
	WIDTH = cursor_get_width()/2;
	HEIGHT = cursor_get_height()-4;
	while (1)
	{
		int** map = malloc(sizeof(int*)*HEIGHT);
		for (int i = 0; i < HEIGHT; i++)
			map[i] = malloc(sizeof(int)*WIDTH);
		for (int i = 0; i < HEIGHT; i++)
			for (int j = 0; j <WIDTH; j++)
				map[i][j] = 0;
		for (int i = 0; i < HEIGHT; i++)
			map[i][0] = 1;
		for (int i = 0; i < HEIGHT; i++)
			map[i][WIDTH-1] = 1;
		for (int i = 0; i < WIDTH; i++)
			map[0][i] = 1;
		for (int i = 0; i < WIDTH; i++)
			map[HEIGHT-1][i] = 1;
		set(map, 2, 2);
		map[5][WIDTH-3] = 3;
		map_print(map);
		sleep(1);
		solve(map, 2, 2);
		sleep(1);
	}
	cursor_move_to(0, HEIGHT+2);
}
