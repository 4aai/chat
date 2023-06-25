#include "Chat.h"
#include "consts.h"

Chat::Chat(){}
Chat::~Chat() {}

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
void Chat::startChat()
{
    int _menuState = 0;
    std::string chatWithUserLogin;
    NetworkClient _client;
    NetworkServer _server;
    size_t networkStatus{};
    bool isChatServer = false;

    while (_menuState != -1)
    {
        std::string _login;
        std::string _password;
        std::string _name;
        std::string _msg;
        _client._name = getCurrentUserLogin();
        _server._name = getCurrentUserLogin();
        

        system("CLS");
        std::cout << "Chat version: " << CHAT_VERSION;
        if (isCurrentUserLogedIn())
        {
            if (isCurrentUserAdmin()) std::cout << "\033[96m" << chat_god << "\033[0m";
            std::cout << "\nYour Login: \033[93m" << getCurrentUser()->getLogin() << "\033[0m";
            std::cout << "\nYour Name: \033[93m" << getCurrentUser()->getName() << "\033[0m";
        }
        std::cout << "\nUsers: \033[93m" << getUsersCount() << "\033[0m"
                  << " Messages: \033[93m" << getMessagesCount() << "\033[0m\n\n";

        

        switch (_menuState)
        {
            //==================================================================================
        case MENU_FIRST_SCREEN:

            if (isCurrentUserLogedIn())
            {
                _menuState = MENU_CHAT_SCREEN;
                break;
            }

            std::cout << "Menu:\n";
            std::cout << "1. Login\n";
            std::cout << "2. Registration\n";
            std::cout << "3. Exit\n";
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_FIRST_SCREEN);
            break;
            //==================================================================================
        case MENU_LOGIN_SCREEN:

            std::cout << "Login:\n";
            std::cout << "input Login > "; std::cin >> _login;
            if (isUserExist(_login) == -1)
            {   // если пользователя не существует, предлагаем зарегистрироваться
                std::cout << "User " << _login << " dosn't exist enter name and password to registration:\n";
                std::cout << "input Name > "; std::cin >> _name;
                std::cout << "input Password > "; std::cin >> _password;
                createUser(std::make_shared<User>(_name, _login, _password));
                // авто вход
                login(_login, _password);
            }
            else // если пользователь уже существует то просим войти
            {    // приветствуем пользователя по имени
                std::cout << "Hello, " << getUserByLogin(_login)->getName() << "\n";
                while (!login(_login, _password))
                {
                    std::cout << "input Password > "; std::cin >> _password;
                }
            }
            _menuState = MENU_CHAT_SCREEN;
            break;
            //==================================================================================
        case MENU_REGISTRATION_SCREEN:

            std::cout << "Registration:\n";
            std::cout << "input Login > "; std::cin >> _login;
            if (isUserExist(_login) == -1) // если пользователя нету то регистрация
            {
                std::cout << "input Name > "; std::cin >> _name;
                std::cout << "input Password > "; std::cin >> _password;
                createUser(std::make_shared<User>(_name, _login, _password));
                // авто вход
                login(_login, _password);
            }
            else // если пользователь уже существует то просим войти
            {    // приветствуем пользователя по имени
                std::cout << "Hello, " << getUserByLogin(_login)->getName() << "\n";
                while (!login(_login, _password))
                {
                    std::cout << "input Password > "; std::cin >> _password;
                }
            }
            _menuState = MENU_CHAT_SCREEN;
            break;
            //==================================================================================
        case MENU_EXIT_PROGRAM: _menuState = -1; break;
            //==================================================================================
        case MENU_CHAT_SCREEN:

            std::cout << "Chat:\n";
            std::cout << "1. General chat\n";
            std::cout << "2. Show users to chating\n";
            std::cout << "3. Log out World\n";

            std::cout << "4. \033[93mUDP Network chat \033[30m\033[106m[experimental]\033[0m\n";

            if (isCurrentUserAdmin())
            {
                std::cout << "127. Delete users\n";
                std::cout << "1337. Show all messages\n";
            }
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_SCREEN);

            switch (_menuState)
            {
            case 1: _menuState = MENU_CHAT_WITH_ALL_USERS_SCREEN; break;
            case 2: _menuState = MENU_SELECT_USER_SCREEN; break;
            case 3: logOff(); _menuState = MENU_FIRST_SCREEN; break;
            case 4: _menuState = MENU_CHAT_UDP_NETWORK; break;
            case MENU_CHAT_ADMIN_SCREEN: break; // удаление пользователей
            case MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES: break; // все сообщения
            default: _menuState = MENU_CHAT_SCREEN; break;
            }
            break;
            //==================================================================================
        case MENU_SELECT_USER_SCREEN:

            printUsers();
            std::cout << "____________________________________________\n";
            std::cout << "type ID or -1 to Back\n";
            std::cout << "input user id > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_SELECT_USER_SCREEN);

            if (0 <= _menuState && _menuState < getUsersCount())
            {
                chatWithUserLogin = getUserLoginByID(_menuState);
                _menuState = MENU_CHAT_WITH_SCREEN;
            }
            else
            {
                if (_menuState == -1) _menuState = MENU_CHAT_SCREEN;
                else                  _menuState = MENU_SELECT_USER_SCREEN;
            }
            break;
            //==================================================================================
        case MENU_CHAT_WITH_SCREEN:

            std::cout << "Chat with " << chatWithUserLogin << ":\n";
            getCurrentUser()->printConversation(chatWithUserLogin);

            std::cout << "____________________________________________\n";
            std::cout << " 1. send message\n-1. back\n";
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_WITH_SCREEN);

            switch (_menuState)
            {
            case MENU_CHAT_WITH_SCREEN: break;
            case -1: chatWithUserLogin.clear(); _menuState = MENU_SELECT_USER_SCREEN; break;
            case 1:

                std::cout << "input message > ";
                std::getline(std::cin >> std::ws, _msg); // ввод сообщения с пробелами
                // отправка сообщения выбранному пользователю chatWithUserName
                sendMessage(getUserByLogin(chatWithUserLogin), std::make_shared<Message>(getCurrentUserLogin(), chatWithUserLogin, _msg));
                _menuState = MENU_CHAT_WITH_SCREEN;

                break;
            default: _menuState = MENU_SELECT_USER_SCREEN; break;
            }
            break;
            //==================================================================================
        case MENU_CHAT_WITH_ALL_USERS_SCREEN:

            std::cout << "Chat with all users:\n";
            getCurrentUser()->printConversation("ALL");

            std::cout << "____________________________________________\n";
            std::cout << " 1. send message\n-1. back\n";
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_WITH_ALL_USERS_SCREEN);

            switch (_menuState)
            {
            case MENU_CHAT_WITH_ALL_USERS_SCREEN: break;
            case -1: _menuState = MENU_CHAT_SCREEN; break;
            case 1:

                std::cout << "input message > ";
                std::getline(std::cin >> std::ws, _msg); // ввод сообщения с пробелами
                // отправка сообщения всем зарегистрированным пользователям
                sendMessageToAll(std::make_shared<Message>(getCurrentUserLogin(), "ALL", _msg));
                _menuState = MENU_CHAT_WITH_ALL_USERS_SCREEN;

                break;
            default: _menuState = MENU_CHAT_SCREEN; break;
            }
            break;
            //==================================================================================
        case MENU_CHAT_ADMIN_SCREEN:

            printUsers();
            std::cout << "____________________________________________\n";
            std::cout << "type ID or -1 to Back\n";
            std::cout << "input user id > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_ADMIN_SCREEN);

            if (0 <= _menuState && _menuState < getUsersCount())
            {
                if (!getUserByID(_menuState)->isUserAdmin())
                {
                    deleteUser(_menuState);
                }
                _menuState = MENU_CHAT_ADMIN_SCREEN;
            }
            else
            {
                if (_menuState == -1) _menuState = MENU_CHAT_SCREEN;
                else                  _menuState = MENU_CHAT_ADMIN_SCREEN;
            }
            break;
            //==================================================================================
        case MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES:

            std::cout << "Messages: \n";
            printMessages();
            std::cout << "___________________________________________________________\n";
            std::cout << "-1 to Back\n";
            std::cout << "input menu > "; std::cin >> _menuState;
            checkInput(_menuState, MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES);

            if (_menuState == -1) _menuState = MENU_CHAT_SCREEN;
            else                  _menuState = MENU_CHAT_ADMIN_SHOW_ALL_MESSAGES;

            break;
            //==================================================================================
        case MENU_CHAT_UDP_NETWORK:

            std::cout << "UDP chat network:\n";
            std::cout << "Start network, looking for servers...\n";
            networkStatus = _client.Connect();
            _menuState = MENU_UDP_NETWORK_CHATING; isChatServer = false;
            if (networkStatus != 0) {
                _client.Shutdown();
                std::cout << "Start server...\n";
                _menuState = MENU_UDP_NETWORK_CHATING; isChatServer = true;
                networkStatus = _server.Listen();
            }

            break;
            //==================================================================================
        case MENU_UDP_NETWORK_CHATING:
            if (isChatServer)
            {   
                
                std::cout << "[SERVER] Network chattig:\n";
                _server.History();
                if (networkStatus == 0)
                {
                    
                    if (networkStatus == 0) 
                    {
                        
                        //std::cout << "waiting for response... ";
                        networkStatus = _server.Read();
                        _server.HistoryLast();
                    }
                    if (networkStatus == 0) {

                        std::cout << "input message > ";
                        std::getline(std::cin >> std::ws, _msg); // ввод сообщения с пробелами
                        networkStatus = _server.Write(_msg);
                    }
                    
                }
                else
                {
                    _server.Shutdown();
                    _menuState = MENU_CHAT_SCREEN;
                }
            }
            else 
            {
                
                std::cout << "[CLIENT] Network chattig:\n";
                
                if (networkStatus == 0)
                {
                    _client.History();
                    if (networkStatus == 0) {

                        std::cout << "input message > ";
                        std::getline(std::cin >> std::ws, _msg); // ввод сообщения с пробелами
                        networkStatus = _client.Write(_msg);
                        //_client.History();
                        std::cout << "waiting for response... ";
                    }
                    if (networkStatus == 0)
                        networkStatus = _client.Read();
                }
                else
                {
                    _client.Shutdown();
                    _menuState = MENU_CHAT_SCREEN;
                }
            }
            break;
            //==================================================================================
        default: _menuState = MENU_FIRST_SCREEN; break;
        }
    }
}

