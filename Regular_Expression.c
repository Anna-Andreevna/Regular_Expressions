#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "tests.h"
#include "NFA.h"
#include "compare.h"

#ifdef _DEBUG
#define TEST test_main()
#else
#define TEST
#endif 



int main() {
	TEST;
	_CrtDumpMemoryLeaks();
	return 0;
}