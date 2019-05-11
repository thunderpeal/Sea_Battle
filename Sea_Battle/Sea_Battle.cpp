#include "Sea_Battle.h"

void SetColor(int text, ConsoleColor background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

bool Ships::is_it(int x, int y) {
	if (((x >= x1 && x <= x2) || (x >= x2 && x <= x1)) && ((y >= y1 && y <= y2) || (y >= y2 && y <= y1))) {
		return true;
	}
	else {
		return false;
	}
}

void Ships::set(int a1, int b1, int a2, int b2, int l) {
	x1 = a1;
	y1 = b1;
	x2 = a2;
	y2 = b2;

	if (y1 == y2) { horizontal_orientation = true; }
	else { horizontal_orientation = false; }

	if (y1 >= y2) { direction = 0; }
	else if (y2 > y1) { direction = 1; }
	if (x1 > x2) { direction = 2; }
	else if (x2 > x1) { direction = 3; }

	length = l;
	health_bar = l;
};

bool Ships::is_dead() {
	if (health_bar == 0) {
		return true;
	}
	else {
		return false;
	}
}

void Ships::death(int(&zones)[10][10], int k1) { //для бота 12
	SetColor(LightGray, Cyan);
	if (horizontal_orientation == true) {
		int k = -1;
		if (direction == 2) { k = 1; }
		for (int i = -1; i <= length; i++) {
			if (x1 - k * i < 0 || x1 - k * i >= 10) { continue; }
			for (int j = -1; j < 2; j++) {
				if (y1 - k * j > 9 || y1 - k * j < 0 || zones[y1 - k * j][x1 - k * i] == -2) { continue; }
				zones[y1 - k * j][x1 - k * i] = -1;
				setCursorPosition(k1 + x1 * 2 - k * 2 * i, 4 + y1 - k * j);
				cout << "o ";
			}
		}
	}
	else if (horizontal_orientation == false) {
		int k = -1;
		if (direction == 0) { k = 1; }
		for (int i = -1; i <= length; i++) {
			if (y1 - k * i < 0 || y1 - k * i >= 10) { continue; }
			for (int j = -1; j < 2; j++) {
				if (x1 - k * j > 9 || x1 - k * j < 0 || zones[y1 - k * i][x1 - k * j] == -2) { continue; }
				zones[y1 - k * i][x1 - k * j] = -1;
				setCursorPosition(k1 + x1 * 2 - 2 * k * j, 4 + y1 - k * i);
				cout << "o ";
			}
		}
	}
	SetColor(White, Cyan);
}

void Game::set_0(int(&a)[10][10]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) { a[i][j] = 0; }
	}
}

void Game::print_gamezone(int(&array)[10][10], const char alphabet[10], int i, bool is_dev) {
	for (int j = 0; j < 12; j++) {
		if (i == 0 && j >= 2) { cout << alphabet[j - 2] << " "; }
		else if (i == 1) { break; }
		else if ((i > 1 && j == 1) || (i == 0 && j < 2)) { cout << "  "; }
		else if (i > 1 && j == 0) {
			if (i - 1 != 10) { cout << i - 1 << " "; }
			else { cout << i - 1; }
		}
		else {
			if (is_dev == false) {
				switch (array[i - 2][j - 2]) {
				case 0:
					cout << ". ";
					break;
				case 1:
				case 2:
				case 3:
				case 4:
					cout << array[i - 2][j - 2] << " ";
					break;
				}
			}
			else {
				cout << ". ";
			}
		}
	}
}

bool Game::is_dead(Ships * a) {
	bool death = true;
	for (int i = 0; i < 10; i++) {
		if (a[i].health_bar != 0) {
			death = false;
			return false;
			break;
		}
	}
	if (death == true) {
		return true;
	}
}

