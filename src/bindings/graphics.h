#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "../data/types.h"

Scheme_object* g_init(Scheme_object* _);

Scheme_object* g_create_window(Scheme_object* args);

Scheme_object* g_swap_buffers(Scheme_object* _);

Scheme_object* g_terminate(Scheme_object* _);

Scheme_object* g_poll_evnets(Scheme_object* _);

Scheme_object* key_is_down(int btn);

Scheme_object* g_key_is_down_UP(Scheme_object* _);

Scheme_object* g_key_is_down_DOWN(Scheme_object* _);

Scheme_object* g_key_is_down_W(Scheme_object* _);

Scheme_object* g_key_is_down_S(Scheme_object* _);

Scheme_object* g_draw_rect(Scheme_object* args);

Scheme_object* g_clear(Scheme_object* args);
#endif
