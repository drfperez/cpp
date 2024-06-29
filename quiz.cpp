#include <iostream> // Biblioteca per a la manipulació d'entrada i sortida estàndard
#include <vector> // Biblioteca per a utilitzar vectors, que són contenidors dinàmics

// Definim una estructura per emmagatzemar les preguntes del joc
struct Question {
    std::string text; // Emmagatzema el text de la pregunta
    std::vector<std::string> options; // Vector que emmagatzema les opcions de resposta
    int correctAnswer; // Índex de la resposta correcta (basat en 1)
};

// Funció que mostra la pregunta i verifica la resposta de l'usuari
void askQuestion(const Question& q) {
    std::cout << q.text << std::endl; // Mostra el text de la pregunta
    for (int i = 0; i < q.options.size(); ++i) { // Itera per cada opció disponible
        std::cout << i + 1 << ". " << q.options[i] << std::endl; // Mostra cada opció numerada
    }
    int answer; // Variable per capturar la resposta de l'usuari
    std::cin >> answer; // Llegeix la resposta de l'usuari
    if (answer == q.correctAnswer) { // Comprova si la resposta és correcta
        std::cout << "Correcte!" << std::endl; // Missatge de resposta correcta
    } else {
        std::cout << "Incorrecte. La resposta correcta era: " << q.correctAnswer << std::endl; // Missatge de resposta incorrecta
    }
}

int main() {
    // Creem un vector de preguntes, on cada pregunta és una instància de Question
    std::vector<Question> questions = {
        // Categoria: Sintaxi
        {"Què imprimeix el següent codi: std::cout << \"Hola, món!\";", {"Hola, món!", "Hello, World!", "Error"}, 1},
        {"Quina és la sortida de 2 + 2 * 2?", {"6", "8", "10"}, 1},

        // Categoria: Estructures de control
        {"Què fa el següent bucle: for(int i = 0; i < 5; ++i) { std::cout << i; }", {"01234", "12345", "54321"}, 1},
        {"Quin és el resultat de: if(3 > 2) { std::cout << \"Sí\"; } else { std::cout << \"No\"; }", {"Sí", "No", "Error"}, 1},

        // Categoria: Funcions
        {"Què retorna aquesta funció: int sum(int a, int b) { return a + b; } sum(2, 3);", {"5", "2", "3"}, 1},
        {"Com es declara una funció que no retorna res?", {"void func();", "int func();", "return void();"}, 1},

        // Categoria: Classes i objectes
        {"Què és una classe en C++?", {"Un tipus de dades definit per l'usuari", "Una funció", "Una variable"}, 1},
        {"Com es crea un objecte d'una classe anomenada 'Cotxe'?", {"Cotxe meuCotxe;", "Cotxe = nouCotxe;", "crea Cotxe;"}, 1},

        // Categoria: Gestió de memòria
        {"Quina paraula clau s'utilitza per alliberar memòria dinàmica?", {"delete", "free", "remove"}, 1},
        {"Què fa la paraula clau 'new'?", {"Reserva memòria dinàmica", "Allibera memòria", "Inicialitza una variable"}, 1}
    };

    for (const auto& q : questions) { // Itera per cada pregunta del vector
        askQuestion(q); // Crida la funció per mostrar la pregunta i verificar la resposta
    }

    return 0; // Retorna 0 per indicar que el programa ha finalitzat correctament
}
/*
#include <iostream>: Utilitzada per a la manipulació de l'entrada i sortida estàndard, com std::cout i std::cin.
#include <vector>: Permet utilitzar vectors, que són contenidors dinàmics que poden redimensionar-se automàticament i són útils per emmagatzemar elements com les opcions de resposta.
Estructura Question:
std::string text: Guarda el text de la pregunta.
std::vector<std::string> options: Emmagatzema múltiples opcions de resposta en un vector, que és fàcilment escalable i permet accedir a les opcions per índex.
int correctAnswer: Guarda l'índex de la resposta correcta.
*/

