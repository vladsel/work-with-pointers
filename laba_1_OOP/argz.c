/************************************************************************
*file: argz.c
*synopsis: The argz functions use malloc/realloc to allocate/grow argz vectors, and so any argz
* vector creating using these functions may be freed by using free; conversely, any argz
* function that may grow a string expects that string to have been allocated using malloc
* (those argz functions that only examine their arguments or modify them in place will work
* on any sort of memory). All argz functions that do memory allocation have a return type
* of error_t, and return 0 for success, and ENOMEM if an allocation error occurs.
* These functions are declared in the include file "argz.h".
*related files: none
*author: Seletkov Vlad
*written: 24/09/2020
*last modified: 15/10/2020
************************************************************************/

#include "argz.h"

#pragma warning(disable: 6386)

/*
The argz_create_sep function converts the null-terminated string string into an
argz vector (returned in argz and argz len) by splitting it into elements at every
occurrence of the character sep.
*/
error_t argz_create_sep(const char* string, int sep, char** argz, size_t* argz_len) {
	*argz_len = strlen(string) + 1;
	if (*argz_len == 0 || (sep <= 0 && sep >= 256))
		return ERROR;
	size_t i = 0;
	*argz = (char*)malloc(*argz_len * sizeof(char));
	if (*argz == NULL)
		return ERROR;
	for (i = 0; i < *argz_len; i++) {
		if (string[i] == sep)
			*((*argz) + i) = '\0';
		else
			*((*argz) + i) = string[i];
	}
	return OK;
}

//Returns the number of elements in the argz vector.
size_t argz_count(const char* argz, size_t arg_len) {
	if (argz == NULL)
		return ERROR;
	size_t counter = 0;
	for (size_t i = 0; i < arg_len; i++) {
		if (argz[i] == '\0')
			counter++;
	}
	return counter;
}

//The argz_add function adds the string str to the end of the argz vector 
//*argz, and updates *argz and *argz_len accordingly.
error_t argz_add(char** argz, size_t* argz_len, const char* str) {
	if (str == NULL || *argz == NULL || *argz_len == 0)
		return ERROR;
	size_t str_len = strlen(str) + 1;
	char* ptr = NULL;
	ptr = (char*)malloc((*argz_len + str_len) * sizeof(char));
	if (ptr == NULL)
		return ERROR;
	for (size_t i = 0; i < *argz_len; i++)
		ptr[i] = *((*argz) + i);
	for (size_t i = *argz_len; i < str_len + *argz_len; i++)
		ptr[i] = str[i - *argz_len];
	*argz_len += str_len;
	free(*argz);
	(*argz) = ptr;
	return OK;
}

/*
If entry points to the beginning of one of the elements in the argz
vector *argz, the argz_delete function will remove this entry and
reallocate *argz, modifying *argz and *argz_len accordingly. Note that as
destructive argz functions usually reallocate their argz argument,
pointers into argz vectors such as entry will then become invalid.
*/
void argz_delete(char** argz, size_t* argz_len, char* entry) {
	if (entry == NULL || *argz == NULL || *argz_len == 0)
		return;
	size_t entry_len = 0, i = 0, j = 0, flag = 1;
	char* ptr = NULL;
	entry_len = strlen(entry) + 1;
	for (i = 0; i < *argz_len; i++) {
		if (i == 0 || *((*argz) + i) == '\0') {
			flag = 1;
			if (i != 0)
				i++;
			j = i;
			while (entry[j - i] != '\0') {
				if (*((*argz) + j) == entry[j - i])
					;
				else
					flag = 0;
				j++;
			}
		}
		if (flag == 0) {
			while (*((*argz) + i) != '\0')
				i++;
			i--;
		}
		if (flag == 1) {
			for (size_t k = 0; k < entry_len; k++)
				for (size_t l = 0; l < *argz_len - i - k; l++)
					*((*argz) + i + l) = *((*argz) + i + l + 1);
			ptr = (char*)malloc((*argz_len - entry_len + 1) * sizeof(char));
			if (ptr == NULL)
				return;
			for (size_t k = 0; k < *argz_len - entry_len; k++)
				ptr[k] = *((*argz) + k);
			free(*argz);
			*argz = ptr;
			*argz_len -= entry_len;
			if (i == 0)
				i--;
			else
				i -= 2;
		}
	}
}

