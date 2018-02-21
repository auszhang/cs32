#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include <iostream>
#include <string>

using namespace std;

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
};
#endif