void Game::auto_arrangement(int(&array)[10][10], const char alphabet[10], Ships * a) {
	int ships_1[4] = { 1, 2, 3, 4 };
	int ship_count = 0;
	int check = 0;
	int ship_length = 0;
	int x = 0, y = 0;

	while (true) {
		if (ships_1[ship_count] == 0) {
			ship_count += 1;
		}
		switch (ship_count) {
		case 0:
			ship_length = 4;
			break;
		case 1:
			ship_length = 3;
			break;
		case 2:
			ship_length = 2;
			break;
		case 3:
			ship_length = 1;
			break;
		}

		while (true) {
			x = rand() % 10;
			y = rand() % 10;
			bool* directions = new bool[4];
			directions[0] = true; directions[1] = true; directions[2] = true; directions[3] = true;

			bool can_be_placed = true;

			ship_location_checker(array, directions, &can_be_placed, x, y, ship_length);
			if (can_be_placed == false) {
				continue;
			}
			ships_1[ship_count] -= 1;
			int enter = -1;
			while (true) { //рандомно выбираем направление из доступных
				enter = rand() % 4;
				bool a = directions[enter];
				if (a == true) { break; }
			}

			switch (enter) {
			case 0:
				a[check].set(x, y, x, y - ship_length + 1, ship_length);
				for (int i = 0; i < ship_length; i++) {
					array[y - i][x] = ship_length;
				}
				break;
			case 1:
				a[check].set(x, y, x, y + ship_length - 1, ship_length);
				for (int i = 0; i < ship_length; i++) {
					array[y + i][x] = ship_length;
				}
				break;
			case 2:
				a[check].set(x, y, x - ship_length + 1, y, ship_length);
				for (int i = 0; i < ship_length; i++) {
					array[y][x - i] = ship_length;
				}
				break;
			case 3:
				a[check].set(x, y, x + ship_length - 1, y, ship_length);
				for (int i = 0; i < ship_length; i++) {
					array[y][x + i] = ship_length;
				}
				break;
			default:
				cout << "Some other key." << endl;
			}
			check += 1;
			break;
		}
		if (ships_1[ship_count] == 0 && ship_count == 3) {
			break;
		}
	}
}

void Game::ship_location_checker(int(&array)[10][10], bool dir[4], bool* c_b_p, int x, int y, int ship_length) {
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (x + j >= 10 || x + j < 0 || y + i >= 10 || y + i < 0) { //проверяет все клетки вокруг случайно 
				continue;					//выбранной на выход за границы поля
			}
			else if (array[y + i][x + j] != 0) { //проверят их же на наличие других объектов
				*c_b_p = false;
			}
		}
	}

	if (*c_b_p == true) {
		for (int i = 1; i < ship_length; i++) {//в какую сторону нельзя повернуть корабль
			if (y - i < 0) { dir[0] = false; }//вверх нельзя
			else {
				for (int j = -1; j < 2; j++) {
					if (x + j >= 10 || x + j < 0 || y - i - 1 < 0) { continue; }
					else if (array[y - i - 1][x + j] != 0) { dir[0] = false; }
				}
			}
			if (y + i >= 10) {
				dir[1] = false; //вниз нельзя
			}
			else {
				for (int j = -1; j < 2; j++) {
					if (x + j >= 10 || x + j < 0 || y + i + 1 >= 10) { continue; }
					else if (array[y + i + 1][x + j] != 0) { dir[1] = false; }
				}
			}
			if (x - i < 0) {
				dir[2] = false;
			} //влево нельзя
			else {
				for (int j = -1; j < 2; j++) {
					if (y + j >= 10 || y + j < 0 || x - i - 1 < 0) { continue; }
					else if (array[y + j][x - i - 1] != 0) { dir[2] = false; }
				}
			}

			if (x + i >= 10) {
				dir[3] = false; //вправо нельзя
			}
			else {
				for (int j = -1; j < 2; j++) {
					if (y + j >= 10 || y + j < 0 || x + i + 1 >= 10) { continue; }
					else if (array[y + j][x + i + 1] != 0) { dir[3] = false; }
				}
			}
		}
	}
	if (dir[0] == false && dir[1] == false && dir[2] == false && dir[3] == false) {
		*c_b_p = false;
	}
}

int Game::answer(int x, int y, int x1, int y1) {
	int answer;
	while (true) {
		do {
			cin.clear();
			int L = cin.rdbuf()->in_avail();
			cin.ignore(L);
			cin >> answer;
			setCursorPosition(x, y);
			cout << "                                           ";
			if (!cin) {
				setCursorPosition(x, y);
				SetColor(LightRed, Cyan);
				cout << "Введите число !" << endl;
				setCursorPosition(x1, y1);
				SetColor(White, Cyan);
				cout << "                                                                                          ";
				setCursorPosition(x1, y1);
			}
		} while (!cin);
		if (answer == 1 || answer == 0) {
			break;
		}
		else {
			setCursorPosition(x, y);
			SetColor(LightRed, Cyan);
			cout << "Неверный формат ввода! Введите 1 или 0.";
			SetColor(White, Cyan);
			setCursorPosition(x1, y1);
			cout << "                                         ";
			setCursorPosition(x1, y1);
		}
	}
	return answer;
}

