#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <global.h>
#include <lib.h>

// Allegro Global Variables
AllegroData config;
ALLEGRO_EVENT event;
bool keys[ALLEGRO_KEY_MAX] = { 0 };

// Simulation Global Variables
Body projectiles_array[UINT16_MAX] = { 0 };
uint16_t projectiles_count = 0;
Body earth;
Rectangle canon;
Vector initial_projectile_speed = { INITIAL_X_SPEED, INITIAL_Y_SPEED };

void reset_trail_buffer() {
  al_set_target_bitmap(config.trail_buffer);

  al_clear_to_color(al_map_rgba(0, 0, 0, 0));

  al_set_target_backbuffer(config.display);
}

void add_projectile(double x, double y) {
  projectiles_array[projectiles_count++] = (Body){
    .radius = 10,
    .position = {x, y},
    .mass = 7.348e22,
    .color = al_map_rgb(100, 100, 100),
    .speed = initial_projectile_speed,
    .acceleration = {0,0}
  };
  reset_trail_buffer();
}

void draw_trail() {
  al_draw_bitmap(config.trail_buffer, 0, 0, 0);
}

void draw_body(Body *body) {
  al_draw_filled_circle(body->position.x, body->position.y, body->radius, body->color);
}

void draw_canon() {
  al_draw_filled_rectangle(canon.x1, canon.y1, canon.x2, canon.y2, canon.color);
}

void draw_projectiles() {
  for (uint16_t i = 0; i < projectiles_count; i++) {
    Body *canon_ball = &projectiles_array[i];
    draw_body(canon_ball);
  }
  if (projectiles_count) {
    Body last_projectile = projectiles_array[projectiles_count - 1];
    al_draw_circle(last_projectile.position.x, last_projectile.position.y, last_projectile.radius + 1, al_map_rgb(255, 0, 0), 2);
  }
}

void update_projectiles() {
  Vector force;
  const double start_gravitational_force_equation = GRAVITATIONAL_CONSTANT * earth.mass;

  Vector before, after;

  for (uint16_t i = 0; i < projectiles_count; i++) {
    Body *canon_ball = &projectiles_array[i];
    before = canon_ball->position;
    Vector distance_vector = { earth.position.x - canon_ball->position.x, earth.position.y - canon_ball->position.y };
    double distance = sqrt(distance_vector.x * distance_vector.x + distance_vector.y * distance_vector.y);
    double gravitational_force = start_gravitational_force_equation * canon_ball->mass / (distance * distance);
    force.x = gravitational_force * ((distance_vector.x) / distance); // Fx = F * sin (|Fx|)
    force.y = gravitational_force * ((distance_vector.y) / distance); // Fy = F * cos (|Fy|)
    canon_ball->acceleration.x = force.x / canon_ball->mass;
    canon_ball->acceleration.y = force.y / canon_ball->mass;
    canon_ball->speed.x += canon_ball->acceleration.x;
    canon_ball->speed.y += canon_ball->acceleration.y;
    canon_ball->position.x += canon_ball->speed.x;
    canon_ball->position.y += canon_ball->speed.y;
    // atualizar o vetor das distancias
    distance_vector = (Vector){ earth.position.x - canon_ball->position.x, earth.position.y - canon_ball->position.y };
    double squared_distance = distance_vector.x * distance_vector.x + distance_vector.y * distance_vector.y;
    double squared_radius_sum = (earth.radius + canon_ball->radius) * (earth.radius + canon_ball->radius);
    if (squared_distance <= squared_radius_sum) {
      canon_ball->speed.x *= -0.1;
      canon_ball->speed.y *= -0.1;
      while (squared_distance <= squared_radius_sum) {
        canon_ball->position.x += canon_ball->speed.x;
        canon_ball->position.y += canon_ball->speed.y;
        // atualiza o vetor distancia e o quadrado das distancias
        distance_vector = (Vector){ earth.position.x - canon_ball->position.x, earth.position.y - canon_ball->position.y };
        squared_distance = distance_vector.x * distance_vector.x + distance_vector.y * distance_vector.y;
      }
      canon_ball->speed.x = 0;
      canon_ball->speed.y = 0;
      canon_ball->acceleration.x = 0;
      canon_ball->acceleration.y = 0;
    }
    after = canon_ball->position;
  }

  if (projectiles_count) {
    al_set_target_bitmap(config.trail_buffer);

    al_draw_line(before.x, before.y, after.x, after.y, al_map_rgb(255, 0, 0), 2);

    al_set_target_backbuffer(config.display);
  }
}

void reset_simulation() {
  projectiles_count = 0;

  reset_trail_buffer();

  initial_projectile_speed = (Vector){ INITIAL_X_SPEED, INITIAL_Y_SPEED };
}

