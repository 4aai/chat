#include "Chat.h"

int main()
{
    system("chcp 1251");
    Chat* chat = new Chat();

    // добавим несколько пользователей
    chat->createAdminUser(std::make_shared<User>("God",     "admin",        "root"));
    chat->createUser(     std::make_shared<User>("User",    "user",         "123" ));
    chat->createUser(     std::make_shared<User>("Arkadyi", "defaultUser1", "123" ));
    chat->createUser(     std::make_shared<User>("Petr",    "defaultUser2", "123" ));
    chat->createUser(     std::make_shared<User>("Arsenyi", "defaultUser3", "123" ));

    chat->startChat();

    delete chat;
}