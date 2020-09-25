#include "mainwindow.h"
#include "encode.h"
#include "decode.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>

//Dodělat rsa,abort,naposled použitý adresář,

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
    lblImg->setScaledContents(true);
    this->setCentralWidget(lblImg);
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

    //Action Open
    QIcon iconOpen=QIcon::fromTheme("document-open",QIcon(":/icons/open.png"));
    actOpen = new QAction(tr("&Open"));
    actOpen->setIcon(iconOpen);
    mnuStart->addAction(actOpen);

    //Action Decode
    QIcon iconDecode=QIcon::fromTheme("system-log-out",QIcon(":/icons/decode.png"));
    actDecode = new QAction(tr("&Decode"));
    actDecode->setIcon(iconDecode);
    actDecode->setEnabled(false);
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
    toolBar->addAction(actOpen);
    toolBar->addAction(actDecode);
    toolBar->addAction(actSave);
    toolBar->addAction(actSaveas);
    toolBar->addAction(actExit);

    //Signals
    connect(actEncode,SIGNAL(triggered()),this,SLOT(encode()));
    connect(actOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(actDecode,SIGNAL(triggered()),this,SLOT(decode()));
    connect(actSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(actSaveas,SIGNAL(triggered()),this,SLOT(saveas()));
    connect(actExit,SIGNAL(triggered()),this,SLOT(close()));

    //Statusbar
    this->statusBar()->showMessage(tr("Ready.."),2000);
}

void MainWindow::encode(){
    Encode *e=new Encode;
    if(!img.isNull()){
        e->setImg(img);
    }
    e->exec();
    img=e->getImage();//tady už je obrázek pozměněn
    fileName=e->getImgpath();
    if(!img.isNull()){
        imgDraw(img);
        actSave->setEnabled(true);
        actSaveas->setEnabled(true);
        actDecode->setEnabled(true);
        QMessageBox::information(this,tr("Encode"),tr("Encoding completed."));
    }
}

void MainWindow::imgDraw(QImage img){
    //QImage img=e->getImage();
        if(img.isNull()){
            QMessageBox::warning(this, tr("Error"), tr("Failed to draw the image! Image is null"));
            return;
        }
        qDebug()<<"Vykreslování..";
        lblImg->setPixmap(QPixmap::fromImage(img));
        lblImg->adjustSize();
        statusBar()->showMessage(tr("Image was loaded"), 2500);
    //tady zobraz dialog že vše proběhlo úspěšně
    //zobraz obrázek v lblImg
    //odemkni save, saveas
}

void MainWindow::decode(){
    Decode *d=new Decode;
    d->setImg(img);
    d->exec();
}

void MainWindow::save(){
    //Uloží obrázek na do složky s obrázkem
    QDate date=QDateTime::currentDateTime().date();
    QString str;
    QString path;
    QString dir=QFileInfo(fileName).absoluteDir().absolutePath()+"/";
    int k=1;
    if (QString::number(date.day()).size()<2){//vytvoř řetězec datumu ve formátu ddmmrr
        str="0"+QString::number(date.day());
    }else{
        str=QString::number((date.day()));
    }
    if (QString::number(date.month()).size()<2){//vytvoř řetězec datumu ve formátu ddmmrr
        str=str+"0"+QString::number(date.month());
    }else{
        str=str+QString::number((date.month()));
    }
    str.append(QString::number(date.year()).left(2));//ddmmrr
    path=dir+QFileInfo(fileName).baseName()+"_encoded_"+str+"."+QFileInfo(fileName).completeSuffix();
    while(QFileInfo::exists(path)){          //pokud již existuje čísluj další
        path=dir+QFileInfo(fileName).baseName()+"_encoded_"+str+"_"+QString::number(k)+"."+QFileInfo(fileName).completeSuffix();
        k++;
    }
    qDebug()<<"Adresa obrázku: "<<path;
    img.save(path);
    //zobraz zprávu vše se uložilo
    QMessageBox::information(this,tr("Saved!"),tr("Image saved to the original image directory as ").append(QFileInfo(path).completeBaseName()+"."+QFileInfo(path).completeSuffix()));
}

void MainWindow::saveas(){
    //uloží obrázek do zvolené složky

}

void MainWindow::open(){
    fileName=QFileDialog::getOpenFileName(this,tr("Select an image"),QDir::currentPath(),tr("Images (*.png *.jpg *.jpeg);;All Files(*)"));
    img=QImage(fileName);
    if(!img.isNull()){
        actDecode->setEnabled(true);
    }else{
        actDecode->setEnabled(false);
        lblImg->clear();
        QMessageBox::warning(this,tr("Error"),tr("Failed to load the image"));
    }
    imgDraw(img);
    this->statusBar()->showMessage(tr("Image loaded"),2000);
}

void MainWindow::closeEvent(QCloseEvent *evt){

    evt->accept();
}
