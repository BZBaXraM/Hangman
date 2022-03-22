#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

enum class MENU {
    PLAY = 1, EXIT, TELEGRAM
};

class Hangman {
private:
    int lives = 20; // Жизнь игрока (можете написать сколько угодно)
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
    std::string word; // Слово
    int currentLives = 0; // текущая жизнь
    char massSymbol[12]{}; // Считваем буквы в конце игры

    Hangman() {
        std::string randomWord;
        char ch;
        while (randomWord.empty()) {
            decrypt("enC.bin", "Word.bin",
                    20); /* Логика моей шифровки такого: На вход мы передаем тот бинарный файл со словами в шифровынным виде (копм никак не откроет этот файл и не прочитает - даже если прочитает, то там будут непонятные вещи…) А на вывод мы создаем файл (в моем слуаче это Word.bin, и отдельно добавлю, что этот создается при запуске программы) и дальше функция расшифрует слова из enC.bin и записывает в Word.bin! */
            std::ifstream inS("Word.bin", std::ios::in | std::ios::binary); // Считываем созданный файл…
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

    bool checkChar(char c);

    void decrypt(const std::string &inFileName, const std::string &outFileName, int offset);

    void status() {
        clock_t t, t1;
        t = clock();
        std::cout << "Your attempts: " << currentLives << std::endl;
        std::cout << "You time: " << (t1 - t) / CLK_TCK << std::endl;
        std::cout << "Rand word is: " << word << std::endl;
        std::cout << "Your attempts: " << std::endl;
        for (int i = 0; i < currentLives; i++)
            std::cout << i + 1 << " - " << massSymbol[i] << std::endl;
    } // Вывод всего в конце игры
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
        system("clear"); // cls
        std::cout << "Current word: ";
        showHiddenWord();
        std::cout << std::endl;
        succesCounter = 0;

        std::cout << "Count of your lives: " << lives << std::endl;
    } else {
        std::cout << "Incorrect letter [-]!" << std::endl;
        std::cin.ignore(); // pause
//        system("clear");
        std::cout << "Current word: ";
        showHiddenWord();
        std::cout << std::endl;
        lives--;

        std::cout << "Count of your lives: " << lives << std::endl;

        if (lives <= 0) {
            std::cout << "You died!" << std::endl;
            status();
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

bool Hangman::checkChar(char c) {
    if (c == ' ' || c == '.' || c == ',' ||
        c == '!' || c == '?' || c == ':' ||
        c == ';') {
        return true;
    } else {
        return false;
    }
}

void Hangman::decrypt(const std::string &inFileName, const std::string &outFileName, int offset) {
    std::ifstream inFile(inFileName.c_str());

    //Считываем код
    std::vector<std::string> code;
    while (true) {
        std::string str;

        inFile >> str;

        if (inFile.eof()) {
            break;
        }

        code.push_back(str);
    }

    inFile.close();

    //Дешифровка
    std::vector<std::string> text;

    for (auto &i: code) {
        std::string textStr;

        for (char j: i) {
            if (checkChar(j)) {
                textStr.push_back(j);
            } else {
                unsigned char symb = j - offset;

                //Если вылезли за таблицу
                if (symb < 'a') {
                    symb = 'z' - ('a' - symb) + 1;

                    textStr.push_back(symb);
                } else if (symb > 'z') {
                    symb = 'a' + (symb - 'z') - 1;

                    textStr.push_back(symb);
                } else {
                    textStr.push_back(symb);
                }
            }
        }

        text.push_back(textStr);
    }

    //Записываем в файл
    std::ofstream outFile(outFileName.c_str());

    for (unsigned int i = 0; i < text.size(); i++) {
        outFile << text[i] << std::endl;
    }
    outFile.close();

    return;
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
    hangman->showHiddenWord();
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
    std::cout << "You win!" << std::endl;
    hangman->status();

    delete hangman;

    return 0;
}
