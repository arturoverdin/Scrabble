#include "Move.h"
#include "Board.h"
#include <sstream>
#include <iostream>

using namespace std;


/* constructor that calls for the subclasses */
Move::Move(Player* player) {
	_player = player;
}

Move::~Move() {
}

/* Parses the move and returns a pointer to an inheriting
class accordingly */
Move* Move::parseMove(string moveString, Player &p) {

	stringstream parse(moveString);
	string command;
	parse >> command;

	string tiles;

	for(size_t i = 0; i < command.size(); i++) {
		command[i] = toupper(command[i]);
	}

	if(command == "PASS"){

		return new PassMove(&p);

	} else if (command == "EXCHANGE") {

		parse >> tiles;
		return new ExchangeMove(tiles, &p);

	} else if(command == "PLACE") {
		string orientation;
		size_t start_x, start_y;
		bool horizontal;
		
		parse >> orientation;
		parse >> start_x;
		parse >> start_y;
		parse >> tiles;

		if(orientation == "-") {
			horizontal = true;
		} else {
			horizontal = false;
		}

		return new PlaceMove(start_x,start_y,horizontal,tiles, &p);

	} else {
		return nullptr;
	}
	
}

/* Will always be a success */
PassMove::PassMove(Player * player) : Move(player) {
	/* sets the player in private to p*/
	success = true;
}

/*skips turn */
void PassMove::execute(Board &board, Bag &bag, Dictionary &dictionary) {

	cout << "You skipped your turn!" << endl;

}
/* sets the player in private to p and initializes the string*/
ExchangeMove::ExchangeMove(string tileString, Player* p) : Move(p) {

	success = false;
	exchange = tileString;

}

/*executes the exchange move */
void ExchangeMove::execute(Board &board, Bag &bag, Dictionary &dictionary) {

	if(_player->hasTiles(exchange, false)) {
		vector<Tile*> tilesExchanged;

		tilesExchanged = _player->takeTiles(exchange, false);
		bag.addTiles(tilesExchanged);
		_player->addTiles(bag.drawTiles(exchange.size()));
		success = true;

		cout << "You successfully exchanged tiles!" << endl;

	} else {

		cout << "error: Player did not have required tiles to exchange" << endl;
	}

}

/* Initializes all the PlaceMove data varibles*/
PlaceMove::PlaceMove(size_t x, size_t y, bool horizontal,
	string tileString, Player* p) : Move(p) {

	this->x = x;
	this->y = y;
	this->horizontal = horizontal;
	placeString = tileString;

	if(_player->hasTiles(placeString, true)) {
		placeTiles = _player->takeTiles(placeString, true);
	}

	success = false;
}


