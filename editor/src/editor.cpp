#include <stdio.h>

#include "editor.h"
#include "random.h"

INITIALIZE(Initialize) {
    printf("Initialize\n");
}

UPDATE_AND_RENDER(UpdateAndRender) {
    printf("Hello from editor!\n");
}
