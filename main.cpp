#include <iostream>
#include <vector>
#include <deque>

#include "board.h"
#include "utility.h"

using namespace std;

void arrangeModels(BoardPlate& bp, vector<Model>& models);

int main(int argc, char** argv) {

  Model m_a(4, 2, 'a');
  Model m_b(2, 4, 'b');
  Model m_c(2, 2, 'c');
  Model m_d(2, 1, 'd');
  Model m_e(3, 3, 'e');

  BoardPlate bp(10, 8);

  bp.addModel(m_a);
  bp.printBoard();
  
  bp.addModel(m_b);
  bp.printBoard();

  bp.addModel(m_c);
  bp.printBoard();
  
  bp.addModel(m_d);
  bp.printBoard();
  
	return 0;
}


void arrangeModels(BoardPlate& bp, vector<Model>& models) {
}

