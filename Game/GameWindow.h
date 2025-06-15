#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMap>
#include <vector>
#include <random>
#include <stack>
#include "SettingsManager.h"

class GameWindow : public QMainWindow {
    Q_OBJECT
public:
    GameWindow(int size, QWidget *parent = nullptr);
    ~GameWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void initializeBoard();
    void addRandomTile();
    bool moveTiles(Qt::Key direction);
    void updateBoardDisplay();
    void changeColorSchemeC();
    void changeColorSchemeQ();
    bool isGameOver();
    void checkFor2048();
    void undoLastMove();
    void updateMaxScore();

    int boardSize;
    std::vector<std::vector<int>> board;
    int score;
    QLabel *scoreLabel;
    QLabel *maxScoreLabel; // Added for maximal score
    QGridLayout *boardLayout;
    std::vector<std::vector<QLabel*>> tileLabels;
    std::mt19937 rng;
    QList<QColor> cColors; // Colors for 'C' key
    QList<QColor> qColors; // Colors for 'Q' key
    int cColorIndex;
    int qColorIndex;
    QMap<int, QColor> numberColors; // Colors for tile values
    std::stack<std::pair<std::vector<std::vector<int>>, int>> moveHistory; // Undo stack
    bool reached2048;
    bool canUndo; // Flag to allow only one undo per move
    QMap<int, int> maxScores; // Maximal scores per grid size
    SettingsManager *settingsManager; // For saving/loading settings
};

#endif
