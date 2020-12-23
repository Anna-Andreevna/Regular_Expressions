#include <stdlib.h>
#include "structs.h"

int find_trans(Automate* aut, int curr_st, char s) {
	if (curr_st >= aut->state_num - 1) return -1;
	int i = 0;
	while ((i < aut->trans_num) && (aut->machine[i].current_state < curr_st))
		i++;
	while ((aut->machine[i].current_state == curr_st) && (i < aut->trans_num) && (aut->machine[i].symb < s))
		i++;
	if ((aut->machine[i].current_state == curr_st) && (aut->machine[i].symb == s)) return i;
	return -1;
}

int find_str(char* str, Automate* aut, int curr_st) {
	if (curr_st == -1) return 1;
	if (str[0] == '\0') return 0;
	char* next_expr = &str[1];
	char s = str[0];
	int i_t = find_trans(aut, curr_st, str[0]);
	if (i_t == -1) {
		i_t = find_trans(aut, curr_st, 0);
		if (i_t == -1) return 0;
		next_expr = str;
		s = 0;
	}
	while ((i_t < aut->trans_num) && (aut->machine[i_t].current_state == curr_st) && (aut->machine[i_t].symb == s)) {
		if (find_str(next_expr, aut, aut->machine[i_t].next_state) == 1) return 1;
		i_t++;
	}
	return 0;
}

int main_comp(char* str, Automate* aut) {
	for (int i = 0; str[i] != '\0'; i++)
		if (find_str(&str[i], aut, 0)) return 1;
	return 0;
}