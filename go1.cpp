#include <iostream> // Llibreria per utilitzar les funcions d'entrada i sortida
#include <vector>   // Llibreria per utilitzar el contenidor vector

// Enumeració per representar l'estat de cada cel·la del tauler
enum class Stone { EMPTY, BLACK, WHITE };

// Classe que representa el tauler de Go
class GoBoard {
public:
    // Constructor que inicialitza el tauler amb el tamany especificat i totes les cel·les buides
    GoBoard(int size) : board(size, std::vector<Stone>(size, Stone::EMPTY)), board_size(size) {}

    // Funció per col·locar una pedra en una posició especificada
    bool placeStone(int x, int y, Stone stone) {
        // Verifica si la posició és dins del tauler i si la cel·la està buida
        if (x < 0 || x >= board_size || y < 0 || y >= board_size || board[x][y] != Stone::EMPTY) {
            return false; // Retorna fals si el moviment no és vàlid
        }
        board[x][y] = stone; // Col·loca la pedra en la posició especificada
        return true; // Retorna cert si el moviment és vàlid
    }

    // Funció per imprimir el tauler
    void printBoard() {
        // Itera per cada fila del tauler
        for (int i = 0; i < board_size; ++i) {
            // Itera per cada columna del tauler
            for (int j = 0; j < board_size; ++j) {
                char c = '.'; // Caràcter per representar una cel·la buida
                if (board[i][j] == Stone::BLACK) c = 'B'; // Caràcter per una pedra negra
                else if (board[i][j] == Stone::WHITE) c = 'W'; // Caràcter per una pedra blanca
                std::cout << c << ' '; // Imprimeix el caràcter seguit d'un espai
            }
            std::cout << std::endl; // Canvia de línia després de cada fila
        }
    }

private:
    int board_size; // Tamany del tauler
    std::vector<std::vector<Stone>> board; // Matriu bidimensional que representa el tauler
};

int main() {
    int board_size = 9; // Tamany del tauler (pot canviar a 19 per un tauler més gran)
    GoBoard game(board_size); // Crea un objecte GoBoard amb el tamany especificat
    Stone currentPlayer = Stone::BLACK; // Inicialitza el jugador actual com a negre

    while (true) { // Bucle infinit per alternar entre els jugadors
        game.printBoard(); // Imprimeix el tauler
        int x, y; // Variables per les coordenades de la jugada
        // Demana la jugada al jugador actual
        std::cout << "Jugador " << (currentPlayer == Stone::BLACK ? "Negre" : "Blanc") << ", introdueix la teva jugada (x y): ";
        std::cin >> x >> y; // Llegeix les coordenades de la jugada

        if (game.placeStone(x, y, currentPlayer)) { // Si el moviment és vàlid
            currentPlayer = (currentPlayer == Stone::BLACK) ? Stone::WHITE : Stone::BLACK; // Canvia el torn
        } else {
            std::cout << "Moviment invàlid. Torna a intentar-ho.\n"; // Moviment invàlid
        }
    }

    return 0; // Retorna 0 per indicar que el programa ha acabat correctament
}
