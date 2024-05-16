#include "argz.h"

int main() {
	char string[] = "DASHA:LUBIMUY:LOH";
	char* argz = NULL;
	char* entry = NULL;
	size_t argz_len = 0;

	printf("\nargz_create_sep('DASHA:LUBIMUY:LOH', ':', &argz, &argz_len):\n");
	argz_create_sep(string, ':', &argz, &argz_len);
	argz_print(argz, argz_len);
	printf("argz_len = %u\n", argz_len);

	printf("\nargz_count(argz, argz_len): %u\n", argz_count(argz, argz_len));

	printf("\nargz_add(&argz, &argz_len, '/the end'):\n");
	argz_add(&argz, &argz_len, "/the end");
	argz_print(argz, argz_len);
	printf("argz_len = %u\n", argz_len);

	printf("\nargz_delete(&argz, &argz_len, 'SHELL=/bin/bash'):\n");
	argz_delete(&argz, &argz_len, "H");
	argz_print(argz, argz_len);
	printf("argz_len = %u\n", argz_len);

	printf("\nargz_insert(&argz, &argz_len, argz + 10, '/insertion'):\n");
	argz_insert(&argz, &argz_len, argz + 10, "/insertion");
	argz_print(argz, argz_len);
	printf("argz_len = %u\n", argz_len);

	printf("\nargz_next(argz, argz_len, entry):\n");
	while ((entry = argz_next(argz, argz_len, entry)))
		printf("%s |0\n", entry);

	printf("\nargz_replace(&argz, &argz_len, 'PWD=/bin/monty', '/replaced'):\n");
	argz_replace(&argz, &argz_len, "W", "/replaced");
	argz_print(argz, argz_len);
	printf("argz_len = %u\n", argz_len);

	free(argz);
	return 0;
}
