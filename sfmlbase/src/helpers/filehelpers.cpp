#include "filehelpers.h"

#include <fstream>
#include <string>
#include <iostream>
#ifndef WIN32
#include <algorithm>
#include <dirent.h>
#endif

std::string DirnameOf(const std::string& fname)
{
     size_t pos = fname.find_last_of("\\/");
     return (std::string::npos == pos) ? "" : (fname.substr(0, pos)+"/");
}


//Operacions amb fitxers
bool FileRead(const std::string& file, std::vector<std::string>& out_v) { //Cada linia de file es una string del vector v
	std::ifstream input(file.c_str());
	if(! input.is_open()) {
		throw "CANNONT OPEN " + file;
		return true;
	}
	out_v=std::vector<std::string>(1); //Inicialitzem
	for (char c; input.get(c);) if (c == '\n') out_v.resize(out_v.size()+1); else out_v[out_v.size()-1] += c;
	input.close();
	return false;
}

bool FileSave(const std::string& file, const std::vector<std::string>& v) { //Cada string del vector v es un linia de file (esborra el que ja hi havia)
	std::ofstream output(file.c_str());
	if(! output.is_open()) {
		throw "CANNONT OPEN " + file;
		return true;
	}
	for (uint i = 0; i < v.size()-1; ++i) output << v[i] << std::endl;
	output << v[v.size()-1];
	output.close();
	return false;
}

bool FileAppend(const std::string& file, std::string s) { //Afegeix la string s al final de file (no esborra el que ja hi havia)
	std::ofstream out(file.c_str(), std::ios::app);
	if(! out.is_open()) {
		throw "CANNONT OPEN " + file;
		return true;
	}
	out << s;
	out.close();
	return false;
}

void FileList(const std::string& s, std::vector<std::string>& out_noms) { //Retorna un vector amb tots els fitxers de la carpeta excepte aquells que comencen per . (punt)

	#ifdef WIN32
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = FindFirstFile((s+"\\*").c_str(), &findFileData);
		do {
			std::string s = findFileData.cFileName;
			//std::cout << s << std::endl;
			if (findFileData.cFileName[0] != '.' ) out_noms.push_back(s);
		} while(FindNextFile(hFind, &findFileData));
	#else
		DIR *dp = opendir(s.c_str());
		struct dirent *dirp = readdir(dp);
		do {
			if (dirp->d_name[0] != '.') out_noms.push_back(std::string(dirp->d_name));
		} while ((dirp = readdir(dp)) != NULL);
		closedir(dp);
		std::sort(out_noms.begin(),out_noms.end()); //Perque quedi igual que a windows
	#endif


}
