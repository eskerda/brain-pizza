#pragma once

#include "constants.h"

#include <map>
#include <string>
#include <vector>
#include "helpers/stringhelpers.h"
#include "helpers/noncopyable.h"

//Parameters format: 
//./program file1 -myparam=true --bar=foo file2
//"myparam" and "bar" are named parameters (can access them by name with GetBool and GetString)
//"file1" and "file2" are unnamed parameters (GetUnnamed will return a vector of all them)
class Params : noncopyable {
public:

	static void InitFromCmdLine(int argc, char** argv) {
		exec_name = std::string(argv[0]);
		for (int i=1; i < argc; i++) { //skip param 0
			char* c = argv[i];
			if (c[0] == '-') {
				while(*c == '-') c++;
				std::vector<std::string> v = stringsplit(std::string(c),'=');
				std::string key = v[0];
				std::string value = (v.size() == 1)? "true" : v[1];
				params[key] = value;
			} else {
				unnamed_params.push_back(std::string(c));
			}
		}
	}
	
	static std::string& GetExecutableName() {
		return exec_name;
	}

	static std::vector<std::string>& GetUnnamed() {
		return unnamed_params;
	}
	
	//Preferably use the accessors defined below
	static const std::map<std::string,std::string>& GetNamed() {
		return params;
	}

	static bool IsSet(std::string s) {
		std::map<std::string,std::string>::const_iterator it = params.find(s);
		if (it == params.end()) return false;
		return it->second != "";
	}
	
	static bool GetBool(std::string s, bool defaultValue) {
		std::map<std::string,std::string>::const_iterator it = params.find(s);
		if (it == params.end()) return defaultValue;
		return it->second != "false" && it->second != "0" && it->second != "";
	}

	static std::string GetString(std::string s) {
		return params[s];
	}

private:
	Params() { }
	static std::string exec_name;
	static std::map<std::string,std::string> params;
	static std::vector<std::string> unnamed_params;
};

