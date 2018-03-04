#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>
using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
	AwfulPlayer(string nm, const Game& g);
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
private:
	Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
	: Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
	// Clustering ships is bad strategy
	for (int k = 0; k < game().nShips(); k++)
		if (!b.placeShip(Point(k, 0), k, HORIZONTAL))
			return false;
	return true;
}

Point AwfulPlayer::recommendAttack()
{
	if (m_lastCellAttacked.c > 0)
		m_lastCellAttacked.c--;
	else
	{
		m_lastCellAttacked.c = game().cols() - 1;
		if (m_lastCellAttacked.r > 0)
			m_lastCellAttacked.r--;
		else
			m_lastCellAttacked.r = game().rows() - 1;
	}
	return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
	bool /* shotHit */, bool /* shipDestroyed */,
	int /* shipId */)
{
	// AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
	// AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
	bool result(cin >> r >> c);
	if (!result)
		cin.clear();  // clear error state so can do more input operations
	cin.ignore(10000, '\n');
	return result;
}

class HumanPlayer : public Player 
{
public:
	HumanPlayer(string nm, const Game& g);
	virtual bool placeShips(Board& b);
	virtual bool isHuman() const { return true; }
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
private:
	Point m_lastCellAttacked;
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
	: Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool HumanPlayer::placeShips(Board& b)
{
	//follow the executable
	cout << name() << " must place 5 ships." << endl;
	b.display(false);
	for (int i = 0; i < game().nShips(); i++) {
		EnterDirection:
		cout << "Enter h or v for the direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
		string line;
		getline(cin, line);
		if (line[0] != 'h' && line[0] != 'v') {
			cout << "Direction must be h or v." << endl;
			goto EnterDirection;
		}
		if (line[0] == 'h') {
			Direction d = HORIZONTAL;
			int r = 0, c = 0;
			EnterLeftmost:
			cout << "Enter row and column of leftmost cell (e.g. 3 5): " << endl;
			if (getLineWithTwoIntegers(r, c) == false) {
				cout << "You must enter two integers." << endl;
				goto EnterLeftmost;
			}
			else {
				//place the ships or prompt a different point if unsuccessful
				Point p(r, c);
				if (b.placeShip(p, i, d) == false) {
					cout << "The ship cannot be placed there." << endl;
					goto EnterLeftmost;
				}
				else {
					//successfully place.
					b.display(false);
				}
			}

		}
		if (line[0] == 'v') {
			Direction d = VERTICAL;
			int r = 0, c = 0;
			EnterTopmost:
			cout << "Enter row and column of topmost cell (e.g. 3 5): " << endl;
			if (getLineWithTwoIntegers(r, c) == false) {
				cout << "You must enter two integers." << endl;
				goto EnterTopmost;
			}
			else {
				//place the ships or prompt a different point if unsuccessful
				Point p(r, c);
				if (b.placeShip(p, i, d) == false) {
					cout << "The ship cannot be placed there." << endl;
					goto EnterTopmost;
				}
				else {
					//successfully place.
					b.display(false);
				}
			}
		}
	} //all ships placed
	return true;
}

Point HumanPlayer::recommendAttack()
{
	int r = 0, c = 0;
	EnterAttack:
	cout << "Enter the row and column to attack(e.g, 3 5): ";
	if (getLineWithTwoIntegers(r, c) == false) {
		cout << "You must enter two integers." << endl;
		goto EnterAttack;
	}
	else {
		Point p(r, c);
		return p;
	}
}

void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
	bool /* shotHit */, bool /* shipDestroyed */,
	int /* shipId */)
{
	//do nothing
}

void HumanPlayer::recordAttackByOpponent(Point /* p */)
{
	//do nothing
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************
class MediocrePlayer : public Player 
{
public:
	MediocrePlayer(string nm, const Game& g);
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);

	bool recursivelyPlaceShips(Board& b, vector<int> &shipsToPlace, int index);
	bool isInVector(vector<Point> aVector, Point p);
private:
	Point m_lastCellAttacked;
	int state;
	vector<Point> alreadyRecommended;
	vector<Point> state2Targets;

};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
	: Player(nm, g), m_lastCellAttacked(0, 0), state(1)
{}

bool MediocrePlayer::placeShips(Board& b) 
{
	vector<int> ships;
	for (int i = 0; i < game().nShips(); i++)
		ships.push_back(i);

	for (int i = 0; i < 50; i++) {
		b.block();
		bool success = recursivelyPlaceShips(b, ships, 0);
		b.unblock();
		if (success)
			return true;
	}
	return false;
}

