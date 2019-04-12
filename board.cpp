#include "board.h"
#include "utility.h"

#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>
#include <cassert>

using namespace std;

void BoardPlate::setWidth(int width_) {
  width = width_;
}

void BoardPlate::setHeight(int height_) {
  height = height_;
}

void BoardPlate::reCenterModels(Model& newModel) {
  // Find min bounding box for all models
  bb.xmin = min(bb.xmin, newModel.getTopLeftX());
  bb.xmax = max(bb.xmax, newModel.getTopLeftX() + newModel.width - 1);
  bb.ymin = min(bb.ymin, newModel.getTopLeftY());
  bb.ymax = max(bb.ymax, newModel.getTopLeftY() + newModel.height - 1);

  if (bb.xmin > (width - bb.xmax)) {
    int diff = bb.xmin - (width - bb.xmax - 1);
    diff /= 2;
    for (auto& m : models) {
      m.xCoord -= diff;
    } 
    bb.xmin -= diff;
    bb.xmax -= diff;
  }

  else if ((width - bb.xmax - 1) > bb.xmin) {
    int diff = (width - bb.xmax - 1) - bb.xmin;
    diff /= 2;
    for (auto& m : models) {
      m.xCoord += diff;
    }
    bb.xmin += diff;
    bb.xmax += diff;
  }

  if (bb.ymin > (height - bb.ymax)) {
    int diff = bb.ymin - (height - bb.ymax - 1);
    diff /= 2;
    for (auto& m : models) {
      m.yCoord -= diff;
    }
    bb.ymin -= diff;
    bb.ymax -= diff;
  } 

  else if ((height - bb.ymax - 1) > bb.ymin) {
    int diff = (height - bb.ymax - 1) - bb.ymin;
    diff /= 2;
    for (auto& m : models) {
      m.yCoord += diff;
    }
    bb.ymin += diff;
    bb.ymax += diff;
  }
}

// Return true if centering the model at x, y fits within the bounds of the plate
// and does not overlap exisiting models
bool BoardPlate::modelCanFitInPos(const Model& m, int x, int y) {
  int mTopLeftX = x;
  int mTopLeftY = y;

  if (mTopLeftX < 0 || mTopLeftY < 0) {
    return false;
  }

  for (int i = 0; i < m.height; ++i) {
    for (int j = 0; j < m.width; ++j) {
      if (mTopLeftY + i >= height) {
        return false;
      }
      if (mTopLeftX + j >= width) {
        return false;
      }
      if (boardCoords[mTopLeftY + i][mTopLeftX + j] != '.') {
        return false;
      }
    }
  }
  return true;
}

int BoardPlate::calcAreaBoundingBox(const Model& m, const Coord& coord) {
  BoundingBox potentialBB(bb.xmin, bb.ymin, bb.xmax, bb.ymax);
  
  potentialBB.xmin = min(potentialBB.xmin, coord.x);
  potentialBB.xmax = max(potentialBB.xmax, coord.x + m.width - 1);
  potentialBB.ymin = min(potentialBB.ymin, coord.y);
  potentialBB.ymax = max(potentialBB.ymax, coord.y + m.height - 1);

  return (potentialBB.xmax - potentialBB.xmin) * (potentialBB.ymax - potentialBB.ymin);
}

// Start from center and search outwards in breadth first manner
// for an open position for the new model
// Then place model in open position and re center all models
bool BoardPlate::findOptimalPosForNewModel(Model& newModel) {
  Coord pos(cX - (newModel.width / 2), cY - (newModel.height / 2));
  queue<Coord> search;
  search.push(pos);

  vector<vector<bool>> visitedMap(height, vector<bool>(width, false));

  Coord found;
  bool fit = false;
  int minArea = INT_MAX;

  while (!search.empty()) {
    Coord top = search.front();

    // It's possible to get here even with the map checking before pushing
    if (visitedMap[top.y][top.x] == true) {
      search.pop();
      continue;
    }

    visitedMap[top.y][top.x] = true;
    search.pop();

    if (modelCanFitInPos(newModel, top.x, top.y)) {
      fit = true;

      int potentialArea = calcAreaBoundingBox(newModel, Coord(top.x, top.y));
      if (potentialArea < minArea) {
        minArea = potentialArea;
        found = top;
      }
    } 

    if ((top.x + 1 < width)) {
      if (!visitedMap[top.y][top.x + 1])
        search.push(Coord(top.x + 1, top.y));
    }
    if ((top.x - 1 >= 0)) {
      if (!visitedMap[top.y][top.x - 1])
        search.push(Coord(top.x - 1, top.y));
    }
    if ((top.y + 1 < height)) {
      if (!visitedMap[top.y + 1][top.x])
        search.push(Coord(top.x, top.y + 1));
    }
    if ((top.y - 1 >= 0)) {
      if (!visitedMap[top.y - 1][top.x])
        search.push(Coord(top.x, top.y - 1));
    }
  }

  if (!fit) {
    return false;
  }
  else {
    newModel.xCoord = found.x;
    newModel.yCoord = found.y;
    return true;
  }
}

void BoardPlate::shiftBBLeft() {
  bb.xmin -= 1;
  bb.xmax -= 1;
  for (auto& m : models) {
    m.xCoord -= 1; 
  }

  updateBoardCoords();
}

void BoardPlate::shiftBBUp() {
  bb.ymin -= 1;
  bb.ymax -= 1;
  for (auto& m : models) {
    m.yCoord -= 1;
  }

  updateBoardCoords();
}

void BoardPlate::addModel(Model& model) {
  if (models.empty()) {
    model.xCoord = cX - (model.width / 2);
    model.yCoord = cY - (model.height / 2);
  }
  else {
    while (findOptimalPosForNewModel(model) == false) {
      if (bb.xmin == 0 && bb.ymin == 0) break;

      if (bb.xmin > bb.ymin) {
        shiftBBLeft();      
      }
      else {
        shiftBBUp();
      }
    }
  }
  
  models.push_back(model);
  reCenterModels(model); 
  updateBoardCoords();
}

void BoardPlate::updateBoardCoords() {
  for_each(boardCoords.begin(), boardCoords.end(), [this](vector<char>& row) {
    fill(row.begin(), row.end(), '.');
  });
  
  for (auto& m : models) {
    int topLeftX = m.xCoord;
    int topLeftY = m.yCoord;

    for (int i = 0; i < m.height; ++i) {
      for (int j = 0; j < m.width; ++j) {
        boardCoords[topLeftY + i][topLeftX + j] = m.name;
      }
    }
  }
}

void BoardPlate::printBoard() const {
  for (unsigned i = 0; i < boardCoords.size(); ++i) {
    for (unsigned j = 0; j < boardCoords[i].size(); ++j) {
      cout << boardCoords[i][j] << " ";
    }
    cout << endl;
  } 
  cout << endl;
}
