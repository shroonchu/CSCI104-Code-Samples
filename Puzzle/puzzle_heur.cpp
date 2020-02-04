#include "puzzle_heur.h"
#include <cstdlib>

  // counts Manhattan distance of each tile
  // **EXCLUDING the blank tile** from its
  // correct location in the solved board.
int PuzzleManhattanHeuristic::compute(const Board& b){
	int h = 0;
	for(int i=0; i<b.size(); i++){
		if(b[i]==0){;}
		else{
			int side_dim = b.dim();
			// row and col of tile
			int ir = i/side_dim;
			int ic = i%side_dim;
			// row and col of where it should be
			int t = b[i];
			int tr = t/side_dim;
			int tc = t%side_dim;
			h += abs(tr-ir) + abs(tc-ic);
		}
	}
	return h;
}

  // Counts the number of tiles out of place
  // **EXCLUDING the blank tile**.
int PuzzleOutOfPlaceHeuristic::compute(const Board& b){
	int h = 0;
	for(int i=0; i<b.size(); i++){
		if(b[i]==0){;}
		else if(b[i] != i){
			h++;
		}
	}
	return h;
}

int PuzzleBFSHeuristic::compute(const Board& b){
	return 0;
}