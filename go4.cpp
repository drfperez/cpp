#include <iostream> // Llibreria per utilitzar les funcions d'entrada i sortida
#include <vector>   // Llibreria per utilitzar el contenidor vector
#include <queue>    // Llibreria per utilitzar el contenidor queue
#include <set>      // Llibreria per utilitzar el contenidor set
#include <string>   // Llibreria per utilitzar el contenidor string

// Enumeració per representar l'estat de cada cel·la del tauler
enum class Stone { EMPTY, BLACK, WHITE };

// Classe que representa el tauler de Go
class GoBoard {
public:
    // Constructor que inicialitza el tauler amb tamany 19x19 i totes les cel·les buides
    GoBoard() : board(19, std::vector<Stone>(19, Stone::EMPTY)), board_size(19), black_captured(0), white_captured(0) {}

    // Funció per col·locar una pedra en una posició especificada
    bool placeStone(int x, int y, Stone stone) {
        // Verifica si la posició és dins del tauler i si la cel·la està buida
        if (x < 0 || x >= board_size || y < 0 || y >= board_size || board[x][y] != Stone::EMPTY) {
            return false; // Retorna fals si el moviment no és vàlid
        }
        // Guarda l'estat actual del tauler abans de col·locar la pedra
        previous_board = board;
        board[x][y] = stone; // Col·loca la pedra en la posició especificada
        // Verifica i elimina les pedres capturades
        captureStones(x, y, stone);
        return true; // Retorna cert si el moviment és vàlid
    }

    // Funció per imprimir el tauler
    void printBoard() {
        std::cout << "   ";
        for (int j = 0; j < board_size; ++j) {
            std::cout << j % 10 << ' ';
        }
        std::cout << std::endl;

        for (int i = 0; i < board_size; ++i) {
            std::cout << (i % 10) << "  ";
            for (int j = 0; j < board_size; ++j) {
                char c = '.'; // Caràcter per representar una cel·la buida
                if (board[i][j] == Stone::BLACK) c = 'B'; // Caràcter per una pedra negra
                else if (board[i][j] == Stone::WHITE) c = 'W'; // Caràcter per una pedra blanca
                std::cout << c << ' '; // Imprimeix el caràcter seguit d'un espai
            }
            std::cout << std::endl; // Canvia de línia després de cada fila
        }
        // Imprimeix les puntuacions parcials
        std::cout << "Puntuació parcial - Negre: " << black_captured << ", Blanc: " << white_captured << std::endl;
    }

    // Funció per passar el torn
    void passTurn() {
        previous_board = board;
    }

    // Funció per verificar si un moviment repeteix l'estat anterior (Ko rule)
    bool isKo() {
        return board == previous_board;
    }

private:
    int board_size; // Tamany del tauler
    std::vector<std::vector<Stone>> board; // Matriu bidimensional que representa el tauler
    std::vector<std::vector<Stone>> previous_board; // Estat anterior del tauler
    int black_captured; // Pedres capturades pel jugador negre
    int white_captured; // Pedres capturades pel jugador blanc

    // Funció per verificar i eliminar pedres capturades
    void captureStones(int x, int y, Stone stone) {
        Stone opponent = (stone == Stone::BLACK) ? Stone::WHITE : Stone::BLACK; // Determina el color de l'oponent

        // Defineix les direccions de moviment per verificar les posicions adjacents
        const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        // Verifica les posicions adjacents
        for (const auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (nx >= 0 && nx < board_size && ny >= 0 && ny < board_size && board[nx][ny] == opponent) {
                if (!hasLiberties(nx, ny, opponent)) {
                    removeStones(nx, ny, opponent); // Elimina les pedres capturades
                }
            }
        }

        // Verifica si la pedra col·locada ha quedat sense llibertats
        if (!hasLiberties(x, y, stone)) {
            removeStones(x, y, stone); // Elimina la pedra col·locada si no té llibertats
        }
    }

