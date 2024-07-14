#ifndef GRAVITY_TSORT_H
#define GRAVITY_TSORT_H

#define GRAVITY_CLASS_TSORT_NAME "ENV"

#include "gravity_value.h"

void gravity_tsort_register (gravity_vm *vm);
void gravity_tsort_register_args(gravity_vm *vm, uint32_t _argc, const char **_argv);
void gravity_tsort_free (void);
bool gravity_istsort_class (gravity_class_t *c);
const char *gravity_tsort_name (void);

#endif
