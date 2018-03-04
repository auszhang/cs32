#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
public:
	BoardImpl(const Game& g);
	void clear();
	void block();
	void unblock();
	bool placeShip(Point topOrLeft, int shipId, Direction dir);
	bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
	void display(bool shotsOnly) const;
	bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
	bool allShipsDestroyed() const;
	
	vector<int> damage;
	vector<int> id;
	int getID(char symbol) {
		for (int i = 0; i < id.size(); i++) {
			if (symbol == m_game.shipSymbol(id[i]))
				return id[i];
		}
		return -1;
	}
	int getIndex(int symbol) {
		for (int i = 0; i < id.size(); i++) {
			if (symbol == m_game.shipSymbol(id[i]))
				return i;
		}
		return -1;
	}
	vector<int> destroyedID;

private:
	// TODO:  Decide what private members you need.  Here's one that's likely
	//        to be useful:
	const Game& m_game;
	char m_board[MAXROWS][MAXCOLS];
	int m_nShips;
};

BoardImpl::BoardImpl(const Game& g)
	: m_game(g)
{
	m_nShips = g.nShips();
	for (int r = 0; r < m_game.rows(); r++)
		for (int c = 0; c < m_game.cols(); c++)
			m_board[r][c] = '.';

}

void BoardImpl::clear()
{
	for (int r = 0; r < m_game.rows(); r++)
		for (int c = 0; c < m_game.cols(); c++)
			m_board[r][c] = '.';

}

void BoardImpl::block()
{
	// Block cells with 50% probability
	for (int r = 0; r < m_game.rows(); r++)
		for (int c = 0; c < m_game.cols(); c++)
			if (randInt(2) == 0) {
				m_board[r][c] = '#';
			}
}

void BoardImpl::unblock()
{
	for (int r = 0; r < m_game.rows(); r++)
		for (int c = 0; c < m_game.cols(); c++) {
			if (m_board[r][c] == '#')
				m_board[r][c] = '.';
		}
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
	//checks if ship ID is valid
	if (shipId < 0 || shipId > m_game.nShips() - 1)
		return false;

	//checks if a ship with the same ID has already been placed on the board
	for (int r = 0; r < m_game.rows(); r++) {
		for (int c = 0; c < m_game.cols(); c++) {
			if (m_board[r][c] == m_game.shipSymbol(shipId))
				return false;
		}
	}

	//check if ship is sticking out of the board, if ship would be placed in an open space
	if (dir == VERTICAL) {
			for (int r = 0; r < m_game.shipLength(shipId); r++) {
				Point current(topOrLeft.r + r, topOrLeft.c);
				if (m_board[topOrLeft.r + r][topOrLeft.c] != '.' || !m_game.isValid(current))
					return false;
			}
	}
	if (dir == HORIZONTAL) {
			for (int c = 0; c < m_game.shipLength(shipId); c++) {
				Point current(topOrLeft.r, topOrLeft.c + c);
				if (m_board[topOrLeft.r][topOrLeft.c + c] != '.' || !m_game.isValid(current))
					return false;
			}
	}

	//make changes to the board.
	if (dir == VERTICAL) {
		for (int r = 0; r < m_game.shipLength(shipId); r++)
			m_board[topOrLeft.r + r][topOrLeft.c] = m_game.shipSymbol(shipId);
	}
	if (dir == HORIZONTAL) {
		for (int c = 0; c < m_game.shipLength(shipId); c++)
			m_board[topOrLeft.r][topOrLeft.c + c] = m_game.shipSymbol(shipId);
	}

	damage.push_back(0);
	id.push_back(shipId);

	return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
	//checks if ship ID is valid
	if (shipId < 0 || shipId > m_game.nShips() - 1)
		return false;
	
	//check if board contains the ship in the indicated directions
	if (dir == VERTICAL) {
		for (int r = 0; r < m_game.shipLength(shipId); r++)
			if (m_board[topOrLeft.r + r][topOrLeft.c] != m_game.shipSymbol(shipId))
				return false;
	}
	if (dir == HORIZONTAL) {
		for (int c = 0; c < m_game.shipLength(shipId); c++)
			if (m_board[topOrLeft.r][topOrLeft.c + c] != m_game.shipSymbol(shipId))
				return false;
	}

	//make changes to the board.
	if (dir == VERTICAL) {
		for (int r = 0; r < m_game.shipLength(shipId); r++)
			m_board[topOrLeft.r + r][topOrLeft.c] = '.';
	}
	if (dir == HORIZONTAL) {
		for (int c = 0; c < m_game.shipLength(shipId); c++)
			m_board[topOrLeft.r][topOrLeft.c + c] = '.';
	}
	return true;
}

void BoardImpl::display(bool shotsOnly) const
{
	//display first line
	cout << "  ";
	for (int c = 0; c < m_game.rows(); c++)
		cout << c;
	cout << endl;

	//for each row...
	for (int r = 0; r < m_game.rows(); r++) {
		cout << r << " ";
		//print the row
		for (int c = 0; c < m_game.cols(); c++) {
			if (!shotsOnly) {
				//prints normally
					cout << m_board[r][c];
			}
			else {
				if (m_board[r][c] != '.' &&
					m_board[r][c] != 'X' &&
					m_board[r][c] != 'o')
					cout << '.';
				else
					cout << m_board[r][c];
			}
		}
		cout << endl;
	}
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
	//if the attack is invalid
	if (!m_game.isValid(p) || m_board[p.r][p.c] == 'X' || m_board[p.r][p.c] == 'o' || m_board[p.r][p.c] == '#')
		return false;
	
	//attack succeeded!
	if (m_board[p.r][p.c] != '.' && m_board[p.r][p.c] != 'X' && m_board[p.r][p.c] != 'o') {
		shotHit = true;
		char hitShip = m_board[p.r][p.c];
		m_board[p.r][p.c] = 'X';
		int ID = getID(hitShip);
		int index = getIndex(hitShip);
		damage[index] = damage[index] + 1;

		//if the ship was completely destroyed...
		if (damage[index] == m_game.shipLength(getID(hitShip))) {
			shipDestroyed = true;
			shipId = ID;
			//destroyedID.push_back(ID); //////////////////////
			m_nShips--;

		}
		else {
			shipDestroyed = false;
		}
		return true;
	}
	else if (m_board[p.r][p.c] == '.'){
		//miss
		m_board[p.r][p.c] = 'o';
		shotHit = false;
		shipDestroyed = false;
		return true;
	}
	else if (m_board[p.r][p.c] == 'o') {
		shotHit = false;
		shipDestroyed = false;
		return false;
	}
	else if (m_board[p.r][p.c] == 'X') {
		shotHit = false;
		shipDestroyed = false;
		return false;
	}

	
}

bool BoardImpl::allShipsDestroyed() const
{
	if (m_nShips == 0) {
		return true;
	}
	else {
		return false;
	}
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
	m_impl = new BoardImpl(g);
}

Board::~Board()
{
	delete m_impl;
}

void Board::clear()
{
	m_impl->clear();
}

void Board::block()
{
	return m_impl->block();
}

void Board::unblock()
{
	return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
	return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
	return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
	m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
	return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
	return m_impl->allShipsDestroyed();
}