/*Does all the logic for whether a move is legal or not. This is where it is 
all tested. It eventually calls the board execute if it deems that move is
legal. */
void PlaceMove::execute(Board & board, Bag & bag, Dictionary & dictionary) {

	vector<Tile*> executeTiles = tileVector();

	if(executeTiles.empty()) {
		cout << "error: Player does not have required tiles. Try again."
		<< endl;
		return;
	}
	size_t row = x;
	size_t col = y;
	if(horizontal) {
		if(col+placeTiles.size()-1> board.getColumns() || col < 1) {
			cout << "error: Tiles out of bounds! Try again." << endl;
			_player->addTiles(executeTiles);
			return;
		}
	} else {
		if(row+placeTiles.size()-1 > board.getRows() || row < 1) {
			cout << "error: Tiles out of bounds! Try again." << endl;
			_player->addTiles(executeTiles);
			return;
		}
	}

	Square* curr = board.getSquare(col,row);
	if(curr->isOccupied()) {
		cout << "Square is already occupied! Try again." << endl;
		_player->addTiles(executeTiles);
		return;
	}
	bool startUsed = true;
	if(!board.getStart()->isOccupied()) {
		startUsed = false;
	}

	for(size_t tiles = 0; tiles < executeTiles.size(); ++tiles) {

		if(curr->isOccupied()) {
			while(curr->isOccupied()) {
				if(col > board.getColumns() || row > board.getRows() 
				 || col < 1 || row < 1) {
						cout << "error: Tiles out of bounds! Try again."
						<< endl;
						_player->addTiles(executeTiles);
						return;
				}
				if(horizontal) {
					col++;
				} else {
					row++;
				}
				curr = board.getSquare(col,row);
			}
		} else {
			if(col > board.getColumns() || row > board.getRows()
				|| col < 1 || row < 1) {
				cout << "error: Tiles out of bounds! Try again." << endl;
				_player->addTiles(executeTiles);
				return;
			}
			if(horizontal) {
				col++;
			} else {
				row++;
			}
			if(curr == board.getStart()) {
				startUsed = true;
			}
			curr = board.getSquare(col,row);
		}
	}

	if(!startUsed) {
		cout << "error: Must use a start square! Try again." << endl;
		_player->addTiles(placeTiles);
		return;
	}
	if(board.getStart()->isOccupied() && !adjacency(board)) {
		cout << 
		"error: At least one tile must be adjacent to another. Try again."
		<< endl;
		_player->addTiles(executeTiles);
		return;
	}
	vector<pair<string,unsigned int> >::iterator it;
	vector<pair<string,unsigned int> > results;

	results = board.getPlaceMoveResults(*this);
	cout << results.size() << endl;

	for(it = results.begin(); it != results.end(); ++it) {
		if(!dictionary.isLegalWord(it->first)) {
			_player->addTiles(executeTiles);
			cout << "error: Illegal word created." << endl;
			return;
		}
	}
	int playerScore = 0;
	for(it = results.begin(); it != results.end();++it) {
		playerScore += it->second;
	}
	playerScore += _player->getScore();
	_player->setScore(playerScore);
	board.executePlaceMove(*this);
	success = true;
	_player->addTiles(bag.drawTiles(executeTiles.size()));
	cout << "You successfully placed down tiles." << endl;

}
/* checks for a legal move by making sure 
the player sets down an adjacent tile */
bool PlaceMove::adjacency(Board& board) {

	size_t row = x;
	size_t col = y;

	Square* curr = board.getSquare(col,row);

	Square* top;
	Square* bottom;
	Square* left;
	Square* right;

	for(size_t tiles = 0; tiles < placeTiles.size(); ++tiles){

		if(curr->isOccupied()) {
			return true;

		} else {

			curr = board.getSquare(col, row);
			top = board.getSquare(col,row-1);
			bottom = board.getSquare(col,row+1);
			left = board.getSquare(col-1,row);
			right = board.getSquare(col+1, row);

			if((!top && !left)) {
				if(bottom->isOccupied() 
				|| right->isOccupied()) {
					return true;
				}

			} else if(!top && !right) {
				if(bottom->isOccupied()
				||left->isOccupied()) {
					
					return true;
				}
			} else if(!top) {
				if(bottom->isOccupied()
				||left->isOccupied() || right->isOccupied()) {
					
					return true;
				}

			} else if(!bottom && !left) {
				if(top->isOccupied()
				||right->isOccupied()) {
					
					return true;
				}
			} else if(!bottom && !right) {
				if(top->isOccupied()
				||left->isOccupied()) {
					
					return true;
				}
			} else if(!bottom) {
				if(top->isOccupied()
				||left->isOccupied() || right->isOccupied()) {
					
					return true;
				}
			} else if(!left) {
				if(top->isOccupied() || bottom->isOccupied()
				|| right->isOccupied()) {
					
					return true;
				}
			} else if(!right) {
				if(top->isOccupied() || bottom->isOccupied()
				||left->isOccupied()) {
					
					return true;
				}
			} else {
				if(top->isOccupied() || bottom->isOccupied() 
				|| left->isOccupied() || right->isOccupied()) {

					return true;
				}
			}
			if(horizontal) {
				col++;
			} else {
				row++;
			}
		}

	}
	
	return false;
}

/*returns the tiles that were in the player's hand */
vector<Tile*> const & PlaceMove::tileVector () const {

	return placeTiles;

}