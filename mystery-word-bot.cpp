#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <string>
#include "quiz_model.h"

std::vector<std::string> bot_commands = {"start", "newgame"};

int main() {
    TgBot::Bot bot("6382915085:AAEEgKpi3Z9AWtA_wdoEMgtr_oBnm08wHlE");
    
    Game game;
    bool gameover = true;

    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Привет!🤖\nЯ бот-загадчик слов \n\nВводите буквы, чтобы отгадать слово, которое я загадал. \nДля старта введите /newgame ");
    });

    bot.getEvents().onCommand("newgame", [&](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Я загадал слово!🎯 \n\nПопробуйте угадать его, начав с любой буквы.");
        game.start_game();
        gameover = false;
    });

    bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {

        for (const auto& command : bot_commands) {
            if ("/" + command == message->text) {
                return;
            }
        }

        if (gameover) {
            bot.getApi().sendMessage(message->chat->id, "Вы уже отгадали слово.\nЧтобы начать новую игру, введите /newgame");
            return;
        }

        char letter = message->text[0];
        bool guess = game.guess_letter(letter);

        bot.getApi().sendMessage(message->chat->id, game.word.get_current());
        if (guess) {
            bot.getApi().sendMessage(message->chat->id, "Браво! С этой буквой вы на правильном пути!");
        }
        else {
            bot.getApi().sendMessage(message->chat->id, "Ой-ой, не попали. Но это же игра, давайте дальше!");
        }
        if (game.check_success()) {
            bot.getApi().sendMessage(message->chat->id, "Поздравляю! Вы отгадали слово. \n\nХотите начать новую игру? \nВведите /newgame");
            gameover = true;
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