#pragma once

#include <global.h>

void start_allegro(AllegroData *config);

void end_allegro(AllegroData *config);

void register_keyboard_state(ALLEGRO_EVENT *event, bool *keys);