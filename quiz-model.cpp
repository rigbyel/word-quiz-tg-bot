#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

int get_randint(int max) {
    srand (time(NULL));
    return rand() % max;
}

class Word
{
private:
    std::string key;
    int size;
    int xletters;   // кол-во неизвестных пользователю букв
    char *current_word = new char[size];

public:
    Word() {}
    Word(std::string keyword)
    {
        key = keyword;
        size = key.size();
        xletters = size;
        for (int i = 0; i < size; i++)
        {
            current_word[i] = 45;   // 45 соответствует символу "-" в ASCII
        }

    }

    // проверяет есть ли буква в загаданном слове
    bool check_letter(char letter)
    {
        bool guess = false;
        for (int i = 0; i < key.size(); i++)
        {
            if ((letter == key[i]) & (current_word[i] != letter))
            {
                current_word[i] = letter;
                guess = true;
                xletters -= 1;
            }
        }
        print_word();
        if (guess)
        {
            std::cout << "Угадали." << std::endl;
        }
        else 
        {
            std::cout << "Нет такой буквы." << std::endl;
        }
        return guess;
    }

    void print_word()
    {
        for (int i = 0; i < size; i++) 
        {
            std::cout << current_word[i];
        }
        std::cout << std::endl;
    }

    // проверяет угаданы ли все буквы в слове
    bool check_success()
    {
        return xletters == 0;
    }
};


class GameService
{
private:
    Word word;
public:
    GameService() {}
    GameService(Word keyword)
    {
        word = keyword;
    }
    
    void guess_letter(char letter)
    {
        word.check_letter(letter);
    }

    bool check_success()
    {
        return word.check_success();
    }
};


class Game
{
private:
    std::vector<std::string> wordlist;
    Word word;
    GameService gameService;
public:
    Game() {
        // создаем словарь, читая слова из файла "dictionary.txt"
        std::ifstream in("dictionary.txt"); // окрываем файл для чтения
        if (in.is_open())
        {
            std::string new_word;
            while (in >> new_word)
            {
                wordlist.push_back(new_word);
            }
        }
        in.close();

    }

    void start_game()
    {
        // создаем объект Word с рандомным словом из словаря
        word = Word(wordlist[get_randint(wordlist.size())]);
        gameService = GameService(word);
    }

    void guess_letter(char letter)
    {
        word.check_letter(letter);
    }

    bool check_success()
    {
        return word.check_success();
    }
};

Word get_randword() {
    std::vector<std::string> wordlist;
    std::ifstream in("dictionary.txt"); // окрываем файл для чтения
        if (in.is_open())
        {
            std::string new_word;
            while (in >> new_word)
            {
                wordlist.push_back(new_word);
            }
        }
    in.close();
    int size = wordlist.size();
    return Word(wordlist[get_randint(size)]);
}

void test_word() {
    Word word;
    word = get_randword();
    char s;
    while (word.check_success() != 1)
    {
        std::cout << "Ведите букву: ";
        std::cin >> s;
        word.check_letter(s);
    }
    std::cout << "Вы угадали слово — ";
}

void test_gameservice() {
    Word word = get_randword();
    GameService gameService;
    gameService = GameService(word);

    char s;
    while (gameService.check_success() != 1)
    {
        std::cout << "Ведите букву: ";
        std::cin >> s;
        gameService.guess_letter(s);
    }
    std::cout << "Вы угадали слово — ";

}

void test_game() {
    Game game;
    game.start_game();
    char s;
    while (game.check_success() != 1)
    {
        std::cout << "Ведите букву: ";
        std::cin >> s;
        game.guess_letter(s);
    }
    std::cout << "Вы угадали слово — ";

}

int main() {
    test_game();
    return 0;
}