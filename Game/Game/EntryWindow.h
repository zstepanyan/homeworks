#ifndef ENTRYWINDOW_H
#define ENTRYWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "GameWindow.h"

class EntryWindow : public QMainWindow {
    Q_OBJECT
public:
    EntryWindow(QWidget *parent = nullptr);

private slots:
    void showPlayOptions();
    void showInfo();
    void startGame(int size);

private:
    QPushButton *playButton;
    QPushButton *infoButton;
    QPushButton *quitButton;
};

#endif
