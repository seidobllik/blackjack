/*
	Tom Baker

	--Blackjack--
	Emulate the game of blackjack using a 2D array as a deck of cards.
*/

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

void shuffle_deck(char, int&, vector<int>&, vector<string>&, vector<bool>&);
void deal(int, char, vector<int>&, vector<string>&, vector<bool>&);
void print_table(int, int, vector<string>&);
void game(int&);
void end_game(vector<int>&, char[4][13]);

int main()
{
	int players = 0;			// Number of players in the game.
	srand(GetTickCount64());	// Seed the random number generator.
	bool quit = false;			// Exit game when set to true.

	cout << "Welcome to Honest Tom's Blackjack Table" << endl << endl
		<< "It's nice to have you back!" << endl;

	do
	{
		cout << endl << "Would you like to start a new game of Blackjack? (Y/N): ";
		try
		{
			// Get char type input from user. String stream is used because I like the
			// getline() function, and multiple char inputs still renders one input
			// attempt. This technique is used throughout the program.
			char selection = 'x';
			string input = "void";
			getline(cin, input);
			stringstream is(input);
			is >> selection;
			switch (selection)
			{
			case 'Y': case 'y':
				game(players);
				break;
			case 'N': case 'n':
				quit = true;
				break;
			default:
				throw exception("\nPlease enter Y or N.");
				break;
			}
		}
		catch (exception& error)
		{
			cerr << error.what() << "\npress enter to continue..." << endl;
			cin.ignore();
			continue;
		}
	} while (!quit);

	cout << endl << "THANKS FOR PLAYING" << endl;
	cin.ignore();
	return 0;
}

void shuffle_deck(char card_deck[4][13], int &round_count, vector<int> &score, vector<string> &table, vector<bool> &drawing)
{	// Sets values of deck array to ' '. Clears table and drawing values.
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			card_deck[i][j] = ' ';
		}
	}

	table.clear();
	drawing.clear();
	round_count = 0;
	score.clear();
}

void deal(int player, char card_deck[4][13], vector<int> &score, vector<string> &table, vector<bool> &drawing)
{	// Assigns cards to players and updates scores.
	int i, j;		// Integers used to assign random cards.
	string a, b;	// Used to update table vector with player card values.

	do
	{
		i = rand() % 4;
		j = rand() % 13;
	} while (card_deck[i][j] != ' ');

	card_deck[i][j] = player + '0'; // Assign the spot in the deck to current player.

	switch (i)	// Translate i into card suit.
	{
	case 0:
		a = (char)4;
		break;
	case 1:
		a = (char)3;
		break;
	case 2:
		a = (char)5;
		break;
	case 3:
		a = (char)6;
		break;
	}

	switch (j)	// Translate j into card value, and update player score.
	{
	case 0:
		b = "2";
		score[player] += 2;
		break;
	case 1:
		b = "3";
		score[player] += 3;
		break;
	case 2:
		b = "4";
		score[player] += 4;
		break;
	case 3:
		b = "5";
		score[player] += 5;
		break;
	case 4:
		b = "6";
		score[player] += 6;
		break;
	case 5:
		b = "7";
		score[player] += 7;
		break;
	case 6:
		b = "8";
		score[player] += 8;
		break;
	case 7:
		b = '9';
		score[player] += 9;
		break;
	case 8:
		b = "10";
		score[player] += 10;
		break;
	case 9:
		b = "J";
		score[player] += 10;
		break;
	case 10:
		b = "Q";
		score[player] += 10;
		break;
	case 11:
		b = "K";
		score[player] += 10;
		break;
	case 12:
		b = "A";
		if ((score[player] + 11) > 21)
		{
			score[player] += 1;
		}
		else score[player] += 11;
		break;
	}

	// Update table vector.
	table.push_back(a);
	table.push_back(b);

	if (score[player] > 21)		// If a player has busted, halt their drawing.
	{
		drawing[player] = false;
	}
}

