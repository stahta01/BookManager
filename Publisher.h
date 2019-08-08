#pragma once
#include <string>
#include "Utils.h"

class Publisher
{
public:
	Publisher(std::string name_){
		name = Utils::StringUtils::lowerCaseExceptFirstLetter(name_);
	}

	std::string const getName() const { return this->name; }

	void setName(std::string name) { this->name = Utils::StringUtils::lowerCaseExceptFirstLetter(name); }

	bool operator<(const Publisher& rhs)
	{
		if((this->name).compare(rhs.name) < 0)
			return true;
		return false;
	}

	bool operator==(const Publisher& rhs)
	{
		if ((this->name).compare(rhs.name) == 0)
			return true;
		return false;
	}
private:
	std::string name;
};