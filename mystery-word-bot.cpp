#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include <map>
#include "quiz_model.h"

std::vector<std::string> bot_commands = {"start", "newgame", "hint"};
std::vector<std::string> succes_replies = {"Прекрасно! Эта буква в слове есть!",
                                        "Браво! С этой буквой вы на правильном пути!",
                                        "В точку! Есть такая буква!",
                                        "Великолепно! Буква на месте!"};    
std::vector<std::string> fail_replies = {"Эх, с этой буквой вам не повезло...",
                                        "Ой-ой, не попали. Но это же игра, давайте дальше...",
                                        "Ай-ай-ай, мимо...",
                                        "Увы! Такой буквы нет..."};

int len_success = succes_replies.size();
int len_fail = fail_replies.size();

int main() {
    TgBot::Bot bot("6382915085:AAEEgKpi3Z9AWtA_wdoEMgtr_oBnm08wHlE");
    std::map<int64_t, Game> users;

    bot.getEvents().onCommand("start", [&](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Привет!🤖\nЯ бот-загадчик слов \n\nВводите буквы, чтобы отгадать слово, которое я загадал.\n\nИспользуйте латинские буквы.\n\n✅ Чтобы получить подсказку введите /hint");
        users[message->chat->id] = Game();
        users[message->chat->id].start_game();
    });

    bot.getEvents().onCommand("newgame", [&](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Я загадал слово!🎯 \n\nПопробуйте угадать его, начав с любой буквы.");
        users[message->chat->id] = Game();
        users[message->chat->id].start_game();
    });

    bot.getEvents().onCommand("hint", [&](TgBot::Message::Ptr message) {
        if (!(users.count(message->chat->id))){
            bot.getApi().sendMessage(message->chat->id, "Для начала начните игру\n\n Чтобы это сделать, введите /newgame");
            return;
        }
        if (users[message->chat->id].open_letter()) {
            bot.getApi().sendMessage(message->chat->id, "Чувствую, вам нужен небольшой намек... Вот!");
            bot.getApi().sendMessage(message->chat->id, users[message->chat->id].word.get_current());
            if (users[message->chat->id].check_success()) {
                bot.getApi().sendMessage(message->chat->id, "Поздравляю! Вы отгадали слово.\n\nХотите начать новую игру?\nВведите /newgame");
                users[message->chat->id].end_game();
            }
        }
        else {
            bot.getApi().sendMessage(message->chat->id, "Подcказка?\nНо вы уже отгадали слово...\n\nЧтобы начать новую игру введите /newgame");
        }
    });

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        std::cout << "Username: " << message->chat->firstName << " Text: " << message->text<< std::endl;

        for (const auto& command : bot_commands) {
            if ("/" + command == message->text) {
                return;
            }
        }

        if (std::tolower(message->text[0]) < 97 | (std::tolower(message->text[0]) > 122))
        {
            bot.getApi().sendMessage(message->chat->id, "Используйте латинские буквы.");
            return;
        }

        if (message->text.size() > 1)
        {
            bot.getApi().sendMessage(message->chat->id, "У вас большие запросы...\nВведите одну букву.");
            return;
        }

        if (users[message->chat->id].isover()) {
            bot.getApi().sendMessage(message->chat->id, "Вы уже отгадали слово.\nЧтобы начать новую игру, введите /newgame");
            return;
        }

        char letter = message->text[0];
        bool guess = users[message->chat->id].guess_letter(letter);

        if (guess & !users[message->chat->id].check_success()) {
            bot.getApi().sendMessage(message->chat->id, succes_replies[get_randint(len_success)]);
        }
        if (!guess) {
            bot.getApi().sendMessage(message->chat->id, fail_replies[get_randint(len_fail)]);
        }
        bot.getApi().sendMessage(message->chat->id, users[message->chat->id].word.get_current());
        if (users[message->chat->id].check_success()) {
            bot.getApi().sendMessage(message->chat->id, "Поздравляю! Вы отгадали слово.\n\nХотите начать новую игру?\nВведите /newgame");
            users[message->chat->id].end_game();
        }
        
    });


    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}

/*
    COMPILATION COMMAND
    g++ quiz-model.cpp mystery-word-bot.cpp -o telegram_bot --std=c++14 -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread
*/ 