#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <filesystem>
#include <sstream>
#include <Windows.h>
#include <sys/stat.h>
#include <vector>

#include "fileTools.h"
#include "searchTools.h"
#include "unzip.h"
#include "zip.h"

// Defining custom namespace to save typing long namespace later
namespace fs = std::experimental::filesystem;

// Function prototypes
void unzip(std::string newPath, std::string folderPath);
void zip(std::string tempZip, std::string folderPath);
int searchFile(std::string path, std::vector<search>& searches, std::string storiesFolderPath);
bool fileExists(const std::string& filename);


// Main function
int main() 
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x3);

	// Initialise strings
	std::string username, userDesktop, fileName, filePath, newPath, storiesFolderPath, 
		folderPath, currentFilePath, tempZip, nameInZip;

	std::vector<search> searches;

	char selection[1];

	// Take input from user to assertain path to users desktop
	std::cout << "Please enter your username: ";
	std::cin >> username;
	userDesktop = "C:\\Users\\" + username + "\\Desktop\\";

	// Take input from user to find correct file on desktop
	std::cout << std::endl << std::endl << "Please enter book filename without extension: ";
	std::cin >> fileName;

	// Create string for path to idml file and make sure file exists
	filePath = userDesktop + fileName + ".idml";
	while (fileExists(filePath) == false)
	{
		std::cout << std::endl << std::endl << "No such file exists!";
		std::cout << std::endl << std::endl << "Please enter book filename without extension: ";
		std::cin >> fileName;

		// Create string for path to idml file
		filePath = userDesktop + fileName + ".idml";
	}

	// Create string for path to zip file
	newPath = userDesktop + fileName + ".zip";

	// Instantiate myFile object to change file extension
	myFile newFile(filePath, newPath);
	newFile.IDMLToZIP();

	// Create new folder to extract zip into
	folderPath = userDesktop + fileName;
	fs::create_directory(folderPath);

	// Call to unzip function
	unzip(newPath, folderPath);

	// Delete zip file
	remove(newPath.c_str());

	// Create string for path to subfolders
	storiesFolderPath = folderPath + "\\Stories";

	// Assertain what user would like to remove, instantiate respective search objects, and populate searches vector
	do
	{
		std::cout << std::endl << std::endl << "Remove tracking? (y/n) ";
		std::cin >> selection;
		if (strcmp(selection, "y") == 0)
		{
			search trackingSearch;
			trackingSearch.setSearchTerm(std::regex(" Tracking=\"[-]?\\d+?\""));
			trackingSearch.setStatMessage(" tracking overrides removed");
			searches.push_back(trackingSearch);
		}
	} while (strcmp(selection, "y") != 0 && strcmp(selection, "n") != 0);

	do
	{
		std::cout << std::endl << "Remove kerning? (y/n) ";
		std::cin >> selection;
		if (strcmp(selection, "y") == 0)
		{
			search kerningSearch;
			kerningSearch.setSearchTerm(std::regex(" KerningValue=\"[-]?\\d+?\""));
			kerningSearch.setStatMessage(" kerning overrides removed");
			searches.push_back(kerningSearch);
		}
	} while (strcmp(selection, "y") != 0 && strcmp(selection, "n") != 0);

	do
	{
		std::cout << std::endl << "Remove space before? (y/n) ";
		std::cin >> selection;
		if (strcmp(selection, "y") == 0)
		{
			search spaceBeforeSearch;
			spaceBeforeSearch.setSearchTerm(std::regex(" SpaceBefore=\"\\d*[\\.]?\\d*\""));
			spaceBeforeSearch.setStatMessage(" 'space before' overrides removed");
			searches.push_back(spaceBeforeSearch);
		}
	} while (strcmp(selection, "y") != 0 && strcmp(selection, "n") != 0);

	do
	{
		std::cout << std::endl << "Remove space after? (y/n) ";
		std::cin >> selection;
		if (strcmp(selection, "y") == 0)
		{
			search spaceAfterSearch;
			spaceAfterSearch.setSearchTerm(std::regex(" SpaceAfter=\"\\d*[\\.]?\\d*\""));
			spaceAfterSearch.setStatMessage(" 'space after' overrides removed");
			searches.push_back(spaceAfterSearch);
		}
	} while (strcmp(selection, "y") != 0 && strcmp(selection, "n") != 0);

	do
	{
		std::cout << std::endl << "Remove align: flush left? (y/n) ";
		std::cin >> selection;
		if (strcmp(selection, "y") == 0)
		{
			search flushLeftSearch;
			flushLeftSearch.setSearchTerm(std::regex(" Justification=\"LeftAlign\""));
			flushLeftSearch.setStatMessage(" 'align: flush left' overrides removed");
			searches.push_back(flushLeftSearch);
		}
	} while (strcmp(selection, "y") != 0 && strcmp(selection, "n") != 0);

	do
	{
		std::cout << std::endl << "Remove DropcapDetail=0? (y/n) ";
		std::cin >> selection;
		if (strcmp(selection, "y") == 0)
		{
			search dropCapDetailSearch;
			dropCapDetailSearch.setSearchTerm(std::regex(" DropcapDetail=\"0\""));
			dropCapDetailSearch.setStatMessage(" 'DropCapDetail' overrides removed");
			searches.push_back(dropCapDetailSearch);
		}
	} while (strcmp(selection, "y") != 0 && strcmp(selection, "n") != 0);

	do
	{
		std::cout << std::endl << "Remove AppliedLanguage=English:UK? (y/n) ";
		std::cin >> selection;
		if (strcmp(selection, "y") == 0)
		{
			search appliedLanguageSearch;
			appliedLanguageSearch.setSearchTerm(std::regex(" AppliedLanguage=\"\\$ID/English: UK\""));
			appliedLanguageSearch.setStatMessage(" 'AppliedLanguage' overrides removed");
			searches.push_back(appliedLanguageSearch);
		}
	} while (strcmp(selection, "y") != 0 && strcmp(selection, "n") != 0);

	do
	{
		std::cout << std::endl << "Remove AppliedLanguage=English:USA? (y/n) ";
		std::cin >> selection;
		if (strcmp(selection, "y") == 0)
		{
			search appliedLanguageUSASearch;
			appliedLanguageUSASearch.setSearchTerm(std::regex(" AppliedLanguage=\"\\$ID/English: USA\""));
			appliedLanguageUSASearch.setStatMessage(" 'AppliedLanguageUSA' overrides removed");
			searches.push_back(appliedLanguageUSASearch);
		}
	} while (strcmp(selection, "y") != 0 && strcmp(selection, "n") != 0);

	std::cout << std::endl << std::endl << "Working...";

	// Loop through stories folder calling search function for each file found
	for (auto& f : fs::directory_iterator(storiesFolderPath))
	{
		std::stringstream fn;
		fn << f;
		currentFilePath = fn.str();
		searchFile(currentFilePath, searches, storiesFolderPath);
	}

	// Create string for temporary zip location
	tempZip = userDesktop + "temp.zip";

	// Call to zip function
	zip(tempZip, folderPath);

	// Deleting extracted folder and simultaneously renaming and changing file extension of new zip file
	fs::remove_all(folderPath);
	newFile.ZIPtoIDML(tempZip.c_str());

	// Success message
	std::string clear;
	std::getline(std::cin, clear);
	std::cout << std::endl << std::endl << "*********************************** Success! ***********************************";
	for (unsigned int i = 0; i < searches.size(); i++)
	{
		searches[i].getStats();
	}
	std::cout << std::endl << std::endl << "                             Press any key to exit\n\n\n\n\n\n\n\n\n\n\n";
	std::cin.get();

	return 0;
}


