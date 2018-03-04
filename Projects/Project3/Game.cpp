#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

class GameImpl
{
public:
	GameImpl(int nRows, int nCols);
	int rows() const;
	int cols() const;
	bool isValid(Point p) const;
	Point randomPoint() const;
	bool addShip(int length, char symbol, string name);
	int nShips() const;
	int shipLength(int shipId) const;
	char shipSymbol(int shipId) const;
	string shipName(int shipId) const;
	Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
	vector<char> m_symbol; //relates ID to symbol
	vector<int> m_length; //relates ID to length
	vector<string> m_name; //relates ID to name
	int getID(char symbol) {
		for (int i = 0; i < m_symbol.size(); i++) {
			if (m_symbol[i] == symbol)
				return i;
		}
		return -1;
	}
private:
	int m_rows, m_cols;
	
};

void waitForEnter()
{
	cout << "Press enter to continue: ";
	cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
}

int GameImpl::rows() const
{
	return m_rows;
}

int GameImpl::cols() const
{
	return m_cols;
}

bool GameImpl::isValid(Point p) const
{
	return p.r >= 0 && p.r < rows() && p.c >= 0 && p.c < cols();
}

Point GameImpl::randomPoint() const
{
	return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
	if (length <= 0 || length > MAXROWS || length > MAXCOLS)
		return false;
	if (symbol == 'o' || symbol == 'X' || symbol == '.' || symbol == '#')
		return false;
	if (name == "")
		return false;

	//add the ship
	m_symbol.push_back(symbol);
	m_length.push_back(length);
	m_name.push_back(name);
	return true;
}

int GameImpl::nShips() const
{
	return m_symbol.size();
}

int GameImpl::shipLength(int shipId) const
{
	return m_length[shipId];
}

char GameImpl::shipSymbol(int shipId) const
{
	return m_symbol[shipId];
}

string GameImpl::shipName(int shipId) const
{
	return m_name[shipId];
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
	//both players place their ships
	
	if (p1->placeShips(b1) == false)
		return nullptr;
	if (p2->placeShips(b2) == false)
		return nullptr;

	//game start
	while (b1.allShipsDestroyed() == false && b2.allShipsDestroyed() == false) {
		//p1's turn
		cout << p1->name() << "'s turn. Board for " << p2->name() << ":" << endl;
		if (p1->isHuman())
			b2.display(true);
		else
			b2.display(false); //otherwise, displays the whole board
							   //first player attacks
		bool result1 = false;
		bool hit1 = false , destroyed1 = false;
		int shipId1 = -1;
		Point point1(0, 0);
		
		while (result1 == false) {
			point1 = p1->recommendAttack();
			result1 = b2.attack(point1, hit1, destroyed1, shipId1);
			if (p1->isHuman())
				break;
		}	
		p1->recordAttackResult(point1, result1, hit1, destroyed1, shipId1);
		p2->recordAttackByOpponent(point1);
		
		if (result1 == false)
			cout << p1->name() << " wasted a shot at (" << point1.r << "," << point1.c << ")." << endl;
		else 
		if (hit1) {
			if(!destroyed1)
				cout << p1->name() << " attacked (" << point1.r << "," << point1.c << ") and hit something, resulting in: " << endl;
			if(destroyed1)
				cout << p1->name() << " attacked (" << point1.r << "," << point1.c << ") and destroyed the " << shipName(shipId1) << ", resulting in: " << endl;
		}
		else if(!hit1) {
			cout << p1->name() << " attacked (" << point1.r << "," << point1.c << ") and missed, resulting in: " << endl;
		} 
		if (result1 == true) {
			if (p1->isHuman())
				b2.display(true);
			else
				b2.display(false);
		}
		
		if (b2.allShipsDestroyed()) {
			break;
		}
		if (shouldPause)
			waitForEnter();

		//p2's turn
		cout << p2->name() << "'s turn. Board for " << p1->name() << ":" << endl;
		if (p2->isHuman())
			b1.display(true);
		else
			b1.display(false); //otherwise, displays the whole board
							   //second player attacks
		bool result2 = false;
		bool hit2 = false, destroyed2 = false;
		int shipId2 = -1;
		Point point2(0, 0);
		while (result2 == false) {
			point2 = p2->recommendAttack();
			result2 = b1.attack(point2, hit2, destroyed2, shipId2);
			if (p2->isHuman())
				break;
		}
		p2->recordAttackResult(point2, result2, hit2, destroyed2, shipId2);
		p1->recordAttackByOpponent(point2);
		
		if (result2 == false)
			cout << p2->name() << " wasted a shot at (" << point2.r << "," << point2.c << ")." << endl;
		else
		if (hit2) {
			if(!destroyed2)
				cout << p2->name() << " attacked (" << point2.r << "," << point2.c << ") and hit something, resulting in: " << endl;
			if(destroyed2)
				cout << p2->name() << " attacked (" << point2.r << "," << point2.c << ") and destroyed the " << shipName(shipId2) << ", resulting in: " << endl;
		}
		else if(!hit2){
			cout << p2->name() << " attacked (" << point2.r << "," << point2.c << ") and missed, resulting in: " << endl;
		}
		
		if (result2 == true) {
			if (p2->isHuman())
				b1.display(true);
			else
				b1.display(false);
		}
		if (b1.allShipsDestroyed()) {
			break;
		}
		if (shouldPause)
			waitForEnter();

	}
	if (b1.allShipsDestroyed()) {
		cout << p2->name() << " wins!" << endl;
		return p2;
	}
	if (b2.allShipsDestroyed()) {
		cout << p1->name() << " wins!" << endl;
		return p1;
	}
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
	if (nRows < 1 || nRows > MAXROWS)
	{
		cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
		exit(1);
	}
	if (nCols < 1 || nCols > MAXCOLS)
	{
		cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
		exit(1);
	}
	m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
	delete m_impl;
}

int Game::rows() const
{
	return m_impl->rows();
}

int Game::cols() const
{
	return m_impl->cols();
}

bool Game::isValid(Point p) const
{
	return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
	return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
	if (length < 1)
	{
		cout << "Bad ship length " << length << "; it must be >= 1" << endl;
		return false;
	}
	if (length > rows() && length > cols())
	{
		cout << "Bad ship length " << length << "; it won't fit on the board"
			<< endl;
		return false;
	}
	if (!isascii(symbol) || !isprint(symbol))
	{
		cout << "Unprintable character with decimal value " << symbol
			<< " must not be used as a ship symbol" << endl;
		return false;
	}
	if (symbol == 'X' || symbol == '.' || symbol == 'o')
	{
		cout << "Character " << symbol << " must not be used as a ship symbol"
			<< endl;
		return false;
	}
	int totalOfLengths = 0;
	for (int s = 0; s < nShips(); s++)
	{
		totalOfLengths += shipLength(s);
		if (shipSymbol(s) == symbol)
		{
			cout << "Ship symbol " << symbol
				<< " must not be used for more than one ship" << endl;
			return false;
		}
	}
	if (totalOfLengths + length > rows() * cols())
	{
		cout << "Board is too small to fit all ships" << endl;
		return false;
	}
	return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
	return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
	assert(shipId >= 0 && shipId < nShips());
	return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
	if (p1 == nullptr || p2 == nullptr || nShips() == 0)
		return nullptr;
	Board b1(*this);
	Board b2(*this);
	return m_impl->play(p1, p2, b1, b2, shouldPause);
}

