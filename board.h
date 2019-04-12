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

  void printBoard() const;
  void addModel(Model& model);

  void setWidth(int width_);
  void setHeight(int height_);

private:

  std::vector<std::vector<char>> boardCoords;
  int width = 0;
  int height = 0;

  // Board Plate center coordinates
  int cX = 0;
  int cY = 0;

  std::vector<Model> models; 
  BoundingBox bb;

  void reCenterModels(Model& newModel);
  bool modelCanFitInPos(const Model& m, int x, int y);
  bool findOptimalPosForNewModel(Model& newModel);
  void updateBoardCoords();

  void shiftBBLeft();
  void shiftBBUp();
  int calcAreaBoundingBox(const Model& m, const Coord& coord);
  double calcDistanceBBToEdge(const Model& m);

};

