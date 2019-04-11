#pragma once

#include "utility.h"

#include <vector>

class BoardPlate {
  
public:

  BoardPlate() {}
  BoardPlate(int width_, int height_) : width(width_), height(height_) {
    boardCoords.resize(height, std::vector<char>(width, '.'));
    cX = width / 2;
    cY = height / 2;
  }

  // TODO: Not all of these need be public
  void reCenterModels(Model& newModel);
  bool modelCanFitInPos(Model& m, int x, int y);
  void findOptimalPosForNewModel(Model& newModel);
  void addModel(Model& model);
  
  void updateBoardCoords();
  void printBoard();

private:

  std::vector<std::vector<char>> boardCoords;
  int width = 0;
  int height = 0;

  // Board Plate center coordinates
  int cX = 0;
  int cY = 0;

  std::vector<Model> models; 
  BoundingBox bb;

};

