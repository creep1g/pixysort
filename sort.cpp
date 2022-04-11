#include "sort.h"
#include "ui_mainwindow.h"
#include <iostream>
using namespace std;
sort::sort(sortDir dir, sortMode mode, QImage* image, int brightnessVal){
  this->image = image;
  this->brightnessVal = brightnessVal;
  this->mode = mode;
  this->dir = dir;
  this->blackVal = -16000000;
  this->whiteVal = -13000000;
}

sort::~sort(){
}



bool comp(const QRgb &p1, const QRgb &p2){
  int max1 = qMax(qRed(p1), qMax(qGreen(p1), qBlue(p1)));
  int max2 = qMax(qRed(p2), qMax(qGreen(p2), qBlue(p2)));
  return max1 < max2;
}


// Using ASDFPixel sorting algorithm
// https://github.com/kimasendorf/ASDFPixelSort/blob/master/ASDFPixelSort.pde


void sort::sortCol(int height, int width, int col){
  int x = col;
  int y = 0;
  int y_end = 0;

  while (y_end < height-1){
    switch(mode){
      case(Black):
          y =    this->getFirstNotBlackY(x, y);
          y_end = this->getNextBlackY(x, y);
        break;
      case(Brightness):
        y =    this->getFirstBrightY(x, y);
        y_end = this->getNextDarkY(x, y);
        break;
      case(White):
          y =    this->getFirstNotWhiteY(x,y);
          y_end = getNextWhiteY(x,y);
       break;
     default:
       break;
   }
    if (y < 0) break;
    int sortlen = y_end - y;

    QVector<QRgb> colSortedColors;
    for(int i = 0; i < sortlen; i++){
      QRgb *pixel = reinterpret_cast<QRgb*>(this->image->scanLine(y+i))+x;
      colSortedColors.append(*pixel);
    }
    std::sort(colSortedColors.begin(), colSortedColors.end(), comp);
    for(int i = 0; i < sortlen; i++){
      QRgb *pixel = reinterpret_cast<QRgb*>(this->image->scanLine(y+i))+x;
      *pixel = colSortedColors[i];
    }
    y = y_end+1;
  }
}

void sort::sortRow(int height, int width, int row){
  int x = 0;
  int y = row;
  int x_end = 0;
  while (x_end < width-1){
   switch(this->mode){
      case(Black):
         x =    this->getFirstNotBlackX(x, y);
        x_end = this->getNextBlackX(x, y);
        break;
     case(Brightness):
        x = 	 this->getFirstBrightX(x, y);
        x_end = this->getNextDarkX(x, y);
        break;
     case(White):
         x =   this->getFirstNotWhiteX(x,y);
          x_end = this->getNextWhiteX(x,y);
        break;
      default:
        break;
    }

    if (x < 0) break;
    int sortlen = x_end - x;
    QVector<QRgb> rowSortedColors;

    QRgb *pixel = reinterpret_cast<QRgb*>(this->image->scanLine(y)) + x;

    for (int i = 0; i < sortlen; i++){
      rowSortedColors.append(*pixel++);
    }
    std::sort(rowSortedColors.begin(), rowSortedColors.end(), comp);

    pixel = reinterpret_cast<QRgb*>(this->image->scanLine(y)) + x;

    for(int i = 0; i < sortlen; i++){
      *pixel++ = rowSortedColors[i];
    }
    x = x_end+1;

  }
}

void sort::vertical(int height, int width, int &row, int &col){
  while(col < width-1){
    this->sortCol(height, width, col);
    col++;
  }
}

void sort::horizontal(int height, int width, int &row, int &col){
  while(row < height-1){
    this->sortRow(height, width, row);
    row++;
  }
}

void sort::sorter(){

  int width = this->image->width();
  int height = this->image->height();
  int row = 	0;
  int col = 	0;

  // Which direction?
  if(this->dir == Vertical){
    this->vertical(height, width, row, col);
  }
  else{
    this->horizontal(height, width, row, col);
  }
  cout << "Done" << endl;
}
//Get brightness value of a pixel
// in referenced ASDF sort brightness(img.pixels[x + y * img.width])
int sort::brightness(int xIn, int yIn){
  QRgb pxl = this->image->pixel(xIn,yIn);
  int maks = qMax(qRed(pxl), qMax(qGreen(pxl), qBlue(pxl)));
  return maks;
}

