#include <stdint.h>

int32_t add(int32_t a, int32_t b) {
	return a + b;
}

float fadd(float a, float b) {
	return a + b;
}

typedef struct {
	int32_t a;
	int32_t b;
	int32_t r;
} data_t;

void sadd(data_t* data) {
	data->r = data->a + data->b;
}
