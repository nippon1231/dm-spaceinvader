#include <genesis.h>
#include "game.h"
#include "resources.h"

void bullets_init() {
    for (u16 i = 0; i < MAX_BULLETS; i++) {
        game_state.bullets[i].active = FALSE;
        game_state.bullets[i].sprite = NULL;
    }
    
    for (u16 i = 0; i < MAX_ENEMY_BULLETS; i++) {
        game_state.enemy_bullets[i].active = FALSE;
        game_state.enemy_bullets[i].sprite = NULL;
    }
}

void bullets_spawn(s16 x, s16 y) {
    for (u16 i = 0; i < MAX_BULLETS; i++) {
        if (!game_state.bullets[i].active) {
            game_state.bullets[i].x = x;
            game_state.bullets[i].y = y;
            game_state.bullets[i].active = TRUE;
            
            if (game_state.bullets[i].sprite == NULL) {
                game_state.bullets[i].sprite = SPR_addSprite(&bullet_sprite,
                                                              x, y,
                                                              TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
            } else {
                SPR_setPosition(game_state.bullets[i].sprite, x, y);
                SPR_setVisibility(game_state.bullets[i].sprite, VISIBLE);
            }
            break;
        }
    }
}

void bullets_update() {
    for (u16 i = 0; i < MAX_BULLETS; i++) {
        if (game_state.bullets[i].active) {
            game_state.bullets[i].y -= BULLET_SPEED;
            
            // Désactiver si hors écran
            if (game_state.bullets[i].y < -8) {
                game_state.bullets[i].active = FALSE;
                SPR_setVisibility(game_state.bullets[i].sprite, HIDDEN);
            } else {
                SPR_setPosition(game_state.bullets[i].sprite,
                              game_state.bullets[i].x,
                              game_state.bullets[i].y);
            }
        }
    }
}

void enemy_bullets_spawn(s16 x, s16 y) {
    for (u16 i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!game_state.enemy_bullets[i].active) {
            game_state.enemy_bullets[i].x = x;
            game_state.enemy_bullets[i].y = y;
            game_state.enemy_bullets[i].active = TRUE;
            
            if (game_state.enemy_bullets[i].sprite == NULL) {
                game_state.enemy_bullets[i].sprite = SPR_addSprite(&bullet_sprite,
                                                                     x, y,
                                                                     TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
            } else {
                SPR_setPosition(game_state.enemy_bullets[i].sprite, x, y);
                SPR_setVisibility(game_state.enemy_bullets[i].sprite, VISIBLE);
            }
            break;
        }
    }
}

void enemy_bullets_update() {
    for (u16 i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game_state.enemy_bullets[i].active) {
            game_state.enemy_bullets[i].y += ENEMY_BULLET_SPEED;
            
            // Désactiver si hors écran
            if (game_state.enemy_bullets[i].y > SCREEN_HEIGHT) {
                game_state.enemy_bullets[i].active = FALSE;
                SPR_setVisibility(game_state.enemy_bullets[i].sprite, HIDDEN);
            } else {
                SPR_setPosition(game_state.enemy_bullets[i].sprite,
                              game_state.enemy_bullets[i].x,
                              game_state.enemy_bullets[i].y);
            }
        }
    }
}
