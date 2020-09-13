#include "encode.h"
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QBitArray>
#include <bitset>
Encode::Encode(QWidget *parent) : QWidget(parent)
{
       this->setWindowTitle(tr("Encode"));
       createActions();
}
void Encode::createActions(){
    //Actions
    editMsg = new QTextEdit;
    editMsg->setText(tr("Write your message here"));
    btnOpen = new QPushButton(tr("&Open"));
    btnEncode = new QPushButton(tr("&Encode"));
    btnEncode->setEnabled(false);

    //Setup layouts
    lMain = new QVBoxLayout;
    lBtns = new QHBoxLayout;
    lMain->addWidget(editMsg);
    lBtns->addWidget(btnOpen);
    lBtns->addWidget(btnEncode);
    lMain->addLayout(lBtns);
    this->setLayout(lMain);

    connect(btnOpen,SIGNAL(clicked()),this,SLOT(open()));
    connect(btnEncode,SIGNAL(clicked()),this,SLOT(slotencode()));
    connect(editMsg,SIGNAL(textChanged()),this,SLOT(editchange()));
}

void Encode::editchange(){
    if(!img.isNull()&&!editMsg->toPlainText().isEmpty()){
        btnEncode->setEnabled(true);
    }else{
        btnEncode->setEnabled(false);
    }
}

void Encode::open(){
    QString fileName=QFileDialog::getOpenFileName(this,tr("Select an image"),QDir::currentPath(),tr("Images (*.png *.jpg *.jpeg);;All Files(*)"));
    img=QImage(fileName);
    if(!img.isNull()&&!editMsg->toPlainText().isEmpty()){
        btnEncode->setEnabled(true);
    }else{
        btnEncode->setEnabled(false);
        QMessageBox::warning(this,tr("Error"),tr("Failed to load the image"));
    }

}



void Encode::slotencode(){
    if(img.isNull()||editMsg->toPlainText().isEmpty()){
        return;
    }
    QBitArray msgBits=text2bits(editMsg->toPlainText());

    img.convertToFormat(QImage::Format_ARGB32);
    unsigned int h=img.size().height();
    unsigned int w=img.size().width();

    //vypočíst počet zapsaných bitů do jedné barvy
    //zápis bitů do jednotlivých pixelů
    std::bitset<32> bits;
    unsigned int index=0;
    for(unsigned int y=0;y<h;y++){
        for(unsigned int x=0;x<w;x++){
            bits=img.pixelColor(x,y).green();       //zápis zatím pouze do zelené barvy
            bits[0]=msgBits[index];
            index++;
            if(index>(msgBits.size()-1))break;
        }
        if(index>(msgBits.size()-1))break;
    }
    //nevyzkoušeno

    this->close();
}

QBitArray Encode::text2bits(QString str){
    std::string s=str.toStdString();
    QBitArray msgBits(s.size()*8);
    std::bitset<8> byte;
    unsigned int it=0;
    for(unsigned int i=0;i<s.size();i++){
        byte=s[i];
        for(int b=0;b<=7;b++){
            msgBits.setBit(it,byte[b]);
            it++;
        }
    }
    return msgBits;
}
QString Encode::bits2text(QBitArray msgBits){
    std::string vystup;
    std::bitset<8> byte;
    unsigned int size=msgBits.size();
    int let;
    char znak;
    for(unsigned int i=0;i<size;i++){
        byte[i%8]=msgBits[i];   //sestav byte
        if(i%8==7){             //zapiš znak
            let=(i-i%8)/8;      //znak číslo let
            znak=char(byte.to_ulong());
            vystup.push_back(znak);
        }
    }
    return QString::fromStdString(vystup);
}

