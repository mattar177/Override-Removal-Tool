#include <regex>
#include <iostream>
#include <fstream>
#include "searchTools.h"


search::search()
{
	stats = 0;
}


int search::setSearchTerm(std::regex term)
{
	searchTerm = term;
	return 0;
}


int search::setSearchTarget(std::string& target)
{
	searchTarget = target;
	return 0;
}


bool search::checkForMatch()
{
	if (std::regex_search(searchTarget, matches, searchTerm))
	{
		stats++;
		return true;
	}
	
	return false;
}


int search::replace(std::string replaceTerm)
{
	searchTarget = std::regex_replace(searchTarget, searchTerm, replaceTerm, std::regex_constants::format_first_only);
	return 0;
}


std::string search::getResult()
{
	return searchTarget;
}


int search::setStatMessage(std::string message)
{
	statMessage = message;
	return 0;
}


int search::getStats()
{
	std::cout << std::endl << std::endl << "                       " << stats << statMessage;
	return 0;
}