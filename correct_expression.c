#include <stdio.h>
#include <stdlib.h>
#include "spec_fun.h"

// корректность внутри скобок

int check_inside_brackets(char* expr, int len, int open_br, int* close_br) {
	if (expr[open_br + 1] == ']') return 0;
	for (int i = open_br + 1; i < len; i++) {
		if (is_one_of_spec_symb(expr, i, "]")) {
			*close_br = i;
			return 1;
		}
		if (is_one_of_spec_symb(expr, i, "()[|")) return 0;
	}
	return 0;
}

int braket_balance(char* expr, int len) {
	int stack_top = -1;
	int i = 0;
	while (i < len) {
		if (is_one_of_spec_symb(expr, i, "(")) {
			stack_top++;
			if ((i == len - 1) || (expr[i + 1] == ')')) return 0;;
		}
		else if (is_one_of_spec_symb(expr, i, "[")) {
			if (check_inside_brackets(expr, len, i, &i) == 0) return 0;
		}
		else if (is_one_of_spec_symb(expr, i, "]")) return 0;
		else if (is_one_of_spec_symb(expr, i, ")")) stack_top--;
		i++;
	}
	if (stack_top == -1)
		return 1;
	else
		return 0;
}

int check_unite(char* expr, int len) {
	if (expr[0] == '|') return 0;
	if (is_one_of_spec_symb(expr, len - 1, "|")) return 0;
	int i = 1;
	for (int i = 1; i < len - 1; i++)
		if (is_one_of_spec_symb(expr, i, "|")) {
			if (is_one_of_spec_symb(expr, i - 1, "(")) return 0;
			if (is_one_of_spec_symb(expr, i+1, ")")) return 0;
			if (expr[i + 1] == '|') return 0;
		}
	return 1;
}

int check_expr(char* expr, int len) {
	if (len == 0) return 0;
	if (check_unite(expr, len) == 0) return 0;
	if (braket_balance(expr, len) == 0) return 0;;
	return 1;
}