void Chat::createUser(std::shared_ptr<User> user)
{
	_users.emplace_back(user);
}

void Chat::createAdminUser(std::shared_ptr<User> user)
{
	user->setUserAdmin(true);
	_users.emplace_back(user);
}

void Chat::deleteUser(int index)
{
	_users.erase(_users.begin() + index);
}

size_t Chat::isUserExist(const std::string& login) const
{
	size_t length = _users.size();
	for (size_t i = 0; i < length; i++)
	{
		if ( login == _users[i]->getLogin() ) return i;
	}
	return -1;
}

void Chat::sendMessage(std::shared_ptr<User> receiver, std::shared_ptr<Message> message)
{
	_messages.emplace_back(message);
	if (receiver == _currentUser)
	{
		_currentUser->addMessageToConversation(receiver->getLogin(), message);
	}
	else
	{
		receiver->addMessageToConversation(_currentUser->getLogin(), message);
		_currentUser->addMessageToConversation(receiver->getLogin(), message);
	}
	
}

void Chat::sendMessageToAll(std::shared_ptr<Message> message)
{
	_messages.emplace_back(message);
	size_t length = _users.size();
	for (size_t i = 0; i < length; i++)
	{
		_users[i]->addMessageToConversation("ALL", message);
	}
}

void Chat::printUsers() 
{
	size_t length = _users.size();
	std::cout << std::setw(5)  << "ID"
			  << std::setw(15) << "Login"
			  << std::setw(15) << "Name"
			  << std::setw(15) << "PWD" << std::endl;
	std::cout << "___________________________________________________________\n";
	for (size_t i = 0; i < length; i++)
	{
		std::cout << std::setw(5) << i 
			<< std::setw(15) << _users[i]->getLogin() 
			<< std::setw(15) << _users[i]->getName() 
			<< std::setw(15) << _users[i]->getPassword() << std::endl;
	}
}

