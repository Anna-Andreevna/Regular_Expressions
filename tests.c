#include <stdio.h>
#include "spec_fun.h"
#include "correct_expression.h"
#include "NFA.h"

int test_num = 0;
int successful_test_num = 0;

#define ASSERT_EQUAL_INT(expected, actual) do {    \
		int exp = (expected); int act = (actual);  \
		test_num++;	\
		if (exp != act) printf("TEST FAILED: %d != %d at line %s:%d\n", \
			expected, actual, __FILE__, __LINE__); \
		else successful_test_num++; } while(0)

#define ASSERT_EQUAL_TRANS(ex1, ex2, ex3, actual) do {  \
		test_num++; \
		if ((ex1 != actual.current_state) || (ex2 != actual.symb) || (ex3 != actual.next_state)) \
			printf("TEST FAILED: {%d, %c, %d} != {%d, %c, %d} at line %s:%d\n", \
			ex1, ex2, ex3, \
			actual.current_state, actual.symb, actual.next_state, __FILE__, __LINE__); \
		else successful_test_num++; } while(0)

void test_spec_fun() {
	ASSERT_EQUAL_INT(1, is_one_of_spec_symb("as\\\\)", 4, ")"));
	ASSERT_EQUAL_INT(1, is_one_of_spec_symb("\\\\\\\\(qwe", 4, "("));
	ASSERT_EQUAL_INT(0, is_one_of_spec_symb("a\\|sh", 2, "|"));
	ASSERT_EQUAL_INT(1, is_one_of_spec_symb("()", 0, "("));
	ASSERT_EQUAL_INT(0, is_one_of_spec_symb("a\\\\\\\\\\(", 6, "("));
	ASSERT_EQUAL_INT(1, is_one_of_spec_symb("asd|sd", 3, "*(|)"));
	ASSERT_EQUAL_INT(0, is_one_of_spec_symb("asd|sd", 3, "*()"));
	ASSERT_EQUAL_INT(0, is_one_of_spec_symb("\\|sd", 1, "*(|"));
}

void test_correct_expr() {
	ASSERT_EQUAL_INT(0, check_expr("()(([[]][]))", 12));
	ASSERT_EQUAL_INT(1, check_expr("(aaa\\()[ddd]", 12));
	ASSERT_EQUAL_INT(0, check_expr("((d)))[f]", 7));
	ASSERT_EQUAL_INT(0, check_expr("", 0));
	ASSERT_EQUAL_INT(0, check_expr("((d)[e]", 5));
	ASSERT_EQUAL_INT(0, check_expr("|a", 2));
	ASSERT_EQUAL_INT(0, check_expr("b|", 2));
	ASSERT_EQUAL_INT(1, check_expr("(a|b)|ab(c|d)", 13));
	ASSERT_EQUAL_INT(0, check_expr("(|b)", 4));
	ASSERT_EQUAL_INT(1, check_expr("\\(|b", 4));
	ASSERT_EQUAL_INT(1, check_expr("as[0-9as]dj", 11));
	ASSERT_EQUAL_INT(0, check_expr("df|r[qw|g]o(d|f)", 16));
}

void test_1() {
	Automate* aut = construct_NFA("ab|ac|abc", 9);
	ASSERT_EQUAL_INT(15, aut->trans_num);
	ASSERT_EQUAL_INT(14, aut->state_num);
	ASSERT_EQUAL_TRANS(0, 0, 1, aut->machine[0]);
	ASSERT_EQUAL_TRANS(0, 0, 4, aut->machine[1]);
	ASSERT_EQUAL_TRANS(1, 'a', 3, aut->machine[2]);
	ASSERT_EQUAL_TRANS(2, 0, -1, aut->machine[3]);
	ASSERT_EQUAL_TRANS(3, 'b', 2, aut->machine[4]);
	ASSERT_EQUAL_TRANS(4, 0, 6, aut->machine[5]);
	ASSERT_EQUAL_TRANS(4, 0, 9, aut->machine[6]);
	ASSERT_EQUAL_TRANS(5, 0, -1, aut->machine[7]);
	ASSERT_EQUAL_TRANS(6, 'a', 8, aut->machine[8]);
	ASSERT_EQUAL_TRANS(7, 0, 5, aut->machine[9]);
	ASSERT_EQUAL_TRANS(8, 'c', 7, aut->machine[10]);
	ASSERT_EQUAL_TRANS(9, 'a', 11, aut->machine[11]);
	ASSERT_EQUAL_TRANS(10, 0, 5, aut->machine[12]);
	ASSERT_EQUAL_TRANS(11, 'b', 12, aut->machine[13]);
	ASSERT_EQUAL_TRANS(12, 'c', 10, aut->machine[14]);
	ASSERT_EQUAL_INT(1, main_comp("cfdacbcdf", aut));
	ASSERT_EQUAL_INT(0, main_comp("", aut));
	ASSERT_EQUAL_INT(1, main_comp("cfdabcdf", aut));
	ASSERT_EQUAL_INT(0, main_comp("cd", aut));
	destruct_NFA(aut);
}

