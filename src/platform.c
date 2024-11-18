#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "platform.h"

// macOS specific key binding and screen clearing
#ifdef __APPLE__
void clear_screen() {
    printf("\033[H\033[J");
}

void set_key_bindings() {
    rl_bind_key('\t', rl_complete);
}

#else  // Linux or other platforms

void set_key_bindings() {
    rl_bind_key('\t', rl_complete);
    rl_bind_keyseq("\\C-l", rl_clear_screen);
}

#endif /* __APPLE__ */
