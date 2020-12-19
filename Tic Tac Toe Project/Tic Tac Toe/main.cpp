#include<iostream>
#include <time.h>
#include<stdlib.h>
using namespace std;
// Values
char boardValues[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

//Functions
void board();
void clean_board();

int check_move(int);
int computerMove();

int checkComputerWin();
int checkPlayerWin();
int checkTie();

int main()
{
	int allMoves = 0; // both player and computer moves count
	int userMove; //users input
	int checkMove; //checks if user move is legal
	clean_board(); // just in case cleans the board

	cout << "Welcome to Tic Tac Toe!" << endl;
	cout << "Pick from 1 to 9 where you want to put X" << endl;
	do
	{
		board();
		// player goes
		cout << "Player goes: ";
		cin >> userMove;
		while (cin.fail()||check_move(userMove-1)) // needs check for numbers
		{
			if (cin.fail())
			{
				cout << "Use numbers from 1 to 9" << endl;
				cin.clear();
				cin.ignore(256, '\n');
			}
			else cout << "Wring move" << endl;
			cout << "Player goes: ";
			cin >> userMove;
			checkMove = check_move(userMove - 1);
		}
		boardValues[userMove - 1] = 'X';
		allMoves++;
		if (allMoves == 9) break;
		//computer goes
		cout << "Computers turn..." << endl << endl;
		boardValues[computerMove()] = 'O';
		allMoves++;
	} while ((checkComputerWin()==-1)&&(checkPlayerWin()==-1)&&(checkTie()==-1));
	board();
	//results
	if (checkPlayerWin() == 1) cout << "Player won";
	else if (checkComputerWin() == 2) cout << "Computer won";
	else if (checkTie() == 0 && checkPlayerWin()!=1) cout << "It's a tie";
	else if (checkTie() == 0 && checkComputerWin() != 2) cout << "It's a tie";
	cout << endl;
	clean_board();
	allMoves = 0;
	return 0; 
}
void board()
{
	system("cls");
	cout << "TIC TAC TOE" << endl << endl;
	cout << "     |     |     " << endl;
	cout << "  " << boardValues[0] << "  |  " <<  boardValues[1] << "  |  " <<  boardValues[2] << endl;
	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;
	cout << "  " << boardValues[3] << "  |  " <<  boardValues[4] << "  |  " <<  boardValues[5] << endl;
	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;
	cout << "  " << boardValues[6] << "  |  " <<  boardValues[7] << "  |  " <<  boardValues[8] << endl;
	cout << "     |     |     " << endl << endl;
}
void clean_board()
{
	boardValues[0] = '1';
	boardValues[1] = '2';
	boardValues[2] = '3';
	boardValues[3] = '4';
	boardValues[4] = '5';
	boardValues[5] = '6';
	boardValues[6] = '7';
	boardValues[7] = '8';
	boardValues[8] = '9';
}
int check_move(int userMove)
{
	if (boardValues[userMove] != 'O' && boardValues[userMove] != 'X') return 0;
	else return -1;
}
int checkPlayerWin()
{
	if (boardValues[0] == 'X' && boardValues[1] == 'X' && boardValues[2] == 'X') return 1;//horizonatal 1-2-3
	if (boardValues[3] == 'X' && boardValues[4] == 'X' && boardValues[5] == 'X') return 1;//horizonal 4-5-6
	if (boardValues[6] == 'X' && boardValues[7] == 'X' && boardValues[8] == 'X') return 1;//horizontal 7-8-9
	
	if (boardValues[0] == 'X' && boardValues[3] == 'X' && boardValues[6] == 'X') return 1;//vertical 1-4-7
	if (boardValues[1] == 'X' && boardValues[4] == 'X' && boardValues[7] == 'X') return 1;//vertical 2-5-8
	if (boardValues[2] == 'X' && boardValues[5] == 'X' && boardValues[8] == 'X') return 1;//vertical 3-6-9
	
	if (boardValues[0] == 'X' && boardValues[4] == 'X' && boardValues[8] == 'X') return 1;//diagonal 1-5-9
	if (boardValues[2] == 'X' && boardValues[4] == 'X' && boardValues[6] == 'X') return 1;//diagonal 3-5-7

	return -1;
}

int checkComputerWin()
{
	if (boardValues[0] == 'O' && boardValues[1] == 'O' && boardValues[2] == 'O') return 2;//horizonatal 1-2-3
	if (boardValues[3] == 'O' && boardValues[4] == 'O' && boardValues[5] == 'O') return 2;//horizonal 4-5-6
	if (boardValues[6] == 'O' && boardValues[7] == 'O' && boardValues[8] == 'O') return 2;//horizontal 7-8-9

	if (boardValues[0] == 'O' && boardValues[3] == 'O' && boardValues[6] == 'O') return 2;//vertical 1-4-7
	if (boardValues[1] == 'O' && boardValues[4] == 'O' && boardValues[7] == 'O') return 2;//vertical 2-5-8
	if (boardValues[2] == 'O' && boardValues[5] == 'O' && boardValues[8] == 'O') return 2;//vertical 3-6-9

	if (boardValues[0] == 'O' && boardValues[4] == 'O' && boardValues[8] == 'O') return 2;//diagonal 1-5-9
	if (boardValues[2] == 'O' && boardValues[4] == 'O' && boardValues[6] == 'O') return 2;//diagonal 3-5-7

	return -1;
}
int checkTie()
{
	if (boardValues[0] != '1' && boardValues[1] != '2' && boardValues[2] != '3' && boardValues[3] != '4' && boardValues[4] != '5' && boardValues[5] != '6' && boardValues[6] != '7' && boardValues[7] != '8' && boardValues[8] != '9')
		return 0;
	return -1;
}
int computerMove()
{
	int move = -1;
	do move = rand() % 9; 
	while (check_move(move) == -1);
	return move;
}
