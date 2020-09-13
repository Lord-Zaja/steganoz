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
    QAction *actDecode;
    QAction *actSave;
    QAction *actSaveas;
    QAction *actExit;
    QToolBar *toolBar;
    QLabel *lblImg;
    void closeEvent(QCloseEvent *evt);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupForm();
    void setupActions();
public slots:
    void encode();
    void decode();
    void save();
    void saveas();
};
#endif // MAINWINDOW_H
