#pragma once

#include "includes.hpp"

void patch(BYTE* dst, BYTE* src, unsigned int size);

bool hook(char* src, char* dst, int len);

char* trampHook(char* src, char* dst, unsigned int len);