void Decor::x(int x, int y) {
	setCursorPosition(x, y);
	cout << "||";
}

void Decor::SeaBattle() {
	SetColor(LightRed, Red);

	x(62, 3);

	x(11, 4); x(16, 4); x(21, 4); x(23, 4); x(28, 4); x(29, 4); x(31, 4); x(37, 4); x(39, 4); x(44, 4);
	x(48, 4); x(53, 4); x(55, 4); x(60, 4); x(64, 4);

	x(11, 5); x(12, 5); x(15, 5); x(16, 5); x(20, 5); x(24, 5); x(28, 5); x(32, 5);  x(36, 5);  x(40, 5);
	x(44, 5); x(47, 5); x(52, 5); x(56, 5); x(60, 5); x(64, 5);

	x(11, 6); setCursorPosition(14, 6); cout << "|"; x(16, 6); x(20, 6); x(24, 6); x(28, 6); x(30, 6);
	x(31, 6); x(36, 6); x(44, 6); x(46, 6); x(52, 6); x(56, 6); x(60, 6); x(63, 6); x(64, 6);

	x(11, 7); x(16, 7); x(20, 7); x(24, 7); x(28, 7); x(36, 7); x(40, 7); x(44, 7); x(47, 7);
	x(52, 7); x(56, 7); x(60, 7); x(61, 7); x(64, 7);

	x(11, 8);  x(16, 8); x(21, 8); x(23, 8);  x(28, 8); x(37, 8); x(39, 8); x(44, 8); x(48, 8);
	x(53, 8);  x(55, 8); x(60, 8); x(64, 8);

	x(46, 10);

	x(28, 11); x(30, 11); x(31, 11); x(37, 11);  x(39, 11);  x(44, 11); x(48, 11);

	x(28, 12); x(36, 12); x(40, 12); x(44, 12); x(48, 12);

	x(28, 13); x(30, 13); x(31, 13); x(36, 13);  x(40, 13); x(44, 13); x(47, 13); x(48, 13);

	x(28, 14); x(32, 14); x(36, 14); x(40, 14); x(44, 14); x(45, 14);  x(48, 14);

	x(28, 15); x(30, 15); x(31, 15); x(37, 15); x(39, 15);  x(44, 15);  x(48, 15);
	SetColor(White, Cyan);
}

void Decor::game_beginning_text() {
	setCursorPosition(0, 16);
	cout << "     Вы находитесь в режиме боя. Право первого хода определяется жребием." << endl;
	cout << "     Ваши корабли располагаются на поле слева. Удачи!";
	Sleep(7000);
	for (int i = 0; i < 5; i++) {
		Sleep(250);
		setCursorPosition(0, 16);
		cout << "     Вы находитесь в режиме боя. Право первого хода определяется жребием." << endl;
		cout << "     Ваши корабли располагаются на поле слева. Удачи!";
		Sleep(250);
		setCursorPosition(0, 16);
		cout << "                                                                           " << endl;
		cout << "                                                                           " << endl;
	}
}

void Decor::endgame(int(&zones)[10][10], int c_o_m, bool victory) {
	if (victory == false) {
		cout << endl << endl;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (zones[j][i] != 0 && zones[j][i] != -1 && zones[j][i] != -2) {
					setCursorPosition(48 + 1 + i * 2, 4 + j);
					cout << zones[j][i];
				}
			}
		}
	}
	
	setCursorPosition(9, 19);
	cout << "Общая продолжительность игры составила " << c_o_m << " ход(-a)(-ов).";
	setCursorPosition(9, 20);
	cout << "Спасибо за использование программы!";
	setCursorPosition(9, 22);
	cout << "Для выхода нажмите любую клавишу...";
	int c;
	c = _getch();
}

void Decor::hit(int x, int y, int x1, int u) {  //47 или 12 для u
	setCursorPosition(0, 18);
	cout << "                                                ";
	SetColor(LightRed, Cyan);
	setCursorPosition(u + 1 + x * 2, 4 + y);
	Sleep(1500);
	cout << "x ";
	SetColor(White, Cyan);

	setCursorPosition(19 + x1, 15);
	cout << "Ранен!";
	Sleep(1500);
	setCursorPosition(19 + x1 - 2, 15);
	cout << "                 ";
}