void print_table(int players, int round_count, vector<string> &table)
{	// Displays game information in a nice format.
	cout << endl << endl;
	cout << setw(9) << " " << setw(8) << left << "Dealer";
	for (int i = 0; i < players; i++)
	{
		cout << setw(9) << right << "Player " << i + 1;
	}
	cout << endl;

	int j = 0;		// Iterator used to navigate tabel vector.
	for (int i = 0; i < round_count; i++)
	{
		cout << left << "Card " << i + 1 << setw(3) << ":";
		int k = 0;
		for (; j < table.size() && k <= players; j += 2)
		{
			cout << setw(2) << right << table[j + 1] << setw(8)
				<< left << table[j];
			k++;
		}
		cout << endl;
	}
}

void game(int &players)
{
	int dealer_draw = rand() % 4;	// The number of cards the dealer will draw.
	char deck[4][13];				// 2D array to represent deck of cards.
	int rounds = 0;					// Round counter.
	string dealers_card[2];			// String array to hold dealers first card.
	vector<int> score;				// Integer Vector to hold player scores.
	vector<string> table;			// String vector to hold cards in play.
	vector<bool> players_drawing;	// Which players are still drawing cards?

	shuffle_deck(deck, rounds, score, table, players_drawing);		// Initialize values for deck array and table vector.

	if (players == 0)	// If this is the initial game, ask how many people are
	{					// playing. Only allows 1-4 players as input.
		do
		{
			try
			{
				cout << endl << "How many people want to play? (1-4): ";
				int selection = 0;
				string input = "void";
				getline(cin, input);
				stringstream is(input);
				is >> selection;
				if (!is || (selection <= 0 || selection > 4))
				{
					throw exception("\nPlease enter a number between 1 and 4.");
				}
				else players = selection;
			}
			catch (exception& error)
			{
				cerr << error.what() << "\npress enter to continue..." << endl;
				cin.ignore();
				continue;
			}
		} while (players <= 0 || players > 4);
	}
	else
	{	// If a game was played already, see who wants to keep playing.
		do
		{
			int drop_outs = 0;	// The count of players leaving the table.
			try
			{
				for (int i = 0; i < players; i++)
				{
					cout << "\nDoes Player " << i + 1 << " want to play again? (Y/N): ";
					char selection = 'x';
					string input = "void";
					getline(cin, input);
					stringstream is(input);
					is >> selection;
					switch (selection)
					{
					case 'Y': case 'y':
						break;
					case 'N': case 'n':
						drop_outs++;
						break;
					default:
						throw exception("\nPlease enter Y or N.");
						break;
					}
				}
			}
			catch (exception& error)
			{
				cerr << error.what() << "\npress enter to continue...";
				cin.ignore();
				continue;
			}
			players -= drop_outs;	// Update players count.
			break;
		} while (true);
	}

	for (int i = 0; i < players + 1; i++)
	{	// Begin the game with all players drawing cards.
		players_drawing.push_back(true);
	}

	for (int i = 0; i < players + 1; i++)
	{	// Set each players score to 0.
		score.push_back(0);
	}

	for (; rounds < 2; rounds++)
	{	// Deal the initial two cards to each player and display them.
		for (int j = 0; j <= players; j++)
		{
			deal(j, deck, score, table, players_drawing);
		}
	}

	dealers_card[0] = table[0];	// Mask dealers first card.
	dealers_card[1] = table[1];
	table[0] = "?";
	table[1] = "?";

	print_table(players, rounds, table); // Display information.

	do
	{	// This is the main game loop. 
		if (dealer_draw > 0)
		{	// Set dealer's drawing status.
			players_drawing[0] = true;
		}
		else
		{
			players_drawing[0] = false;
		}

		try
		{	// Set each players drawing status.
			for (int i = 1; i <= players; i++)
			{
				if (players_drawing[i] == true)
				{
					cout << "\nDoes Player " << i << " hit or stay? (H/S): ";
					char selection = 'x';
					string input = "void";
					getline(cin, input);
					stringstream is(input);
					is >> selection;
					switch (selection)
					{
					case 'H': case 'h':
						players_drawing[i] = true;
						break;
					case 'S': case 's':
						players_drawing[i] = false;
						break;
					default:
						throw exception("\nPlease enter H or S.");
						break;
					}
				}
			}
		}
		catch (exception& error)
		{
			cerr << error.what() << "\npress enter to continue...";
			cin.ignore();
			continue;
		}

		for (int i = 0; i < players + 1; i++)
		{
			if (players_drawing[i] == false && score[i] <= 21)
			{
				table.push_back("ay");	// Enter "stay" in place of players
				table.push_back("st");  // cards if they chose not to draw.
			}
			else if (score[i] > 21)
			{
				players_drawing[i] == false; // Enter "bust" in place of players
				table.push_back("st");		 // cards if they go over 21.
				table.push_back("bu");
			}
			else
			{	// If the players are able, they receive another card.
				deal(i, deck, score, table, players_drawing);
			}
		}
		rounds++;	// Update rounds count.

		if (rounds == 5)
		{	// On the last round, reveal the dealers card. 
			table[0] = dealers_card[0];
			table[1] = dealers_card[1];
		}

		dealer_draw--;	// Update dealer draw count.
		print_table(players, rounds, table);	// Update screen with new info.

	} while (rounds < 5);
	end_game(score, deck);
}

