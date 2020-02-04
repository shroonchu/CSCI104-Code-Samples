#include "puzzle_move.h"
#include <cstdlib>

// Constructor for starting Board (i.e. initial move)
PuzzleMove::PuzzleMove(Board* board){
	tileMove = 0;
	b = new Board(*board);
	g=0;
	h=0;
	prev = nullptr;
}

// Constructor for subsequent search boards 
// (i.e. those returned by Board::potentialMoves() )
PuzzleMove::PuzzleMove(int tile, Board* board, PuzzleMove* parent, 
	PuzzleHeuristic* ph){
	tileMove = tile;
	b = new Board(*board);
	g = parent->g +1;
	// use the puzzleheuristic to calculate h
	h = ph->compute(*board);
	prev = parent;
}

// Copy constructor
PuzzleMove::PuzzleMove(const PuzzleMove& pm){
	tileMove = pm.tileMove;
	b = new Board(*(pm.b));
	g=pm.g;
	h=pm.h;
	prev=pm.prev;
}

// Destructor
PuzzleMove::~PuzzleMove(){
	delete b;
}

// returns the fscore
int PuzzleMove::fscore(){
	return g+h;
}