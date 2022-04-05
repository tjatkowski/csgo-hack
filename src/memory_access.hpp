#pragma once
#include "includes.hpp"

//read value under given address of given type
template<typename T> T read_memory(uintptr_t address) {
    try { 
        return *(T*)address; 
    }
    catch (...) { 
        return T(); 
    }
}
