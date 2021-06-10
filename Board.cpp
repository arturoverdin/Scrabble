#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Board::Board(string board_file_name) {

	ifstream inputBoard(board_file_name);

	if(!inputBoard) {
		cout << "Error with file" << endl;
		return;
	}

	inputBoard >> _columns;
	inputBoard >> _rows;

	board = new Square**[_columns];
	for(size_t col = 0; col < _columns; ++col) {
		board[col] = new Square*[_rows];

	}

	size_t start_x;
	size_t start_y;

	inputBoard >> start_x;
	inputBoard >> start_y;

	startY = start_y;
	startX = start_x;
	
	for(size_t row = 0; row < _rows; ++row) {

		string sym;
		inputBoard >> sym;

		for(size_t col = 0; col < _columns; ++col) {

			Square* push;
			char symbol = sym[col];

			if(col+1 == start_x && row+1 == start_y) {
				if(symbol == '.') {
					push = new Square(1,1,true);
				} else if(symbol == 'd') {
					push = new Square(1,2,true);
				} else if(symbol == 't') {
					push = new Square(1,3,true);
				} else if(symbol == '2') {
					push = new Square(2,1,true);
				} else if(symbol == '3') {
					push = new Square(3,1,true);
				}
			} else {
				if(symbol == '.') {
					push = new Square(1,1,false);
				} else if(symbol == 'd') {
					push = new Square(1,2,false);
				} else if(symbol == 't') {
					push = new Square(1,3,false);
				} else if(symbol == '2') {
					push = new Square(2,1,false);
				} else if(symbol == '3') {
					push = new Square(3,1,false);
				}
			}	
			board[col][row] = push;
		}
	}
	inputBoard.close();
}

Board::~Board() { 

	for(size_t col = 0; col < _columns; ++col) {
		for(size_t row = 0; row < _rows; ++row) {
			delete board[col][row];
		}
		delete [] board[col];
	}
	delete [] board;
}

void Board::getWord(vector<pair<string, unsigned int>>& words ,
const PlaceMove& m, Square* curr, size_t col, size_t row, char letter) const {

	Square* top;
	Square* left;


	top = getSquare(col,row-1);
	left = getSquare(col-1,row);
	vector<Tile*> executeTiles = m.tileVector();
	bool horizontal = m.getHorizontal();
	string input = "";
	int wordScore = 0;
	int WMult = 1;

	if(horizontal) {
		if(!left) {
			size_t place = 0;
			while(col <= getColumns()) {
				if(curr->isOccupied()) {
					input += curr->getLetter();

					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
				} else {
					if(place >= executeTiles.size()) {
						break;
					}
					input += executeTiles[place]->getUse();
					wordScore += (executeTiles[place]->getPoints() * curr->getLMult());
					WMult *= curr->getWMult();
					place++;
				}
				col++;
				curr = getSquare(col, row);
			}
		} else {
			col--;
			curr = getSquare(col,row);
			while(col > 0 && curr->isOccupied()) {
				col--;
				curr = getSquare(col,row);
			}
			col++;
			curr = getSquare(col,row);
			size_t place = 0;
			while(col <= getColumns()) {
				if(curr->isOccupied()) {
					input += curr->getLetter();

					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
				} else {
					if(place >= executeTiles.size()) {
						break;
					}
					input += executeTiles[place]->getUse();
					wordScore += (executeTiles[place]->getPoints() * curr->getLMult());
					WMult *= curr->getWMult();
					place++;
				}
				col++;
				curr = getSquare(col, row);
			}
		}
	} else {  
		if(!top) {
			size_t place = 0;
			while(row <= getRows()) {
				if(curr->isOccupied()) {
					input += curr->getLetter();

					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
				} else {
					if(place >= executeTiles.size()) {
						break;
					}
					input += executeTiles[place]->getLetter();
					wordScore += (executeTiles[place]->getPoints() * curr->getLMult());
					WMult *= curr->getWMult();
					place++;
				}
				row++;
				curr = getSquare(col, row);
			}
		} else {
			row--;
			curr = getSquare(col,row);
			while(row > 0 && curr->isOccupied()) {
				row--;
				curr = getSquare(col,row);
			}
			row++;
			curr = getSquare(col,row);
			size_t place = 0;
			while(row <= getRows()) {
				if(curr->isOccupied()) {
					input += curr->getLetter();

					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();

				} else {
					if(place >= executeTiles.size()) {
						break;
					}
					input += executeTiles[place]->getUse();

					wordScore += (executeTiles[place]->getPoints() * curr->getLMult());
					WMult *= curr->getWMult();
					place++;
					
				}
				row++;
				curr = getSquare(col, row);
			}
		}
	} 
	wordScore *= WMult;
	pair<string, unsigned int> duo(input, wordScore);
	words.push_back(duo);

}

