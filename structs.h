#ifndef _AUT_AND_TRANS
#define _AUT_AND_TRANS

typedef struct Transition {
	int current_state;
	char symb;
	int next_state;
} Transition;

typedef struct Automate {
	Transition* machine;
	int state_num;
	int trans_num;
} Automate;

#endif