bool MediocrePlayer::recursivelyPlaceShips(Board& b, vector<int> &shipsToPlace, int index) {
	//base case
	if (index == shipsToPlace.size())
		return true;
	//start here...
	Point p(0, 0);
	Direction d = VERTICAL;
	int length = game().shipLength(shipsToPlace[index]); //ship length for target ship
	while (b.placeShip(p, shipsToPlace[index], d) == false) {
	tryToPlace:
		if (d = HORIZONTAL) {
			//do horizontal stuff
			if (p.r == game().rows() - 1 && p.c == game().cols() - 1 - length) {
				d = VERTICAL;
				p.r = 0;
				p.c = 0;
				continue;
			}

			if (p.c <= game().cols() - length)
				p.c++;
			else {
				p.c = 0;
				p.r++;
			}
		}
		else {
			//do vertical stuff
			if (p.r == game().rows() - 1 - length && p.c == game().cols() - 1) {
				return false; // No place for this ship.
			}
			if (p.r <= game().rows() - length)
				p.r++;
			else {
				p.r = 0;
				p.c++;
			}
		}
	} //end while loop
	
	bool success = recursivelyPlaceShips(b, shipsToPlace, index + 1);
	if (success)
		return success;
	else {
		b.unplaceShip(p, shipsToPlace[index], d);
		goto tryToPlace;
	}
}

bool MediocrePlayer::isInVector(vector<Point> aVector, Point p) {
	for (size_t i = 0; i < alreadyRecommended.size(); i++) {
		if (alreadyRecommended[i].r == p.r && alreadyRecommended[i].c == p.c)
			return true; //found!
	}
	return false;
}

Point MediocrePlayer::recommendAttack() 
{
	Point p(0, 0);
	if (state == 2 && state2Targets.empty())
		state = 1;
	if (state == 1) {
		//reccommend a state 1 action
		bool done = false;
		while (!done) {
			p = game().randomPoint();
			if (isInVector(alreadyRecommended, p) == false)
				done = true;
		}
		alreadyRecommended.push_back(p);
		return p;
	}
	else if(state == 2) {
		//reccommend a state 2 action
		p = state2Targets[state2Targets.size() - 1];
		state2Targets.pop_back();
		return p;
	}
	return p; //probably will never reach here... this is just so all paths return a value
}

void MediocrePlayer::recordAttackResult(Point  p , bool  validShot , bool shotHit , bool  shipDestroyed, int shipId )
{
	if (validShot) {
		m_lastCellAttacked = p;
		if (state == 1) {
			//state1
			if (!shotHit)
				state = 1;
			if (shotHit && shipDestroyed)
				state = 1;
			if (shotHit && !shipDestroyed) {
				//transition into state 2
				state = 2;
				for (int i = 0; i <5; i++) {
					Point up(p.r - i, p.c);
					if (game().isValid(up))
						state2Targets.push_back(up);
					Point down(p.r + i, p.c);
					if (game().isValid(down))
						state2Targets.push_back(down);
					Point left(p.r, p.c - i);
					if (game().isValid(left))
						state2Targets.push_back(left);
					Point right(p.r, p.c + i);
					if (game().isValid(right))
						state2Targets.push_back(right);
				}
			}
		}
		else {
			if (!shotHit || (shotHit && !shipDestroyed))
				state = 2;
			if (shotHit && !shipDestroyed) {
				state = 2;
			}
			if (shotHit && shipDestroyed) {
				while (!state2Targets.empty())
					state2Targets.pop_back();
				state = 1;
					//if(state2Targets.empty())
					//inState1 = true;
			}
		}
	}
}

void MediocrePlayer::recordAttackByOpponent(Point /* p */)
{
	//does nothing
}


