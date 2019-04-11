#include "board.h"
#include "utility.h"

#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

void BoardPlate::reCenterModels(Model& newModel) {
  // Find min bounding box for all models
  bb.xmin = min(bb.xmin, newModel.getTopLeftX());
  bb.xmax = max(bb.xmax, newModel.getTopLeftX() + newModel.width - 1);
  bb.ymin = min(bb.ymin, newModel.getTopLeftY());
  bb.ymax = max(bb.ymax, newModel.getTopLeftY() + newModel.height - 1);

  if (bb.xmin > (width - bb.xmax)) {
    int diff = bb.xmin - (width - bb.xmax);
    for (auto& m : models) {
      m.xCoord -= diff;
    } 
    bb.xmin -= diff;
    bb.xmax -= diff;
  }

  else if ((width - bb.xmax - 1) > bb.xmin) {
    int diff = (width - bb.xmax - 1) - bb.xmin;
    for (auto& m : models) {
      m.xCoord += diff;
    }
    bb.xmin += diff;
    bb.xmax += diff;
  }

  if (bb.ymin > (height - bb.ymax)) {
    int diff = bb.ymin - (height - bb.ymax);
    for (auto& m : models) {
      m.yCoord -= diff;
    }
    bb.ymin -= diff;
    bb.ymax -= diff;
  } 

  else if ((height - bb.ymax - 1) > bb.ymin) {
    int diff = (height - bb.ymax - 1) - bb.ymin;
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

void BoardPlate::findOptimalPosForNewModel(Model& newModel) {
  // Start from center and search outwards in breadth first manner
  // for an open position for the new model
  // Then place model in open position and re center all models
  Coord pos(cX - (newModel.width / 2), cY - (newModel.height / 2));
  queue<Coord> search;
  search.push(pos);

  Coord found;

  while (!search.empty()) {
    Coord top = search.front();
    search.pop();
    
    if (modelCanFitInPos(newModel, top.x, top.y)) {
      // Here we need to find new bounding box, re center, and calc distance to edges
      found = top; 
      break;
    } 

    search.push(Coord(top.x + 1, top.y));
    search.push(Coord(top.x - 1, top.y));
    search.push(Coord(top.x, top.y + 1));
    search.push(Coord(top.x, top.y - 1));
  }

  newModel.xCoord = found.x;
  newModel.yCoord = found.y;
}

void BoardPlate::addModel(Model& model) {
  if (models.empty()) {
    model.xCoord = cX - (model.width / 2);
    model.yCoord = cY - (model.height / 2);
  }
  else {
    findOptimalPosForNewModel(model);
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
