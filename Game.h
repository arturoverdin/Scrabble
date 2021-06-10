#ifndef GAME_H_
#define GAME_H_

#include "Board.h"
#include "Move.h"


class Game {
public:
	Board();
	~Board();

protected:
	Board srabble_board;
	Bag tile_bag;


};


#endif /* GAME_H_ */