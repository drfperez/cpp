#include <iostream>
#include <limits>

using namespace std;

/* 
   Com jugar a "Tic-Tac-Toe" amb aquest programa:

   1. Compilar el codi:
      - Si utilitzes un compilador de C++ com `g++`, obre una terminal i introdueix la següent comanda:
        g++ -o tictactoe tictactoe.cpp

   2. Executar l'executable:
      - Un cop compilat el codi, executa l'executable generat:
        ./tictactoe

   3. Selecció del Marcador:
      - El joc començarà demanant al jugador 1 que esculli el seu marcador ('X' o 'O'). Introdueix el caràcter i prem Enter.
        Exemple:
        Player one, choose your marker: X

   4. Torn del Jugador:
      - Els jugadors es tornaran per introduir el número de la casella on volen col·locar el seu marcador. 
        El tauler té els números de les caselles de l'1 al 9 disposats de la següent manera:
          1 | 2 | 3
         ---|---|---
          4 | 5 | 6
         ---|---|---
          7 | 8 | 9

   5. Introducció de la Casella:
      - Durant el seu torn, cada jugador haurà d'introduir el número de la casella on vol col·locar el seu marcador. 
        Exemple:
        It's player 1's turn. Enter your slot: 5

   6. Actualització del Tauler:
      - Després de cada moviment, el tauler es redibuixarà per mostrar les posicions actualitzades dels marcadors.

   7. Verificació del Guanyador:
      - Després de cada moviment, el joc verificarà si hi ha un guanyador. Si un jugador aconsegueix alinear tres marcadors consecutius 
        (horitzontalment, verticalment o diagonalment), el joc anunciarà el guanyador.
        Exemple:
        Player one won! Congratulations!

   8. Empat:
      - Si el tauler s'omple sense que cap jugador aconsegueixi alinear tres marcadors consecutius, el joc declararà un empat.
        Exemple:
        That is a tie game!
*/

char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
char current_marker;
int current_player;

// Aquesta funció neteja la pantalla de la consola
void clearScreen() {
    cout << "\033[2J\033[1;1H"; // Seqüència d'escapament ANSI per netejar la pantalla
}

// Aquesta funció dibuixa el tauler a la consola
void drawBoard() {
    cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
    cout << "---|---|---" << endl;
    cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
    cout << "---|---|---" << endl;
    cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl;
}

// Aquesta funció col·loca el marcador del jugador actual en la posició escollida
bool placeMarker(int slot) {
    int row = slot / 3;
    int col;

    if(slot % 3 == 0) {
        row = row - 1;
        col = 2;
    } else {
        col = (slot % 3) - 1;
    }

    if(board[row][col] != 'X' && board[row][col] != 'O') {
        board[row][col] = current_marker;
        return true;
    } else {
        return false;
    }
}

// Aquesta funció comprova si hi ha un guanyador
int winner() {
    for(int i = 0; i < 3; i++) {
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2]) return current_player;
    }
    for(int i = 0; i < 3; i++) {
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i]) return current_player;
    }
    if(board[0][0] == board[1][1] && board[1][1] == board[2][2]) return current_player;
    if(board[0][2] == board[1][1] && board[1][1] == board[2][0]) return current_player;

    return 0;
}

// Aquesta funció canvia el torn entre els jugadors
void swapPlayerAndMarker() {
    if(current_marker == 'X') current_marker = 'O';
    else current_marker = 'X';

    if(current_player == 1) current_player = 2;
    else current_player = 1;
}

// Aquesta funció controla el flux principal del joc
void game() {
    clearScreen(); // Assegura que la pantalla estigui neta al començar el joc
    cout << "Player one, choose your marker: ";
    char marker_p1;
    cin >> marker_p1;

    current_player = 1;
    current_marker = marker_p1;

    int player_won = 0;

    for(int i = 0; i < 9; i++) {
        clearScreen(); // Neteja la pantalla abans de redibuixar el tauler
        drawBoard();
        cout << "It's player " << current_player << "'s turn. Enter your slot: ";
        int slot;
        cin >> slot;

        if(cin.fail() || slot < 1 || slot > 9) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "That slot is invalid! Try another slot!" << endl;
            i--;
            continue;
        }

        if(!placeMarker(slot)) {
            cout << "That slot is occupied! Try another slot!" << endl;
            i--;
            continue;
        }

        player_won = winner();

        if(player_won == 1) {
            clearScreen();
            drawBoard();
            cout << "Player one won! Congratulations!" << endl;
            break;
        }
        if(player_won == 2) {
            clearScreen();
            drawBoard();
            cout << "Player two won! Congratulations!" << endl;
            break;
        }

        swapPlayerAndMarker();
    }

    if(player_won == 0) {
        clearScreen();
        drawBoard();
        cout << "That is a tie game!" << endl;
    }
}

int main() {
    game();
    return 0;
}

