#include "History.h"
#include <iostream>

History::History(int nRows, int nCols) 
{
	mRows = nRows;
	mCols = nCols;
	mBoard[nRows][nCols];
	for (int i = 1; i <= nRows; i++)
	{
		for (int j = 1; j <= nCols; j++)
			mBoard[i][j] = '.';
	}
}

bool History::record(int r, int c)
{
	//The function returns false if row r, column c is not within bounds
	if (r < 1 || c < 1 || r > MAXROWS || c > MAXCOLS)
		return false;

	//notify the History object that a poisoned rat has ended its turn poisoned but alive at a grid point.
	if (mBoard[r][c] == '.')
		mBoard[r][c] = 'A';
	else if(mBoard[r][c] < 'Z')
		mBoard[r][c] += 1;
	
	return true; 
}

void History::display() const
{
	clearScreen();
	for (int i = 1; i <= mRows; i++)
	{
		for (int j = 1; j <= mCols; j++)
			cout << mBoard[i][j];
		cout << endl;
	}
	cout << endl;
}

