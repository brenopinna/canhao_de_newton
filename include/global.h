#pragma once

#define DISPLAY_WIDTH    1400
#define DISPLAY_HEIGHT    920
#define FRAMES_PER_SECOND  30

#define GRAVITATIONAL_CONSTANT 6.67430e-20 // (Cte original): G = 6.67430e-11
#define INITIAL_X_SPEED 20
#define INITIAL_Y_SPEED 0

typedef struct {
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *event_queue;
  ALLEGRO_TIMER *timer;
  ALLEGRO_FONT *font;
} AllegroData;

typedef struct {
  double x, y;
} Vector;

typedef struct {
  double radius;
  Vector position;
  ALLEGRO_COLOR color;
  double mass;
  Vector speed;
  Vector acceleration;
} Body;

typedef struct {
  float x1, y1, x2, y2;
  ALLEGRO_COLOR color;
} Rectangle;