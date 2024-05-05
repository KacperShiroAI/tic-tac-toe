#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Player {
private:
    int id;
    char symbol;
    int wins;
    int losses;
    int draws;
    int points;

public:
    Player(int id, char symbol) : id(id), symbol(symbol), wins(0), losses(0), draws(0), points(0) {}

    void addWin() {
        wins++;
        points += 3;
    }

    void addLoss() {
        losses++;
    }

    void addDraw() {
        draws++;
        points += 1;
    }

    int getWins() const {
        return wins;
    }

    int getLosses() const {
        return losses;
    }

    int getDraws() const {
        return draws;
    }

    int getPoints() const {
        return points;
    }

    void displayStats() const {
        std::cout << id << " (" << symbol << "):" << " ";
        std::cout << " " << wins << " ";
        std::cout << " " << losses << " ";
        std::cout << " " << draws << " ";
        std::cout << " " << points << std::endl;
    }

    int getId() const {
        return id;
    }

    char getSymbol() const {
        return symbol;
    }
};

class GameBoard {
public:
    std::vector<std::vector<char>> board;
    char player1;
    char player2;

    GameBoard(char n1, char n2, char n3, char n4, char n5, char n6, char n7, char n8, char n9) {
        std::vector<char> row1 = {n1, n2, n3};
        std::vector<char> row2 = {n4, n5, n6};
        std::vector<char> row3 = {n7, n8, n9};
        board.push_back(row1);
        board.push_back(row2);
        board.push_back(row3);

        bool foundPlayer1 = false;
        bool foundPlayer2 = false;

        for (const auto& row : board) {
            for (char cell : row) {
                if (cell != '.' && !foundPlayer1) {
                    player1 = cell;
                    foundPlayer1 = true;
                } else if (cell != '.' && cell != player1 && !foundPlayer2) {
                    player2 = cell;
                    foundPlayer2 = true;
                }
            }
        }
    }

    void displayBoard() const {
        for (const auto& row : board) {
            for (char cell : row) {
                std::cout << cell;
            }
            std::cout << std::endl;
        }
    }

    char checkWinner() const {
        // Sprawdzenie wygranej w wierszach
        for (const auto& row : board) {
            if (row[0] != '.' && row[0] == row[1] && row[1] == row[2]) {
                return row[0];
            }
        }
        // Sprawdzenie wygranej w kolumnach
        for (int i = 0; i < 3; ++i) {
            if (board[0][i] != '.' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
                return board[0][i];
            }
        }
        // Sprawdzenie wygranej na przek¹tnych
        if (board[0][0] != '.' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            return board[0][0];
        }
        if (board[0][2] != '.' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            return board[0][2];
        }
        return '.';
    }
};

int main() {
    bool exit = false;
    int option;
    int playerId;
    std::vector<Player> players;
    std::vector<GameBoard> boards;
    std::ifstream playersFile("players.txt");
    std::ifstream boardFile("tictactoe.txt");

    if (playersFile.is_open()) {
        int id;
        char symbol;
        while (playersFile >> id >> symbol) {
            players.push_back(Player(id, symbol));
        }
        playersFile.close();
    } else {
        std::cerr << "Unable to open file players.txt" << std::endl;
        return 1;
    }

    if (boardFile.is_open()) {
        char n1, n2, n3, n4, n5, n6, n7, n8, n9;
        int i = 1;
        std::string line;
        while (std::getline(boardFile, line)) {
            if (i%4 != 0) {
                if (i%4 == 1) {
                    n1 = line[0];
                    n2 = line[1];
                    n3 = line[2];
                } else if (i%4 == 2) {
                    n4 = line[0];
                    n5 = line[1];
                    n6 = line[2];
                } else if (i%4 == 3) {
                    n7 = line[0];
                    n8 = line[1];
                    n9 = line[2];
                }
                i++;
            } else {
                boards.push_back(GameBoard(n1, n2, n3, n4, n5, n6, n7, n8, n9));
                i++;
            }
        }
        boardFile.close();
    } else {
        std::cerr << "Unable to open file tictactoe.txt" << std::endl;
        return 1;
    }

    for (const auto& board : boards) {
        // Sprawdzenie zwyciêzcy dla danej planszy
        bool draw = false;
        char winner = board.checkWinner();
        char loser;
        if (board.player1 == winner)
            loser = board.player2;
        else if (board.player2 == winner)
            loser = board.player1;
        else
            draw = true;

        // Aktualizacja punktów graczy na podstawie zwyciêstwa
        for (auto& player : players) {
            if (player.getSymbol() == winner) {
                player.addWin(); // Dodajemy zwyciêstwo
            }
            if (player.getSymbol() == loser)
            {
                player.addLoss();
            }
            if (draw == true)
            {
                if(player.getSymbol() == board.player1 || player.getSymbol() == board.player2)
                    player.addDraw();
            }
        }
        winner = '.';
        loser = '.';
    }

    std::vector<Player>::iterator it;

    do {
        std::cout << "1 - show table" << std::endl;
        std::cout << "2 - show result" << std::endl;
        std::cout << "3 - show player stats" << std::endl;
        std::cout << "4 - exit" << std::endl;
        std::cout << "Choose option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
                    return a.getPoints() > b.getPoints();
                });
                for (const auto& player : players) {
                    player.displayStats();
                }
                break;
            case 2:
                for (const auto& board : boards){
                    board.displayBoard();
                    std::cout << std::endl;
                }
                break;
            case 3:
                std::cout << "Enter player ID: ";
                std::cin >> playerId;
                it = std::find_if(players.begin(), players.end(), [&](const Player& player) {
                    return player.getId() == playerId;
                });
                if (it != players.end()) {
                    it->displayStats();
                } else {
                    std::cout << "Player with ID " << playerId << " not found." << std::endl;
                }
                break;
            case 4:
                exit = true;
                break;
            default:
                std::cout << "Invalid option! Please choose again." << std::endl;
                break;
        }
    } while (!exit);
    return 0;
}



