#pragma once

class myFile{

		std::string fileName, filePath, newPath;

	public:

		myFile(std::string path, std::string nPath);
		int IDMLToZIP();
		int ZIPtoIDML(std::string tempZip);
};