void rotate_vector(Vector *v, double angle) {
  double x = v->x, y = v->y;
  v->x = x * cos(angle) - y * sin(angle);
  v->y = x * sin(angle) + y * cos(angle);
}

void draw_arrow(Vector *origin, Vector *direction, Vector *size, ALLEGRO_COLOR color) {
  al_draw_line(
    origin->x,
    origin->y,
    origin->x + size->x,
    origin->y + size->y,
    color,
    5
  );

  double angle = asin((direction->y) / sqrt(pow(direction->x, 2) + pow(direction->y, 2)));
  // OBS: asin(x) = y <=> -pi/2 <= y <= 0
  // Como queremos o dominio pi < y <= 0:
  if (direction->x < 0) angle = 3.14 - angle;
  // isso faz com que gire angulos maiores que pi/2 tambem.

  double triangle_size = 8;
  Vector t1 = { triangle_size, 0 };
  Vector t2 = { 0, triangle_size };
  Vector t3 = { 0, -triangle_size };

  rotate_vector(&t1, angle);
  rotate_vector(&t2, angle);
  rotate_vector(&t3, angle);

  t1.x += origin->x + size->x;
  t1.y += origin->y + size->y;
  t2.x += origin->x + size->x;
  t2.y += origin->y + size->y;
  t3.x += origin->x + size->x;
  t3.y += origin->y + size->y;

  al_draw_filled_triangle(
    t1.x, t1.y,
    t2.x, t2.y,
    t3.x, t3.y,
    color
  );
}

int main() {
  start_allegro(&config);

  earth = (Body){
    .position = {DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2},
    .radius = 250,
    .mass = 5.972e24,
    .color = al_map_rgb(10, 50, 200)
  };

  canon = (Rectangle){
    .x1 = earth.position.x - 20,
    .y1 = earth.position.y - earth.radius,
    .x2 = earth.position.x + 20,
    .y2 = earth.position.y - earth.radius - 40,
    .color = al_map_rgb(200,0,0)
  };

  Vector initial_projectile_position = { (canon.x2 + canon.x1) / 2,canon.y2 - 10 };

  while (event.type != ALLEGRO_EVENT_DISPLAY_CLOSE) {
    al_wait_for_event(config.event_queue, &event);

    register_keyboard_state(&event, keys);

    if (keys[ALLEGRO_KEY_ESCAPE]) break;

    const double acceleration = 0.6;

    if (keys[ALLEGRO_KEY_UP])
      initial_projectile_speed.y -= acceleration;
    if (keys[ALLEGRO_KEY_DOWN])
      initial_projectile_speed.y += acceleration;
    if (keys[ALLEGRO_KEY_LEFT])
      initial_projectile_speed.x -= acceleration;
    if (keys[ALLEGRO_KEY_RIGHT])
      initial_projectile_speed.x += acceleration;

    if (keys[ALLEGRO_KEY_R])
      reset_simulation();

    if (event.type == ALLEGRO_EVENT_TIMER && al_is_event_queue_empty(config.event_queue)) {
      if (keys[ALLEGRO_KEY_ENTER])
        add_projectile(initial_projectile_position.x, initial_projectile_position.y);
      al_clear_to_color(al_map_rgb(15, 0, 20));
      draw_body(&earth);
      draw_canon(&canon);
      update_projectiles();
      draw_trail();
      draw_projectiles();
      al_draw_textf(config.font, al_map_rgb(255, 255, 255), 30, 30, 0, "VelX: %.2lf, VelY: %.2lf", initial_projectile_speed.x, -initial_projectile_speed.y);
      draw_arrow(&initial_projectile_position, &initial_projectile_speed, &(Vector){4 * initial_projectile_speed.x, 4 * initial_projectile_speed.y}, al_map_rgb(0, 255, 0));
      Body last_projectile = projectiles_array[projectiles_count - 1];
      double total_speed = sqrt(pow(last_projectile.speed.x, 2) + pow(last_projectile.speed.y, 2));

      al_draw_textf(config.font, al_map_rgb(255, 255, 255), 30, 60, 0, "Total Speed (Last Projectile): %.2lf", total_speed);

      al_draw_text(config.font, al_map_rgb(255, 255, 255), 30, DISPLAY_HEIGHT - 30, 0, "Press [R] key to restart the simulation");
      al_draw_text(config.font, al_map_rgb(255, 255, 255), 30, DISPLAY_HEIGHT - 60, 0, "Press the arrow keys to control the initial speed.");
      al_draw_text(config.font, al_map_rgb(255, 255, 255), 30, DISPLAY_HEIGHT - 90, 0, "Press [Enter] key to throw a new projectile.");

      al_flip_display();
    }
  }

  end_allegro(&config);
}