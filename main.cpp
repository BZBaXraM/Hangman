#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

enum class MENU {
    PLAY = 1, EXIT, TELEGRAM
};

class Hangman {
private:
    time_t start = 0, end = 0;
    int lives = 20; // Жизнь игрока (можете написать сколько угодно)
    char answer = 0;
    string hiddenWord;
    int succesCounter = 0;
    int counter = 0;
    string randomWord;
    string word; // Слово
    int currentLives = 0; // текущая жизнь
    char massSymbol[12]{}; // Считваем буквы в конце игры


    inline bool ifCorrect() const {
        for (int i = 0; i < word.length(); i++) {
            if (word[i] == answer)
                return true;
        }
        return false;
    }


public:

    Hangman(int lives, char answer, string &hiddenWord, int succesCounter, int counter, string &randomWord,
            string &word, int currentLives) : lives(lives), answer(answer), randomWord(randomWord), counter(counter),
                                              hiddenWord(hiddenWord), succesCounter(succesCounter),
                                              word(word), currentLives(currentLives) {}


    void setAnswer();

    void showHiddenWord();

    bool isWordCorrect() const;

    int getLives() const { return lives; }

    bool checkChar(char c);

    void decrypt(const string &inFileName, const string &outFileName, int offset);

    void status();// Вывод всего в конце игры

    Hangman();
};

void Hangman::setAnswer() {
    cout << "Enter the letter" << std::endl;
    cout << ">>> ";
    cin >> answer;
    massSymbol[currentLives] = answer;
    currentLives++;
    for (int i = 0; i < word.length(); i++) {
        if (word[i] == answer) {
            hiddenWord[i] = answer;
            succesCounter++;
        }
    }
    if (succesCounter > 0) {
        system("clear"); // Замените на system("cls")
        cout << "Correct letter [+]!" << endl;
        cout << "Current word: ";
        showHiddenWord();
        cout << endl;
        succesCounter = 0;

        cout << "Count of your lives: " << lives << endl;
    } else {
        system("clear"); // Замените на system("cls")
        cout << "Incorrect letter [-]!" << endl;
        for (int i = 0; i < currentLives; i++)
            cout << massSymbol[i] << " ";
        cout << endl;
        cout << "Current word: ";
        showHiddenWord();
        cout << endl;
        lives--;

        cout << "Count of your lives: " << lives << endl;

        if (lives <= 0) {
            cout << "You died!" << endl;
            status();
            exit(1);
        }
    }
}

void Hangman::showHiddenWord() {
    if (!(counter > 0)) {
        for (int i = 0; i < word.length(); i++) {
            hiddenWord.insert(hiddenWord.begin(), '*');
        }
    }
    cout << hiddenWord << endl;
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

void Hangman::decrypt(const string &inFileName, const string &outFileName, int offset) {
    ifstream inFile(inFileName.c_str());

    //Считываем код
    vector<string> code;
    while (true) {
        string str;

        inFile >> str;

        if (inFile.eof()) {
            break;
        }
        code.push_back(str);
    }

    inFile.close();

    vector<string> text;

    for (auto &i: code) {
        string textStr;

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
    ofstream outFile(outFileName.c_str());

    for (auto &i: text) {
        outFile << i << std::endl;
    }
    outFile.close();

    return;
}

void Hangman::status() {
    time(&end);
    double seconds = difftime(end, start);
    int minutes = 0;
    minutes = seconds / 60;
    seconds = seconds - (minutes * 60);
    cout << "You time: " << minutes << " minutes " << seconds << " seconds" << endl;
    cout << "Your attempts: " << currentLives << endl;
    cout << "Rand word is: " << word << endl;
    cout << "Your attempts: " << endl;
    for (int i = 0; i < currentLives; i++)
        cout << i + 1 << " - " << massSymbol[i] << endl;
}

Hangman::Hangman() {
    time(&start);
    char ch;
    while (randomWord.empty()) {

        decrypt("enC.txt", "Word",
                22); /* Логика моей шифровки такого: На вход мы передаем тот txt файл со словами в шифровынным виде и, если комп прочитает его, то там будут непонятные вещи. (Игра без этого файла не запустится; скиньте этот файл (enC.txt) в папку проекта Visual Studio. А на вывод мы создаем файл (в моем слуаче это Word или Word.txt, и отдельно добавлю, что этот файл создается при запуске программы) и дальше функция расшифрует слова из enC.txt и записывает в Word! */
        ifstream inS("Word", ios::in); // Считываем созданный файл…
        while (inS.get(ch)) {
            if (ch != '\n') {
                randomWord += ch;
            } else if ((rand() * 10) % randomWord.size()) {
                word = randomWord;
                inS.close();
                break;
            } else {
                randomWord.clear();
            }
        }
    }
}

int main() {
    srand(time(nullptr));

    cout << "\t\tHangman by Bahram Bayramzade \n";
    string str;
    str = "https://t.me/baxram97";

    int menu;
    cout <<
         "1. Play game\n"
         "2. Exit from game\n"
         "3. Get my Telegram \n"
         "Your choice: " << endl;
    cin >> menu;
    unique_ptr<Hangman> hangman = make_unique<Hangman>();
    hangman->showHiddenWord();
    while (!hangman->isWordCorrect()) {
        switch (menu) {
            case static_cast<int>(MENU::PLAY):
                hangman->setAnswer();
                break;
            case static_cast<int>(MENU::EXIT):
                exit(0);
            case static_cast<int>(MENU::TELEGRAM):
                cout << str << endl;
                exit(0);
            default:
                cout << "Error type! " << endl;
                break;
        }
    }
    cout << "You win!" << endl;
    hangman->status();

    return 0;
}