/*focus on pushing back words and then calculate their score later */
void Board::getWordHelper(vector<pair<string, unsigned int>>& words ,
	const PlaceMove& m, Square* curr, size_t col, size_t row, Tile* letter) const {

	Square* top;
	Square* bottom;
	Square* left;
	Square* right;

	top = getSquare(col,row-1);
	bottom = getSquare(col, row+1);
	left = getSquare(col-1,row);
	right = getSquare(col+1, row);

	bool horizontal = m.getHorizontal();
	string input = "";
	int wordScore = 0;
	int WMult = 1;

	if(horizontal) {
		if(!top) {
			if(bottom->isOccupied()) {
				input += letter->getUse();
				wordScore += (letter->getPoints()* curr->getLMult());
				WMult *= curr->getWMult();
				row++;
				curr = getSquare(col,row);
				while(row <= getRows() && curr->isOccupied()) {
					input += curr->getLetter();
					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
					row++;
					curr = getSquare(col,row);
				}
				wordScore *= WMult;
				pair<string, unsigned int> duo(input, wordScore);
				words.push_back(duo);
			}	
			
		} else if(!bottom) {
			if(top->isOccupied()) {
				row--;
				curr = getSquare(col,row);
				while(row > 0 && curr->isOccupied()) {
					row--;
					curr = getSquare(col,row);	
				}
				row++;
				curr = getSquare(col,row);
				while(row <= getRows() && curr->isOccupied()) {
					input += curr->getLetter();
					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
					row++;
					curr = getSquare(col,row);
				}
				input+=letter->getUse();
				wordScore += (letter->getPoints()* curr->getLMult());

				WMult *= curr->getWMult();
				wordScore *= WMult;

				pair<string, unsigned int> duo(input, wordScore);
				words.push_back(duo);
			}	
		} else {
			if(top->isOccupied()) {
				row--;
				curr = getSquare(col,row);
				while(row > 0 && curr->isOccupied()) {
					row--;
					curr = getSquare(col,row);	
				}
				row++;
				curr = getSquare(col,row);
				while(row <= getRows() && curr->isOccupied()) {
					input += curr->getLetter();
					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
					row++;
					curr = getSquare(col,row);
				}

				input+=letter->getUse();
				wordScore += (letter->getPoints()* curr->getLMult());
				WMult *= curr->getWMult();
				row++;
				curr = getSquare(col,row);
				while(curr->isOccupied() && row <= getRows()) {
					input+= curr->getLetter();
					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
					row++;
					curr = getSquare(col,row);
				}
				wordScore *= WMult;
				pair<string, unsigned int> duo(input, wordScore);
				words.push_back(duo);
			}		
		}
	} else {
		if(!left) {
			if(right->isOccupied()) {
				input += letter->getUse();
				wordScore += (letter->getPoints()* curr->getLMult());
				WMult *= curr->getWMult();
				col++;
				curr = getSquare(col,row);
				while(col <= getColumns() && curr->isOccupied()) {
					input += curr->getLetter();
					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
					col++;
					curr = getSquare(col,row);
				}
				wordScore *= WMult;
				pair<string, unsigned int> duo(input, wordScore);
				words.push_back(duo);
			}	
		} else if(!right) {
			if(left->isOccupied()) {
				col--;
				curr = getSquare(col,row);
				while(col > 0 && curr->isOccupied()) {
					col--;
					curr = getSquare(col,row);	
				}
				col++;
				curr = getSquare(col,row);
				while(col <= getColumns() && curr->isOccupied()) {
					input += curr->getLetter();
					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
					col++;
					curr = getSquare(col,row);
				}
				input+=letter->getUse();
				wordScore += (letter->getPoints()* curr->getLMult());
				WMult *= curr->getWMult();
				wordScore *= WMult;
				pair<string, unsigned int> duo(input, wordScore);
				words.push_back(duo);
			}	
		} else {
			if(left->isOccupied()) {
				col--;
				curr = getSquare(col,row);
				while(col > 0 && curr->isOccupied()) {
					col--;
					curr = getSquare(col,row);	
				}
				col++;
				curr = getSquare(col,row);
				while(col <= getColumns() && curr->isOccupied()) {
					input += curr->getLetter();
				}
				input+=letter->getUse();
				wordScore += (letter->getPoints()* curr->getLMult());
				WMult *= curr->getWMult();
				col++;
				curr = getSquare(col,row);
				while(curr->isOccupied() && col <= getColumns()) {
					input+= curr->getLetter();
					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
					col++;
					curr = getSquare(col,row);
				}
				wordScore *= WMult;
				pair<string, unsigned int> duo(input, wordScore);
				words.push_back(duo);
			} else if(right->isOccupied()) {
				input += letter->getUse();
				wordScore += (letter->getPoints()* curr->getLMult());
				WMult *= curr->getWMult();
				col++;
				curr = getSquare(col,row);
				while(col <= getColumns() && curr->isOccupied()) {
					input += curr->getLetter();
					wordScore += (curr->getScore() * curr->getLMult());
					WMult *= curr->getWMult();
					col++;
					curr = getSquare(col,row);
				}
				wordScore *= WMult;
				pair<string, unsigned int> duo(input, wordScore);
				words.push_back(duo);
			}		
		}
	}
}

