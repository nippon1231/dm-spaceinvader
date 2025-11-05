#include <genesis.h>
#include "game.h"
#include "resources.h"

void player_init() {
    game_state.player.x = SCREEN_WIDTH / 2 - 8;
    game_state.player.y = SCREEN_HEIGHT - 40;
    game_state.player.lives = 3;
    game_state.player.active = TRUE;

    // Créer le sprite du joueur
    game_state.player.sprite = SPR_addSprite(&player_sprite, 
                                              game_state.player.x, 
                                              game_state.player.y, 
                                              TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
    SPR_setFrame(game_state.player.sprite, 3);
                        }

void player_update() {
    if (!game_state.player.active) return;
    
    u16 joy = JOY_readJoypad(JOY_1);
    SPR_setFrame(game_state.player.sprite, 3);
    // Déplacement gauche/droite
    if (joy & BUTTON_LEFT) {
        game_state.player.x -= PLAYER_SPEED;
        if (game_state.player.x < 0) game_state.player.x = 0;
        SPR_setFrame(game_state.player.sprite, 4);
    }
    if (joy & BUTTON_RIGHT) {
        game_state.player.x += PLAYER_SPEED;
        if (game_state.player.x > SCREEN_WIDTH - 16) 
            game_state.player.x = SCREEN_WIDTH - 16;
        SPR_setFrame(game_state.player.sprite, 2);
    }
    
    
    // Tir
    static u16 fire_cooldown = 0;
    if (fire_cooldown > 0) fire_cooldown--;
    
    if ((joy & BUTTON_A) && fire_cooldown == 0) {
        player_shoot();
        fire_cooldown = 10; // Cooldown de 10 frames
    }
    
    // Mettre à jour la position du sprite
    SPR_setPosition(game_state.player.sprite, game_state.player.x, game_state.player.y);
    
    // Vérifier les collisions avec les projectiles ennemis
    for (u16 i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (game_state.enemy_bullets[i].active) {
            if (check_collision(game_state.player.x, game_state.player.y, 16, 16,
                              game_state.enemy_bullets[i].x, game_state.enemy_bullets[i].y, 8, 8)) {
                // Le joueur est touché
                game_state.enemy_bullets[i].active = FALSE;
                SPR_setVisibility(game_state.enemy_bullets[i].sprite, HIDDEN);
                
                game_state.player.lives--;
                if (game_state.player.lives <= 0) {
                    game_state.game_over = TRUE;
                    game_state.player.active = FALSE;
                    SPR_setVisibility(game_state.player.sprite, HIDDEN);
                }
            }
        }
    }
}

void player_shoot() {
    bullets_spawn(game_state.player.x + 4, game_state.player.y - 8);
}
