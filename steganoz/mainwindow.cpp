#include "mainwindow.h"
#include "encode.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupForm();
    setupActions();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupForm(){
    this->resize(800,600);
    this->setWindowTitle(tr("Steganoz"));
    lblImg = new QLabel;
    lblImg->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
}

void MainWindow::setupActions(){
    //Menu Start-----------------------------------------------------------------------
    mnuStart = new QMenu(tr("&Start"));
    this->menuBar()->addMenu(mnuStart);

    //Action Encode
    QIcon iconEncode=QIcon::fromTheme("system-lock-screen",QIcon(":/icons/encode.png"));
    actEncode = new QAction(tr("&Encode"));
    actEncode->setIcon(iconEncode);
    mnuStart->addAction(actEncode);

    //Action Decode
    QIcon iconDecode=QIcon::fromTheme("system-log-out",QIcon(":/icons/decode.png"));
    actDecode = new QAction(tr("&Decode"));
    actDecode->setIcon(iconDecode);
    mnuStart->addAction(actDecode);

    //Action Save
    QIcon iconSave=QIcon::fromTheme("document-save",QIcon(":/icons/save.png"));
    actSave = new QAction(tr("&Save"));
    actSave->setIcon(iconSave);
    mnuStart->addAction(actSave);
    actSave->setEnabled(false);

    //Action Save as
    QIcon iconSaveas=QIcon::fromTheme("document-save-as",QIcon(":/icons/saveas.png"));
    actSaveas = new QAction(tr("Save &as.."));
    actSaveas->setIcon(iconSaveas);
    mnuStart->addAction(actSaveas);
    actSaveas->setEnabled(false);


    //Action Exit
    QIcon iconExit=QIcon::fromTheme("system-shutdown",QIcon(":/icons/exit.png"));
    actExit = new QAction(tr("Exit"));
    actExit->setIcon(iconExit);
    mnuStart->addAction(actExit);
    //---------------------------------------------------------------------------------

    //Toolbar
    toolBar=new QToolBar();
    this->addToolBar(toolBar);
    toolBar->addAction(actEncode);
    toolBar->addAction(actDecode);
    toolBar->addAction(actSave);
    toolBar->addAction(actSaveas);
    toolBar->addAction(actExit);

    //Signals
    connect(actEncode,SIGNAL(triggered()),this,SLOT(encode()));
    connect(actDecode,SIGNAL(triggered()),this,SLOT(decode()));
    connect(actSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(actSaveas,SIGNAL(triggered()),this,SLOT(saveas()));
    connect(actExit,SIGNAL(triggered()),this,SLOT(close()));

    //Statusbar
    this->statusBar()->showMessage(tr("Ready.."),2000);
}

void MainWindow::encode(){
    Encode *e=new Encode;
    e->show();
    //tady zobraz dialog že vše proběhlo úspěšně
    //zobraz obrázek v lblImg
    //odemkni save, saveas
}

void MainWindow::decode(){

}

void MainWindow::save(){

}

void MainWindow::saveas(){

}

void MainWindow::closeEvent(QCloseEvent *evt){

    evt->accept();
}
