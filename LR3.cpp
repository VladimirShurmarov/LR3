#include <iostream>
#include <string>

#define BOARD_SIZE 8

class ChessPiece {
public:
    std::string type; // Тип фигуры: "P", "N", "B", "R", "Q", "K"
    char color;       // Цвет: 'W' или 'B'
    int row, col;     // Текущие координаты фигуры

    ChessPiece(const std::string& t = "", char c = ' ', int r = -1, int cl = -1)
        : type(t), color(c), row(r), col(cl) {}

    // Проверка допустимости хода (пока базовая)
    virtual bool isValidMove(int targetRow, int targetCol) {
        return targetRow >= 0 && targetRow < BOARD_SIZE &&
            targetCol >= 0 && targetCol < BOARD_SIZE;
    }

    virtual ~ChessPiece() = default; // Виртуальный деструктор
};

class ChessBoard {
public:
    ChessPiece* board[BOARD_SIZE][BOARD_SIZE];

    ChessBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                board[i][j] = nullptr;
            }
        }
    }

    // Инициализация доски
    void initializeBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                board[i][j] = nullptr;
            }
        }
        // Пример инициализации начальных позиций пешек
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[6][j] = new ChessPiece("P", 'W', 6, j); // Белые пешки
            board[1][j] = new ChessPiece("P", 'B', 1, j); // Черные пешки
        }
    }

    // Печать шахматной доски
    void printBoard() {
        std::cout << "  a b c d e f g h\n";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            std::cout << 8 - i << " ";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j]) {
                    std::cout << board[i][j]->type[0] << " ";
                }
                else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
    }

    // Движение фигуры
    bool movePiece(ChessPiece* piece, int targetRow, int targetCol) {
        if (!piece || !piece->isValidMove(targetRow, targetCol)) {
            return false;
        }
        if (board[targetRow][targetCol] && board[targetRow][targetCol]->color == piece->color) {
            return false; // Нельзя ходить на свою фигуру
        }
        // Перемещаем фигуру
        board[piece->row][piece->col] = nullptr;
        if (board[targetRow][targetCol]) {
            delete board[targetRow][targetCol]; // Удаляем съеденную фигуру
        }
        piece->row = targetRow;
        piece->col = targetCol;
        board[targetRow][targetCol] = piece;
        return true;
    }

    ~ChessBoard() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                delete board[i][j];
            }
        }
    }
};

class ChessGame {
public:
    ChessBoard board;
    char currentPlayer;

    ChessGame() : currentPlayer('W') {}

    void startGame() {
        board.initializeBoard();
        std::cout << "Начало игры.\n";
        board.printBoard();
    }

    void makeMove(int startRow, int startCol, int targetRow, int targetCol) {
        ChessPiece* piece = board.board[startRow][startCol];
        if (!piece) {
            std::cout << "Ошибка: фигура отсутствует в указанной позиции!\n";
            return;
        }
        if (piece->color != currentPlayer) {
            std::cout << "Ошибка: сейчас ходит игрок " << currentPlayer << "!\n";
            return;
        }
        if (board.movePiece(piece, targetRow, targetCol)) {
            std::cout << "Ход выполнен: " << piece->type << " на "
                << char('a' + targetCol) << (8 - targetRow) << "\n";
            currentPlayer = (currentPlayer == 'W') ? 'B' : 'W'; // Меняем игрока
        }
        else {
            std::cout << "Ошибка: недопустимый ход!\n";
        }
        board.printBoard();
    }
};

int main() {
    setlocale(LC_ALL, "rus");

    ChessGame game;
    game.startGame();

    // Пример ходов
    game.makeMove(6, 4, 4, 4); // Белая пешка e2-e4
    game.makeMove(1, 4, 3, 4); // Черная пешка e7-e5
    game.makeMove(7, 1, 5, 2); // Белый конь b1-c3

    return 0;
}