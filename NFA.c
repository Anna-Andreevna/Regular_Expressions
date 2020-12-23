//status: | ()
// в [] промежутки из разных типов
// сортировать сразу

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "spec_fun.h"

void destruct_NFA(Automate* aut);

void add_transition(Automate** aut, int current_state, char symb, int next_state) {
	for (int i = 0; i < (*aut)->trans_num; i++)
		// попробовать преобразовать
		if ((current_state == (*aut)->machine[i].current_state) && (symb == (*aut)->machine[i].symb) && (next_state == (*aut)->machine[i].current_state))
			return;
	Transition* new_mach = (Transition*)realloc((*aut)->machine, ((*aut)->trans_num + 1) * sizeof(Transition));
	if (new_mach == NULL) {
		destruct_NFA(*aut);
		return;
	}
	new_mach[(*aut)->trans_num] = (Transition){ current_state, symb, next_state };
	(*aut)->machine = new_mach;
	(*aut)->trans_num++;
	return;
}

int find_next_unite(char* expr, int len) {
	int open_br = 0;
	for (int i = 0; i < len; i++) {
		if (is_one_of_spec_symb(expr, i, "|") && (open_br == 0)) return i;
		else if (is_one_of_spec_symb(expr, i, "(")) open_br++;
		else if (is_one_of_spec_symb(expr, i, ")")) open_br--;
	}
	return -1;
}

int find_close_bracket(char* expr, int open_br_ind) {
	int open_br = 1;
	for (int i = open_br_ind + 1;; i++) {
		if (is_one_of_spec_symb(expr, i, "(")) open_br++;
		else if (is_one_of_spec_symb(expr, i, ")")) {
			open_br--;
			if (open_br == 0) return i;
		}
	}
}

void add_trans_inside_br(Automate** aut, char* expr, int len, int inner, int outer) {
	int i = 0;
	while (i < len) {
		if ((expr[i] == '\\') && (is_one_of_spec_symb(expr, i, "\\"))) {
			i++;
			continue;
		}
		if (is_one_of_spec_symb(expr, i, "-")) {
			for (char j = expr[i - 1] + 1; j <= expr[i + 1]; j++) {
				add_transition(aut, inner, j, outer);
				if (*aut == NULL) return;
			}
			i++;
		}
		else {
			add_transition(aut, inner, expr[i], outer);
			if (*aut == NULL) return;
		}
		i++;
	}
	return;
}

int count_inside_br(char* expr) {
	int i = 0;
	while (expr[i] != ']') i++;
	return i;
}

void f1(Automate** aut, char* expr, int len, int inner, int outer) {
	if (len == 1) {
		add_transition(aut, inner, expr[0], outer);
		return;
	}
	int unite_simb = find_next_unite(expr, len);
	if (unite_simb != -1) {
		add_transition(aut, inner, 0, (*aut)->state_num);
		if (*aut == NULL) return;
		add_transition(aut, (*aut)->state_num + 1, 0, outer);
		if (*aut == NULL) return;
		(*aut)->state_num += 2;
		f1(aut, expr, unite_simb, (*aut)->state_num - 2, (*aut)->state_num - 1);
		if (*aut == NULL) return;
		add_transition(aut, inner, 0, (*aut)->state_num);
		if (*aut == NULL) return;
		add_transition(aut, (*aut)->state_num + 1, 0, outer);
		if (*aut == NULL) return;
		(*aut)->state_num += 2;
		f1(aut, &expr[unite_simb + 1], len - unite_simb - 1, (*aut)->state_num - 2, (*aut)->state_num - 1);
		return;
	}
	int curr_st = inner;
	int next_st;
	int new_len;
	char* new_expr;
	int i = 0;
	while (i < len) {
		if ((expr[i] == '\\') && (is_one_of_spec_symb(expr, i, "\\"))) {
			i++;
			continue;
		}
		if (is_one_of_spec_symb(expr, i, "[")) {
			int l = count_inside_br(&expr[i + 1]);
			if (i + l + 2 == len) next_st = -1;
			else {
				next_st = (*aut)->state_num;
				(*aut)->state_num++;
			}
			add_trans_inside_br(aut, &expr[i + 1], l, curr_st, next_st);
			curr_st = next_st;
			i += l + 2;
			continue;
		}
		if (is_one_of_spec_symb(expr, i, "(")) {
			int close_br = find_close_bracket(expr, i);
			new_len = close_br - i - 1;
			new_expr = &expr[i + 1];
			i = close_br + 1;
		}
		else {
			new_len = 1;
			new_expr = &expr[i];
			i++;
		}
		if (i == len) next_st = outer;
		else {
			next_st = (*aut)->state_num;
			(*aut)->state_num++;
		}
		f1(aut, new_expr, new_len, curr_st, next_st);
		curr_st = next_st;
	}
}

