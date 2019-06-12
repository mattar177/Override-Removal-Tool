#pragma once

class search {
	std::regex searchTerm;
	std::smatch matches;
	std::string searchTarget;
	std::string subMatch;
	std::string statMessage;
	int stats;

public:
	search();
	int setSearchTerm(std::regex term);
	int setSearchTarget(std::string& target);
	bool checkForMatch();
	int replace(std::string replaceTerm);
	std::string getResult();
	int setStatMessage(std::string message);
	int getStats();
};