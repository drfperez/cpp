#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

#define LEN 9 // LEN representa la mida del nostre sudoku

// Funció per comprovar si un número és possible en una posició específica del tauler
bool is_possible(std::vector<std::vector<int>>& board, int row, int col, int val) {
    // Comprova si el valor és possible a la columna
    for (int i = 0; i < LEN; i++) {
        if (board[i][col] == val) return false;
        // Comprova si el valor és possible a la fila
        if (board[row][i] == val) return false;
        // Comprova si el valor és possible al subgrup de 3x3
        if (board[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == val) return false;
    }
    return true; // Si passa totes les comprovacions, retorna true
}

// Funció per resoldre el sudoku utilitzant backtracking
bool solve(std::vector<std::vector<int>>& board, int row, int col) {
    if (col == LEN) { // Si arribem al final de la columna
        col = 0; // Passa a la primera columna
        row++; // i a la següent fila
        if (row == LEN) return true; // Si arribem al final del tauler, està resolt
    }

    if (board[row][col] != 0) return solve(board, row, col + 1); // Si la posició no està buida, passa a la següent

    for (int val = 1; val <= LEN; val++) { // Prova tots els valors del 1 al 9
        if (is_possible(board, row, col, val)) { // Si el valor és possible
            board[row][col] = val; // Assigna el valor
            if (solve(board, row, col + 1)) return true; // Si es pot resoldre amb aquest valor, retorna true
            board[row][col] = 0; // Si no, desassigna el valor i prova amb un altre
        }
    }
    return false; // Si no es pot resoldre, retorna false
}

// Funció per imprimir el tauler del sudoku
void print_board(const std::vector<std::vector<int>>& board) {
    for (int row = 0; row < LEN; ++row) {
        for (int col = 0; col < LEN; ++col) {
            std::cout << board[row][col] << " "; // Imprimeix el valor
            if ((col + 1) % 3 == 0 && col < LEN - 1) std::cout << "| "; // Afegeix un separador després de cada subgrup de 3x3
        }
        std::cout << std::endl;
        if ((row + 1) % 3 == 0 && row < LEN - 1) std::cout << "------+-------+------" << std::endl; // Afegeix una línia separadora després de cada subgrup de 3x3
    }
}

// Funció per eliminar números del tauler per generar un sudoku incomplet
void remove_numbers(std::vector<std::vector<int>>& board, int num_to_remove) {
    std::srand(std::time(0)); // Inicialitza el generador de nombres aleatoris
    while (num_to_remove > 0) { // Elimina tants números com s'especifiqui
        int row = std::rand() % LEN;
        int col = std::rand() % LEN;
        if (board[row][col] != 0) { // Si la posició no està buida
            board[row][col] = 0; // Elimina el número
            num_to_remove--;
        }
    }
}

// Funció per barrejar el tauler resolt per obtenir una configuració única
void shuffle_board(std::vector<std::vector<int>>& board) {
    std::vector<int> nums(LEN);
    std::iota(nums.begin(), nums.end(), 1); // Crea una llista de números de l'1 al 9
    std::shuffle(nums.begin(), nums.end(), std::default_random_engine(std::time(0))); // Barreja la llista

    std::vector<std::vector<int>> temp(LEN, std::vector<int>(LEN, 0)); // Crea un tauler temporal
    for (int row = 0; row < LEN; ++row) {
        for (int col = 0; col < LEN; ++col) {
            temp[row][col] = board[row][col] ? nums[board[row][col] - 1] : 0; // Assigna els números barrejats al tauler temporal
        }
    }
    board = temp; // Assigna el tauler temporal al tauler original
}

// Funció per generar un sudoku
void generate_sudoku(std::vector<std::vector<int>>& board) {
    solve(board, 0, 0); // Genera un tauler resolt
    shuffle_board(board); // Barreja el tauler resolt
    remove_numbers(board, 40); // Elimina números per crear el sudoku incomplet (ajusta el nombre per la dificultat)
}

// Funció principal
int main() {
    std::vector<std::vector<int>> sudokuBoard = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    generate_sudoku(sudokuBoard); // Genera un sudoku

    std::cout << "Generated Sudoku Grid:" << std::endl;
    print_board(sudokuBoard); // Imprimeix el sudoku generat

    if (solve(sudokuBoard, 0, 0)) { // Intenta resoldre el sudoku
        std::cout << "\nSolved Sudoku:" << std::endl;
        print_board(sudokuBoard); // Imprimeix el sudoku resolt
    } else {
        std::cout << "\nNo solution exists." << std::endl; // Si no es pot resoldre, imprimeix un missatge
    }

    return 0; // Finalitza el programa
}
