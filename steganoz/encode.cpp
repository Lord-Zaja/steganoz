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

Encode::Encode(QWidget *parent) : QDialog(parent)
{
       this->   setWindowTitle(tr("Encode"));
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

    if(!img.isNull()&&!editMsg->toPlainText().isEmpty()){//teoreticky platí vždy
        btnEncode->setEnabled(true);
    }
}

void Encode::editchange(){
    if(!img.isNull()&&!editMsg->toPlainText().isEmpty()){
        btnEncode->setEnabled(true);
    }else{
        btnEncode->setEnabled(false);
    }
}

void Encode::open(){
    fileName=QFileDialog::getOpenFileName(this,tr("Select an image"),QDir::currentPath(),tr("Images (*.png *.jpg *.jpeg);;All Files(*)"));
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
    //Zpráva začíná smyboly "/start"
    //Zpráva ukončena symboly "/end"
    QBitArray msgBits=text2bits("/start"+editMsg->toPlainText()+"/end");
    QColor color;
    img=img.convertToFormat(QImage::Format_ARGB32);
    int h=img.size().height();
    int w=img.size().width();
    if(h*w*3*32<msgBits.size()){
        //zpráva se nevejde do obrázku
        //chybová hláška
        //return
        QMessageBox::critical(this,tr("Error"),tr("Your message is too large for this image!")+tr("Max number of ASCII characters is" )+QString::number((h*w*12)-10));
        return;
    }

    //Zápis bitů do barev podle velikosti zprávy
    //0 G první bit při zprávě menší jak počet pixelů
    //1 B první bit při zprávě větší jak počet pixelů
    //2 R první bit při zprávě větší jak dvojnásobek počtu pixelů
    //3 G druhý bit při ..
    //4 B ..
    unsigned int colType=0;
    unsigned int colbitIndex=0;
    int msgbitIndex=0;
    std::bitset<32> colBits;
    for (int shots=0;shots<=(msgBits.size()-msgBits.size()%(h*w))/(h*w);shots++) {//kolikrát se projdou všechny pixely
        for (int y=0;y<h;y++) {
            for (int x=0;x<w;x++) {
                color=img.pixelColor(x,y);
                switch (colType) {
                case 0://green
                    colBits=color.green();
                    colBits[colbitIndex]=msgBits[msgbitIndex];
                    color.setGreen(colBits.to_ulong());
                    break;
                case 1://blue
                    colBits=color.blue();
                    colBits[colbitIndex]=msgBits[msgbitIndex];
                    color.setBlue(colBits.to_ulong());
                    break;
                case 2://red
                    colBits=color.red();
                    colBits[colbitIndex]=msgBits[msgbitIndex];
                    color.setRed(colBits.to_ulong());
                    break;
                }
                img.setPixelColor(x,y,color);
                msgbitIndex++;
                if(msgbitIndex>(msgBits.size()-1))break;//všechny bity zprávy  zapsány
            }
            if(msgbitIndex>(msgBits.size()-1))break;
        }
        colType++;
        if(colType>2){
            colType=0;
            colbitIndex++;//do všech pixelů byl zapsán jeden bit u všech tří barev, posuň bit
        }
    }

    /*
    //zápis pouze do zelené barvy bez ošetření
    //vypočíst počet zapsaných bitů do jedné barvy
    //zápis bitů do jednotlivých pixelů

    QColor col;
    std::bitset<32> bits;
    int index=0;
    for(unsigned int y=0;y<h;y++){
        for(unsigned int x=0;x<w;x++){
            col=img.pixelColor(x,y);
            bits=col.green();
            //bits=img.pixelColor(x,y).green();       //zápis zatím pouze do zelené barvy
            bits[0]=msgBits[index];             //zápis do prvního bitu proměnné bits
            col.setGreen(bits.to_ulong());
            img.setPixelColor(x,y,col);
            //img.pixelColor(x,y).setGreen(bits.to_ulong());  //výměna původní barvy za barvu s jedním bitem zprávy
            index++;
            if(index>(msgBits.size()-1))break;
        }
        if(index>(msgBits.size()-1))break;
    }
    */

    /*
    qDebug()<<msgBits;
    QBitArray t(30);
    for(int x=0;x<29;x++){
        bits=img.pixelColor(x,0).green();
        t[x]=bits[0];
    }
    qDebug()<<t;
    qDebug()<<"Před";
    bits=img.pixelColor(0,0).green();
    qDebug()<<QString::fromStdString(bits.to_string());
    if(bits[0]==false){
        bits[0]=true;
    }else{
        bits[0]=false;
    }
    QColor c=img.pixelColor(0,0);
    c.setGreen(1);
    img.setPixelColor(0,0,c);
    qDebug()<<"Po";
    bits=img.pixelColor(0,0).green();
    qDebug()<<QString::fromStdString(bits.to_string());

    std::bitset<8> byte;
    std::string strMsg;
    for(unsigned int y=0;y<h;y++){
        for(unsigned int x=0;x<w;x++){//projdi celý obrázek
            bits=img.pixelColor(x,y).green();       //vezmi barvu
            byte[index]=bits[0];                    //vezmi jeden bit z barvy
            index++;
            if(index>7){//mám poskládaný jeden znak
                break;
                strMsg=strMsg+char(byte.to_ulong());//jeden bajt převede na číslo a to pak na znak, platí zatím pouze ascii
                if(strMsg.size()>=4&&strMsg.substr(strMsg.size()-4)=="/end")break;
                index=0;
            }
        }
    }
    qDebug()<<QString::fromStdString(strMsg.substr(0,20));
*/
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
void Encode::setImg(QImage s_img){
    img=s_img;
    btnEncode->setEnabled(true);
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