vector<pair<string, unsigned int>> Board::getPlaceMoveResults(const PlaceMove & m) const {
	vector<pair<string, unsigned int> > words;
	size_t row = m.getStartX();
	size_t col = m.getStartY();
	bool horizontal = m.getHorizontal();
	vector<Tile*> executeTiles = m.tileVector();

	Square* curr = getSquare(col, row);
	string initial = "";
	for(size_t tiles = 0; tiles < executeTiles.size(); ++tiles) {
		if(horizontal) {
			while(curr->isOccupied()) {
				col++;
				curr = getSquare(col, row);	
			}
		} else {
			while(curr->isOccupied()) {
				row++;
				curr = getSquare(col,row);
			}
		}
		getWordHelper(words, m, curr, col, row, executeTiles[tiles]);
		if(horizontal) {
			col++; 
		} else {
			row++;
		}
		curr = getSquare(col,row);
	}
	curr = getSquare(getStartY(),getStartX());
	getWord(words,m,curr,m.getStartY(),m.getStartX(),executeTiles[0]->getLetter());
	return words;
}

void Board::executePlaceMove (const PlaceMove & m) {

	size_t row = m.getStartX();
	size_t col = m.getStartY();
	bool horizontal = m.getHorizontal();
	vector<Tile*> executeTiles = m.tileVector();

	Square* curr = getSquare(col,row);

	for(size_t tiles = 0; tiles < executeTiles.size(); ++tiles) {
		if(horizontal) {
			while(curr->isOccupied()) {
				col++;
				curr = getSquare(col,row);
			}
		} else {
			while(curr->isOccupied()) {
				row++;		
				curr = getSquare(col,row);
			}
		}
		curr->placeTile(executeTiles[tiles]);
	}

}

Square * Board::getSquare (size_t x, size_t y) const {

	if((y > 0 && y <= _columns) && (x > 0 && x <= _rows)) {
		return board[y-1][x-1];
	}
	return nullptr;
}

size_t Board::getRows() const {

	return _rows;
}

size_t Board::getColumns() const {

	return _columns;
}