void Decor::miss(int x, int y, int x1, int u) {
	setCursorPosition(0, 18);
	cout << "                                                ";
	setCursorPosition(u + 1 + x * 2, 4 + y);
	Sleep(1500);
	setCursorPosition(u + 1 + x * 2, 4 + y);
	SetColor(LightGray, Cyan);
	cout << "о";
	SetColor(White, Cyan);

	setCursorPosition(19 + x1, 15);
	cout << "Мимо!";
	Sleep(1500);
	setCursorPosition(19 + x1 - 2, 15);
	cout << "                            ";
}

void Decor::kill(int x, int y, int x1, int u) {
	setCursorPosition(0, 18);
	cout << "                                                ";
	SetColor(LightRed, Cyan);
	setCursorPosition(u + 1 + x * 2, 4 + y);
	Sleep(1500);
	cout << "x ";
	SetColor(White, Cyan);

	setCursorPosition(19 + x1, 15);
	cout << "Убит!";
	Sleep(3000);
	setCursorPosition(19 + x1 - 2, 15);
	cout << "                            ";
}

void Decor::instructions() {
	setCursorPosition(0, 19);
	cout << "	   Введите свое имя: ";
	cin >> this->name;
	setCursorPosition(0, 19);
	cout << "                                                                          ";
	setCursorPosition(0, 19);
	cout << "	   Добро пожаловать в игру, " << name << "!" << endl << endl;
	cout << "	   Нажмите клавишу F1, чтобы открыть инструкцию;" << endl;
	cout << "	   Нажмите клавишу F2, чтобы открыть информацию об авторе;" << endl;


	cout << "	   или нажмите любую другую клавишу, чтобы начать игру . . .";
	int c;
	while (true) {
		c = _getch();
		if (c == 0) {
			c = _getch();
			if (c == 59) {
				system("Инструкция.txt");
			}
			else if (c == 60) {
				system("Автор.txt");
			}
		}
		else break;
	}
}

