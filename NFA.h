#pragma once
#include "structs.h"

Automate* construct_NFA(char* expr, int len);

void destruct_NFA(Automate* aut);