//*********************************************************************
//  GoodPlayer
//*********************************************************************
class GoodPlayer : public Player
{
public:
	GoodPlayer(string nm, const Game& g);
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
	bool recursivelyPlaceShips(Board & b, vector<int> &shipsToPlace, int index);
	bool isInVector(vector<Point> aVector, Point p);
	Point avoidWaste();
private:
	Point m_lastCellAttacked;
	//vector<Point> toCheck;
	//vector<Point> hits;
	vector<Point> alreadyRecommended;
	int state;
	bool firstTurn;
	int turn;
	char attacked[MAXROWS][MAXCOLS];
	stack <Point> checkThese;
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
	: Player(nm, g), m_lastCellAttacked(0, 0), state(/*0*/1), firstTurn(true), turn(1)
{
	for (int i = 0; i<game().rows(); i++)
		for (int j = 0; j<game().cols(); j++)
		{
			attacked[i][j] = 'n';
		}
}

bool GoodPlayer::placeShips(Board& b)
{
	vector<int> ships;
	for (int i = 0; i < game().nShips(); i++)
		ships.push_back(i);
	return recursivelyPlaceShips(b, ships, 0);
}

bool GoodPlayer::recursivelyPlaceShips(Board & b, vector<int> &shipsToPlace, int index) 
{
	if (index == shipsToPlace.size())
		return true;
	tryPlacing:
	Point p = game().randomPoint();
	if (b.placeShip(p, index, VERTICAL)) {
		if (recursivelyPlaceShips(b, shipsToPlace, index + 1)) 
			return true;
			b.unplaceShip(p, index, VERTICAL);
	
	} else
	if (b.placeShip(p, index, HORIZONTAL)) {
		if (recursivelyPlaceShips(b, shipsToPlace, index + 1)) 
			return true;
			b.unplaceShip(p, index, HORIZONTAL);
	}
	else {
		goto tryPlacing;
	}
	return false;
}

bool GoodPlayer::isInVector(vector<Point> aVector, Point p) {
	for (size_t i = 0; i < alreadyRecommended.size(); i++) {
		if (alreadyRecommended[i].r == p.r && alreadyRecommended[i].c == p.c)
			return true; //found!
	}
	return false;
}

Point GoodPlayer::avoidWaste()
{

	for (int i = 0; ; i++)
	{
		Point p = game().randomPoint();

		{
			if (p.r % 2 == 0 && p.c % 2 != 0)
				p.r = p.r + 1;
			else if (p.c % 2 == 0 && p.r % 2 != 0)
				p.c = p.c + 1;
		}
		if (attacked[p.r][p.c] == 'n')
		{
			attacked[p.r][p.c] = 'y';
			return p;
		}
	}

}

Point GoodPlayer::recommendAttack() 
{
	//state 1
	if (state == 1) {
		if (turn == 1) {
			Point p(game().rows() / 2, game().cols() / 2);
			attacked[p.r][p.c] = 'y';
			turn++;
			return p;
		}
		if (turn == 2) {
			Point p(game().rows() / 4, 0);
			if (attacked[p.r][p.c] == 'n') {
				attacked[p.r][p.c] = 'y';
				turn++;
				return p;
			}
		}
		if (turn == 3) {
			Point p((game().rows()) - (game().rows() / 4), (game().cols()) - 1);
			if (attacked[p.r][p.c] == 'n') {
				attacked[p.r][p.c] = 'y';
				turn++;
				return p;
			}
		}
		if (turn == 4) {
			Point p(game().rows() / 4, (game().cols()) - 1);
			if (attacked[p.r][p.c] == 'n') {
				attacked[p.r][p.c] = 'y';
				turn++;
				return p;
			}
		}
		if (turn == 5) {
			Point p((game().rows()) - (game().rows() / 4), 0);
			if (attacked[p.r][p.c] == 'n')
			{
				attacked[p.r][p.c] = 'y';
				turn++;
				return p;
			}
		}
		for (int i = 0; ; i++) {
			Point p = game().randomPoint();
			
			if (p.r % 2 == 0 && p.c % 2 != 0)
				p.r = p.r + 1;
			else if (p.c % 2 == 0 && p.r % 2 != 0)
				p.c = p.c + 1;
			if (attacked[p.r][p.c] == 'n') {
				attacked[p.r][p.c] = 'y';
				return p;
			}
		}
		return avoidWaste();
	}

	if (state == 2) {
		while (!checkThese.empty()) {
			Point p = checkThese.top();
			checkThese.pop();
			if (attacked[p.r][p.c] == 'n') {
				attacked[p.r][p.c] = 'y';
				return p;
			}
		}
		return avoidWaste();

	}
}

void GoodPlayer::recordAttackResult(Point  p, bool  validShot,
	bool shotHit, bool  shipDestroyed,
	int  shipId)
{
	if (validShot && shotHit && !shipDestroyed)
	{
		if (p.r + 1<game().rows())
			checkThese.push(Point(p.r + 1, p.c));
		if (p.c + 1<game().cols())
			checkThese.push(Point(p.r, p.c + 1));
		if (p.r - 1 >= 0)
			checkThese.push(Point(p.r - 1, p.c));
		if (p.c - 1 >= 0)
			checkThese.push(Point(p.r, p.c - 1));

		state = 2;
	}
	if (checkThese.empty())
		state = 1;

	if (!shotHit)
		;
	if (shipDestroyed == true)
	{
		if (!checkThese.empty())
			state = 2;
		else
			state = 1;
	}
}

void GoodPlayer::recordAttackByOpponent(Point /* p */)
{
	//do nothing
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
	static string types[] = {
		"human", "awful", "mediocre", "good"
	};

	int pos;
	for (pos = 0; pos != sizeof(types) / sizeof(types[0]) &&
		type != types[pos]; pos++)
		;
	switch (pos)
	{
	case 0:  return new HumanPlayer(nm, g);
	case 1:  return new AwfulPlayer(nm, g);
	case 2:  return new MediocrePlayer(nm, g);
	case 3:  return new GoodPlayer(nm, g);
	default: return nullptr;
	}
}
