#pragma once
#include <iostream>
#include "Message.h"
#include "User.h"
#include <vector>

class Chat
{

public:
	Chat();
	~Chat();

	void createUser(std::shared_ptr<User> user);
	void createAdminUser(std::shared_ptr<User> user);
	void deleteUser(int index);
	void sendMessage(std::shared_ptr<User> receiver, std::shared_ptr<Message> message);
	void sendMessageToAll(std::shared_ptr<Message> message);
	void printUsers();
	void printMessages();
	bool login(const std::string& login, const std::string& password);
	void logOff();
	bool isCurrentUserLogedIn() const;
	bool isCurrentUserAdmin() const;
	size_t isUserExist(const std::string& login) const;
	size_t getUsersCount() const;
	size_t getMessagesCount() const;
	std::string getCurrentUserLogin() const;
	std::string getUserLoginByID(int userID) const;
	std::shared_ptr<User> getUserByID(int userID) const;
	std::shared_ptr<User> getUserByLogin(const std::string& login) const;
	std::shared_ptr<User> getCurrentUser() const;
	
private:
	std::shared_ptr<User> _currentUser = nullptr;
	bool _isCurrentUserLogedIn = false;

	std::vector<std::shared_ptr<Message>> _messages;
	std::vector<std::shared_ptr<User>> _users;
};

