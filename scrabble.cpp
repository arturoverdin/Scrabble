#include "Move.h"
#include "ConsolePrinter.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {

	ifstream configFile;
	configFile.open(argv[1]);

	if(!configFile) {
		cout << "Error with config" << endl;
		return 1;
	}

	if(argc < 2) {
		cout << "Enter Config file" << endl;
		return 1;
	}

	string line;
	string dictionary;
	string tileSet;
	string board;
	uint32_t seed;
	size_t maxSize;

	while(!configFile.eof()) {
		configFile >> line;
		if(line == "HANDSIZE:") {
			configFile >> maxSize;
		} else if(line == "TILES:") {
			configFile >> tileSet;
		} else if(line == "DICTIONARY:") {
			configFile >> dictionary;
		} else if(line == "BOARD:") {
			configFile >> board;
		} else if(line == "SEED:") {
			configFile >> seed;
		} else {
			break;
		}
	}
	configFile.close();

	vector<Player*> gamePlayers;
	Bag gameBag(tileSet, seed);
	Dictionary gameDictionary(dictionary);
	Board gameBoard(board);
	ConsolePrinter print;

	size_t num_players;
	cout << "How many players? " << endl;
	cin >> num_players;
	if(num_players < 1) {
		cout << "Must have more players" << endl;
		return 1;
	}
	string empty;
	getline(std::cin, empty);

	for(size_t play = 0; play < num_players; ++play) {
		string name;
		cout << "What is player " <<  play+1 << "'s" << " name?" << endl;

		getline(std::cin, name);
		Player* newPlayer = new Player(name, maxSize);
		newPlayer->addTiles(gameBag.drawTiles(maxSize));
		gamePlayers.push_back(newPlayer);
	}

	unsigned int pass = 0;
	unsigned int player = 0;
	bool emptyHand = false;
	while(gameBag.tilesRemaining() > 0) {
		for(size_t turn = 0; turn < num_players; ++turn) {

			print.printBoard(gameBoard);
			print.printHand(*gamePlayers[turn]);
			string moveString;
			cout << "What would you like to do? PASS, EXCHANGE, or PLACE?" << endl;
			getline(std::cin,moveString);
			Move* player_turn = player_turn->parseMove(moveString, *gamePlayers[turn]);

			if(!player_turn) {

				cout << "Please enter a valid command." << endl;
				turn--;

			} else {

				player_turn->execute(gameBoard, gameBag, gameDictionary);

				if(player_turn->getSuccess()) {

					if(player_turn->isWord()) {

						print.printBoard(gameBoard);
						print.printHand(*gamePlayers[turn]);
						cout << "Successfully placed down tiles!" << endl;
						for(size_t j = 0; j < num_players; ++j) {
							cout << gamePlayers[j]->getPlayerName() <<
							"'s Points: " << gamePlayers[j]->getScore() << endl;
						}
						pass = 0;
					} else if(player_turn->isExchange()) {

						print.printHand(*gamePlayers[turn]);
						pass = 0;

					} else if(player_turn->isPass()) {
						pass++;

						if(pass == num_players) {
							delete player_turn;
							break;
						}
					}

				std::cin.ignore();
				} else {

					if(turn == 0) {
						turn = 0;
					} else {
						turn--;
					}
				}
			}
			delete player_turn;
			if(gamePlayers[turn]->getHandTiles().empty()) {
				player = turn;
				emptyHand = true;
				break;
			}
		}
		if(pass == num_players) {
			break;
		}
	}

	cout << "Game Over!" << endl; 
	int totalScore = 0;
	if(emptyHand) {
		for(size_t i = 0; i < num_players; ++i) {
			if(i == player) {
				//do nothing 
			} else {
				int sub = gamePlayers[i]->getHandScore();
				int current = gamePlayers[i]->getScore();
				gamePlayers[i]->setScore(current - sub);
				totalScore += gamePlayers[i]->getScore();
			}
		}
	}
	gamePlayers[player]->setScore(gamePlayers[player]->getScore() + totalScore);
	int highScore = gamePlayers[0]->getScore();
	for(size_t i = 0; i < num_players; ++i) {
		if(gamePlayers[i]->getScore() > highScore) {
			highScore = gamePlayers[i]->getScore();
		}
		cout << gamePlayers[i]->getPlayerName() << "'s Points: " 
		<< gamePlayers[i]->getScore() << endl;
	}

	vector<Player*> winners;

	for(size_t i = 0; i < num_players; ++i) {
		if(gamePlayers[i]->getScore() == highScore) {
			winners.push_back(gamePlayers[i]);
		}
	}
	if(winners.size() > 1) {
		cout << "Winners: ";
	} else {
		cout << "Winner: ";
	}
	for(size_t i = 0; i < num_players; ++i) {
		if(gamePlayers[i]->getScore() == highScore) {
			cout << gamePlayers[i]->getPlayerName() << " ";
		}
	}
	cout << endl;
	
	for(size_t size = 0; size < num_players; ++size) {
		delete gamePlayers[size];
	}
	
	return 1;
}
