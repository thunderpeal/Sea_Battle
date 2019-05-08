#pragma once

#ifndef SEA_BATTLE_H
#define SEA_BATTLE_H
#include <iostream>
#include <ctime>
#include <conio.h>
#include <Windows.h>
using namespace std;

#define UP_ARROW 72
#define LEFT_ARROW 75
#define DOWN_ARROW 80
#define RIGHT_ARROW 77

const char alphabet[10] = { 'A','B','C','D','E','F','G','H','I','J' };
const int NotUsed = system("color 3F");
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void SetColor(int text, ConsoleColor background);

void setCursorPosition(int x, int y);

class Ships {
public:
	int x1, y1, x2, y2;
	int length;
	bool horizontal_orientation;
	int direction; //0 вверх 1 вниз 2 влево 3 вправо
	int health_bar;

	void set(int a1, int b1, int a2, int b2, int l);

	bool is_it(int x, int y);

	bool is_dead();

	void death(int(&zones)[10][10], int k1);
};

class Game {
public:
	int zones[10][10];

	void set_0(int(&a)[10][10]);

	void print_gamezone(int(&array)[10][10], const char alphabet[10], int i, bool is_dev);

	bool is_dead(Ships* a);

	void auto_arrangement(int(&array)[10][10], const char alphabet[10], Ships* a);

	void ship_location_checker(int(&array)[10][10], bool dir[4], bool* c_b_p, int x, int y, int ship_length);

	int answer(int x, int y, int x1, int y1);
};

class Decor :public Game {
public:
	void x(int x, int y);

	void SeaBattle();

	void game_beginning_text();

	void hit(int x, int y, int x1, int u);

	void miss(int x, int y, int x1, int u);

	void kill(int x, int y, int x1, int u);

	void instructions();

	Decor();
};

class Player : public Game {
public:
	Ships ships[10];
	Player();

	void player_arrangement_type(int(&array)[10][10], const char alphabet[10], Ships* a);

	void arrangement(int(&zones)[10][10], const char alphabet[10], Ships* player1_ships);

	void move(bool* is_prev_success, int* sequence, int(&zones)[10][10], Ships* ships, Decor decor);
};

class Computer : public Game {
public:
	Ships ships[10];
	Computer();

	Ships* move(bool* is_prev_success_comp, bool* is_prev_success_comp_2, int* sequence, int* enter, bool* help,
		int* x, int* y, int* x1, int* y1, int(&zones)[10][10], bool(*directions_con), int* count_of_hits, int* x_supreme,
		int* y_supreme, Ships* current_ship, Ships* ships, Decor decor);
};
#endif