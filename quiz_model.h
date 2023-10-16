#ifndef QUIZ_MODEL_H
#define QUIZ_MODEL_H

#include <vector>
#include <string>

int get_randint(int max);

class Word
{
private:
    std::string key;
    int size;
    int xletters;   // кол-во неизвестных пользователю букв
    std::vector<char> current_word;
public:
    Word();
    Word(std::string keyword);

    bool check_letter(char letter);
    void print_word();
    bool check_success();
    std::string get_current();

};

class Game
{
private:
    std::vector<std::string> wordlist;
    bool gameover;
public:
    Game();
    void start_game();
    bool guess_letter(char letter);
    bool check_success();
    void end_game();
    bool isover();
    Word word;
};

#endif