int sort::getPixl(int xIn, int yIn){
  QRgb pxl = this->image->pixel(xIn, yIn);
  return pxl;
}

// X
int sort::getFirstBrightX(int xIn, int yIn){
  int x = xIn;
  int y = yIn;
  while (this->brightness(x, y) < this->brightnessVal){
    x++;
    if(x >= this->image->width()){
      return -1;
    }
  }
  return x;
}

int sort::getNextDarkX(int xIn, int yIn){
  int x = xIn + 1;
  int y = yIn;

  while(this->brightness(x, y) > this->brightnessVal){
    x++;
    if(x >= this->image->width()){
      return this->image->width()-1;
    }
  }
  return x-1;
}

int sort::getFirstNotWhiteX(int xIn, int yIn){
  int x = xIn;
  int y = yIn;

  while(this->getPixl(x,y) > this->whiteVal){
    x++;
    if (x >= this->image->width()){
      return -1;
    }
  }
  return x;
}

int sort::getNextWhiteX(int xIn, int yIn){
  int x = xIn+1;
  int y = yIn;

  while (this->getPixl(x, y) < this->whiteVal){
    x++;
    if(x >= this->image->width()){
      return this->image->width() - 1;
    }
  }
  return x-1;
}

int sort::getNextBlackX(int xIn, int yIn){
  int x = xIn+1;
  int y = yIn;

  while(this->getPixl(x,y) > blackVal){
    x++;
    if(x >= this->image->width()){
      return this->image->width() - 1;
    }
  }
  return x-1;
}



int sort::getFirstNotBlackX(int xIn, int yIn){
  int x = xIn;
  int y = yIn;

  while(this->getPixl(x,y) < this->blackVal){
    x++;
    if (x >= this->image->width()){
      return -1;
    }
  }
  return x;
}



// Y
int sort::getFirstBrightY(int xIn, int yIn){
  int x = xIn;
  int y = yIn;
  if (y < this->image->height()){
    while(this->brightness(x,y) < this->brightnessVal){
      y++;
      if (y >= this->image->height()){
        return -1;
      }
    }
  }
  return y;
}


int sort::getNextDarkY(int xIn, int yIn){
  int x = xIn;
  int y = yIn + 1;
  if (y < this->image->height()){
    while(this->brightness(x,y) > this->brightnessVal){
      y++;
      if (y >= this->image->height()){
          return this->image->height()-1;
      }
    }
  }
  return y-1;
}

int sort::getFirstNotBlackY(int xIn, int yIn){
 int x = xIn;
 int y = yIn;

 if (y < this->image->height()){
   while(this->getPixl(x,y) < this->blackVal){
     y++;
     if(y >= this->image->height()){
       return -1;
     }
   }
 }
 return y;
}

int sort::getNextBlackY(int xIn, int yIn){
  int x = xIn;
  int y = yIn + 1;

  if (y < this->image->height()){
    while(getPixl(x, y) > this->blackVal){
      y++;
      if (y >= this->image->height()){
        return this->image->height()-1;
      }
    }
  }
  return y - 1;
}

int sort::getNextWhiteY(int xIn, int yIn){
  int x = xIn;
  int y = yIn + 1;

  if (y < this->image->height()){
    while(this->getPixl(x, y) < whiteVal){
      y++;
      if(y >= this->image->height()){
        return this->image->height() - 1;
      }
    }
  }
  return y - 1;
}

int sort::getFirstNotWhiteY(int xIn, int yIn){
  int x = xIn;
  int y = yIn;

  if (y < this->image->height()){
    while(this->getPixl(x,y) > whiteVal){
      y++;
      if(y >= this->image->height()){
        return -1;
      }
    }
  }
  return y;
}
