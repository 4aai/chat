#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Message.h"

class User
{
public:
	User(const std::string& name, const std::string& login, const std::string& password);
	~User();
	std::string getName() const;
	std::string getLogin() const;
	std::string getPassword() const;
	bool isUserAdmin() const;
	void setUserAdmin(bool admin);
	void addMessageToConversation(const std::string& conversation, std::shared_ptr<Message> message);
	void printConversation(const std::string& conversationName) ;
private:
	bool _isAdmin = false;
	std::string _name;
	std::string _login;
	std::string _password;
	std::map<std::string, std::vector<std::shared_ptr<Message>>> _conversations;
};

