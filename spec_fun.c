/*int is_one_of_spec_symb(char* expr, char s, int ind) {
	if (expr[ind] != s) return 0;
	for (int i = 1; ind - i > 0; i++)
		if (expr[ind - i] != '\\') return i % 2;
	return (ind + 1) % 2;
}*/

int is_one_of_spec_symb(char* expr, int ind, char* symbols) {
	for (int j = 0; symbols[j] != '\0'; j++) {
		if (expr[ind] != symbols[j]) continue;
		int flag = 1;
		for (int i = 1; ind - i >= 0; i++)
			if (expr[ind - i] != '\\') {
				if (i % 2 == 1) return 1;
				else {
					flag = 0;
					break;
				}
			}
		if ((flag == 1) && ((ind + 1) % 2 == 1)) return 1;
	}
	return 0;
}