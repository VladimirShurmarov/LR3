#include <iostream>
#include <string>
using namespace std;

#define BOARD_SIZE 8

// Структура для координат на доске
struct Position {
    int row, col;

    Position(int r = 0, int c = 0) : row(r), col(c) {}
};

// Класс для шахматной фигуры
class ChessPiece {
private:
    string type;   // Тип фигуры (пешка, конь, слон, ладья, ферзь, король)
    char color;    // Цвет ('W' - белый, 'B' - черный)
    Position pos;  // Позиция на доске
    bool moved;    // Проверка, двигалась ли фигура (нужно для рокировки)

public:
    ChessPiece(string t, char c, int row, int col)
        : type(t), color(c), pos(row, col), moved(false) {}

    string getType() const { return type; }
    char getColor() const { return color; }
    Position getPos() const { return pos; }
    bool isMoved() const { return moved; }
    void setPos(Position newPos) { pos = newPos; }
    void setMoved(bool movedStatus) { moved = movedStatus; }
};

// Класс для игрока
class Player {
private:
    string name;
    char color;    // Цвет игрока ('W' или 'B')

public:
    Player(string n, char c) : name(n), color(c) {}

    string getName() const { return name; }
    char getColor() const { return color; }
};

// Класс для шахматной доски
class ChessBoard {
private:
    ChessPiece* board[BOARD_SIZE][BOARD_SIZE];  // Доска

public:
    ChessBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i)
            for (int j = 0; j < BOARD_SIZE; ++j)
                board[i][j] = nullptr;
    }

    // Инициализация фигур на доске
    void initBoard() {
        // Инициализация белых фигур
        board[0][0] = new ChessPiece("R", 'W', 0, 0);
        board[0][1] = new ChessPiece("N", 'W', 0, 1);
        board[0][2] = new ChessPiece("B", 'W', 0, 2);
        board[0][3] = new ChessPiece("Q", 'W', 0, 3);
        board[0][4] = new ChessPiece("K", 'W', 0, 4);
        board[0][5] = new ChessPiece("B", 'W', 0, 5);
        board[0][6] = new ChessPiece("N", 'W', 0, 6);
        board[0][7] = new ChessPiece("R", 'W', 0, 7);
        for (int i = 0; i < BOARD_SIZE; ++i)
            board[1][i] = new ChessPiece("P", 'W', 1, i);

        // Инициализация черных фигур
        board[7][0] = new ChessPiece("R", 'B', 7, 0);
        board[7][1] = new ChessPiece("N", 'B', 7, 1);
        board[7][2] = new ChessPiece("B", 'B', 7, 2);
        board[7][3] = new ChessPiece("Q", 'B', 7, 3);
        board[7][4] = new ChessPiece("K", 'B', 7, 4);
        board[7][5] = new ChessPiece("B", 'B', 7, 5);
        board[7][6] = new ChessPiece("N", 'B', 7, 6);
        board[7][7] = new ChessPiece("R", 'B', 7, 7);
        for (int i = 0; i < BOARD_SIZE; ++i)
            board[6][i] = new ChessPiece("P", 'B', 6, i);
    }

    // Печать состояния доски
    void printBoard() const {
        char columns[] = "abcdefgh";  // Для вывода буквенных индексов столбцов
        cout << "  a b c d e f g h" << endl;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << BOARD_SIZE - i << " ";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j])
                    cout << board[i][j]->getType() << " ";
                else
                    cout << ". ";
            }
            cout << endl;
        }
    }

    bool isValidMove(Position start, Position end, char color) {
        ChessPiece* piece = board[start.row][start.col];
        if (!piece) return false; // Нет фигуры в стартовой позиции

        // Проверяем, что фигура — это пешка
        if (piece->getType() != "P") {
            return false;
        }

        int direction = (color == 'W') ? -1 : 1;  // Направление движения в зависимости от цвета
        int startRow = start.row;
        int startCol = start.col;
        int endRow = end.row;
        int endCol = end.col;

        // Пешка может двигаться на одну клетку вперед
        if (endCol == startCol && (endRow == startRow + direction)) {
            if (board[endRow][endCol] == nullptr) {
                return true; // Пустая клетка, можно двигаться
            }
        }

        // Пешка может двигаться на две клетки вперед только с начальной позиции
        if (endCol == startCol && (endRow == startRow + 2 * direction) && !piece->isMoved()) {
            if (board[endRow][endCol] == nullptr && board[startRow + direction][startCol] == nullptr) {
                return true; // Пустые клетки, можно двигаться
            }
        }

        // Пешка может брать фигуры по диагонали
        if (abs(endCol - startCol) == 1 && endRow == startRow + direction) {
            ChessPiece* targetPiece = board[endRow][endCol];
            if (targetPiece && targetPiece->getColor() != color) {
                return true; // Вражеская фигура на диагонали, можно взять
            }
        }

        return false; // Если ни одно из условий не выполнено
    }

    // Выполнение хода
    void movePiece(Position start, Position end) {
        ChessPiece* piece = board[start.row][start.col];
        if (piece) {
            board[end.row][end.col] = piece;
            board[start.row][start.col] = nullptr;
            piece->setPos(end);
            piece->setMoved(true);
        }
    }
};

