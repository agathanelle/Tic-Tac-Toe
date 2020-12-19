#include <winsock2.h>
#include <string>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#define PORT 54000
//Values
char boardValues[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
//Functions
int checkPossibleWin(char);
int check_move(int);
int computerMove();
int checkWin(char);
int checkTie();
void result();
int checkResults();
void clean_board();
using namespace std;
int main()
{
	string ipAddress = "127.0.0.1";
	//Initialize winsock
	WSADATA wsData;
	WORD versionRequested = MAKEWORD(2, 2);
	int error = WSAStartup(versionRequested, &wsData);
	if (error != 0)
	{
		cout << "WSAStartup failed. Error: " << error << endl;
		return -1;
	}
	//Create a socket
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		cerr << "Can't create a socket!" << endl;
		return -1;
	}
	//Bind the ip address and port to a socket
	SOCKADDR_IN host;
	host.sin_family = AF_INET; 
	host.sin_port = htons(PORT);
	host.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (SOCKADDR*)& host, sizeof(host)) == SOCKET_ERROR)
	{
		cerr << "Binding failed with error: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}
	//Tell Winsock the socket is for listening
	if (listen(serverSocket, 1) == SOCKET_ERROR)
	{
		cerr << "Listening failed with error: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}
	// First loop to wait for connections
	SOCKET AcceptSocket;
	while (true)
	{
		//Wait for a connection
		cout << "Server is waiting for client..." << endl;
		AcceptSocket = accept(serverSocket, NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET)
		{
			cerr << "Client acception failed with error: " << WSAGetLastError() << endl;
			closesocket(serverSocket);
			WSACleanup();
			return -1;
		}
		else cerr << "Client connected" << endl;

		char buffer[200];
		int computerInput;
		string computerChoose;
		string input;
		// Second loop for a client to play plenty of times
		while (true)
		{
			//Third loop for Tic Tac Toe game
			do
			{
				ZeroMemory(buffer, 200); //fills a block of memory with 0
				// Wait for client to send data
				int bytesReceived = recv(AcceptSocket, buffer, 200, 0);
				if (bytesReceived == SOCKET_ERROR)
				{
					cerr << "Error in recv(). Quitting" << endl;
					break;
				}

				if (bytesReceived == 0)
				{
					cout << "Client disconnected " << endl;
					break;
				}

				cout << "Player's move: " << stoi(string(buffer, 0, bytesReceived)) << endl; //string to int
				boardValues[stoi(string(buffer, 0, bytesReceived)) - 1] = 'X';

				if (checkResults() == 0) break; //just in case someone won before all board is in X or O
				
				computerInput = computerMove(); //computer makes a move
				cout << "Computer's move: ";
				cout << computerInput + 1 << endl;;
				boardValues[computerInput] = 'O';
				computerChoose = to_string(computerInput + 1); //int to string
				
                // Echo message back to client
				send(AcceptSocket, computerChoose.c_str(), computerChoose.size() + 1, 0);

			} while ((checkWin('O') == -1) && (checkWin('X') == -1) && (checkTie() == -1));
			cout << "----------------------------------------------------------------------------------------" << endl;
			result();
			cout << "----------------------------------------------------------------------------------------" << endl << endl;
			clean_board(); //in case client decides to play again
			int bytesReceived = recv(AcceptSocket, buffer, 200, 0);
			if (bytesReceived == SOCKET_ERROR)
			{
				cerr << "Error in recv(). Quitting" << endl;
				break;
			}
			if (bytesReceived == 0)
			{
				cout << "Client disconnected " << endl;
				break;
			}
			if (string(buffer, 0, bytesReceived) == "yes")
			{
				cout << "----------------------------------------------------------------------------------------" << endl;
				cout << "--------------------------------------NEW-GAME------------------------------------------" << endl;
				cout << "----------------------------------------------------------------------------------------" << endl;
			}

		}
		// Close the socket
		closesocket(AcceptSocket);
	}
	// Cleanup winsock
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
int computerMove()
{
	int move = -1;
	if (checkPossibleWin('O') != -1) return move = checkPossibleWin('O'); // Win game
	if (checkPossibleWin('X') != -1) return move = checkPossibleWin('X'); // Block player
	do move = rand() % 9; // do not have possibilities to win or block player
	while (check_move(move) == -1);
	return move;
}
int checkPossibleWin(char move)
{
	if (boardValues[0] == move && boardValues[1] == move && boardValues[2] == '3') return 2;//horizonatal 1-2-3
	if (boardValues[0] == move && boardValues[1] == '2' && boardValues[2] == move) return 1;//horizonatal 1-2-3
	if (boardValues[0] == '1' && boardValues[1] == move && boardValues[2] == move) return 0;//horizonatal 1-2-3
	if (boardValues[3] == move && boardValues[4] == move && boardValues[5] == '6') return 5;//horizonal 4-5-6
	if (boardValues[3] == move && boardValues[4] == '5' && boardValues[5] == move) return 4;//horizonal 4-5-6
	if (boardValues[3] == '4' && boardValues[4] == move && boardValues[5] == move) return 3;//horizonal 4-5-6
	if (boardValues[6] == move && boardValues[7] == move && boardValues[8] == '9') return 8;//horizontal 7-8-9
	if (boardValues[6] == move && boardValues[7] == '8' && boardValues[8] == move) return 7;//horizontal 7-8-9
	if (boardValues[6] == '7' && boardValues[7] == move && boardValues[8] == move) return 6;//horizontal 7-8-9
	if (boardValues[0] == move && boardValues[3] == move && boardValues[6] == '7') return 6;//vertical 1-4-7
	if (boardValues[0] == move && boardValues[3] == '4' && boardValues[6] == move) return 3;//vertical 1-4-7
	if (boardValues[0] == '1' && boardValues[3] == move && boardValues[6] == move) return 0;//vertical 1-4-7
	if (boardValues[1] == move && boardValues[4] == move && boardValues[7] == '8') return 7;//vertical 2-5-8
	if (boardValues[1] == move && boardValues[4] == '5' && boardValues[7] == move) return 4;//vertical 2-5-8
	if (boardValues[1] == '2' && boardValues[4] == move && boardValues[7] == move) return 1;//vertical 2-5-8
	if (boardValues[2] == move && boardValues[5] == move && boardValues[8] == '9') return 8;//vertical 3-6-9
	if (boardValues[2] == move && boardValues[5] == '6' && boardValues[8] == move) return 5;//vertical 3-6-9
	if (boardValues[2] == '3' && boardValues[5] == move && boardValues[8] == move) return 2;//vertical 3-6-9
	if (boardValues[0] == move && boardValues[4] == move && boardValues[8] == '9') return 8;//diagonal 1-5-9
	if (boardValues[0] == move && boardValues[4] == '5' && boardValues[8] == move) return 4;//diagonal 1-5-9
	if (boardValues[0] == '1' && boardValues[4] == move && boardValues[8] == move) return 0;//diagonal 1-5-9
	if (boardValues[2] == move && boardValues[4] == move && boardValues[6] == '7') return 6;//diagonal 3-5-7
	if (boardValues[2] == move && boardValues[4] == '5' && boardValues[6] == move) return 4;//diagonal 3-5-7
	if (boardValues[2] == '3' && boardValues[4] == move && boardValues[6] == move) return 2;//diagonal 3-5-7

	return -1;
}
void result()
{
	if (checkWin('X') == 1) cout << "Player won" << endl;
	else if (checkWin('O') == 1) cout << "Computer won" << endl;
	else if (checkTie() == 0 && checkWin('X') != 1 && checkWin('O') != 1) cout << "It's a tie" << endl;
}
int checkResults()
{
	if (checkWin('X') == 1) return 0;
	else if (checkWin('O') == 1) return 0;
	else if (checkTie() == 0 && checkWin('X') != 1 && checkWin('O') != 1) return 0;
	else return -1;
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