#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
//Values
char boardValues[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
//Functions
void board();
int check_move(int);
int checkWin(char);
int checkTie();
void result();
void clean_board();
int checkResults();
using namespace std;

int main()
{
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock. Error: #" << wsResult << endl;
		return -1;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket. Error: #" << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)& hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server. Error: #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	int userInput;
	string userMove;
	int computerInput;
	
	cout << "Welcome to Tic Tac Toe!" << endl;
	cout << "Press Enter to play!" << endl;
	if (cin.get() == '\n')
	{
		//First loop for a client until he finishes his playthough
		while (true)
		{
			//Second loop for Tic Tac Toe
			do
			{
				board();
				// Prompt the user for some text
				cout << "Player's move > ";
				cin >> userInput;
				while (cin.fail() || check_move(userInput - 1)|| userInput>9||userInput<1)
				{
					cout << "Wrong move" << endl;
					cin.clear();
					cin.ignore(256, '\n');
					cout << "Player's move > ";
					cin >> userInput;
				}
				boardValues[userInput - 1] = 'X';
				board(); // show current board to client
				userMove = to_string(userInput);
				// Send the text
				if (userMove.size() > 0)
				{
					int sendResult = send(sock, userMove.c_str(), userMove.size() + 1, 0);

					if (sendResult != SOCKET_ERROR)
					{
						if (checkResults() == 0) break;
						// Wait for response
						ZeroMemory(buf, 4096); //fills a block of memory with 0
						int bytesReceived = recv(sock, buf, 4096, 0);
						if (bytesReceived > 0)
						{
							// Echo response to console
							cout << "Computer's move > " << string(buf, 0, bytesReceived) << endl;
							Sleep(500);
							computerInput = stoi(string(buf, 0, bytesReceived));
							boardValues[computerInput - 1] = 'O';
							board();
						}
					}
				}
			} while ((checkWin('X') == -1) && (checkWin('O') == -1) && (checkTie() == -1));
			//results
			result();
			clean_board();
			cout << "Do you want to play again? Write yes if you want to play again" << endl;
			cin >> userMove;
			if (userMove == "yes") int sendResult = send(sock, userMove.c_str(), userMove.size() + 1, 0);
			else break;
		}
	}
	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
	return 0;
}
int check_move(int userMove)
{
	if (boardValues[userMove] != 'O' && boardValues[userMove] != 'X') return 0;
	else return -1;
}
int checkWin(char move)
{
	if (boardValues[0] == move && boardValues[1] == move && boardValues[2] == move) return 1;//horizonatal 1-2-3
	if (boardValues[3] == move && boardValues[4] == move && boardValues[5] == move) return 1;//horizonal 4-5-6
	if (boardValues[6] == move && boardValues[7] == move && boardValues[8] == move) return 1;//horizontal 7-8-9
	if (boardValues[0] == move && boardValues[3] == move && boardValues[6] == move) return 1;//vertical 1-4-7
	if (boardValues[1] == move && boardValues[4] == move && boardValues[7] == move) return 1;//vertical 2-5-8
	if (boardValues[2] == move && boardValues[5] == move && boardValues[8] == move) return 1;//vertical 3-6-9
	if (boardValues[0] == move && boardValues[4] == move && boardValues[8] == move) return 1;//diagonal 1-5-9
	if (boardValues[2] == move && boardValues[4] == move && boardValues[6] == move) return 1;//diagonal 3-5-7

	return -1;
}
int checkTie()
{
	if (boardValues[0] != '1' && boardValues[1] != '2' && boardValues[2] != '3' && boardValues[3] != '4' && boardValues[4] != '5' && boardValues[5] != '6' && boardValues[6] != '7' && boardValues[7] != '8' && boardValues[8] != '9') return 0;
	else return -1;
}
void board()
{
	system("cls");
	cout << "TIC TAC TOE" << endl << endl;
	cout << "     |     |     " << endl;
	cout << "  " << boardValues[0] << "  |  " << boardValues[1] << "  |  " << boardValues[2] << endl;
	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;
	cout << "  " << boardValues[3] << "  |  " << boardValues[4] << "  |  " << boardValues[5] << endl;
	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;
	cout << "  " << boardValues[6] << "  |  " << boardValues[7] << "  |  " << boardValues[8] << endl;
	cout << "     |     |     " << endl << endl;
}
void result()
{
	if (checkWin('X') == 1) cout << "Player won";
	else if (checkWin('O') == 1) cout << "Computer won";
	else if (checkTie() == 0 && checkWin('X') != 1 && checkWin('O') != 1) cout << "It's a tie";
	cout << endl;
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
int checkResults()
{
	if (checkWin('X') == 1) return 0;
	else if (checkWin('O') == 1) return 0;
	else if (checkTie() == 0 && checkWin('X') != 1 && checkWin('O') != 1) return 0;
	else return -1;
}