void Chat::printMessages()
{
	size_t length = _messages.size();
	for (size_t i = 0; i < length; i++)
	{
		std::cout << "___________________________________________________________\n" 
			<< i << ". (" << _messages[i]->getSender() << " >>> "
						  << _messages[i]->getReceiver() << "): "
						  << _messages[i]->getMessage() << std::endl;
	}
}

bool Chat::login(const std::string& login, const std::string& password)
{
	
	size_t userID = isUserExist(login);
	if (userID == -1) return false;
	if (_users[userID]->getPassword() == password)
	{
		_currentUser = _users[userID];
		_isCurrentUserLogedIn = true;
		return true;
	}
	else
	{
		return false;
	}
}

void Chat::logOff()
{
	_currentUser = nullptr;
	_isCurrentUserLogedIn = false;
}

bool Chat::isCurrentUserLogedIn() const
{
	return _isCurrentUserLogedIn;
}

bool Chat::isCurrentUserAdmin() const
{
	return _currentUser->isUserAdmin();
}


size_t Chat::getUsersCount() const
{
	return _users.size();
}

size_t Chat::getMessagesCount() const
{
	return _messages.size();
}

std::string Chat::getCurrentUserLogin() const
{
    if (_currentUser == nullptr) return "null";
	return _currentUser->getLogin();
}

std::string Chat::getUserLoginByID(int userID) const
{
	try
	{
		return _users[userID]->getLogin();
	}
	catch (const std::exception&)
	{
		return "!user";
	}
	
}

std::shared_ptr<User> Chat::getUserByID(int userID) const
{
	try
	{
		return _users[userID];
	}
	catch (const std::exception&)
	{
		return nullptr;
	}
}

std::shared_ptr<User> Chat::getUserByLogin(const std::string& userName) const
{
	size_t length = _users.size();
	for (size_t i = 0; i < length; i++)
	{
		if (userName == _users[i]->getLogin()) return _users[i];
	}
	return nullptr;
}

std::shared_ptr<User> Chat::getCurrentUser() const
{
	return _currentUser;
}
