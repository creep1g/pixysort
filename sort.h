#ifndef SORT_H
#define SORT_H
#include <QVector>
#include <QObject>
#include <QRgb>
#include <QtGlobal>
#include <QColor>

class sort : public QObject
{
  Q_OBJECT

public:
  enum sortDir {
    Horizontal, Vertical
  };
  enum sortMode{
    Black, Brightness, White
  };

  sort(sortDir, sortMode, QImage* image, int brightnessVal);
  ~sort();
public slots:
  void sorter();

signals:
  void progressChanged(int);

private:
  // Variables
  QImage *image;
  int brightnessVal;
  int blackVal;
  int whiteVal;
  sortDir dir;
  sortMode mode;
private:
  // Functions
  int brightness(int xIn, int yIn);
  int getPixl(int xIn, int yIn);
  void sortCol(int height, int width, int col);
  void sortRow(int height, int width, int col);
  //Xess
  int getFirstBrightX(int xIn, int yIn);   //* Check
  int getNextDarkX(int xIn, int yIn);      //* Check
  int getFirstNotBlackX(int xIn, int yIn); //* Check
  int getNextBlackX(int xIn, int yIn);     //* Check
  int getFirstNotWhiteX(int xIn, int yIn); //* Check
  int getNextWhiteX(int xIn, int yIn);	   //* Check


  //Yess
  int getFirstBrightY(int xIn, int yIn);   //* Check
  int getNextDarkY(int xIn, int yIn);      //* Check
  int getFirstNotBlackY(int xIn, int yIn); //* Check
  int getNextBlackY(int xIn, int yIn);     //* Check
  int getNextWhiteY(int xIn, int yIn);		 //* Check
  int getFirstNotWhiteY(int xIn, int yIn);  //* Check


  //Directions
  void vertical(int height, int width, int &row, int &col);
  void horizontal(int height, int width, int &row, int &col);

};


#endif // SORT_H
