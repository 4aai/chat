#include <iostream>
#include "Chat.h"


#define CHAT_VERSION                        "0.0.771 made by Maria & Pavel"

#define MENU_FIRST_SCREEN                   0
#define MENU_LOGIN_SCREEN                   1
#define MENU_REGISTRATION_SCREEN            2
#define MENU_EXIT_PROGRAM                   3
#define MENU_CHAT_SCREEN                    4
#define MENU_SELECT_USER_SCREEN             5
#define MENU_CHAT_WITH_SCREEN               6
#define MENU_CHAT_WITH_ALL_USERS_SCREEN     7
#define MENU_CHAT_ADMIN_SCREEN              127
#define MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES   1337

const std::string chat_god = R"(
                _______                _______  _______  _______       _______       _______  _______  ______  
      |\     /|(  ___  )|\     /|     (  ___  )(  ____ )(  ____ \     (  ___  )     (  ____ \(  ___  )(  __  \ 
      ( \   / )| (   ) || )   ( |     | (   ) || (    )|| (    \/     | (   ) |     | (    \/| (   ) || (  \  )
       \ (_) / | |   | || |   | |     | (___) || (____)|| (__         | (___) |     | |      | |   | || |   ) |
        \   /  | |   | || |   | |     |  ___  ||     __)|  __)        |  ___  |     | | ____ | |   | || |   | |
         ) (   | |   | || |   | |     | (   ) || (\ (   | (           | (   ) |     | | \_  )| |   | || |   ) |
         | |   | (___) || (___) |     | )   ( || ) \ \__| (____/\     | )   ( |     | (___) || (___) || (__/  )
         \_/   (_______)(_______)     |/     \||/   \__/(_______/     |/     \|     (_______)(_______)(______/
)";

// функция которая частично фиксит неправильнный ввод пользователя
void checkInput(int& menu, int screen)
{
    if (std::cin.fail()) 
    {
        std::cin.clear();
        std::cin.ignore();
        menu = screen;
    }
}


int main()
{
    Chat* chat = new Chat();
    int menuState = 0;
    std::string chatWithUserLogin;
    
    // добавим несколько пользователей
    chat->createAdminUser(std::make_shared<User>("God", "admin", "root"));
    chat->createUser(std::make_shared<User>("User", "user", "123"));
    chat->createUser(std::make_shared<User>("Arkadyi", "defaultUser1", "123"));
    chat->createUser(std::make_shared<User>("Petr", "defaultUser2", "123"));
    chat->createUser(std::make_shared<User>("Arsenyi", "defaultUser3", "123"));

    while (menuState != -1)
    {
        std::string login;
        std::string password;
        std::string name;
        std::string msg;
        
        system("CLS");
        std::cout << "Chat version: " << CHAT_VERSION;
        if (chat->isCurrentUserLogedIn())
        {
            if (chat->isCurrentUserAdmin()) std::cout << chat_god;
            std::cout << "\nYour Login: " << chat->getCurrentUser()->getLogin();
            std::cout << "\nYour Name: "  << chat->getCurrentUser()->getName();
        }
        std::cout << "\nUsers: " << chat->getUsersCount() << " Messages: " << chat->getMessagesCount() << "\n\n";

        switch (menuState)
        {
        //==================================================================================
        case MENU_FIRST_SCREEN:

            if (chat->isCurrentUserLogedIn())
            {
                menuState = MENU_CHAT_SCREEN;
                break;
            }
            std::cout << "Menu:\n";
            std::cout << "1. Login\n";
            std::cout << "2. Registration\n";
            std::cout << "3. Exit\n";
            std::cout << "input menu > "; std::cin >> menuState;
            checkInput(menuState, MENU_FIRST_SCREEN);
            break;
        //==================================================================================
        case MENU_LOGIN_SCREEN:
            
            std::cout << "Login:\n";
            std::cout << "input Login > "; std::cin >> login;
            if (chat->isUserExist(login) == -1)
            {   // если пользователя не существует, предлагаем зарегистрироваться
                std::cout << "User " << login << " dosn't exist enter name and password to registration:\n";
                std::cout << "input Name > "; std::cin >> name;
                std::cout << "input Password > "; std::cin >> password;
                chat->createUser(std::make_shared<User>(name, login, password));
                // авто вход
                chat->login(login, password);
            }
            else // если пользователь уже существует то просим войти
            {    // приветствуем пользователя по имени
                std::cout << "Hello, " << chat->getUserByLogin(login)->getName() << "\n";
                while (!chat->login(login, password))
                {
                    std::cout << "input Password > "; std::cin >> password;
                }
            }
            menuState = MENU_CHAT_SCREEN;
            break;
        //==================================================================================
        case MENU_REGISTRATION_SCREEN:

            std::cout << "Registration:\n";
            std::cout << "input Login > "; std::cin >> login;
            if (chat->isUserExist(login) == -1) // если пользователя нету то регистрация
            {
                std::cout << "input Name > "; std::cin >> name;
                std::cout << "input Password > "; std::cin >> password;
                chat->createUser(std::make_shared<User>(name, login, password));
                // авто вход
                chat->login(login, password);
            }
            else // если пользователь уже существует то просим войти
            {    // приветствуем пользователя по имени
                std::cout << "Hello, " << chat->getUserByLogin(login)->getName() << "\n";
                while (!chat->login(login, password))
                {
                    std::cout << "input Password > "; std::cin >> password;
                }
            }
            menuState = MENU_CHAT_SCREEN;
            break;
        //==================================================================================
        case MENU_EXIT_PROGRAM: menuState = -1; break;
        //==================================================================================
        case MENU_CHAT_SCREEN:

            std::cout << "Chat:\n";
            std::cout << "1. General chat\n";
            std::cout << "2. Show users to chating\n";
            std::cout << "3. Log out\n";
            if (chat->isCurrentUserAdmin())
            {
                std::cout << "127. Delete users\n";
                std::cout << "1337. Show all messages\n";
            }
            std::cout << "input menu > "; std::cin >> menuState;
            checkInput(menuState, MENU_CHAT_SCREEN);

            switch (menuState)
            {
                case 1: menuState = MENU_CHAT_WITH_ALL_USERS_SCREEN; break;
                case 2: menuState = MENU_SELECT_USER_SCREEN; break;
                case 3: chat->logOff(); menuState = MENU_FIRST_SCREEN; break;
                case MENU_CHAT_ADMIN_SCREEN: break; // удаление пользователей
                case MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES: break; // все сообщения
                default: menuState = MENU_CHAT_SCREEN; break;
            }
            break;
        //==================================================================================
        case MENU_SELECT_USER_SCREEN:
            
            std::cout << "ID  Login  Name  PWD \n";
            chat->printUsers();
            std::cout << "____________________________________________\n";
            std::cout << "type ID or -1 to Back\n";
            std::cout << "input user id > "; std::cin >> menuState;
            checkInput(menuState, MENU_SELECT_USER_SCREEN);

            if (0 <= menuState && menuState < chat->getUsersCount())
            {
                chatWithUserLogin = chat->getUserLoginByID(menuState);
                menuState = MENU_CHAT_WITH_SCREEN;
            }
            else
            {
                if (menuState == -1) menuState = MENU_CHAT_SCREEN;
                else                 menuState = MENU_SELECT_USER_SCREEN;
            }
            break;
        //==================================================================================
        case MENU_CHAT_WITH_SCREEN:
            
            std::cout << "Chat with " << chatWithUserLogin <<":\n";
            chat->getCurrentUser()->printConversation(chatWithUserLogin);

            std::cout << "____________________________________________\n";
            std::cout << " 1. send message\n-1. back\n";
            std::cout << "input menu > "; std::cin >> menuState;
            checkInput(menuState, MENU_CHAT_WITH_SCREEN);

            switch (menuState)
            {
                case MENU_CHAT_WITH_SCREEN: break;
                case -1: chatWithUserLogin.clear(); menuState = MENU_SELECT_USER_SCREEN; break;
                case 1:

                    std::cout << "input message > ";
                    std::getline(std::cin >> std::ws, msg); // ввод сообщения с пробелами
                    // отправка сообщения выбранному пользователю chatWithUserName
                    chat->sendMessage(chat->getUserByLogin(chatWithUserLogin), std::make_shared<Message>(chat->getCurrentUserLogin(), chatWithUserLogin, msg));
                    menuState = MENU_CHAT_WITH_SCREEN;

                    break;
                default: menuState = MENU_SELECT_USER_SCREEN; break;
            }
            break;
        //==================================================================================
        case MENU_CHAT_WITH_ALL_USERS_SCREEN:

            std::cout << "Chat with all users:\n";
            chat->getCurrentUser()->printConversation("ALL");

            std::cout << "____________________________________________\n";
            std::cout << " 1. send message\n-1. back\n";
            std::cout << "input menu > "; std::cin >> menuState;
            checkInput(menuState, MENU_CHAT_WITH_ALL_USERS_SCREEN);

            switch (menuState)
            {
                case MENU_CHAT_WITH_ALL_USERS_SCREEN: break;
                case -1: menuState = MENU_CHAT_SCREEN; break;
                case 1:

                    std::cout << "input message > ";
                    std::getline(std::cin >> std::ws, msg); // ввод сообщения с пробелами
                    // отправка сообщения всем зарегистрированным пользователям
                    chat->sendMessageToAll(std::make_shared<Message>(chat->getCurrentUserLogin(), "ALL", msg));
                    menuState = MENU_CHAT_WITH_ALL_USERS_SCREEN;

                    break;
                default: menuState = MENU_CHAT_SCREEN; break;
            }
            break;
        //==================================================================================
        case MENU_CHAT_ADMIN_SCREEN:
            std::cout << "ID  Login  Name  PWD \n";
            chat->printUsers();
            std::cout << "____________________________________________\n";
            std::cout << "type ID or -1 to Back\n";
            std::cout << "input user id > "; std::cin >> menuState;
            checkInput(menuState, MENU_CHAT_ADMIN_SCREEN);

            if (0 <= menuState && menuState < chat->getUsersCount())
            {
                if (!chat->getUserByID(menuState)->isUserAdmin())
                {
                    chat->deleteUser(menuState);
                }
                menuState = MENU_CHAT_ADMIN_SCREEN;
            }
            else
            {
                if (menuState == -1) menuState = MENU_CHAT_SCREEN;          
                else                 menuState = MENU_CHAT_ADMIN_SCREEN; 
            }
            break;
        //==================================================================================
        case MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES:

            std::cout << "Messages: \n";
            chat->printMessages();
            std::cout << "___________________________________________________________\n";
            std::cout << "-1 to Back\n";
            std::cout << "input menu > "; std::cin >> menuState;
            checkInput(menuState, MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES);

            if (menuState == -1) menuState = MENU_CHAT_SCREEN; 
            else                 menuState = MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES;
             
            break;
        //==================================================================================
        default: menuState = MENU_FIRST_SCREEN; break;
        }
    }
    delete chat;
}