void unzip(std::string newPath, std::string folderPath)
{
	std::string extractTo;
	HZIP hz = OpenZip((const TCHAR*)newPath.c_str(), 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;
	// -1 gives overall information about the zipfile
	for (int zi = 0; zi < numitems; zi++)
	{
		ZIPENTRY ze;
		GetZipItem(hz, zi, &ze); // fetch individual details
		std::stringstream temp;
		temp << ze.name;
		extractTo = folderPath + "\\" + temp.str();
		UnzipItem(hz, zi, (const TCHAR*)extractTo.c_str()); // e.g. the item's name.
	}
	CloseZip(hz);

	// System call utilising 7-zip to extract zip file (TEMPORARY FIX!)
	//system(("cd /D C:\\Program Files\\7-zip && 7z x " + newPath + " -o" + folderPath).c_str());
}


void zip(std::string tempZip, std::string folderPath)
{
	// Create zip file using temporary location
	HZIP hzi = CreateZip((const TCHAR*)tempZip.c_str(), 0);

	// Loop utilising zip-utils to populate zip avoiding system call - Currently unable to populate zip without compression so unusable as is
	/*for (auto& f : fs::recursive_directory_iterator(folderPath))
	{
	std::stringstream fn;
	fn << f;
	currentFilePath = fn.str();
	nameInZip = currentFilePath;
	nameInZip.erase(0, userDesktop.length());
	std::cout << nameInZip;
	if (nameInZip != fileName + "\\mimetype")
	{
	ZipAdd(hz, (const TCHAR*)nameInZip.c_str(), (const TCHAR*)currentFilePath.c_str());
	}
	}*/

	CloseZip(hzi);

	// System call utilising 7-zip to populate previously created zip file without compression
	system(("cd /D C:\\Program Files\\7-Zip && 7z a -tzip " + tempZip + " " + folderPath + "\\MasterSpreads " +
		folderPath + "\\META-INF " + folderPath + "\\Resources " + folderPath + "\\Spreads " + folderPath +
		"\\Stories " + folderPath + "\\XML " + folderPath + "\\designmap.xml " + folderPath + "\\mimetype\" -mx0").c_str());
}


int searchFile(std::string path, std::vector<search>& searches, std::string storiesFolderPath)
{
	std::string tempPath = storiesFolderPath + "\\temp.txt";
	std::ofstream tempFile(tempPath);
	std::ifstream currentFile(path);
	std::string text;

	if (currentFile.is_open())
	{
		if (tempFile.is_open())
		{
			std::stringstream buffer;
			buffer << currentFile.rdbuf();
			text = buffer.str();

			for (unsigned int i = 0; i < searches.size(); i++)
			{
				searches[i].setSearchTarget(text);
				while (searches[i].checkForMatch()) searches[i].replace("");
				text = searches[i].getResult();
			}

			tempFile << text;
			tempFile.close();
			currentFile.close();
			remove(path.c_str());
			rename(tempPath.c_str(), path.c_str());
		}
		else
		{
			std::cout << "Unable to open temp file";
		}

	}
	else
	{
		std::cout << "Unable to open file";
	}

	return 0;
}


/*int searchFile(std::string path, search& tracking, search& kerning, search& spaceBefore, std::string storiesFolderPath)
{
	std::string line;
	std::ifstream currentFile(path);
	std::string tempPath = storiesFolderPath + "\\temp.txt";
	std::ofstream tempFile(tempPath);


	if (currentFile.is_open())
	{
		if (tempFile.is_open())
		{
			while (getline(currentFile, line))
			{
				if (tracking.checkForMatch(line))
				{
					line = tracking.replace("");
				}
				if (kerning.checkForMatch(line))
				{
					line = kerning.replace("");
				}
				if (spaceBefore.checkForMatch(line))
				{
					line = spaceBefore.replace("");
				}

				tempFile << line;

			}
			currentFile.close();
			remove(path.c_str());
			tempFile.close();
			rename(tempPath.c_str(), path.c_str());
		}
		else
		{
			std::cout << "Unable to open temp file";
		}

	}
	else
	{
		std::cout << "Unable to open file";
	}

	return 0;
}*/


bool fileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}