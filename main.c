#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <global.h>
#include <lib.h>

// Global Variables
LibData config;
Body projectiles_array[INT16_MAX] = { 0 };
int projectiles = 0;
Vector trail_array[INT16_MAX] = { 0 };
int trail_count = 0;
Body earth;

void add_projectil(double x, double y) {
  trail_count = 0;
  Body projectile = projectiles_array[projectiles++] = (Body){
    .radius = 10,
    .position = {x, y},
    .mass = 44,
    .color = al_map_rgb(100, 100, 100),
    .speed = {INITIAL_X_SPEED, INITIAL_Y_SPEED},
    .acceleration = {0,0}
  };
  trail_array[trail_count++] = projectile.position;
}

void draw_trail() {
  Vector before = trail_array[0];
  for (int i = 0; i < trail_count; i++) {
    Vector after = trail_array[i];
    al_draw_line(before.x, before.y, after.x, after.y, al_map_rgb(255, 0, 0), 1);
    before = after;
  }
}

void draw_body(Body *body) {
  al_draw_filled_circle(body->position.x, body->position.y, body->radius, body->color);
}

void draw_canon(Rectangle *canon) {
  al_draw_filled_rectangle(canon->x1, canon->y1, canon->x2, canon->y2, canon->color);
}

void draw_projectiles() {
  for (int i = 0; i < projectiles; i++) {
    Body *canon_ball = &projectiles_array[i];
    draw_body(canon_ball);
  }
}

void update_projectiles() {
  Vector force;

  for (int i = 0; i < projectiles; i++) {
    Body *canon_ball = &projectiles_array[i];
    // prevent_colision(canon_ball, &earth);
    double xProjectile = canon_ball->position.x;
    double yProjectile = canon_ball->position.y;
    double distance = sqrt(pow(earth.position.x - canon_ball->position.x, 2) + pow(earth.position.y - canon_ball->position.y, 2));
    if (distance < 0.1) distance = 0.1;
    double gravitational_force = G * earth.mass * canon_ball->mass / pow(distance, 2);
    force.x = gravitational_force * ((earth.position.x - canon_ball->position.x) / distance); // Fx = F * sin (|Fx|)
    force.y = gravitational_force * ((earth.position.y - canon_ball->position.y) / distance); // Fy = F * cos (|Fy|)
    canon_ball->acceleration.x = force.x / canon_ball->mass;
    canon_ball->acceleration.y = force.y / canon_ball->mass;
    canon_ball->speed.x += canon_ball->acceleration.x;
    canon_ball->speed.y += canon_ball->acceleration.y;
    canon_ball->position.x += canon_ball->speed.x;
    canon_ball->position.y += canon_ball->speed.y;
    distance = sqrt(pow(earth.position.x - canon_ball->position.x, 2) + pow(earth.position.y - canon_ball->position.y, 2));
    if (distance < earth.radius + canon_ball->radius) {
      canon_ball->position.x = xProjectile;
      canon_ball->position.y = yProjectile;
      canon_ball->speed.x = 0;
      canon_ball->speed.y = 0;
      canon_ball->acceleration.x = 0;
      canon_ball->acceleration.y = 0;
    }
    // double x = abs(canon_ball->position.x - earth.position.x);
    // double y = abs(canon_ball->position.y - earth.position.y);
    // canon_ball->acceleration.x = (y == 0) ? 0 : sqrt((ACCELERATION * (x / y)) / (1 + x / y)) * (canon_ball->position.x > earth.position.x ? -1 : 1);
    // canon_ball->acceleration.y = (y == 0) ? 0 : sqrt(ACCELERATION / (1 + x / y)) * (canon_ball->position.y > earth.position.y ? -1 : 1);
  }

  trail_array[trail_count++] = projectiles_array[projectiles - 1].position;
}

int main() {
  start_allegro(&config);
  bool keys[ALLEGRO_KEY_MAX] = { 0 };
  ALLEGRO_EVENT event;

  earth = (Body){
    .position = {DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2},
    .radius = 250,
    .mass = 1000000,
    .color = al_map_rgb(10, 50, 200)
  };

  Rectangle canon = {
    .x1 = earth.position.x - 20,
    .y1 = earth.position.y - earth.radius,
    .x2 = earth.position.x + 20,
    .y2 = canon.y1 - 40,
    .color = al_map_rgb(200,0,0)
  };

  Vector initial_projectile_position = { (canon.x2 + canon.x1) / 2,canon.y2 - 10 };

  add_projectil(initial_projectile_position.x, initial_projectile_position.y);

  while (event.type != ALLEGRO_EVENT_DISPLAY_CLOSE) {
    al_wait_for_event(config.queue, &event);

    // Le as teclas pressionadas
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
      keys[event.keyboard.keycode] = true;
    } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
      keys[event.keyboard.keycode] = false;
    }

    if (keys[ALLEGRO_KEY_ESCAPE]) break;

    if (keys[ALLEGRO_KEY_ENTER])
      add_projectil(initial_projectile_position.x, initial_projectile_position.y);

    // Executado em sincronia com o timer
    if (event.type == ALLEGRO_EVENT_TIMER && al_event_queue_is_empty(config.queue)) {
      al_clear_to_color(al_map_rgb(15, 0, 20)); // fundo da simulacao
      // Atualiza os projeteis
      draw_body(&earth);
      draw_canon(&canon);
      update_projectiles();
      draw_trail();
      draw_projectiles();
      // Atualiza o displau
      al_flip_display();
    }

  }

  end_allegro(&config);
}