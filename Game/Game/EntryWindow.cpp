#include "EntryWindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QDialog>
#include <QLabel>

EntryWindow::EntryWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("2048 Game");
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    playButton = new QPushButton("Play", this);
    infoButton = new QPushButton("Info", this);
    quitButton = new QPushButton("Quit", this);

    layout->addWidget(playButton);
    layout->addWidget(infoButton);
    layout->addWidget(quitButton);

    connect(playButton, &QPushButton::clicked, this, &EntryWindow::showPlayOptions);
    connect(infoButton, &QPushButton::clicked, this, &EntryWindow::showInfo);
    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
}

void EntryWindow::showPlayOptions() {
    QDialog *sizeDialog = new QDialog(this);
    sizeDialog->setWindowTitle("Select Board Size");
    QVBoxLayout *layout = new QVBoxLayout(sizeDialog);
    QComboBox *sizeCombo = new QComboBox(sizeDialog);
    sizeCombo->addItems({"3x3", "4x4", "5x5", "6x6", "7x7", "8x8"});
    QPushButton *startButton = new QPushButton("Start", sizeDialog);

    layout->addWidget(new QLabel("Choose board size:", sizeDialog));
    layout->addWidget(sizeCombo);
    layout->addWidget(startButton);

    connect(startButton, &QPushButton::clicked, [=]() {
        int size = sizeCombo->currentIndex() + 3;
        startGame(size);
        sizeDialog->accept();
    });

    sizeDialog->exec();
}

void EntryWindow::showInfo() {
    QMessageBox::information(this, "2048 Game Info",
                             "How to Play 2048:\n\n"
                             "Objective: Combine tiles with the same number to create a tile with the value of 2048.\n\n"
                             "Movement Controls:\n"
                             "- W or Up Arrow: Move tiles up\n"
                             "- S or Down Arrow: Move tiles down\n"
                             "- A or Left Arrow: Move tiles left\n"
                             "- D or Right Arrow: Move tiles right\n\n"
                             "Gameplay:\n"
                             "- Tiles slide in the chosen direction and merge if they have the same value.\n"
                             "- After each move, a new tile (2 or 4) appears randomly on the board.\n"
                             "- The game ends when no moves are possible.\n\n"
                             "Additional Feature:\n"
                             "- Press C to cycle through different background colors for the numbers windows.\n"
                             "- Press Q to cycle through different background colors for the game window\n\n"
                             "Select a board size (3x3 to 8x8) from the Play menu and enjoy!");
}

void EntryWindow::startGame(int size) {
    GameWindow *gameWindow = new GameWindow(size, nullptr); // parent to nullptr to make it a top-level window
    gameWindow->setAttribute(Qt::WA_DeleteOnClose); // Ensure GameWindow deletes itself when closed
    connect(gameWindow, &QMainWindow::destroyed, this, [this]() {
        show(); // Show EntryWindow when GameWindow closes
    });

    gameWindow->show();
    hide();
}
