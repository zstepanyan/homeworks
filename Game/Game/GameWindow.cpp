#include "GameWindow.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QTime>
#include <QApplication>
#include <QHBoxLayout>

GameWindow::GameWindow(int size, QWidget *parent) : QMainWindow(parent), boardSize(size), score(0), cColorIndex(0), qColorIndex(0), reached2048(false), canUndo(false) {
    setWindowTitle("2048 Game");
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Top layout for score and max score
    QHBoxLayout *topLayout = new QHBoxLayout();
    maxScoreLabel = new QLabel("Max Score: 0", this);
    maxScoreLabel->setAlignment(Qt::AlignLeft);
    maxScoreLabel->setFixedHeight(30);
    topLayout->addWidget(maxScoreLabel);

    scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->setAlignment(Qt::AlignRight);
    scoreLabel->setFixedHeight(30);
    topLayout->addWidget(scoreLabel);
    mainLayout->addLayout(topLayout);

    boardLayout = new QGridLayout();
    boardLayout->setSpacing(5);
    boardLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(boardLayout);

    board.resize(boardSize, std::vector<int>(boardSize, 0));
    tileLabels.resize(boardSize, std::vector<QLabel*>(boardSize, nullptr));

    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            tileLabels[i][j] = new QLabel(this);
            tileLabels[i][j]->setAlignment(Qt::AlignCenter);
            tileLabels[i][j]->setFixedSize(60, 60);
            tileLabels[i][j]->setStyleSheet("background-color: #cdc1b4; border: 1px solid #bbada0; font-size: 20px; color: #776e65;");
            boardLayout->addWidget(tileLabels[i][j], i, j);
        }
    }

    cColors = {QColor("#f0f0f0"), QColor("#e0e0ff"), QColor("#ffe0e0"), QColor("#e0ffe0")};
    qColors = {QColor("#d0f0d0"), QColor("#f0d0f0"), QColor("#d0d0f0"), QColor("#f0f0d0")};
    numberColors = {
        {2, QColor("#eee4da")}, {4, QColor("#ede0c8")}, {8, QColor("#f2b179")},
        {16, QColor("#f59563")}, {32, QColor("#f67c5f")}, {64, QColor("#f65e3b")},
        {128, QColor("#edcf72")}, {256, QColor("#edcc61")}, {512, QColor("#edc850")},
        {1024, QColor("#edc53f")}, {2048, QColor("#edc22e")}, {4096, QColor("#3c3a32")},
        {8192, QColor("#2e2a22")}, {16384, QColor("#1f1c18")}, {32768, QColor("#0f0e0c")}
    };

    // init settings manager and load settings
    settingsManager = new SettingsManager();
    cColorIndex = settingsManager->loadColorIndex("cColorIndex", 0);
    qColorIndex = settingsManager->loadColorIndex("qColorIndex", 0);
    maxScores = settingsManager->loadMaxScores();

    rng.seed(QTime::currentTime().msec());
    initializeBoard();
    updateBoardDisplay();
    updateMaxScore();

    int tileSize = 60;
    int gridSize = boardSize * tileSize + (boardSize - 1) * boardLayout->spacing();
    int margin = 20;
    int windowWidth = gridSize + margin * 2;
    int windowHeight = gridSize + scoreLabel->height() + margin * 2 + mainLayout->spacing() * 2;
    setFixedSize(windowWidth, windowHeight);
}

GameWindow::~GameWindow() {
    // Save settings on destruction
    settingsManager->saveColorIndex("cColorIndex", cColorIndex);
    settingsManager->saveColorIndex("qColorIndex", qColorIndex);
    settingsManager->saveMaxScores(maxScores);
    delete settingsManager;
}

void GameWindow::initializeBoard() {
    board.assign(boardSize, std::vector<int>(boardSize, 0));
    score = 0;
    scoreLabel->setText("Score: 0");
    moveHistory = std::stack<std::pair<std::vector<std::vector<int>>, int>>(); // Clear history
    reached2048 = false;
    canUndo = false;
    addRandomTile();
    addRandomTile();
    updateBoardDisplay();
    updateMaxScore();
}

void GameWindow::addRandomTile() {
    std::vector<std::pair<int, int>> emptyTiles;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] == 0) emptyTiles.emplace_back(i, j);
        }
    }

    if (!emptyTiles.empty()) {
        std::uniform_int_distribution<> dist(0, emptyTiles.size() - 1);
        auto [i, j] = emptyTiles[dist(rng)];
        board[i][j] = (dist(rng) % 2 == 0) ? 2 : 4;
    }
}

