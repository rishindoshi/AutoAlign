#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

#include "board.h"
#include "utility.h"

using namespace std;

void arrangeModels(BoardPlate& bp, vector<Model>& models);

int main(int argc, char** argv) {

  Model m_a(4, 2, 'a');
  Model m_b(2, 4, 'b');
  Model m_c(2, 2, 'c');
  Model m_d(2, 1, 'd');

  vector<Model> models;
  models.push_back(m_a);
  models.push_back(m_b);
  models.push_back(m_c);
  models.push_back(m_d);

  BoardPlate bp(10, 8);
  
  arrangeModels(bp, models);
  
  return 0;
}

void arrangeModels(BoardPlate& bp, vector<Model>& models) {
  sort(models.begin(), models.end(), [&](Model& a, Model& b) {
    return (a.width * a.height) > (b.width * b.height);
  });

  for (auto& m : models) {
    bp.addModel(m);
  }

  bp.printBoard();
}

