#ifndef PLAYER_H
#define PLAYER_H

using namespace std;



class Arena; //TODO: CANNOT INCLUDE THIS

class Player
{
public:
	// Constructor
	Player(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	string dropPoisonPellet();
	string move(int dir);
	void   setDead();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool   m_dead;
};
#endif