void Player::player_arrangement_type(int(&array)[10][10], const char alphabet[10], Ships * a) {
	int ships_1[4] = { 1, 2, 3, 4 };
	int ship_length = 0;
	int ship_count = 0;
	int number = 0;
	int number_letter = 0;
	char letter;
	int check = 0;

	cout << endl << endl;
	for (int i = 0; i < 12; i++) {
		cout << "                        "; //отступ от границы слева 
		print_gamezone(array, alphabet, i, false);
		cout << endl;
	}
	cout << endl << endl;
	cout << "     Это режим расстановки ваших кораблей." << endl;
	cout << "     Введите точку (например, Ж4), где хотите поставить 'голову' корабля, " << endl;
	cout << "     а затем укажите стрелочкой направление тела корабля." << endl << endl;

	while (true) {
		if (ships_1[ship_count] == 0) {
			ship_count += 1;
		}
		setCursorPosition(0, 20);
		cout << "                                                                ";
		setCursorPosition(0, 20);
		switch (ship_count) {
		case 0:
			ship_length = 4;
			cout << "     Сейчас расположите линкор: ";
			break;
		case 1:
			ship_length = 3;
			cout << "     Теперь расположите крейсер: ";
			break;
		case 2:
			ship_length = 2;
			cout << "     Очередь расположить эсминец: ";
			break;
		case 3:
			ship_length = 1;
			cout << "     Сейчас расположите катер: ";
			break;
		}

		cin >> letter >> number;
		if (letter == 'К') {
			number_letter = letter - 'А' - 1;
		}
		else {
			number_letter = letter - 'А';
		}
		number -= 1;
		

		setCursorPosition(0, 21);
		cout << "                                                                                 ";
		bool can_be_placed = true;
		bool* directions = new bool[4];
		directions[0] = true; directions[1] = true; directions[2] = true; directions[3] = true;

		ship_location_checker(array, directions, &can_be_placed, number_letter, number, ship_length);

		if (can_be_placed == false) {
			setCursorPosition(0, 21);
			cout << "     Вы не можете расположить корабль в этой точке. Попробуйте еще раз.";
			continue;
		}

		else { ships_1[ship_count] -= 1; }
		if (ship_length == 1) {
			a[check].set(number_letter, number, number_letter, number - ship_length + 1, ship_length);
			check += 1;
		}
		array[number][number_letter] = ship_length; //закидываем в массив нашего поля голову корабля
		setCursorPosition(28 + number_letter * 2, 4 + number); //*2 учитывает пробелы в выводимом массиве
		cout << array[number][number_letter];


		if (ship_length != 1) {
			setCursorPosition(0, 20);
			cout << "     Теперь введите направление расположения корабля стрелкой: ";
			bool exit = false;
			while (true) {
				if (exit == true) { break; }
				int KeyStroke;
				KeyStroke = _getch();

				if (KeyStroke == 224)
				{
					KeyStroke = _getch(); // Even though there are 2 getch() it reads one keystroke
					switch (KeyStroke)
					{
					case UP_ARROW:
						if (directions[0] == true) {
							a[check].set(number_letter, number, number_letter, number - ship_length + 1, ship_length);
							check += 1;
							for (int i = 1; i < ship_length; i++) {
								array[number - i][number_letter] = ship_length;
								setCursorPosition(28 + number_letter * 2, 4 + number - i);
								cout << array[number - i][number_letter];
								exit = true;
							}
						}
						else {
							setCursorPosition(0, 21);
							cout << "     Вы не можете расположить корабль в эту сторону. Попробуйте еще раз.";
							setCursorPosition(63, 20);
						}
						break;
					case DOWN_ARROW:
						if (directions[1] == true) {
							a[check].set(number_letter, number, number_letter, number + ship_length - 1, ship_length);
							check += 1;
							for (int i = 1; i < ship_length; i++) {
								array[number + i][number_letter] = ship_length;
								setCursorPosition(28 + number_letter * 2, 4 + number + i);
								cout << array[number + i][number_letter];
								exit = true;
							}
						}
						else {
							setCursorPosition(0, 21);
							cout << "     Вы не можете расположить корабль в эту сторону. Попробуйте еще раз.";
							setCursorPosition(63, 20);
						}
						break;
					case LEFT_ARROW:
						if (directions[2] == true) {
							a[check].set(number_letter, number, number_letter - ship_length + 1, number, ship_length);
							check += 1;
							for (int i = 1; i < ship_length; i++) {
								array[number][number_letter - i] = ship_length;
								setCursorPosition(28 + number_letter * 2 - 2 * i, 4 + number);
								cout << array[number][number_letter - i];
								exit = true;
							}
						}
						else {
							setCursorPosition(0, 21);
							cout << "     Вы не можете расположить корабль в эту сторону. Попробуйте еще раз.";
							setCursorPosition(63, 20);
						}
						break;
					case RIGHT_ARROW:
						if (directions[3] == true) {
							a[check].set(number_letter, number, number_letter + ship_length - 1, number, ship_length);
							check += 1;
							for (int i = 1; i < ship_length; i++) {
								array[number][number_letter + i] = ship_length;
								setCursorPosition(28 + number_letter * 2 + 2 * i, 4 + number);
								cout << array[number][number_letter + i];
								exit = true;
							}
						}
						else {
							setCursorPosition(0, 21);
							cout << "     Вы не можете расположить корабль в эту сторону. Попробуйте еще раз.";
							setCursorPosition(63, 20);
						}
						break;
					default:
						cout << "Some other key." << endl;
					}
				}
				else cout << KeyStroke << endl;
			}
			setCursorPosition(0, 21);
			cout << "                                                                                         ";
		}
		if (ships_1[ship_count] == 0 && ship_count == 3) { break; }
	}
}

void Player::arrangement(int(&zones)[10][10], const char alphabet[10], Ships * player1_ships) {
	while (true) {
		setCursorPosition(16, 7);
		cout << "Желаете расставить корабли самостоятельно" << endl <<
			endl << "             или использовать автоматическую расстановку?(1/0) ";

		if (answer(35, 11, 63, 9) == 1) {
			system("cls");
			player_arrangement_type(zones, alphabet, player1_ships);
			setCursorPosition(13, 22);
			cout << "Подтвердить установку или повторить с нуля?(1/0) ";
			if (answer(49, 24, 48, 22) == 0) {
				system("cls");
				set_0(zones);
				continue;
			}
			else { break; }
		}
		else {
			system("cls");
			auto_arrangement(zones, alphabet, player1_ships);
			cout << endl << endl;
			for (int i = 0; i < 12; i++) {
				cout << "                        "; //отступ от границы слева 
				print_gamezone(zones, alphabet, i, false);
				cout << endl;
			}

			setCursorPosition(13, 16);
			cout << "Подтвердить установку или повторить с нуля?(1/0) ";
			if (answer(35, 18, 62, 16) == 0) {
				system("cls");
				set_0(zones);
				continue;
			}
			else {
				setCursorPosition(0, 18);
				cout << "                          Расстановка окончена!";
				cout << endl << "          Компьютер также расставил свои корабли. Приступим к игре. ";
				Sleep(3500);
				break;
			}
		}
	}
}

