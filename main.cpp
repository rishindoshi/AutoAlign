#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <sstream>

#include "board.h"
#include "utility.h"

using namespace std;

template < template < class ... > class Container, class ... Args >
void test(Container<Model, Args...>& tester) {
  for (auto& m : tester) {
    cout << m.width << endl;
  }
}

void arrangeModels(BoardPlate& bp, vector<Model>& models);
void getInput(int& boardWidth, int& boardHeight, vector<Model>& models);

int main(int argc, char** argv) {
  vector<Model> models;
  int boardWidth, boardHeight;

  getInput(boardWidth, boardHeight, models);

  BoardPlate bp(boardWidth, boardHeight);

  arrangeModels(bp, models);
  
  return 0;
}

void getInput(int& boardWidth, int& boardHeight, vector<Model>& models) {
  cin >> boardWidth >> boardHeight;
  string str; 
  // Trash newline
  getline(cin, str);

  while (getline(cin, str)) {
    stringstream ss(str);
    int width, height;
    char name;
    ss >> width >> height >> name;

    Model m(width, height, name);
    models.push_back(m);
  }
}

void arrangeModels(BoardPlate& bp, vector<Model>& models) {
  sort(models.begin(), models.end(), [&](Model& a, Model& b) {
    return (a.width * a.height) > (b.width * b.height);
  });

  for (auto& m : models) {
    bp.addModel(m);
    bp.printBoard();
  }
}