bool GameWindow::moveTiles(Qt::Key direction) {
    bool moved = false;
    std::vector<std::vector<int>> newBoard = board;
    int newScore = score;

    moveHistory = std::stack<std::pair<std::vector<std::vector<int>>, int>>();
    moveHistory.push({board, score}); // Save state before move

    auto slideAndMerge = [&](std::vector<int>& line) {
        std::vector<int> nonZeros;
        bool merged = false;
        for (int val : line) {
            if (val != 0) nonZeros.push_back(val);
        }
    
        std::vector<int> result;
        for (size_t i = 0; i < nonZeros.size(); ++i) {
            if (i + 1 < nonZeros.size() && nonZeros[i] == nonZeros[i + 1] && !merged) {
                result.push_back(nonZeros[i] * 2);
                newScore += nonZeros[i] * 2;
                ++i;
                merged = true;
            } 
            else {
                result.push_back(nonZeros[i]);
                merged = false;
            }
        }

        while (result.size() < line.size()) {
            result.push_back(0);
        }
        
        if (result != line) moved = true;
        return result;
    };

    if (direction == Qt::Key_Up || direction == Qt::Key_W) {
        for (int j = 0; j < boardSize; ++j) {
            std::vector<int> column(boardSize);
            for (int i = 0; i < boardSize; ++i) column[i] = newBoard[i][j];
            auto result = slideAndMerge(column);
            for (int i = 0; i < boardSize; ++i) newBoard[i][j] = result[i];
        }
    } 
    else if (direction == Qt::Key_Down || direction == Qt::Key_S) {
        for (int j = 0; j < boardSize; ++j) {
            std::vector<int> column(boardSize);
            for (int i = 0; i < boardSize; ++i) column[i] = newBoard[boardSize - 1 - i][j];
            auto result = slideAndMerge(column);
            for (int i = 0; i < boardSize; ++i) newBoard[boardSize - 1 - i][j] = result[i];
        }
    } 
    else if (direction == Qt::Key_Left || direction == Qt::Key_A) {
        for (int i = 0; i < boardSize; ++i) {
            std::vector<int> row = newBoard[i];
            newBoard[i] = slideAndMerge(row);
        }
    } 
    else if (direction == Qt::Key_Right || direction == Qt::Key_D) {
        for (int i = 0; i < boardSize; ++i) {
            std::vector<int> row(boardSize);
            for (int j = 0; j < boardSize; ++j) {
                row[j] = newBoard[i][boardSize - 1 - j];
            }
            
            auto result = slideAndMerge(row);
            for (int j = 0; j < boardSize; ++j) {
                newBoard[i][boardSize - 1 - j] = result[j];
            }
        }
    }

    if (moved) {
        board = newBoard;
        score = newScore;
        canUndo = true;
        addRandomTile();
        updateBoardDisplay();
        scoreLabel->setText(QString("Score: %1").arg(score));
        updateMaxScore();
        checkFor2048();
        if (isGameOver()) {
            QMessageBox::information(this, "Game Over", QString("Game Over! Score: %1\nPress 'U' to undo or any move key to restart.").arg(score));
        }
    } 
    else {
        moveHistory.pop();
        canUndo = false;
    }
    
    return moved;
}

void GameWindow::updateBoardDisplay() {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] == 0) {
                tileLabels[i][j]->setText("");
                tileLabels[i][j]->setStyleSheet(QString("background-color: %1; border: 1px solid #bbada0; font-size: 20px; color: #776e65;").arg(cColors[cColorIndex].name()));
            } 
            else {
                tileLabels[i][j]->setText(QString::number(board[i][j]));
                QColor bgColor = numberColors.value(board[i][j], QColor("#cdc1b4"));
                tileLabels[i][j]->setStyleSheet(QString("background-color: %1; border: 1px solid #bbada0; font-size: 20px; color: #776e65;").arg(bgColor.name()));
            }
        }
    }

    centralWidget()->setStyleSheet(QString("background-color: %1;").arg(qColors[qColorIndex].name()));
}

void GameWindow::changeColorSchemeC() {
    cColorIndex = (cColorIndex + 1) % cColors.size();
    updateBoardDisplay();
    settingsManager->saveColorIndex("cColorIndex", cColorIndex);
}

void GameWindow::changeColorSchemeQ() {
    qColorIndex = (qColorIndex + 1) % qColors.size();
    updateBoardDisplay();
    settingsManager->saveColorIndex("qColorIndex", qColorIndex);
}

void GameWindow::checkFor2048() {
    if (reached2048) return;
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] == 2048) {
                reached2048 = true;
                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Congratulations!");
                msgBox.setText("You reached 2048! Do you want to continue?");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::Yes);
                if (msgBox.exec() == QMessageBox::No) {
                    close();
                }
    
                return;
            }
        }
    }
}

void GameWindow::undoLastMove() {
    if (canUndo && !moveHistory.empty()) {
        auto [prevBoard, prevScore] = moveHistory.top();
        moveHistory.pop();
        board = prevBoard;
        score = prevScore;
        scoreLabel->setText(QString("Score: %1").arg(score));
        updateBoardDisplay();
        updateMaxScore();
        canUndo = false;
    }
}

void GameWindow::updateMaxScore() {
    int currentMax = maxScores.value(boardSize, 0);
    if (score > currentMax) {
        maxScores[boardSize] = score;
        maxScoreLabel->setText(QString("Max Score: %1").arg(score));
        settingsManager->saveMaxScores(maxScores);
    } 
    else {
        maxScoreLabel->setText(QString("Max Score: %1").arg(currentMax));
    }
}

bool GameWindow::isGameOver() {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] == 0) {
                return false;
            }
            if (i < boardSize - 1 && board[i][j] == board[i + 1][j]) {
                return false;
            }
            if (j < boardSize - 1 && board[i][j] == board[i][j + 1]) {
                return false;
            }
        }
    }

    return true;
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = static_cast<Qt::Key>(event->key());
    if (key == Qt::Key_W || key == Qt::Key_Up ||
        key == Qt::Key_S || key == Qt::Key_Down ||
        key == Qt::Key_A || key == Qt::Key_Left ||
        key == Qt::Key_D || key == Qt::Key_Right) {
        if (!isGameOver()) {
            moveTiles(key);
        } 
        else {
            initializeBoard();
            updateBoardDisplay();
        }
    } 
    else if (key == Qt::Key_C) {
        changeColorSchemeC();
    } 
    else if (key == Qt::Key_Q) {
        changeColorSchemeQ();
    } 
    else if (key == Qt::Key_U) {
        undoLastMove();
    }
    
    QMainWindow::keyPressEvent(event);
}