void Player::move(bool* is_prev_success, int* sequence, int(&zones)[10][10], Ships * ships, Decor decor) {

	int number = 0;
	int number_letter = 0;
	char letter;
	if (*is_prev_success == true) {
		setCursorPosition(48, 17);
		cout << "    ";
		setCursorPosition(48, 17);
	}
	else {
		setCursorPosition(0, 17);
		cout << "                                                                       ";
		setCursorPosition(9, 17);
		cout << "Ваш ход. Введите координаты для атаки: ";
	}

	cin >> letter >> number;
	if (letter == 'К') {
		number_letter = letter - 'А' - 1;
	}
	else {
		number_letter = letter - 'А';
	}
	
	if (zones[number - 1][number_letter] == 1 || zones[number - 1][number_letter] == 2
		|| zones[number - 1][number_letter] == 3 || zones[number - 1][number_letter] == 4) {
		zones[number - 1][number_letter] = -2;
		for (int i = 0; i < 10; i++) {
			if (ships[i].is_it(number_letter, number - 1) == true) {
				ships[i].health_bar -= 1;
				if (ships[i].is_dead() == true) {
					decor.kill(number_letter, number - 1, 37, 48);
					ships[i].death(zones, 49);

				}
				else {
					decor.hit(number_letter, number - 1, 36, 48);
				}
				break;
			}
		}
		*sequence = 1;
		*is_prev_success = true;
	}
	else if (zones[number - 1][number_letter] == -2) {
		setCursorPosition(48 + 1 + number_letter * 2, 4 + number - 1);
		Sleep(1500);

		setCursorPosition(56, 15);
		cout << "Мимо!";
		Sleep(1500);
		setCursorPosition(55, 15);
		cout << "              ";
		
		*sequence = 0;
		*is_prev_success = false;
	}
	else {
		decor.miss(number_letter, number - 1, 37, 48);
		*sequence = 0;
		*is_prev_success = false;
	}
}

Player::Player() {
	set_0(zones);
	arrangement(zones, alphabet, ships);
}

Decor::Decor() {
	SeaBattle();
	instructions();
}

Computer::Computer() {
	set_0(zones);
	auto_arrangement(zones, alphabet, ships);
}

