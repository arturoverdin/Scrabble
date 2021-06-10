#include "Player.h"
#include <iostream>
using namespace std;

Player::Player(string const & name, size_t maxTiles) {

	player_name = name;
	handsize = maxTiles;
	score = 0; 

}

Player::~Player(){
	
	for(size_t currTile = 0; currTile < hand.size(); ++currTile) {
		delete hand[currTile];
	}
}

set<Tile*> Player::getHandTiles() const {

	set<Tile*> new_hand;

	for(size_t i = 0; i < hand.size(); i++) {
		new_hand.insert(hand[i]);
	}
	return new_hand;

}

bool Player::hasTiles(string const& move, bool resolveBlanks) const {

	vector<Tile*> tempHand = hand;
	bool found;

	for(size_t i = 0; i < move.size(); i++) {
		found = false;
		char letter = tolower(move[i]);
		for(size_t j = 0; j < tempHand.size(); j++) {

			if(tolower(tempHand[j]->getLetter()) == letter) {
				tempHand.erase(tempHand.begin() + j);
				found = true;
				break;
			}
		}
		if(resolveBlanks && letter == '?') {
			i++; ;
		}
		if(!found) {
			return false;
		}
	}
	return true;
}

int Player::getHandScore() const {
	int temp = 0;
	for(size_t i = 0; i < hand.size(); ++i) {
		temp += hand[i]->getPoints();

	}
	return temp;
}


vector<Tile*> Player::takeTiles(std::string const & move, bool resolveBlanks){

	vector<Tile*> place;

	for(size_t i = 0; i < move.size(); i++) {

		char letter = tolower(move[i]);	
	
		for(size_t j = 0; j < hand.size(); j++) {
			if(tolower(hand[j]->getLetter()) == letter ) {
				if(resolveBlanks && letter == '?') {
					hand[j]->useAs(move[i+1]);
					i++;
				}
				place.push_back(hand[j]);
				hand.erase(hand.begin() + j);
				break;
			}
		}
	}

	return place;
}

void Player::addTiles(vector<Tile*> const & tilesToAdd) {

	for(size_t i = 0; i < tilesToAdd.size(); i++) {
		hand.push_back(tilesToAdd[i]);
	}

}

size_t Player::getMaxTiles() const {
	return handsize;
}
