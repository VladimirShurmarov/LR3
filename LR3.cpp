#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#define RULES "Rules.txt"
using namespace std;

class Rules {
private:
	string content;
public:
	Rules() {
		FILE* fp = fopen(RULES, "r");
		if (fp) {
			char buffer[256];
			while (fgets(buffer, sizeof(buffer), fp)) {
				content += buffer;
			}
			fclose(fp);
		}
		else {
			content = "Rules file not found.";
		}
	}
	void showRules() const {
		cout << content << endl;
	}
};

class Player {
private:
	string name;
	int score;
public:
	Player(const string& playerName) : name(playerName), score(0) {}
	void increaseScore() { score++; }
	int getScore() const { return score; }
	string getName() const { return name; }
};

class Field {
private:
	char pole[7][7];
public:
	Field() {
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 7; j++)
				pole[i][j] = '.';
	}
	void display() const {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 7; j++)
				cout << pole[i][j] << ' ';
			cout << endl;
		}
	}
};

class Figure {
private:
	string type;
	string color;
	int x, y;
public:
	Figure(const string& figType, const string& figColor, int posX, int posY)
		: type(figType), color(figColor), x(posX), y(posY) {}
	void move(int newX, int newY) {
		x = newX;
		y = newY;
	}
	void display() const {
		cout << "Type: " << type << ", Color: " << color
			<< ", Position: (" << x << ", " << y << ")\n";
	}
};

class MatchHistory {
private:
	string winner;
	int moves;
public:
	MatchHistory(const string& winnerName, int moveCount) : winner(winnerName), moves(moveCount) {}
	void displayHistory() const {
		cout << "Winner: " << winner << ", Moves: " << moves << endl;
	}
};

class Score {
private:
	int blackScore;
	int whiteScore;
public:
	Score() : blackScore(0), whiteScore(0) {}
	void updateScore(const string& color) {
		if (color == "Black")
			blackScore++;
		else if (color == "White")
			whiteScore++;
	}
	void displayScore() const {
		cout << "Black: " << blackScore << " | White: " << whiteScore << endl;
	}
};

class Game {
private:
	Field field;
	Player player1;
	Player player2;
	Rules rules;
	vector<Figure> figures;
public:
	Game(const string& p1, const string& p2) : player1(p1), player2(p2) {}
	void addFigure(const string& type, const string& color, int x, int y) {
		figures.emplace_back(type, color, x, y);
	}
	void startGame() {
		cout << "Starting game between " << player1.getName() << " and " << player2.getName() << endl;
		rules.showRules();
		field.display();
		for (const auto& fig : figures) {
			fig.display();
		}
	}
};

int main() {
	// Работа с классами
	Game chessGame("Alice", "Bob");

	chessGame.addFigure("Pawn", "White", 1, 2);
	chessGame.addFigure("Knight", "Black", 2, 1);

	chessGame.startGame();

	MatchHistory history("Alice", 42);
	history.displayHistory();

	Score gameScore;
	gameScore.updateScore("White");
	gameScore.updateScore("Black");
	gameScore.displayScore();

	return 0;
}