#include "Dictionary.h"
#include "Player.h"

using namespace std;

//makes words lowercase and inputs them into the dictionary
Dictionary::Dictionary(string dictionary_file_name) {

	ifstream input;
	input.open(dictionary_file_name);
	string line;
	while(getline(input, line)) {
		string word = "";
		for(size_t i = 0; i < line.size(); i++) {
			word += tolower(line[i]);
		}
		dictionary.insert(word);
	}
	input.close();
}

Dictionary::~Dictionary() {}

//makes word lowercase and looks for it
bool Dictionary::isLegalWord(string const& word) const {
	set<string>::iterator it;
	string new_word = "";
	if(word.size() == 1) {
		return false;
	}

	for(size_t i = 0; i < word.size(); i++){
		new_word += tolower(word[i]);
	}
	it = dictionary.find(new_word);

	if(it != dictionary.end()) {
		return true;
	}
	return false;
}

