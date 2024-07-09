#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>  


enum class Stone { EMPTY, BLACK, WHITE };

class GoBoard {
public:
    GoBoard() : board(19, std::vector<Stone>(19, Stone::EMPTY)), board_size(19), black_captured(0), white_captured(0) {}

    bool placeStone(int x, int y, Stone stone) {
        if (x < 0 || x >= board_size || y < 0 || y >= board_size || board[x][y] != Stone::EMPTY) {
            return false;
        }
        previous_board = board;
        move_history.push({x, y, stone});
        board[x][y] = stone;
        captureStones(x, y, stone);
        return true;
    }

    void printBoard() {
        std::cout << "   ";
        for (int j = 0; j < board_size; ++j) {
            std::cout << j % 10 << ' ';
        }
        std::cout << std::endl;

        for (int i = 0; i < board_size; ++i) {
            std::cout << (i % 10) << "  ";
            for (int j = 0; j < board_size; ++j) {
                char c = '.';
                if (board[i][j] == Stone::BLACK) c = 'B';
                else if (board[i][j] == Stone::WHITE) c = 'W';
                std::cout << c << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << "Puntuació parcial - Negre: " << black_captured << ", Blanc: " << white_captured << std::endl;
    }

    void passTurn() {
        previous_board = board;
        move_history.push({-1, -1, Stone::EMPTY});
    }

    void undoMove() {
        if (move_history.empty()) return;

        auto [x, y, stone] = move_history.top();
        move_history.pop();

        if (x != -1 && y != -1) {
            board[x][y] = Stone::EMPTY;
        }
        board = previous_board;
    }

    bool isKo() {
        return board == previous_board;
    }

private:
    int board_size;
    std::vector<std::vector<Stone>> board;
    std::vector<std::vector<Stone>> previous_board;
    int black_captured;
    int white_captured;
    std::stack<std::tuple<int, int, Stone>> move_history;

    void captureStones(int x, int y, Stone stone) {
        Stone opponent = (stone == Stone::BLACK) ? Stone::WHITE : Stone::BLACK;

        const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (const auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (nx >= 0 && nx < board_size && ny >= 0 && ny < board_size && board[nx][ny] == opponent) {
                if (!hasLiberties(nx, ny, opponent)) {
                    removeStones(nx, ny, opponent);
                }
            }
        }

        if (!hasLiberties(x, y, stone)) {
            removeStones(x, y, stone);
        }
    }

    bool hasLiberties(int x, int y, Stone stone) {
        std::set<std::pair<int, int>> visited;
        std::queue<std::pair<int, int>> toVisit;
        toVisit.push({x, y});
        visited.insert({x, y});

        const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        while (!toVisit.empty()) {
            auto [cx, cy] = toVisit.front();
            toVisit.pop();

            for (const auto& dir : directions) {
                int nx = cx + dir.first;
                int ny = cy + dir.second;
                if (nx >= 0 && nx < board_size && ny >= 0 && ny < board_size) {
                    if (board[nx][ny] == Stone::EMPTY) {
                        return true;
                    }
                    if (board[nx][ny] == stone && visited.find({nx, ny}) == visited.end()) {
                        toVisit.push({nx, ny});
                        visited.insert({nx, ny});
                    }
                }
            }
        }
        return false;
    }

    void removeStones(int x, int y, Stone stone) {
        std::queue<std::pair<int, int>> toRemove;
        toRemove.push({x, y});

        const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        int removedCount = 0;

        while (!toRemove.empty()) {
            auto [cx, cy] = toRemove.front();
            toRemove.pop();
            board[cx][cy] = Stone::EMPTY;
            ++removedCount;

            for (const auto& dir : directions) {
                int nx = cx + dir.first;
                int ny = cy + dir.second;
                if (nx >= 0 && nx < board_size && ny >= 0 && ny < board_size && board[nx][ny] == stone) {
                    toRemove.push({nx, ny});
                }
            }
        }

        if (stone == Stone::BLACK) {
            white_captured += removedCount;
        } else if (stone == Stone::WHITE) {
            black_captured += removedCount;
        }
    }

    int countRemovedStones(int x, int y, Stone stone) {
        std::queue<std::pair<int, int>> toRemove;
        toRemove.push({x, y});
        std::set<std::pair<int, int>> visited;
        visited.insert({x, y});
        int removedCount = 0;

        const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        while (!toRemove.empty()) {
            auto [cx, cy] = toRemove.front();
            toRemove.pop();
            ++removedCount;

            for (const auto& dir : directions) {
                int nx = cx + dir.first;
                int ny = cy + dir.second;
                if (nx >= 0 && nx < board_size && ny >= 0 && ny < board_size && board[nx][ny] == stone && visited.find({nx, ny}) == visited.end()) {
                    toRemove.push({nx, ny});
                    visited.insert({nx, ny});
                }
            }
        }

        return removedCount;
    }
};

int main() {
    GoBoard game;
    Stone currentPlayer = Stone::BLACK;
    int passCount = 0;

    while (true) {
        system("clear");
        game.printBoard();
        int x, y;
        std::string input;

        std::cout << "Jugador " << (currentPlayer == Stone::BLACK ? "Negre" : "Blanc") << ", introdueix la teva jugada (x y), 'p' per passar el torn, 'u' per desfer l'últim moviment, o 'q' per rendir-te: ";
        std::cin >> input;

        if (input == "p") {
            game.passTurn();
            passCount++;
            if (passCount >= 2) {
                std::cout << "Ambdós jugadors han passat consecutivament. Fi de la partida!" << std::endl;
                break;
            }
            currentPlayer = (currentPlayer == Stone::BLACK) ? Stone::WHITE : Stone::BLACK;
            continue;
        } else if (input == "u") {
            game.undoMove();
            currentPlayer = (currentPlayer == Stone::BLACK) ? Stone::WHITE : Stone::BLACK;
            continue;
        } else if (input == "q") {
            std::cout << "Jugador " << (currentPlayer == Stone::BLACK ? "Negre" : "Blanc") << " s'ha rendit. Fi de la partida!" << std::endl;
            break;
        } else {
            try {
                x = std::stoi(input);
                std::cin >> y;

                if (game.placeStone(x, y, currentPlayer)) {
                    if (game.isKo()) {
                        std::cout << "Moviment no permès (regla de Ko). Torna a intentar-ho.\n";
                        game.undoMove();
                    } else {
                        currentPlayer = (currentPlayer == Stone::BLACK) ? Stone::WHITE : Stone::BLACK;
                        passCount = 0;
                    }
                } else {
                    std::cout << "Moviment invàlid. Torna a intentar-ho.\n";
                }
            } catch (std::invalid_argument&) {
                std::cout << "Entrada no vàlida. Torna a intentar-ho.\n";
            }
        }
    }

    return 0;
}

        
