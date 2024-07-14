/**
 * @file
 * This file provides the environment class (`ENV`).
 * It utilizes a couple of custom overloads to enhance usage and mimic the 
 * usage within other scripting and programming languages.
 */

#include <stdlib.h>
#include <string.h>

#include "gravity_vm.h"
#include "gravity_core.h"
#include "gravity_hash.h"
#include "gravity_utils.h"
#include "gravity_macros.h"
#include "gravity_vmmacros.h"
#include "gravity_opcodes.h"
#include "gravity_debug.h"

#include "gravity_opt_tsort.h"

#if defined(_WIN32)
#define setenv(_key, _value_, _unused)      _putenv_s(_key, _value_)
#define unsetenv(_key)                      _putenv_s(_key, "")
#endif

static gravity_class_t              *gravity_class_tsort = NULL;
static uint32_t                     refcount = 0;

static int                          argc = -1;
static gravity_list_t               *argv = NULL;

static bool gravity_tsort_get(gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(nargs)
}

static bool gravity_tsort_argc(gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
   #pragma unused(vm, args, nargs)
   RETURN_VALUE((argc != -1) ? VALUE_FROM_INT(argc) : VALUE_FROM_NULL, rindex);
}

static bool gravity_tsort_argv(gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
   #pragma unused(vm, args, nargs)
   RETURN_VALUE((argv) ? VALUE_FROM_OBJECT(argv) : VALUE_FROM_NULL, rindex);
}

// MARK: - Internals -

static void create_optional_class (void) {
    gravity_class_tsort = gravity_class_new_pair(NULL, GRAVITY_CLASS_TSORT_NAME, NULL, 0, 0);
    gravity_class_t *meta = gravity_class_get_meta(gravity_class_tsort);
    
    // .get(key) and .set(key, value)
    gravity_class_bind(meta, "nanos",	 	NEW_CLOSURE_VALUE(gravity_tsort_nanos));
    gravity_class_bind(meta, "open",	 	NEW_CLOSURE_VALUE(gravity_tsort_open));
    gravity_class_bind(meta, "close",	 	NEW_CLOSURE_VALUE(gravity_tsort_close));
    gravity_class_bind(meta, "read",	 	NEW_CLOSURE_VALUE(gravity_tsort_read));
    gravity_class_bind(meta, "write",	 	NEW_CLOSURE_VALUE(gravity_tsort_write));
    gravity_class_bind(meta, "position",	NEW_CLOSURE_VALUE(gravity_tsort_position));
    gravity_class_bind(meta, "config",	 	NEW_CLOSURE_VALUE(gravity_tsort_config));
    gravity_class_bind(meta, "delete",	 	NEW_CLOSURE_VALUE(gravity_tsort_delete));
    gravity_class_bind(meta, "free",	 	NEW_CLOSURE_VALUE(gravity_tsort_free));
    gravity_class_bind(meta, "yield",	 	NEW_CLOSURE_VALUE(gravity_tsort_yield));
    gravity_class_bind(meta, "yieldAll",	NEW_CLOSURE_VALUE(gravity_tsort_yieldAll));
    gravity_class_bind(meta, "isDone",	 	NEW_CLOSURE_VALUE(gravity_tsort_isDone));
    gravity_class_bind(meta, "await",	 	NEW_CLOSURE_VALUE(gravity_tsort_await));
    gravity_class_bind(meta, "response",	NEW_CLOSURE_VALUE(gravity_tsort_response));

    SETMETA_INITED(gravity_class_tsort);
}

// MARK: - Commons -

void gravity_tsort_register(gravity_vm *vm) {
    if (!gravity_class_tsort) create_optional_class();
    ++refcount;
    
    if (!vm || gravity_vm_ismini(vm)) return;
    gravity_vm_setvalue(vm, GRAVITY_CLASS_TSORT_NAME, VALUE_FROM_OBJECT(gravity_class_tsort));
}

void gravity_tsort_register_args(gravity_vm *vm, uint32_t _argc, const char **_argv) {
    argc = _argc;
    argv = gravity_list_new(vm, argc);
    for (int i = 0; i < _argc; ++i) {
        gravity_value_t arg = VALUE_FROM_CSTRING(vm, _argv[i]);
        marray_push(gravity_value_t, argv->array, arg);
    }
}

void gravity_tsort_free (void) {
    if (!gravity_class_tsort) return;
    if (--refcount) return;

    gravity_class_t *meta = gravity_class_get_meta(gravity_class_tsort);
    computed_property_free(meta, "argc", true);
    computed_property_free(meta, "argv", true);
    gravity_class_free_core(NULL, meta);
    gravity_class_free_core(NULL, gravity_class_tsort);

    gravity_class_tsort = NULL;
}

bool gravity_istsort_class (gravity_class_t *c) {
    return (c == gravity_class_tsort);
}

const char *gravity_tsort_name (void) {
    return GRAVITY_CLASS_TSORT_NAME;
}
