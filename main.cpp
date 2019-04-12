#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <sstream>

#include "board.h"
#include "utility.h"

using namespace std;

template < template < class ... > class Container, class ... Args >
void arrangeModels(BoardPlate& bp, Container<Model, Args...>& models);

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

template < template < class ... > class Container, class ... Args >
void arrangeModels(BoardPlate& bp, Container<Model, Args...>& models) {
  sort(models.begin(), models.end(), [&](Model& a, Model& b) {
    return (a.width * a.height) > (b.width * b.height);
  });

  for (auto& m : models) {
    bp.addModel(m);
    bp.printBoard();
  }
}

