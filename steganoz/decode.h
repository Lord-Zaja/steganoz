#ifndef DECODE_H
#define DECODE_H

#include <QDialog>
#include <QObject>

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QLineEdit;
class QLabel;

class Decode : public QDialog
{
    Q_OBJECT
    QLabel *lblMsg;
    QLabel *lblKey;
    QLineEdit *editKey;
    QPushButton *btnOpen;
    QPushButton *btnDecode;
    QPushButton *btnRsa;
    void createActions();
    QVBoxLayout *lMain;
    QHBoxLayout *lKey;
    QHBoxLayout *lBtns;
    QString fileName;
    QImage img;
public:
    explicit Decode(QWidget *parent=nullptr);
    void setImg(QImage s_img);
public slots:
    void open();
    void decoderaw();
    void decodersa();
    void keychange();
};

#endif // DECODE_H