Ships* Computer::move(bool* is_prev_success_comp, bool* is_prev_success_comp_2, int* sequence, int* enter, bool* help,
	int* x, int* y, int* x1, int* y1, int(&zones)[10][10], bool(*directions_con), int* count_of_hits, int* x_supreme,
	int* y_supreme, Ships * current_ship, Ships * ships, Decor decor) {

	if (*is_prev_success_comp_2 == false) {
		setCursorPosition(0, 17);
		cout << "                                                                              ";
		setCursorPosition(9, 17);
		cout << "Ход противника. Дождитесь окончания.";
	}
	if (*is_prev_success_comp == false) {
		int cheat = 0;
		cheat = rand() % 13;
		while (true) {
			*x = rand() % 10;
			*y = rand() % 10;

			if (cheat == 7 && zones[*y][*x] == 0) {
				continue;
			}
			if (zones[*y][*x] == -2 || zones[*y][*x] == -1 // || zones[*y][*x] == 0
				) {
				continue;
			}
			else {
				if (zones[*y][*x] == 1 || zones[*y][*x] == 2 || zones[*y][*x] == 3 || zones[*y][*x] == 4) {
					zones[*y][*x] = -2;

					for (int i = 0; i < 10; i++) {
						if (ships[i].is_it(*x, *y) == true) {
							ships[i].health_bar -= 1;
							if (ships[i].is_dead() == true) {
								decor.kill(*x, *y, 0, 12);
								ships[i].death(zones, 13);
								*is_prev_success_comp = false;
								*is_prev_success_comp_2 = true;
							}
							else {
								*x_supreme = *x;
								*y_supreme = *y;
								*count_of_hits += 1;
								decor.hit(*x, *y, -1, 12);
								current_ship = &ships[i];
								setCursorPosition(0, 29);
								*is_prev_success_comp = true;
								*is_prev_success_comp_2 = true;
							}
							break;
						}
					}
					*sequence = 0;
				}

				else {
					zones[*y][*x] = -1;
					decor.miss(*x, *y, 0, 12);
					*sequence = 1;
					*is_prev_success_comp = false;
					*is_prev_success_comp_2 = false;
				}
				break;
			}
		}
	}
	else {
		if (*help == false) {
			if (*y - 1 < 0 || zones[*y - 1][*x] == -2 || zones[*y - 1][*x] == -1) {
				directions_con[0] = false;
			}
			if (*y + 1 >= 10 || zones[*y + 1][*x] == -2 || zones[*y + 1][*x] == -1) {
				directions_con[1] = false;
			}
			if (*x - 1 < 0 || zones[*y][*x - 1] == -2 || zones[*y][*x - 1] == -1) {
				directions_con[2] = false;
			}
			if (*x + 1 >= 10 || zones[*y][*x + 1] == -2 || zones[*y][*x + 1] == -1) {
				directions_con[3] = false;
			}
			*help = true;

			while (true) {
				//рандомно выбираем направление из доступных
				*enter = rand() % 4;
				bool a = directions_con[*enter];
				if (a == true) {
					break;
				}
			}
		}

		switch (*enter) {
		case 0:
			*y1 = *y - 1;
			*x1 = *x;
			break;
		case 1:
			*y1 = *y + 1;
			*x1 = *x;
			break;
		case 2:
			*x1 = *x - 1;
			*y1 = *y;
			break;
		case 3:
			*x1 = *x + 1;
			*y1 = *y;
			break;
		}

		if (zones[*y1][*x1] == 1 || zones[*y1][*x1] == 2 || zones[*y1][*x1] == 3 || zones[*y1][*x1] == 4) {
			zones[*y1][*x1] = -2;
			*x = *x1;
			*y = *y1;
			*count_of_hits += 1;
			if (*count_of_hits >= 2) {
				if (*enter == 0 || *enter == 1) {
					directions_con[2] = false;
					directions_con[3] = false;
				}
				else if (*enter == 2 || *enter == 3) {
					directions_con[0] = false;
					directions_con[1] = false;
				}
			}
			current_ship->health_bar -= 1;
			if (current_ship->is_dead() == true) {
				decor.kill(*x, *y, 0, 12);
				current_ship->death(zones, 13);
				*is_prev_success_comp = false;
				*is_prev_success_comp_2 = true;
				directions_con[0] = true; directions_con[1] = true; directions_con[2] = true; directions_con[3] = true;
				current_ship = 0;
				*enter = -1;
				*count_of_hits = 0;
				*help = false;
				*x_supreme = 0;
				*y_supreme = 0;
			}
			else {
				decor.hit(*x, *y, -1, 12);
				*is_prev_success_comp = true;
				*is_prev_success_comp_2 = true;
			}
			*sequence = 0;
		}

		else {

			directions_con[*enter] = false;
			while (true) { //рандомно выбираем направление из доступных
				*enter = rand() % 4;
				bool a = directions_con[*enter];
				if (a == true) { break; }
			}
			*x = *x_supreme;
			*y = *y_supreme;
			zones[*y1][*x1] = -1;

			decor.miss(*x1, *y1, 0, 12);
			*is_prev_success_comp = true;
			*is_prev_success_comp_2 = false;
			*sequence = 1;
		}
		if (((*x1 + 1 > 9) && (*enter == 3)) || ((*x1 - 1 < 0) && (*enter == 2))
			|| ((*y1 - 1 < 0) && (*enter == 0)) || ((*y1 + 1 > 9) && (*enter == 1))) {
			*x = *x_supreme;
			*y = *y_supreme;
			if (*enter == 0) { *enter = 1; }
			else if (*enter == 1) { *enter = 0; }
			if (*enter == 3) { *enter = 2; }
			else if (*enter == 2) { *enter = 3; }
		}
	}
	return current_ship;
}
