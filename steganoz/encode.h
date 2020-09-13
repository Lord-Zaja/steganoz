#ifndef ENCODE_H
#define ENCODE_H

#include <QWidget>
class QPushButton;
class QTextEdit;
class QVBoxLayout;
class QHBoxLayout;

class Encode : public QWidget
{
    Q_OBJECT
    QPushButton *btnOpen;
    QPushButton *btnEncode;
    QTextEdit *editMsg;
    QVBoxLayout *lMain;
    QHBoxLayout *lBtns;
    QImage img;
    void createActions();
    QBitArray text2bits(QString s);
    QString bits2text(QBitArray msgBits);
public:
    explicit Encode(QWidget *parent = nullptr);
public slots:
    void open();
    void slotencode();
    void editchange();
};

#endif // ENCODE_H