    // Funció per verificar si un grup de pedres té llibertats
    bool hasLiberties(int x, int y, Stone stone) {
        std::set<std::pair<int, int>> visited; // Conjunt de posicions visitades
        std::queue<std::pair<int, int>> toVisit; // Cua de posicions per visitar
        toVisit.push({x, y});
        visited.insert({x, y});

        // Defineix les direccions de moviment per verificar les posicions adjacents
        const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        // Bucle per explorar el grup de pedres
        while (!toVisit.empty()) {
            auto [cx, cy] = toVisit.front();
            toVisit.pop();

            // Verifica les posicions adjacents
            for (const auto& dir : directions) {
                int nx = cx + dir.first;
                int ny = cy + dir.second;
                if (nx >= 0 && nx < board_size && ny >= 0 && ny < board_size) {
                    if (board[nx][ny] == Stone::EMPTY) {
                        return true; // Retorna cert si hi ha una llibertat
                    }
                    if (board[nx][ny] == stone && visited.find({nx, ny}) == visited.end()) {
                        toVisit.push({nx, ny}); // Afegeix la posició a la cua
                        visited.insert({nx, ny}); // Marca la posició com visitada
                    }
                }
            }
        }
        return false; // Retorna fals si no hi ha llibertats
    }

    // Funció per eliminar un grup de pedres
    void removeStones(int x, int y, Stone stone) {
        std::queue<std::pair<int, int>> toRemove; // Cua de posicions per eliminar
        toRemove.push({x, y});

        // Defineix les direccions de moviment per verificar les posicions adjacents
        const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        int removedCount = 0; // Contador de pedres eliminades

        // Bucle per eliminar el grup de pedres
        while (!toRemove.empty()) {
            auto [cx, cy] = toRemove.front();
            toRemove.pop();
            board[cx][cy] = Stone::EMPTY; // Elimina la pedra
            ++removedCount; // Incrementa el contador de pedres eliminades

            // Verifica les posicions adjacents
            for (const auto& dir : directions) {
                int nx = cx + dir.first;
                int ny = cy + dir.second;
                if (nx >= 0 && nx < board_size && ny >= 0 && ny < board_size && board[nx][ny] == stone) {
                    toRemove.push({nx, ny}); // Afegeix la posició a la cua
                }
            }
        }

        // Actualitza les pedres capturades
        if (stone == Stone::BLACK) {
            white_captured += removedCount;
        } else {
            black_captured += removedCount;
        }
    }
};
int main() {
    GoBoard game; // Crea un objecte GoBoard amb tamany 19x19
    Stone currentPlayer = Stone::BLACK; // Inicialitza el jugador actual com a negre
    int passCount = 0; // Comptador per els torns passats consecutius

    while (true) { // Bucle infinit per alternar entre els jugadors
        game.printBoard(); // Imprimeix el tauler
        int x, y; // Variables per les coordenades de la jugada
        std::string input; // Variable per llegir l'entrada de l'usuari

        // Demana la jugada al jugador actual
        std::cout << "Jugador " << (currentPlayer == Stone::BLACK ? "Negre" : "Blanc") << ", introdueix la teva jugada (x y) o 'p' per passar el torn, 'q' per rendir-te: ";
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
        } else if (input == "q") {
            std::cout << "Jugador " << (currentPlayer == Stone::BLACK ? "Negre" : "Blanc") << " s'ha rendit. Fi de la partida!" << std::endl;
            break;
        } else {
            std::cin >> y; // Llegeix la segona part de l'entrada (coordenada y)
            x = std::stoi(input); // Converteix la primera part de l'entrada (coordenada x) a enter

            if (game.placeStone(x, y, currentPlayer)) { // Si el moviment és vàlid
                if (game.isKo()) {
                    std::cout << "Moviment no permès (regla de Ko). Torna a intentar-ho.\n";
                    game.passTurn(); // Restableix l'estat del tauler anterior
                } else {
                    currentPlayer = (currentPlayer == Stone::BLACK) ? Stone::WHITE : Stone::BLACK; // Canvia el torn
                    passCount = 0; // Reseteja el comptador de torns passats
                }
            } else {
                std::cout << "Moviment invàlid. Torna a intentar-ho.\n"; // Moviment invàlid
            }
        }
    }

    return 0; // Retorna 0 per indicar que el programa ha acabat correctament
}

