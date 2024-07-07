#include <iostream> // Incloem la llibreria iostream per poder utilitzar funcions d'entrada i sortida com cout i cin

using namespace std; // Utilitzem l'espai de noms estàndard per evitar escriure std:: abans de cout i cin

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

// Definim el tauler de joc com una matriu de 3x3 de caràcters
char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
char current_marker; // Aquest serà el marcador ('X' o 'O') del jugador actual
int current_player;  // Aquest serà el número del jugador actual (1 o 2)

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
    // Convertim el número de la posició a les coordenades del tauler
    int row = slot / 3; 
    int col;

    if(slot % 3 == 0) {
        row = row - 1;
        col = 2;
    } else {
        col = (slot % 3) - 1;
    }

    // Verifiquem si la posició està lliure (no hi ha un 'X' ni un 'O')
    if(board[row][col] != 'X' && board[row][col] != 'O') {
        board[row][col] = current_marker; // Col·loquem el marcador
        return true;
    } else {
        return false; // La posició està ocupada
    }
}

// Aquesta funció comprova si hi ha un guanyador
int winner() {
    // Comprovem les files
    for(int i = 0; i < 3; i++) {
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2]) return current_player;
    }
    // Comprovem les columnes
    for(int i = 0; i < 3; i++) {
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i]) return current_player;
    }
    // Comprovem les diagonals
    if(board[0][0] == board[1][1] && board[1][1] == board[2][2]) return current_player;
    if(board[0][2] == board[1][1] && board[1][1] == board[2][0]) return current_player;

    return 0; // No hi ha guanyador encara
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
    cout << "Player one, choose your marker: ";
    char marker_p1;
    cin >> marker_p1;

    current_player = 1;
    current_marker = marker_p1;

    drawBoard();

    int player_won = 0; // Inicialitzem la variable de guanyador

    // Bucle per alternar els torns fins a un màxim de 9 moviments
    for(int i = 0; i < 9; i++) {
        cout << "It's player " << current_player << "'s turn. Enter your slot: ";
        int slot;
        cin >> slot;

        // Verifiquem si la posició és vàlida
        if(slot < 1 || slot > 9) {
            cout << "That slot is invalid! Try another slot!" << endl;
            i--; // Repetim el torn
            continue;
        }

        // Intentem col·locar el marcador
        if(!placeMarker(slot)) {
            cout << "That slot is occupied! Try another slot!" << endl;
            i--; // Repetim el torn
            continue;
        }

        drawBoard(); // Dibuixem el tauler després de cada moviment

        player_won = winner(); // Comprovem si hi ha un guanyador

        // Si hi ha un guanyador, ho anunciem i sortim del bucle
        if(player_won == 1) {
            cout << "Player one won! Congratulations!" << endl;
            break;
        }
        if(player_won == 2) {
            cout << "Player two won! Congratulations!" << endl;
            break;
        }

        swapPlayerAndMarker(); // Canviem el torn de jugador
    }

    // Si no hi ha guanyador després de 9 moviments, és un empat
    if(player_won == 0) {
        cout << "That is a tie game!" << endl;
    }
}

// La funció main és el punt d'entrada del programa
int main() {
    game(); // Iniciem el joc
    return 0; // Retornem 0 per indicar que el programa ha acabat correctament
}

