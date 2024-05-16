The goal of the work is to master the skills of assembling function prototypes, coding functions using pointers, vectors, and testing them.
2.2. Job assignment. (16.10.2020)
Remark. All functions operate EXCLUSIVELY WITH FIELDS of the form name = value

1. Write the function error_t argz_create_sep (const char *string, int sep, char **argz, size_t *argz_len). The function converts a null-terminated string to an argz vector, breaking the string into elements each time the sep character is encountered. The vector argz is a null-terminated string in which the elements are separated by the character ’\0’. Elements of a vector are pairs
name=value
Example,
char *const string = "SHELL=/bin/bash:usr=monty:PWD=/bin/monty:LANG=en_US.UTF-8";
char *argz;
int argz_len;

argz_create_sep (string, 58/*ASCII for : */, &argz, &argz_len);

As a result of the call, the vector will take the form
SHELL=/bin/bash'\0'usr=monty'\0'PWD=/bin/monty'\0'LANG=en_US.UTF-8'\0'
and argz_len will be equal to 58.

2. Write the function size_t argz_count (const char *argz, size_t arg_len), which returns the number of argz elements. For the previous example, the result would be 4.

3. Write a function error_t argz_add (char **argz, size_t *argz_len, const char *str) that adds the string str to the end of argz, modifying argz and argz_len accordingly.

4. Write a void argz_delete (char **argz, size_t *argz_len, char *entry) function that deletes the entry element from argz, modifying argz and argz_len accordingly.

5. Write a function error_t argz_insert (char **argz, size_t *argz_len, char *before, const char *entry), which inserts the entry line before the before element, modifying argz and argz_len accordingly.

6. Write the function char * argz_next (char *argz, size_t argz_len, const char *entry), which provides the ability to move by argz. It returns a pointer to the argz element that follows the entry element, or 0 if there is none. If entry is 0, a pointer to the first element of argz is returned. Thus, one of two ways to access the elements of argz is provided

char *entry = 0;
while ((entry = argz_next (argz, argz_len, entry)))
or action;
char *entry;
for (entry = argz; entry; entry = argz_next (argz, argz_len, entry))
actions;

7. Write a function error_t argz_replace(char **argz, size_t *argz_len, const char *str, const char *with), which replaces the string str with the element with, modifying argz and argz_len accordingly.

8. Write the function void argz_print(const char *argz, size_t argz_len) to print argz.

The error_t type is defined as follows
typedef enum {OK, ERROR} error_t;
Then. the first constant (or 0) is returned in case of success and the second (1) in the opposite case.
