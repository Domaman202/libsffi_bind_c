#include <stdio.h>
#include <stdlib.h>

#include <sffi.h>

#define TLIB_FILE "./testlib.so"

sffi_lib_t tlib_open() {
	sffi_lib_t lib;
	if (sffi_error_t err = sffi_lib_open(&lib, TLIB_FILE)) {
		printf("Error: %s\n", sffi_error_msg(err));
		sffi_error_free(err);
		abort();
	}
	return lib;
}

sffi_symbol_t tlib_symbol(sffi_lib_t lib, const char* name) {
	sffi_symbol_t symbol;
	if (sffi_error_t err = sffi_lib_symbol(&symbol, lib, name)) {
		printf("Error: %s\n", sffi_error_msg(err));
		sffi_error_free(err);
		abort();
	}
	return symbol;
}

sffi_func_t tlib_func(sffi_lib_t lib, const char* name, const char* desc) {
	sffi_func_t func;
	if (sffi_error_t err = sffi_lib_func(&func, lib, name, desc)) {
		printf("Error: %s\n", sffi_error_msg(err));
		sffi_error_free(err);
		abort();
	}
	return func;
}

sffi_struct_type_t tlib_struct(const char* desc) {
	sffi_struct_type_t struct_type;
	if (sffi_error_t err = sffi_struct_type_alloc(&struct_type, desc)) {
		printf("Error: %s\n", sffi_error_msg(err));
		sffi_error_free(err);
		abort();
	}
	return struct_type;
}

sffi_adapter_t tlib_adapter(const char* desc) {
	sffi_adapter_t adapter;
	if (sffi_error_t err = sffi_adapter_parse(&adapter, desc)) {
		printf("Error: %s\n", sffi_error_msg(err));
		sffi_error_free(err);
		abort();
	}
	return adapter;
}

void tlib_adapter_call(const sffi_adapter_t adapter, sffi_func_t func, void* rvalue, uint_t argc, void** argv) {
	if (sffi_error_t err = sffi_adapter_call(adapter, func, rvalue, argc, argv)) {
		printf("Error: %s\n", sffi_error_msg(err));
		sffi_error_free(err);
		abort();	
	}
}

void tlib_adapter_set(const sffi_adapter_t adapter, const sffi_struct_type_t struct_type, void* structure, uint_t index, const void* avalue) {
	if (sffi_error_t err = sffi_adapter_set(adapter, struct_type, structure, index, avalue)) {
		printf("Error: %s\n", sffi_error_msg(err));
		sffi_error_free(err);
		abort();
	}
}

void tlib_adapter_get(const sffi_adapter_t adapter, const sffi_struct_type_t struct_type, const void* structure, uint_t index, void* rvalue) {
	if (sffi_error_t err = sffi_adapter_get(adapter, struct_type, structure, index, rvalue)) {
		printf("Error: %s\n", sffi_error_msg(err));
		sffi_error_free(err);
		abort();
	}
}

void tlib_close(sffi_lib_t lib) {
	sffi_lib_close(lib);
}


void test_symbol(sffi_lib_t lib) {
	sffi_symbol_t symbol = tlib_symbol(lib, "puts");
	((void (*)(const char*)) symbol)("(symbol call)\t Hello, Symbol!");
}

void test_func_call_a(sffi_lib_t lib) {
	sffi_func_t func = tlib_func(lib, "puts", "(&str)void");
	const char* str = "(func call)\t Hello, Function!";
	void* args[] = { &str };
	sffi_func_call(func, NULL, args);
}

void test_func_call_b(sffi_lib_t lib) {
	sffi_func_t func = tlib_func(lib, "add", "(i32,i32)i32");
	int32_t result = 0;
	int32_t a = 12;
	int32_t b = 21;
	void* args[] = { &a, &b };
	sffi_func_call(func, &result, args);
	printf("(func call)\t %d\n", result);
}

void test_adapter_call(sffi_lib_t lib) {
	sffi_func_t func = tlib_func(lib, "add", "(i32,i32)i32");
	sffi_adapter_t adapter = tlib_adapter("(f32,f32)f32");
	float result = 0;
	float a = 1.444;
	float b = 2.333;
	void* args[] = { &a, &b };
	tlib_adapter_call(adapter, func, &result, 2, args);
	sffi_adapter_free(adapter);
	printf("(adapter call)\t %f\n", result);
}

void test_struct_raw(sffi_lib_t lib) {
	sffi_func_t func = tlib_func(lib, "sadd", "([i32,i32,i32])void");
	sffi_struct_type_t struct_type = tlib_struct("[i32,i32,i32]");
	void* structure = sffi_struct_calloc(struct_type);
	int32_t a = 4;
	int32_t b = 5;
	sffi_struct_set_raw(struct_type, structure, 0, &a);
	sffi_struct_set_raw(struct_type, structure, 1, &b);
	void* args[] = { &structure };
	sffi_func_call(func, NULL, args);
	int32_t r = 0;
	sffi_struct_get_raw(struct_type, structure, 2, &r);
	sffi_struct_free(structure);
	sffi_struct_type_free(struct_type);
	printf("(struct raw)\t %d\n", r);
}

void test_struct_adapter(sffi_lib_t lib) {
	sffi_func_t func = tlib_func(lib, "sadd", "([i32,i32,i32])void");
	sffi_struct_type_t struct_type = tlib_struct("[i32,i32,i32]");
	sffi_adapter_t adapter = tlib_adapter("[f32,f32,f32]");
	void* structure = sffi_struct_calloc(struct_type);
	float a = 6.12;
	float b = 4.21;
	tlib_adapter_set(adapter, struct_type, structure, 0, &a);
	tlib_adapter_set(adapter, struct_type, structure, 1, &b);
	void* args[] = { &structure };
	sffi_func_call(func, NULL, args);
	float r = 0.0;
	tlib_adapter_get(adapter, struct_type, structure, 2, &r);
	sffi_struct_free(structure);
	sffi_adapter_free(adapter);
	sffi_struct_type_free(struct_type);
	printf("(struct adapter) %f\n", r);
}

int main() {
	sffi_lib_t lib = tlib_open();
	test_symbol(lib);
	test_func_call_a(lib);
	test_func_call_b(lib);
	test_adapter_call(lib);
	test_struct_raw(lib);
	test_struct_adapter(lib);
	tlib_close(lib);
}
