#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <lib.h>

void start_allegro(LibData *config) {
  al_init();
  al_install_keyboard();
  al_init_primitives_addon();

  config->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  config->queue = al_create_event_queue();
  config->timer = al_create_timer(1.0 / FRAMES_PER_SECOND);

  al_register_event_source(config->queue, al_get_keyboard_event_source());
  al_register_event_source(config->queue, al_get_display_event_source(config->display));
  al_register_event_source(config->queue, al_get_timer_event_source(config->timer));

  al_start_timer(config->timer);
}

void end_allegro(LibData *config) {
  al_destroy_display(config->display);
  al_destroy_event_queue(config->queue);
  al_destroy_timer(config->timer);
}
