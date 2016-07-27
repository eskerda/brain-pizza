#pragma once

#include <string>
#include <map>

#include "Logger.h"

template<typename T>
class ResMan {
public:
    const T& Get(const std::string& s) {
        T* ptr = GetPtr(s);
        return *ptr;
    }

    T* GetPtr(const std::string& s) {
        typename MapLoaded::iterator it = loaded.find(s);
		if (it != loaded.end()) { //Found
			it->second.second++;
            return it->second.first;
		} else {
			T* t = Load(s);
			loaded[s] = UsesCount(t,1);
            return t;
		}
	}
	/*
	int Unget(T* elem) {
		//TODO: Reverse lookup
	}
	*/
	int Unget(const std::string& s) {
        typename MapLoaded::iterator it = loaded.find(s);
		if (it == loaded.end()) return -1;
		int uses = (--it->second.second);
		if (uses == 0) {
			Unload(it->second.first);
			loaded.erase(it);
		}
		return uses;

	}

protected:
	virtual T* Load(const std::string& file) = 0;
	virtual void Unload(T* obj) = 0;

private:
	typedef std::pair<T*,int> UsesCount;
	typedef std::map<std::string,UsesCount> MapLoaded;
	MapLoaded loaded;
};