void test_2() {
	Automate* aut = construct_NFA("a(b|a)b|f", 9);
	ASSERT_EQUAL_INT(13, aut->trans_num);
	ASSERT_EQUAL_INT(12, aut->state_num);
	ASSERT_EQUAL_TRANS(0, 0, 1, aut->machine[0]);
	ASSERT_EQUAL_TRANS(0, 0, 9, aut->machine[1]);
	ASSERT_EQUAL_TRANS(1, 'a', 3, aut->machine[2]);
	ASSERT_EQUAL_TRANS(2, 0, -1, aut->machine[3]);
	ASSERT_EQUAL_TRANS(3, 0, 5, aut->machine[4]);
	ASSERT_EQUAL_TRANS(3, 0, 7, aut->machine[5]);
	ASSERT_EQUAL_TRANS(4, 'b', 2, aut->machine[6]);
	ASSERT_EQUAL_TRANS(5, 'b', 6, aut->machine[7]);
	ASSERT_EQUAL_TRANS(6, 0, 4, aut->machine[8]);
	ASSERT_EQUAL_TRANS(7, 'a', 8, aut->machine[9]);
	ASSERT_EQUAL_TRANS(8, 0, 4, aut->machine[10]);
	ASSERT_EQUAL_TRANS(9, 'f', 10, aut->machine[11]);
	ASSERT_EQUAL_TRANS(10, 0, -1, aut->machine[12]);
	ASSERT_EQUAL_INT(1, main_comp("aaabcd", aut));
	ASSERT_EQUAL_INT(1, main_comp("fun", aut));
	ASSERT_EQUAL_INT(1, main_comp("afti", aut));
	ASSERT_EQUAL_INT(0, main_comp("aaac", aut));
	ASSERT_EQUAL_INT(0, main_comp("abcde", aut));
	destruct_NFA(aut);
}

void test_3() {
	Automate* aut = construct_NFA("[a-cA-C] = [4-7],[0-2]", 22);
	ASSERT_EQUAL_INT(17, aut->trans_num);
	ASSERT_EQUAL_INT(8, aut->state_num);
	ASSERT_EQUAL_TRANS(0, 'A', 1, aut->machine[0]);
	ASSERT_EQUAL_TRANS(0, 'B', 1, aut->machine[1]);
	ASSERT_EQUAL_TRANS(0, 'C', 1, aut->machine[2]);
	ASSERT_EQUAL_TRANS(0, 'a', 1, aut->machine[3]);
	ASSERT_EQUAL_TRANS(0, 'b', 1, aut->machine[4]);
	ASSERT_EQUAL_TRANS(0, 'c', 1, aut->machine[5]);
	ASSERT_EQUAL_TRANS(1, ' ', 2, aut->machine[6]);
	ASSERT_EQUAL_TRANS(2, '=', 3, aut->machine[7]);
	ASSERT_EQUAL_TRANS(3, ' ', 4, aut->machine[8]);
	ASSERT_EQUAL_TRANS(4, '4', 5, aut->machine[9]);
	ASSERT_EQUAL_TRANS(4, '5', 5, aut->machine[10]);
	ASSERT_EQUAL_TRANS(4, '6', 5, aut->machine[11]);
	ASSERT_EQUAL_TRANS(4, '7', 5, aut->machine[12]);
	ASSERT_EQUAL_TRANS(5, ',', 6, aut->machine[13]);
	ASSERT_EQUAL_TRANS(6, '0', -1, aut->machine[14]);
	ASSERT_EQUAL_TRANS(6, '1', -1, aut->machine[15]);
	ASSERT_EQUAL_TRANS(6, '2', -1, aut->machine[16]);
	ASSERT_EQUAL_INT(1, main_comp("  A = 7,2+56", aut));
	ASSERT_EQUAL_INT(1, main_comp("c = 6,0", aut));
	ASSERT_EQUAL_INT(1, main_comp("asdfb = 5,1", aut));
	ASSERT_EQUAL_INT(0, main_comp("S = 4,2", aut));
	ASSERT_EQUAL_INT(0, main_comp("a = 3,1", aut));
	destruct_NFA(aut);
}



void test_main() {
	test_spec_fun();
	test_correct_expr();
	test_1();
	test_2();
	test_3();
	printf("%d / %d tests are successful\n", successful_test_num, test_num);
}