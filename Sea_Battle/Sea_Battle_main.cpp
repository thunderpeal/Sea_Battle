#include "Sea_Battle.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=77 lines=27");
	system("title Морской бой");
	srand(time(NULL));

	Decor decor;
	system("cls");

	Game game;
	Computer player0;
	Player player1;
	system("cls");

	cout << endl << endl; //отступ от границы сверху
	for (int i = 0; i < 12; i++) {
		cout << "         "; //отступ от границы слева 
		game.print_gamezone(player1.zones, alphabet, i, false);
		cout << "            ";
		game.print_gamezone(player0.zones, alphabet, i, true);
		cout << endl;
	}
	cout << endl << endl << endl; //отступ снизу

	int count_of_moves = 0;
	int counter = 0;
	bool victory = false;
	int sequence = rand() % 2;
	bool is_prev_success = false;
	bool is_prev_success_comp = false;
	bool is_prev_success_comp_2 = false; //для вывода надписи об очередности хода

	int x = 0, y = 0;
	int y1 = 0, x1 = 0;
	int x_supreme = 0, y_supreme = 0;
	Ships* current_ship = 0;
	bool help = false;
	int enter = -1;
	int count_of_hits = 0;
	bool directions_con[4] = { true, true, true, true };

	decor.game_beginning_text();
	setCursorPosition(0, 0);
	cout << "Ходов " << count_of_moves;

	while (true) {
		setCursorPosition(6, 0);
		cout << count_of_moves << "   ";
		if (sequence == 1) {
			player1.move(&is_prev_success, &sequence, player0.zones, player0.ships, decor);
		}
		else {
			current_ship = player0.move(&is_prev_success_comp, &is_prev_success_comp_2, &sequence, &enter, &help, &x, &y, &x1, &y1,
				player1.zones, directions_con, &count_of_hits, &x_supreme, &y_supreme, current_ship, player1.ships, decor);
		}

		if (player0.is_dead(player0.ships) == true) {
			victory = true;
			setCursorPosition(9, 17);
			cout << "                                                                           ";
			setCursorPosition(9, 17);
			cout << "Вы выиграли, "<<decor.s<<"! Поздравляем!!!";
			break;
		}
		else if (player1.is_dead(player1.ships) == true) {
			setCursorPosition(9, 17);
			cout << "                                                                           ";
			setCursorPosition(9, 17);
			cout << "К сожалению, " << decor.s << ", Вы проиграли!";
			break;
		}
		count_of_moves += 1;
	}

	decor.endgame(player0.zones, count_of_moves, victory);
	return 0;
}