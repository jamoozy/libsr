
#include "helix.h"


helix_t* helix_create() { return calloc(1, sizeof(helix_t)); }

void helix_destroy(helix_t* self) { free(self); }
