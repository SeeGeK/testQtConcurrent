#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QDir>
#include <QScrollArea>

const int N_COLUMNS = 2;
//const int N_ROWS    = 15;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    start();
}

void MainWindow::start()
{
    QDir pathToImageFolder(QString(IMAGE_FOLDER_PATH) + "/images");

    auto imagesPaths = pathToImageFolder.entryList({"*.jpg", "*.JPG", "*.png", "*.PNG", "*.bmp", "*.BMP"});
    for (auto &i: imagesPaths)
        i = pathToImageFolder.path() + "/" + i;

    connect(&watcher, &QFutureWatcher<QString>::resultReadyAt, this,
            [this, imagesPaths](int rindex){
                qDebug() << imagesPaths.at(rindex) << "scaled";
            });

    future = QtConcurrent::mapped(imagesPaths, &MainWindow::scaledImage);
    watcher.setFuture(future);
}

QImage MainWindow::scaledImage(const QString &imgName)
{
    QImage image(imgName);
    QImage result = image.scaled(128, 128, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    return result;
}


MainWindow::~MainWindow()
{
    delete ui;
}



// ============================================
//auto scrollArea = new QScrollArea;
//this->setCentralWidget(scrollArea);
//auto widget = new QWidget;
//scrollArea->setWidget(widget);
//scrollArea->setWidgetResizable(true);
//scrollArea->setBackgroundRole(QPalette::Dark);
//
//auto layout = new QGridLayout(widget);
//QList<QString> images;

//    connect(&watcher, &QFutureWatcher<QString>::resultReadyAt, this,
//            [this, layout](int rindex){
//                auto label = new QLabel;
//                static int fillCounter = 0;
//                layout->addWidget(label, fillCounter/N_COLUMNS+1, fillCounter%N_COLUMNS);
//                fillCounter++;
//                auto result = watcher.future().resultAt(rindex);
//                label->setPixmap(QPixmap::fromImage(result));
//            });

//    future.pause();
//    auto startButton = new QPushButton("start");
//    connect(startButton, &QPushButton::released, this, &MainWindow::start);
//    layout->addWidget(startButton, 0, 0);
//    future.resume();


//    qDebug() << Q_FUNC_INFO << imgName;
//    qDebug() << Q_FUNC_INFO << imgName << " FINISHED";
