#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <fstream>
#include <cstdlib>
#include <ctime>

enum class Stone { EMPTY, BLACK, WHITE };

class GoBoard {
public:
    GoBoard() : board(19, std::vector<Stone>(19, Stone::EMPTY)), board_size(19), black_captured(0), white_captured(0) {
        std::srand(std::time(0)); // Initialize random seed for AI moves
    }

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
        std::cout << "Liberties - Blanc: " << countLiberties(Stone::WHITE) << ", Negre: " << countLiberties(Stone::BLACK) << std::endl;
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

    std::pair<int, int> getAIMove() {
        std::vector<std::pair<int, int>> valid_moves;
        for (int i = 0; i < board_size; ++i) {
            for (int j = 0; j < board_size; ++j) {
                if (board[i][j] == Stone::EMPTY) {
                    valid_moves.push_back({i, j});
                }
            }
        }
        if (valid_moves.empty()) return {-1, -1};
        int idx = std::rand() % valid_moves.size();
        return valid_moves[idx];
    }

    void saveGame(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for saving.\n";
            return;
        }
        file << board_size << '\n';
        for (const auto& row : board) {
            for (const auto& cell : row) {
                file << static_cast<int>(cell) << ' ';
            }
            file << '\n';
        }
        file << black_captured << ' ' << white_captured << '\n';
        while (!move_history.empty()) {
            auto [x, y, stone] = move_history.top();
            file << x << ' ' << y << ' ' << static_cast<int>(stone) << '\n';
            move_history.pop();
        }
        file.close();
        std::cout << "Game saved to " << filename << '\n';
    }

    void loadGame(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for loading.\n";
            return;
        }
        file >> board_size;
        board.resize(board_size, std::vector<Stone>(board_size, Stone::EMPTY));
        for (auto& row : board) {
            for (auto& cell : row) {
                int val;
                file >> val;
                cell = static_cast<Stone>(val);
            }
        }
        file >> black_captured >> white_captured;
        move_history = {};
        int x, y, stone;
        while (file >> x >> y >> stone) {
            move_history.push({x, y, static_cast<Stone>(stone)});
        }
        file.close();
        std::cout << "Game loaded from " << filename << '\n';
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

            for (const auto& dir            : directions) {
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

    int countLiberties(int x, int y) {
        Stone stone = board[x][y];
        std::set<std::pair<int, int>> liberties;
        const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (const auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (nx >= 0 && nx < board_size && ny >= 0 && ny < board_size && board[nx][ny] == Stone::EMPTY) {
                liberties.insert({nx, ny});
            }
        }

        return liberties.size();
    }

    int countLiberties(Stone stone) {
        int totalLiberties = 0;
        for (int i = 0; i < board_size; ++i) {
            for (int j = 0; j < board_size; ++j) {
                if (board[i][j] == stone) {
                    totalLiberties += countLiberties(i, j);
                }
            }
        }
        return totalLiberties;
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

        std::cout << "Jugador " << (currentPlayer == Stone::BLACK ? "Negre" : "Blanc") << ", introdueix la teva jugada (x y), 'p' per passar el torn, 'u' per desfer l'últim moviment, 's' per desar la partida, 'l' per carregar una partida, o 'q' per rendir-te: ";
        std::cin >> input;

        if (input == "p") {
            game.passTurn();
            passCount++;
            if (passCount >= 2) {
                std::cout << "Ambdós jugadors han passat consecutivament. Fi de la partida!" << std::endl;
                break;
            }
            currentPlayer = (currentPlayer == Stone::BLACK) ? Stone::WHITE : Stone::BLACK;
        } else if (input == "u") {
            game.undoMove();
            currentPlayer = (currentPlayer == Stone::BLACK) ? Stone::WHITE : Stone::BLACK;
        } else if (input == "q") {
            std::cout << "Jugador " << (currentPlayer == Stone::BLACK ? "Negre" : "Blanc") << " s'ha rendit. Fi de la partida!" << std::endl;
            break;
        } else if (input == "s") {
            std::string filename;
            std::cout << "Introdueix el nom del fitxer per desar la partida: ";
            std::cin >> filename;
            game.saveGame(filename);
        } else if (input == "l") {
            std::string filename;
            std::cout << "Introdueix el nom del fitxer per carregar la partida: ";
            std::cin >> filename;
            game.loadGame(filename);
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

        // AI turn (if current player is AI)
        if (currentPlayer == Stone::WHITE) {
            std::cout << "Turno de l'AI Blanc...\n";
            auto [ai_x, ai_y] = game.getAIMove();
            if (ai_x != -1 && ai_y != -1) {
                std::cout << "L'AI Blanc ha jugat a: " << ai_x << ' ' << ai_y << std::endl;
                game.placeStone(ai_x, ai_y, Stone::WHITE);
                currentPlayer = Stone::BLACK;
            } else {
                std::cout << "L'AI Blanc ha passat el torn.\n";
                game.passTurn();
                currentPlayer = Stone::BLACK;
            }
            std::cin.ignore(); // Wait for user to press enter
        }
    }

    return 0;
}


