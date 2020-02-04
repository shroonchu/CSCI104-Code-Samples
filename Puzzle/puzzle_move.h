#ifndef PUZZLEMOVE_H
#define PUZZLEMOVE_H
#include "board.h"
#include "puzzle_heur.h"

using namespace std;

struct PuzzleMove
{
  // Data members can be public
  int tileMove;  // tile moved to reach the Board b
  Board *b;      // Pointer to a board representing the updated state
  int g;         // distance from the start board
  int h;         // heuristic distance to the goal
  PuzzleMove *prev;  // Pointer to parent PuzzleMove


  // Constructor for starting Board (i.e. initial move)
  PuzzleMove(Board* board);

  // Constructor for subsequent search boards 
  // (i.e. those returned by Board::potentialMoves() )
  PuzzleMove(int tile, Board* board, PuzzleMove* parent, PuzzleHeuristic* ph);

  // Copy constructor
  PuzzleMove(const PuzzleMove& pm);

  // Destructor
  ~PuzzleMove();

  // returns the fscore
  int fscore();
};

struct PuzzleMoveScoreComp
{
  bool operator()(const PuzzleMove *m1, const PuzzleMove *m2) const
  {
    // Add code to check if m1's f-score is less than m2's
    // If both have the same f-score, break ties by 
    // checking if m1's h-score is less than m2's.
    // If both have the same f and same h score, break ties
    // by returning true when m1's tileMove is less than m2's.
    int f1 = m1->g + m1->h;
    int f2 = m2->g + m2->h;
    if(f1 < f2){return true;}
    else if(f1 == f2){
      if(m1->h < m2->h){return true;}
      else if(m1->h == m2->h){
        return m1->tileMove < m2->tileMove;
      }
    }
    return false;
  }
};

struct PuzzleMoveBoardComp
{
  bool operator()(const PuzzleMove *m1, const PuzzleMove *m2) const
  {
    // Add code to check if m1's board is "less-than" m2's board by 
    // using the Board class' operator<()
    Board temp1 = *(m1->b);
    Board temp2 = *(m2->b);
    return temp1 < temp2;
  }
};

#endif
