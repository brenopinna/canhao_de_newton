#pragma once

#define DISPLAY_WIDTH    1400
#define DISPLAY_HEIGHT    920
#define FRAMES_PER_SECOND  30

#define G 0.12
#define INITIAL_X_SPEED 20
#define INITIAL_Y_SPEED -0

typedef struct {
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_TIMER *timer;
} LibData;

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