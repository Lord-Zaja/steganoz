#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenuBar;
class QMenu;
class QAction;
class QToolBar;
class QStatusBar;
class QMessageBox;
class QCloseEvent;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QMenu *mnuStart;
    QAction *actEncode;
    QAction *actOpen;
    QAction *actDecode;
    QAction *actSave;
    QAction *actSaveas;
    QAction *actExit;
    QToolBar *toolBar;
    QLabel *lblImg;
    QImage img;
    QString fileName;
    void closeEvent(QCloseEvent *evt);
    void imgDraw(QImage img);
    void setupForm();
    void setupActions();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void encode();
    void decode();
    void save();
    void saveas();
    void open();
};
#endif // MAINWINDOW_H