int comp(Transition t1, Transition t2) {
	if (t1.current_state < t2.current_state) return 0;
	if (t1.current_state > t2.current_state) return 1;
	if (t1.symb < t2.symb) return 0;
	if (t1.symb > t2.symb) return 1;
	if (t1.next_state < t2.next_state) return 0;
	return 1;
}

void sort_mach(Transition* mach, int size) {
	Transition aux;
	for (int i = 0; i < size - 1; i++)
		for (int j = 0; j < size - i - 1; j++) 
			if (comp(mach[j], mach[j + 1])) {
				aux = mach[j];
				mach[j] = mach[j + 1];
				mach[j + 1] = aux;
			}
}

Automate* construct_NFA(char* expr, int len) {
	Automate* aut = (Automate*)malloc(sizeof(Automate));
	if (aut == NULL) return NULL;
	*aut = (Automate){ NULL, 1, 0 };
	f1(&aut, expr, len, 0, -1);
	if (aut == NULL) return NULL;
	aut->state_num++;
	sort_mach(aut->machine, aut->trans_num);
	return aut;
}

void destruct_NFA(Automate* aut) {
	free(aut->machine);
	free(aut);
}

/*typedef struct State {
	State* next_state[2];
	char* trans_symb[2];
	int symb_num[2];
	int income_ptr;
} State;

int find_next_un(char* expr, int len, char s) {
	for (int i = 1; i < len - 1; i++) {
		if ((expr[i] == s) && ((i = 0) || (expr[i - 1] != '\\') || ((i >= 2) && (expr[i - 2]) == '\\')))
			return i;
	}
	return -1;
}

State* f1(char* expr, int len, State* start) {
	int stat = 0;
	int unite_simb = find_next_un(expr, len, '|');
	if (unite_simb != -1) {
		start->next_state[0] = (State*)calloc(1, sizeof(State));
		start->next_state[1] = (State*)calloc(1, sizeof(State));
		start->next_state[0]->income_ptr = start->next_state[0]->income_ptr = 1;
		State* fin_0 = f1(expr, unite_simb, start->next_state[0]);
		State* fin_1 = f1(&expr[unite_simb + 1], len - unite_simb - 1, start->next_state[1]);
		State* common_fin = (State*)calloc(1, sizeof(State));
		fin_0->next_state[0] = fin_1->next_state[0] = common_fin;
		common_fin->income_ptr = 2;
		return common_fin;
	} else {
		State* curr_st = start;
		for (int i = 0; i < len; i++) {
			if ((expr[i] == '\\') && ((i == 0) || (expr[i - 1] == '\\'))) continue;
			curr_st->symb_num[0] = 1;
			curr_st->trans_symb[0] = (char*)malloc(sizeof(char));
			*(curr_st->trans_symb[0]) = expr[i];
			curr_st->next_state[0] = (State*)calloc(sizeof(State));
			curr_st = curr_st->next_state[0];
			curr_st->income_ptr = 1;
		}
		return curr_st;
	}
}

State* NFA_main(char* expr, int len) {
	State* inner = (State*)calloc(sizeof(State));
	f1(expr, len, inner);
	return inner;
}

void delete_NFA(State* inner) {
	State* st1 = inner->next_state[0]; 
	State* st2 = inner->next_state[1];
	if (inner->income_ptr == 0) {
		if (inner->symb_num[0] != 0) free(inner->trans_symb[0]);
		if (inner->symb_num[1] != 0) free(inner->trans_symb[1]);
		free(inner);
	}
	if (st1 != NULL) {
		st1->income_ptr--;
		if (st2 != NULL) {
			st2->income_ptr--;
			delete_NFA(st1);
			delete_NFA(st2);
		}
		else 
	}
}*/