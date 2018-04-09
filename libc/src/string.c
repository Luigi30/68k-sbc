#include <string.h>

size_t strlen(const char *str){
	size_t size = 0;

	while(*str != 0x00){
		str++;
		size++;
	}

	return size;
}
