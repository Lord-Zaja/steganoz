#ifndef ENCODE_H
#define ENCODE_H

#include <QDialog>
class QPushButton;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;

class Encode : public QDialog
{
    Q_OBJECT
    QPushButton *btnOpen;
    QPushButton *btnEncode;
    QTextEdit *editMsg;
    QVBoxLayout *lMain;
    QHBoxLayout *lBtns;
    QImage img;
    QString fileName;
    void createActions();
    QBitArray text2bits(QString s);
    QString bits2text(QBitArray msgBits);
public:
    explicit Encode(QWidget *parent = nullptr);
    QImage getImage(){return img;};
    QString getImgpath(){return fileName;};
    void setImg(QImage s_img);
public slots:
    void open();
    void slotencode();
    void editchange();
};

#endif // ENCODE_H
