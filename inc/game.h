#ifndef _GAME_H_
#define _GAME_H_

#include <genesis.h>

// Constantes du jeu
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

#define MAX_ENEMIES 30
#define MAX_BULLETS 10
#define MAX_ENEMY_BULLETS 15

#define PLAYER_SPEED 3
#define BULLET_SPEED 4
#define ENEMY_BULLET_SPEED 2

// Structures
typedef struct {
    s16 x;
    s16 y;
    bool active;
    Sprite* sprite;
} Bullet;
typedef struct {
    s16 x;
    s16 y;
    bool active;
    u16 timer;
    Sprite* sprite;
} Explosion;
typedef struct {
    s16 x;
    s16 y;
    s16 vx;
    s16 vy;
    u8 type;
    bool active;
    u16 timer;
    Sprite* sprite;
} Enemy;

typedef struct {
    s16 x;
    s16 y;
    u8 lives;
    bool active;
    Sprite* sprite;
} Player;

typedef struct {
    Player player;
    Enemy enemies[MAX_ENEMIES];
    Explosion explosions[MAX_ENEMIES];
    Bullet bullets[MAX_BULLETS];
    Bullet enemy_bullets[MAX_ENEMY_BULLETS];
    u32 score;
    u16 wave;
    bool game_over;
    bool return_to_boot;
} GameState;

// Fonctions du jeu
void game_init();
void game_update();
void game_render();
void game_reset();
void game_updateHUD();
void clearScreen() ;
void resetLevel();
void nextLevel();
// Fonctions du joueur
void player_init();
void player_update();
void player_shoot();

// Fonctions des ennemis
void enemies_init();
void enemies_update();
void enemies_spawn_wave();
void enemy_shoot(u16 enemy_index);
void updateExplosions();
// Fonctions des projectiles
void bullets_init();
void bullets_update();
void bullets_spawn(s16 x, s16 y);
void enemy_bullets_update();
void enemy_bullets_spawn(s16 x, s16 y);

// Détection de collisions
bool check_collision(s16 x1, s16 y1, s16 w1, s16 h1, s16 x2, s16 y2, s16 w2, s16 h2);

extern GameState game_state;

#endif
