#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <lib.h>

void start_allegro(AllegroData *config) {
  al_init();
  al_install_keyboard();
  al_init_primitives_addon();
  al_init_font_addon();

  config->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  config->event_queue = al_create_event_queue();
  config->timer = al_create_timer(1.0 / FRAMES_PER_SECOND);
  config->font = al_create_builtin_font();

  al_register_event_source(config->event_queue, al_get_keyboard_event_source());
  al_register_event_source(config->event_queue, al_get_display_event_source(config->display));
  al_register_event_source(config->event_queue, al_get_timer_event_source(config->timer));

  al_start_timer(config->timer);
}

void end_allegro(AllegroData *config) {
  al_destroy_display(config->display);
  al_destroy_event_queue(config->event_queue);
  al_destroy_timer(config->timer);
  al_destroy_font(config->font);
  al_shutdown_primitives_addon();
  al_shutdown_font_addon();
  al_uninstall_keyboard();
  al_uninstall_system();
}

void register_keyboard_state(ALLEGRO_EVENT *event, bool *keys) {
  if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
    keys[event->keyboard.keycode] = true;
  } else if (event->type == ALLEGRO_EVENT_KEY_UP) {
    keys[event->keyboard.keycode] = false;
  }
}
