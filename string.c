#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

int my_strlen(char *str)
{
	size_t counter = 0;
	while (*str++ != '\0')
		counter++;
	return counter;
}

void* my_memcpy(void* dst, const void* src, size_t n)
{
	int i, m;
	unsigned long* wdst = dst;
	unsigned long* wsrc = src; 
	unsigned char* cdst, * csrc;

	for (i = 0, m = n / sizeof(long); i < m; i++) 
		* (wdst++) = *(wsrc++);                   

	cdst = (unsigned char*)wdst;
	csrc = (unsigned char*)wsrc;

	for (i = 0, m = n % sizeof(long); i < m; i++)
		* (cdst++) = *(csrc++);

	return dst;
}

typedef struct my_string
{
	uint8_t *data;
	uint8_t len;
	uint8_t rlen;
}str_t;

str_t* str_create(uint8_t length)
{
	str_t* string = malloc(sizeof(length));
	string->data = malloc(length);
	string->len = length;
	string->rlen = 0;
	return string;
}

void str_free(str_t* string)
{
	free(string->data);
	free(string);
}

uint8_t str_len(str_t* string)
{
	return string->len;
}

void print_string(str_t* string, FILE* fp)
{
	fwrite(string->data, sizeof(uint8_t), string->len, fp);
}

str_t *str_dup(const char* str)
{
	str_t* string = str_create((uint8_t)my_strlen(str));
	my_memcpy(string->data, str, string->len);
	string->rlen = string->len;
	return string;
}

str_t* str_cat(str_t* start, str_t* end)
{
	str_t *cat_string = str_create(start->len + end->len);
	my_memcpy(cat_string->data, start->data, start->len);
	my_memcpy(cat_string->data + start->len, end->data, end->len);
	cat_string->rlen = cat_string->len;
	return cat_string;
}

void str_clear(str_t* string)
{
	str_t* s = str_create(0);
	string->data = s->data;
	string->len = s->len;
}

str_t *get_string()
{
	uint8_t str[1000];
	size_t i = 0;
	while ((str[i] = getchar()) != EOF && str[i] != '\n')
	{
		if (str[i] == '\n')
			i++;
		i++;
	}
	str_t* string = str_create(i);
	my_memcpy(string->data, str, string->len);
	return string;
}

int main()
{
	str_t* string1 = get_string();
	str_t* string2 = str_dup("hello");
	str_t* string3 = str_cat(string1, string2);
	print_string(string3, stdout);
	system("pause");
	return 0;
}