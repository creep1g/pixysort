#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sort.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->actionUndo->setEnabled(false);
  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(Undo()));
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(Exit()));
  connect(ui->sortHorizontal, SIGNAL(clicked()), this, SLOT(sortHorizontal()));
  connect(ui->sortVertical, SIGNAL(clicked()), this, SLOT(sortVertical()));
  //connect(ui->actionSort, SIGNAL(triggered()), this, SLOT(Sort()));
  connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(Save()));
  this->imgWindow = nullptr;
  setAcceptDrops(true);
}

void MainWindow::openFile(){
  std::cout << "Open" << std::endl;
  QString fileName = QFileDialog::getOpenFileName(this, "Open Image", nullptr, "Image files (*.jpg *jpeg *.png)");
  load(fileName);
}

void MainWindow::load(QString fileName){

  this->fileName = fileName;
  if (!image.load(fileName)){
    ui->statusbar->showMessage("Image is invaalid or corrupted");
    return;
  }
  if (image.format() != QImage::Format_ARGB32){
    image = image.convertToFormat(QImage::Format::Format_ARGB32);
  }
  history.clear();
  ui->actionUndo->setEnabled(false);
  ui->statusbar->showMessage("Loaded!");
  display(image);
}

void MainWindow::display(QImage image){
  this->image = image;
  delete imgWindow;
  imgWindow = new QGraphicsScene(ui->graphicsView);
  ui->graphicsView->setScene(imgWindow);
  imgWindow->addPixmap(QPixmap::fromImage(image));
  ui->graphicsView->fitInView(imgWindow->itemsBoundingRect(), Qt::KeepAspectRatio);
}

// Drag and drop image to program
// Drag
void MainWindow::dragEnterEvent(QDragEnterEvent *event){
  if(event->mimeData()->hasUrls()){
    event->acceptProposedAction();
  }
}
// Drop
void MainWindow::dropEvent(QDropEvent *event){
  load(event->mimeData()->urls()[0].toLocalFile());
  event->acceptProposedAction();
}



//Þarf ég þetta?
void MainWindow::progress(int prog){
  ui->statusbar->showMessage(QString("Progress: %1%").arg(prog));
}

// Eða þetta

void MainWindow::isFinished(){
  std::cout << "Finished" << std::endl;
  display(image);
  ui->statusbar->showMessage("Complete");
  ui->actionUndo->setEnabled(true);
}


void MainWindow::Save(){
  if(!this->image.isNull()){
    QFileInfo fileInfo(this->fileName);
    QString fileName = QFileDialog::getSaveFileName(this, "Save", QString("%1%2_sorted%3.png").arg(fileInfo.filePath()).arg(fileInfo.baseName()).arg(history.length()), "PNG (*.png);;JPG (*.jpg)");
    image.save(fileName);
}
  }

void MainWindow::Undo(){
  if(!this->history.empty()){
    this->display(history.pop());
    if(history.isEmpty()){
      ui->actionUndo->setEnabled(false);
    }
  }
}


void MainWindow::sortHorizontal(){

  this->mainsort(sort::Horizontal) ;
}

void MainWindow::sortVertical(){
  this->mainsort(sort::Vertical);
}

void MainWindow::mainsort(sort::sortDir dir){
  sort::sortMode mode;
  if (ui->radioBlack->isChecked()){
    mode = sort::Black;
    std::cout << "BLACK" << std::endl;
  } else if (ui->radioWhite->isChecked()){
    mode = sort::White;
    std::cout << "WHITE" << std::endl;
  } else {
    std::cout << "BRIGHTNESS" << std::endl;
    mode = sort::Brightness;
  }
  ui->actionUndo->setEnabled(false);
  this->history.push(image);
  this->pSort = new sort(dir, mode, &this->image, ui->brighnessSpinBox->value());
  connect(&watcher, SIGNAL(finished()), this, SLOT(isFinished()), Qt::UniqueConnection);
  //connect(pSort, SIGNAL(upProgress(int)), this, SLOT(progress(int)), Qt::UniqueConnection);
  this->future = QtConcurrent::run(&sort::sorter, pSort);
  this->watcher.setFuture(this->future);
}

void MainWindow::Exit(){
  close();
}

MainWindow::~MainWindow()
{
  delete ui;
}

