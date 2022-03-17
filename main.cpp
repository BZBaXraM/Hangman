#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

enum class MENU {
    PLAY = 1, EXIT, TELEGRAM
};

class Hangman {
private:
    int lives = 5; // Жизнь игрока (можете написать сколько угодно)
    char answer = 0;

    std::string hiddenWord;
    int succesCounter = 0;
    int counter = 0;


    inline bool ifCorrect() const {
        for (int i = 0; i < word.length(); i++) {
            if (word[i] == answer)
                return true;
        }
        return false;
    }


public:
    std::string word;
    int currentLives = 0;
    char massSymbol[12]{};

    Hangman() {
        std::string randomWord;
        char ch;
        while (randomWord.empty()) {
            std::ifstream inS("Word", std::ios::in | std::ios::binary);
            while (inS.get(ch)) {
                if (ch != '\n') {
                    randomWord += ch;
                } else if ((rand() * 10) % 3 == 0) {
                    word = randomWord;
                    inS.close();
                    break;
                } else {
                    randomWord.clear();
                }
            }
        }
    }

    ~Hangman();

    void setAnswer();

    void showHiddenWord();

    bool isWordCorrect() const;

    int getLifes() const { return lives; }

};


Hangman::~Hangman() {
    std::cout << "Game destructor is called! " << std::endl;
}

void Hangman::setAnswer() {
    std::cout << "Enter the letter" << std::endl;
    std::cout << ">>> ";
    std::cin >> answer;
    massSymbol[currentLives] = answer;
    currentLives++;
    for (int i = 0; i < word.length(); i++) {
        if (word[i] == answer) {
            hiddenWord[i] = answer;
            succesCounter++;
        }
    }

    if (succesCounter > 0) {
        std::cout << "Correct letter [+]!" << std::endl;
        system("clear");
        std::cout << "Current word: ";
        showHiddenWord();
        std::cout << std::endl;
        succesCounter = 0;

        std::cout << "Count of your lives: " << lives << std::endl;
    } else {
        std::cout << "Incorrect letter [-]!" << std::endl;
        std::cin.ignore();
        system("clear");
        std::cout << "Current word: ";
        showHiddenWord();
        std::cout << std::endl;
        lives--;

        std::cout << "Count of your lives: " << lives << std::endl;

        if (lives <= 0) {
            std::cout << "You died!" << std::endl;
            std::exit(1);
        }
    }
}

void Hangman::showHiddenWord() {
    if (!(counter > 0)) {
        for (int i = 0; i < word.length(); i++) {
            hiddenWord.insert(hiddenWord.begin(), '*');
        }
    }
    std::cout << hiddenWord << std::endl;
    counter++;
}

bool Hangman::isWordCorrect() const {
    if (hiddenWord == word)
        return true;
    else
        return false;
}

int main() {
    srand(time(nullptr));

    std::cout << "\t\tHangman by Bahram Bayramzade \n";
    std::string str;
    str = "https://t.me/baxram97";

    int menu;
    std::cout <<
              "1. Play game\n"
              "2. Exit from game\n"
              "3. Get my Telegram \n"
              "Your choice: " << std::endl;
    std::cin >> menu;
    Hangman *hangman = new Hangman;
    clock_t t, t1;
    hangman->showHiddenWord();
    t = clock();
    while (!hangman->isWordCorrect()) {
        switch (menu) {
            case static_cast<int>(MENU::PLAY):
                hangman->setAnswer();
                break;
            case static_cast<int>(MENU::EXIT):
                exit(0);
            case static_cast<int>(MENU::TELEGRAM):
                std::cout << str << std::endl;
                exit(0);
            default:
                std::cout << "Error type! " << std::endl;
                break;
        }
    }
    t1 = clock();
    std::cout << "You win!" << std::endl;
    std::cout << "Your attempts: " << hangman->currentLives << std::endl;
    std::cout << "You time: " << (t1 - t) / CLK_TCK << std::endl;
    std::cout << "Rand word is: " << hangman->word << std::endl;
    std::cout << "Your attempts: " << std::endl;
    for (int i = 0; i < hangman->currentLives; i++)
        std::cout << i + 1 << " - " << hangman->massSymbol[i] << std::endl;
    delete hangman;

    return 0;
}