void end_game(vector<int> &score, char card_deck[4][13])
{	// Prints final scores and win/lose stats. 
	int high_score = 0;	// Variable to see which player won the hand.

	cout << setfill('-') << setw(score.size() * 11) << "-" << endl;
	cout << setfill(' ') << setw(9) << left << "SCORE:";
	for (int i = 0; i < score.size(); i++)
	{	// Print total scores for each player.
		cout << setw(10) << left << score[i];
	}
	cout << endl;

	for (int i = 0; i < score.size(); i++)
	{	// If any scores were greater than 21, set them to 0.
		if (score[i] > 21)
		{
			score[i] = 0;
		}
	}

	// Find the high score value.
	high_score = *max_element(score.begin(), score.end());

	cout << setw(9) << " ";
	for (int i = 0; i < score.size(); i++)
	{	// Print which players won and lost using high score value.
		if (score[i] == high_score && score[i] <= 21 && score[i] > 0)
		{
			cout << setw(10) << left << "WIN";
		}
		else cout << setw(10) << left << "LOSE";
	}
	cout << endl << endl;
	cout << "press enter to continue..." << endl;
	cin.ignore();

	// Display which player had which card in the deck.
	cout << endl << endl;
	cout << " -- Cards Dealt -- " << endl << setw(2) << " ";
	for (int i = 2; i < 15; i++)
	{	// This displays the column ID.
		if (i < 11)
		{
			cout << setw(2) << left << i;
		}
		else
		{
			switch (i)
			{
			case 11:
				cout << setw(2) << left << "J";
				break;
			case 12:
				cout << setw(2) << left << "Q";
				break;
			case 13:
				cout << setw(2) << left << "K";
				break;
			case 14:
				cout << setw(2) << left << "A";
				break;
			}
		}
	}
	cout << endl;
	for (int i = 0; i < 4; i++)
	{	// This displays the row IDs, and player numbers.
		switch (i)
		{
		case 0:
			cout << setw(2) << left << (char)4;
			break;
		case 1:
			cout << setw(2) << left << (char)3;
			break;
		case 2:
			cout << setw(2) << left << (char)5;
			break;
		case 3:
			cout << setw(2) << left << (char)6;
			break;
		}
		for (int j = 0; j < 13; j++)
		{
			cout << setw(2) << left << card_deck[i][j];
		}
		cout << endl;
	}
	cout << "Dealer = 0; Player 1 = 1; Player 2 = 2; etc..." << endl
		<< "press enter to continue..." << endl;
	cin.ignore();
}
