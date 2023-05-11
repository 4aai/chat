#pragma once
#include <string>

class Message
{
public:
	Message(const std::string& sender, const std::string& receiver, const std::string& text);
	~Message();
	std::string getMessage() const;
	std::string getSender() const;
	std::string getReceiver() const;
private:
	std::string _text;
	std::string _sender;
	std::string _receiver;
	// time
	// date
};

