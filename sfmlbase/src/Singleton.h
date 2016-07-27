#pragma once

#include "helpers/noncopyable.h"

template<typename T>
class Singleton : public noncopyable {
public:
    static T* GetInstance() {
        static T rm; 	//TODO: fer servir un punter static, per poder fer tambe DestroyInstance
        return &rm;
    }
};

