#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <QtWidgets>
#include <QDragEnterEvent>
#include "sort.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void isFinished();
  void progress(int);

private slots:
  void openFile();
  void sortVertical();
  void sortHorizontal();
  void Undo();
  void Save();
  void Exit();
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);

private:
  Ui::MainWindow *ui;

  void display(QImage image);
  void load(QString fileName);

  void mainsort(sort::sortDir);

  QImage image;
  QString fileName;
  QGraphicsScene *imgWindow;

  QStack<QImage> history;
  sort *pSort;
  QFuture<void> future;
  QFutureWatcher<void> watcher;


};
#endif // MAINWINDOW_H
