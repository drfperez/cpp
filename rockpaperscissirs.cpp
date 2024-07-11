#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

// Function to generate a random choice for the machine
string getMachineChoice() {
    int random = rand() % 3;
    switch (random) {
        case 0:
            return "Rock";
        case 1:
            return "Paper";
        case 2:
            return "Scissors";
        default:
            return "Invalid";
    }
}

// Function to determine the winner
string getWinner(string playerChoice, string machineChoice) {
    if (playerChoice == machineChoice) {
        return "Tie";
    } else if ((playerChoice == "Rock" && machineChoice == "Scissors") ||
               (playerChoice == "Paper" && machineChoice == "Rock") ||
               (playerChoice == "Scissors" && machineChoice == "Paper")) {
        return "Player";
    } else {
        return "Machine";
    }
}

int main() {
    srand(time(0)); // Seed the random number generator

    while (true) {
        // Get the player's choice
        string playerChoice;
        cout << "Enter your choice (Rock, Paper, or Scissors): ";
        cin >> playerChoice;

        // Convert the player's choice to uppercase
        for (int i = 0; i < playerChoice.length(); i++) {
            playerChoice[i] = toupper(playerChoice[i]);
        }

        // Get the machine's choice
        string machineChoice = getMachineChoice();

        // Display the choices
        cout << "Player: " << playerChoice << endl;
        cout << "Machine: " << machineChoice << endl;

        // Determine the winner
        string winner = getWinner(playerChoice, machineChoice);
        cout << "Winner: " << winner << endl;

        // Ask the player if they want to play again
        char playAgain;
        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;

        if (playAgain == 'n') {
            break;
        }
    }

    return 0;
}