/*
The argz_insert function inserts the string entry into the argz vector
*argz at a point just before the existing element pointed to by before,
reallocating *argz and updating *argz and *argz_len. If before is 0,
entry is added to the end instead (as if by argz_add). Since the first
element is in fact the same as *argz, passing in *argz as the value of
before will result in entry being inserted at the beginning.
*/
error_t argz_insert(char** argz, size_t* argz_len, char* before, const char* entry) {
	if (*argz == NULL || entry == NULL || before == NULL || *argz_len == 0)
		return ERROR;
	size_t i = 0, j = 0, flag = 0, entry_len = 0;
	entry_len = strlen(entry) + 1;
	char* ptr = NULL;
	for (i = 0; i < *argz_len; i++) {
		if (i == 0 || *((*argz) + i) == '\0') {
			flag = 1;
			if (i != 0)
				i++;
			j = i;
			while (before[j - i] != '\0') {
				if (*((*argz) + j) == before[j - i])
					;
				else
					flag = 0;
				j++;
			}
		}
		if (flag == 0) {
			while (*((*argz) + i) != '\0')
				i++;
			i--;
		}
		if (flag == 1) {
			*argz_len += entry_len;
			ptr = (char*)malloc(*argz_len * sizeof(char));
			if (ptr == NULL)
				return ERROR;
			for (size_t k = 0; k < *argz_len - entry_len; k++)
				ptr[k] = *((*argz) + k);
			for (size_t k = 0; k < entry_len; k++)
				for (size_t l = 0; l < *argz_len - i - k; l++)
					ptr[*argz_len - l - 1] = ptr[*argz_len - l - 2];
			for (size_t k = 0; k < entry_len; k++)
				ptr[i + k] = entry[k];
			free(*argz);
			(*argz) = ptr;
			if (i == 0)
				i--;
			else
				i -= 2;
		}
	}
	return OK;
}

/*
The argz_next function provides a convenient way of iterating over
the elements in the argz vector argz.
It returns a pointer to the next element in argz
after the element entry, or 0 if there are no elements following entry.
*/
char* argz_next(char* argz, size_t argz_len, const char* entry) {
	if (entry == NULL)
		return argz;
	while (entry - argz < argz_len - 1) {
		if (*entry == '\0')
			return entry + 1;
		entry++;
	}
	return OK;
}

/*
Replace the string str in argz with string with, reallocating argz as
necessary.
*/
error_t argz_replace(char** argz, size_t* argz_len, const char* str, const char* with) {
	if (*argz == NULL || str == NULL || with == NULL || *argz_len == 0)
		return ERROR;
	size_t i = 0, j = 0, flag = 0, str_len = 0, with_len = 0;
	char* ptr = NULL;
	str_len = strlen(str) + 1;
	with_len = strlen(with) + 1;
	for (i = 0; i < *argz_len; i++) {
		if (i == 0 || *((*argz) + i) == '\0') {
			flag = 1;
			if (i != 0)
				i++;
			j = i;
			while (str[j - i] != '\0') {
				if (*((*argz) + j) == str[j - i])
					;
				else
					flag = 0;
				j++;
			}
		}
		if (flag == 0) {
			while (*((*argz) + i) != '\0')
				i++;
			i--;
		}
		if (flag == 1) {
			for (size_t k = 0; k < str_len; k++)
				for (size_t l = 0; l < *argz_len - i - k; l++)
					*((*argz) + i + l) = *((*argz) + i + l + 1);
			*argz_len -= str_len;
			*argz_len += with_len;
			ptr = (char*)malloc((*argz_len) * sizeof(char));
			if (ptr == NULL)
				return;
			for (size_t k = 0; k < *argz_len - with_len; k++)
				ptr[k] = *((*argz) + k);
			for (size_t k = 0; k < with_len; k++)
				for (size_t l = 0; l < *argz_len - i - k; l++)
					ptr[*argz_len - l - 1] = ptr[*argz_len - l - 2];
			for (size_t k = 0; k < with_len; k++)
				ptr[i + k] = with[k];
			free(*argz);
			(*argz) = ptr;
			if (i == 0)
				i--;
			else
				i -= 2;
		}
	}
	return OK;
}

/*prints argz vector */
void argz_print(const char* argz, size_t argz_len) {
	size_t i = 0, j = 0;
	char* ptr = argz;
	while (i < argz_len) {
		printf("%s |0\n", ptr);
		j = strlen(ptr) + 1;
		ptr += j;
		i += j;
	}
}
