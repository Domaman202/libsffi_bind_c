#ifndef __LIB_SFFI_H__
#define __LIB_SFFI_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned int uint_t;

typedef void* 		  sffi_error_t;
typedef void* 		  sffi_lib_t;
typedef const void* sffi_symbol_t;
typedef const void* sffi_func_t;
typedef void* 		  sffi_struct_type_t;
typedef void* 		  sffi_adapter_t;

extern uint_t			    sffi_error_code		    (const 	  sffi_error_t error);
extern const char*    sffi_error_msg			  (const    sffi_error_t error);
extern void 			    sffi_error_free		    (         sffi_error_t error);

extern sffi_error_t 	sffi_lib_open			    (                       sffi_lib_t*	  lib, const char* name);
extern sffi_error_t 	sffi_lib_symbol 		  (sffi_symbol_t* symbol, sffi_lib_t		lib, const char* name);
extern sffi_error_t 	sffi_lib_func 			  (sffi_func_t*	  func,	  sffi_lib_t		lib, const char* name, const char* desc);
extern void				    sffi_lib_close			  (sffi_lib_t 	  lib);

extern void				    sffi_func_call			  (sffi_func_t func, void* rvalue, void**   avalue);
#ifdef LIBFFI_H
extern void				    sffi_func_call_raw		(sffi_func_t func, void* rvalue, ffi_raw* avalue);
#endif

extern sffi_error_t		sffi_struct_type_alloc(      sffi_struct_type_t*  struct_type, const char* 	desc);
extern uint_t 			  sffi_struct_type_size	(const sffi_struct_type_t 	struct_type);
extern void				    sffi_struct_type_free	(const sffi_struct_type_t 	struct_type);
extern void*			    sffi_struct_malloc		(const sffi_struct_type_t 	struct_type);
extern void*			    sffi_struct_calloc		(const sffi_struct_type_t 	struct_type);
extern void				    sffi_struct_set_raw		(const sffi_struct_type_t 	struct_type,       void* 	structure, uint_t index, const void* 	avalue);
extern void				    sffi_struct_get_raw		(const sffi_struct_type_t 	struct_type, const void*	structure, uint_t index,       void* 	rvalue);
extern void				    sffi_struct_free		  (                                              void*   structure);

extern sffi_error_t		sffi_adapter_parse		(      sffi_adapter_t* 	adapter, const char* 				desc);
extern sffi_error_t		sffi_adapter_call		  (const sffi_adapter_t 	adapter, sffi_func_t 				func, 	                    void* rvalue, 	  uint_t argc, 	        void** 	argv);
extern sffi_error_t		sffi_adapter_set		  (const sffi_adapter_t 	adapter, const sffi_struct_type_t 	struct_type, 	      void* structure, 	uint_t index, 	const void*   avalue);
extern sffi_error_t		sffi_adapter_get		  (const sffi_adapter_t 	adapter, const sffi_struct_type_t 	struct_type, 	const void* structure, 	uint_t index, 	      void* 	rvalue);
extern void				    sffi_adapter_free		  (      sffi_adapter_t 		adapter);


#endif /*  __LIB_SFFI_H__ */
