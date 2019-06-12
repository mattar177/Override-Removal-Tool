#pragma once
#include <string>
#include <fstream>
#include <Windows.h>
#include <ShlDisp.h>
#include <tchar.h>
#include "fileTools.h"

	
	myFile::myFile(std::string path, std::string nPath) {

		filePath = path;
		newPath = nPath;

	}

	int myFile::IDMLToZIP() {

		rename(filePath.c_str(), newPath.c_str());
		return 0;

	}

	int myFile::ZIPtoIDML(std::string tempZip) {

		rename(tempZip.c_str(), filePath.c_str());
		return 0;

	}