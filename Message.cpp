#include "Message.h"
#include "User.h"

Message::Message(const std::string& sender, const std::string& receiver, const std::string& text) :
	_sender{ sender }, _receiver{ receiver }, _text{ text } 
{
}

Message::~Message() = default;

std::string Message::getMessage() const
{
	return _text;
}

std::string Message::getSender() const
{
	return _sender;
}

std::string Message::getReceiver() const
{
	return _receiver;
}

