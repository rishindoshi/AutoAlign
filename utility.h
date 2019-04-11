#pragma once

#include <climits>
#include <iostream>

struct Model {
  Model() {}
  Model(int width_, int height_, char name_)
       : width(width_), height(height_), name(name_) {}

  int getTopLeftX() {
    return xCoord;
  }

  int getTopLeftY() {
    return yCoord;
  } 

  int width = 0;
  int height = 0;
  char name;

  int xCoord = -1;
  int yCoord = -1;
};

struct BoundingBox {
  int xmin = INT_MAX;
  int xmax = -1;
  int ymin = INT_MAX;
  int ymax = -1;

  void print() {
    std::cout << "(" << xmin << ", " << ymin << ") ";
    std::cout << "(" << xmax << ", " << ymax << ")" << std::endl;
  }
};

struct Coord {
  Coord() {}
  Coord(int x_, int y_) : x(x_), y(y_) {}
  int x;
  int y;
};
