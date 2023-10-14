#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include "quiz_model.h"

int get_randint(int max) {
    srand (time(NULL));
    return rand() % max;
}


Word::Word() {}
Word::Word(std::string keyword)
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
bool Word::check_letter(char letter)
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
    return guess;
}

// возвращает угаданнаяю часть слова
std::string Word::get_current()
{
    std::string arr;
    for (int i=0; i<size; i++) {
        arr.push_back(current_word[i]);
    }
    return arr;
}

// проверяет угаданы ли все буквы в слове
bool Word::check_success()
{
    return xletters == 0;
}


Game::Game() {
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

void Game::start_game()
{
    // создаем объект Word с рандомным словом из словаря
    word = Word(wordlist[get_randint(wordlist.size())]);
}

bool Game::guess_letter(char letter)
{
    bool guess = word.check_letter(letter);
    return guess;
}

bool Game::check_success()
{
    return word.check_success();
}