// Класс для игры
class Game {
private:
    ChessBoard board;
    Player whitePlayer;
    Player blackPlayer;
    Player* currentPlayer;

public:
    Game(Player wp, Player bp) : whitePlayer(wp), blackPlayer(bp), currentPlayer(&whitePlayer) {
        board.initBoard();
    }

    ChessBoard* getBoard() {
        return &board;
    }

    Player* getCurrentPlayer() {
        return currentPlayer;
    }

    // Смена текущего игрока
    void switchPlayer() {
        currentPlayer = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
    }

    // Печать состояния игры
    void printState() const {
        board.printBoard();
        cout << "Текущий игрок: " << currentPlayer->getName() << endl;
    }
};
class GameSummary {
private:
    Player whitePlayer;
    Player blackPlayer;
    int totalMoves;
    std::string winner;

public:
    GameSummary(Player white, Player black, int moves, std::string win)
        : whitePlayer(white), blackPlayer(black), totalMoves(moves), winner(win) {}

    Player getWhitePlayer() const {
        return whitePlayer;
    }

    Player getBlackPlayer() const {
        return blackPlayer;
    }

    int getTotalMoves() const {
        return totalMoves;
    }

    std::string getWinner() const {
        return winner;
    }
    void printSummary() const {
        std::cout << "Итог игры:\n";
        std::cout << "Белые: " << whitePlayer.getName() << "\n";
        std::cout << "Черные: " << blackPlayer.getName() << "\n";
        std::cout << "Всего ходов: " << totalMoves << "\n";
        std::cout << "Победитель: " << winner << "\n";
    }
};

    // Функции игры
    void startGame(Game& game) {
        game.printState();
    }

    // Функция для преобразования строки в индексы массива
    bool convertPosition(const string& pos, int& row, int& col) {
        if (pos.size() != 2)
            return false;

        col = pos[0] - 'a';  // Преобразуем столбец (a-h) в индекс (0-7)
        row = 8 - (pos[1] - '0');  // Преобразуем строку (1-8) в индекс (0-7)

        // Проверяем, что полученные индексы находятся в допустимых пределах
        return col >= 0 && col < 8 && row >= 0 && row < 8;
    }

    // Функция для обработки хода
    void handleMove(Game& game) {
        string startPos, endPos;
        cout << "Введите ход (например, e2 e4): ";
        cin >> startPos >> endPos;

        int startRow, startCol, endRow, endCol;
        if (convertPosition(startPos, startRow, startCol) && convertPosition(endPos, endRow, endCol)) {
            Position start(startRow, startCol);
            Position end(endRow, endCol);

            if (game.getBoard()->isValidMove(start, end, game.getCurrentPlayer()->getColor())) {
                game.getBoard()->movePiece(start, end);
                game.switchPlayer();
                game.printState();
            }
            else {
                cout << "Неверный ход. Попробуйте снова." << endl;
            }
        }
        else {
            cout << "Неверный ввод позиции. Попробуйте снова." << endl;
        }
    }

    int main() {
        setlocale(LC_ALL, "rus");
        Player white("Белый", 'W');
        Player black("Черный", 'B');
        Game game(white, black);

        game.printState();

        while (true) {
            handleMove(game);
        }

        return 0;
    }
