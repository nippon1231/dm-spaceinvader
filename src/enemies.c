#include <genesis.h>
#include "game.h"
#include "resources.h"

void enemies_init() {
    for (u16 i = 0; i < MAX_ENEMIES; i++) {
        game_state.enemies[i].active = FALSE;
        game_state.enemies[i].sprite = NULL;
        game_state.explosions[i].active = FALSE;
        game_state.explosions[i].sprite = NULL;        
        game_state.explosions[i].timer = 0; 
    }
}


void enemies_spawn_wave() {
    u16 enemy_count = 10 + (game_state.wave * 2);
    if (enemy_count > MAX_ENEMIES) enemy_count = MAX_ENEMIES;
    
    for (u16 i = 0; i < enemy_count; i++) {
        s16 col = i % 5;
        s16 row = i / 5;
        
        game_state.enemies[i].x = 40 + col * 48;
        game_state.enemies[i].y = 20 + row * 32;
        game_state.enemies[i].vx = 1;
        game_state.enemies[i].vy = 0;
        game_state.enemies[i].type = i % 3;
        game_state.enemies[i].active = TRUE;
        game_state.enemies[i].timer = 0;
        game_state.explosions[i].timer = 0;
        game_state.explosions[i].active = FALSE;



        if (game_state.enemies[i].sprite == NULL) {
            game_state.enemies[i].sprite = SPR_addSprite(&enemy_sprite,
                                                          game_state.enemies[i].x,
                                                          game_state.enemies[i].y,
                                                          TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
        } else {
            SPR_setPosition(game_state.enemies[i].sprite, 
                          game_state.enemies[i].x, 
                          game_state.enemies[i].y);
            SPR_setVisibility(game_state.enemies[i].sprite, VISIBLE);
        }
     
    }
}

void enemies_update() {
    bool change_direction = FALSE;
    
    for (u16 i = 0; i < MAX_ENEMIES; i++) {
        if (!game_state.enemies[i].active) continue;
        
        game_state.enemies[i].timer++;
        
        // Déplacement
        game_state.enemies[i].x += game_state.enemies[i].vx;
        
        // Vérifier les bords de l'écran
        if (game_state.enemies[i].x <= 10 || game_state.enemies[i].x >= SCREEN_WIDTH - 26) {
            change_direction = TRUE;
        }
        
        // Tir aléatoire
        if ((game_state.enemies[i].timer % 120) == 0 && (random() % 3) == 0) {
            enemy_shoot(i);
        }
        
        // Mettre à jour la position du sprite
        SPR_setPosition(game_state.enemies[i].sprite, 
                       game_state.enemies[i].x, 
                       game_state.enemies[i].y);
        
        // Vérifier les collisions avec les projectiles du joueur
        for (u16 j = 0; j < MAX_BULLETS; j++) {
            if (game_state.bullets[j].active) {
                if (check_collision(game_state.enemies[i].x, game_state.enemies[i].y, 16, 16,
                                  game_state.bullets[j].x, game_state.bullets[j].y, 8, 8)) {

                   
                    if(game_state.explosions[i].sprite == NULL) {
                        game_state.explosions[i].sprite = SPR_addSprite(&explosion_sprite, game_state.enemies[i].x, game_state.enemies[i].y,
                                                            TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
                    } 
                        game_state.explosions[i].active = TRUE;
                        SPR_setPosition(game_state.explosions[i].sprite, game_state.enemies[i].x, game_state.enemies[i].y);
                        SPR_setVisibility(game_state.explosions[i].sprite, VISIBLE);
                                                    
                    // L'ennemi est touché
                    game_state.enemies[i].active = FALSE;
                    SPR_setVisibility(game_state.enemies[i].sprite, HIDDEN);
                    
                    game_state.bullets[j].active = FALSE;
                    SPR_setVisibility(game_state.bullets[j].sprite, HIDDEN);



                    game_state.score += 100;
                }
            }
        }
    }
    
    // Changer de direction et descendre
    if (change_direction) {
        for (u16 i = 0; i < MAX_ENEMIES; i++) {
            if (game_state.enemies[i].active) {
                game_state.enemies[i].vx *= -1;
                game_state.enemies[i].y += 8;
                
                // Game over si les ennemis atteignent le bas
                if (game_state.enemies[i].y > SCREEN_HEIGHT - 60) {
                    game_state.game_over = TRUE;
                }
            }
        }
    }
}
void updateExplosions() {

    for(u16 i = 0; i < MAX_ENEMIES; i++) {
     
        if(game_state.explosions[i].active) {

            game_state.explosions[i].timer++;            
            // Changer de frame
            u16 frame = game_state.explosions[i].timer / 8;
            if(frame < 5) {
                SPR_setFrame(game_state.explosions[i].sprite,frame);
            }
            
           else{
                game_state.explosions[i].active = FALSE;
                SPR_setVisibility(game_state.explosions[i].sprite, HIDDEN);
                 game_state.explosions[i].timer=0;
            }
        }
    }
}
void enemy_shoot(u16 enemy_index) {
    if (!game_state.enemies[enemy_index].active) return;
    
    enemy_bullets_spawn(game_state.enemies[enemy_index].x + 4,
                       game_state.enemies[enemy_index].y + 16);
}
