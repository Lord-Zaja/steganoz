#include "decode.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QBitArray>
#include <bitset>
#include <QDebug>

Decode::Decode(QWidget *parent):QDialog(parent)
{
    this->setWindowTitle(tr("Decode"));
    createActions();
}

void Decode::createActions(){
    //Actions
    lblMsg=new QLabel;
    lblKey=new QLabel(tr("Public key: "));
    editKey=new QLineEdit;
    btnOpen=new QPushButton(tr("&Open"));
    btnDecode=new QPushButton(tr("&Decode raw"));
    btnRsa=new QPushButton(tr("&Decode RSA"));
    lblMsg->setWordWrap(true);
    lblMsg->setTextInteractionFlags(Qt::TextSelectableByMouse);
    lblKey->setBuddy(editKey);
    btnDecode->setEnabled(false);
    btnRsa->setEnabled(false);
    //Layouts
    lMain=new QVBoxLayout;
    lKey=new QHBoxLayout;
    lBtns=new QHBoxLayout;
    lMain->addWidget(lblMsg);
    lMain->addLayout(lKey);
    lMain->addLayout(lBtns);
    lKey->addWidget(btnOpen);
    lKey->addWidget(btnDecode);
    lKey->addWidget(btnRsa);
    this->setLayout(lMain);
    //Connect signals
    connect(editKey,SIGNAL(textChanged()),this,SLOT(keyChange()));
    connect(btnOpen,SIGNAL(clicked()),this,SLOT(open()));
    connect(btnDecode,SIGNAL(clicked()),this,SLOT(decoderaw()));
    connect(btnRsa,SIGNAL(clicked()),this,SLOT(decodersa()));
    if(!img.isNull()){
        btnDecode->setEnabled(true);
    }
    if(!img.isNull()&&!editKey->text().isEmpty()){
        btnRsa->setEnabled(true);
    }
}


void Decode::decoderaw(){
    if(img.isNull()){
        return;
    }
    int h=img.size().height();
    int w=img.size().width();
    QColor color;
    unsigned int colType=0;
    unsigned int colbitIndex=0;
    std::bitset<8> byte;
    unsigned int byteIndex=0;
    std::string strMsg;
    std::bitset<32> colBits;

    for (int shots=0;shots<32;shots++) {//32=počet bitů jedné barvy
        for (int y=0;y<h;y++) {
            for (int x=0;x<w;x++) {
                color=img.pixelColor(x,y);
                switch (colType) {
                case 0://green
                    colBits=color.green();
                    byte[byteIndex]=colBits[colbitIndex];
                    break;
                case 1://blue
                    colBits=color.blue();
                    byte[byteIndex]=colBits[colbitIndex];
                    break;
                case 2://red
                    colBits=color.red();
                    byte[byteIndex]=colBits[colbitIndex];
                    break;
                }
                byteIndex++;
                if(byteIndex>7){//mám poskládaný jeden znak
                    strMsg=strMsg+char(byte.to_ulong());//jeden bajt převede na číslo a to pak na znak, platí zatím pouze ascii
                    if(strMsg.size()>=4&&strMsg.substr(strMsg.size()-4)=="/end")break;
                    if(strMsg.size()==6&&strMsg!="/start"){//startovní sekvence nenalezena
                        qDebug()<<QString::fromStdString(strMsg);
                        qDebug()<<x;
                        QMessageBox::critical(this,tr("Error"),tr("Starting sequence wasnt found in your image! Only images encoded with this software can be decoded here"));
                        return;
                    }
                    byteIndex=0;
                }
            }
            if(strMsg.size()>=4&&strMsg.substr(strMsg.size()-4)=="/end")break;
        }
        colType++;
        if(colType>2){
            colType=0;
            colbitIndex++;//do všech pixelů byl zapsán jeden bit u všech tří barev, posuň bit
        }
        if(strMsg.size()>=4&&strMsg.substr(strMsg.size()-4)=="/end")break;
    }
    if(strMsg.substr(strMsg.size()-4)!="/end"){
        //vyhoď chybovou hlášku: end chars not found
        qDebug()<<QString::fromStdString(strMsg.substr(0,20));
        QMessageBox::critical(this,tr("Error"),tr("End chars not found!"));
        this->close();
    }else{
        //umaž poslední 4 znaky
        strMsg.erase(0,6);//umaž /start
        strMsg.erase((strMsg.length())-4,4);//umaž /end
    }

    /*
    //vezmu 8 bitů, získám znak a ten dále zkoumám
    std::bitset<8> byte;
    std::string strMsg;
    std::bitset<32> bits;
    unsigned int index=0;
    for(unsigned int y=0;y<h;y++){
        for(unsigned int x=0;x<w;x++){//projdi celý obrázek
            bits=img.pixelColor(x,y).green();       //vezmi barvu
            byte[index]=bits[0];                    //vezmi jeden bit z barvy
            index++;
            if(index>7){//mám poskládaný jeden znak
                strMsg=strMsg+char(byte.to_ulong());//jeden bajt převede na číslo a to pak na znak, platí zatím pouze ascii
                if(strMsg.size()>=4&&strMsg.substr(strMsg.size()-4)=="/end")break;
                index=0;
            }
        }
        if(strMsg.size()>=4&&strMsg.substr(strMsg.size()-4)=="/end")break;
    }
    if(strMsg.substr(strMsg.size()-4)!="/end"){
        //vyhoď chybovou hlášku: end chars not found
        qDebug()<<QString::fromStdString(strMsg.substr(0,20));
        QMessageBox::critical(this,tr("Error"),tr("End chars not found!"));
        this->close();
    }else{
        //umaž poslední 4 znaky
        strMsg.pop_back();
        strMsg.pop_back();
        strMsg.pop_back();
        strMsg.pop_back();
    }*/
    //zpráva je uvnitř strMsg
    lblMsg->setText(QString::fromStdString(strMsg));
}
void Decode::open(){
    fileName=QFileDialog::getOpenFileName(this,tr("Select an image"),QDir::currentPath(),tr("Images (*.png *.jpg *.jpeg);;All Files(*)"));
    img=QImage(fileName);
    if(!img.isNull()){
        btnDecode->setEnabled(true);
    }else{
        btnDecode->setEnabled(false);
        QMessageBox::warning(this,tr("Error"),tr("Failed to load the image"));
    }
    if(!img.isNull()&&!editKey->text().isEmpty()){
        btnRsa->setEnabled(true);
    }else{
        btnRsa->setEnabled(false);
    }
}
void Decode::decodersa(){

}
void Decode::setImg(QImage s_img){
    img=s_img;
    btnDecode->setEnabled(true);
}
void Decode::keychange(){

}
