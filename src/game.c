#include <genesis.h>
#include "game.h"
#include "resources.h"

GameState game_state;

void game_init() {
    // Réinitialiser l'état du jeu
    game_state.score = 0;
    game_state.wave = 1;
    game_state.game_over = FALSE;
    game_state.return_to_boot = FALSE;
    
    // Initialiser toutes les structures à zéro
    u16 i;
    for (i = 0; i < MAX_BULLETS; i++) {
        game_state.bullets[i].active = FALSE;
        game_state.bullets[i].sprite = NULL;
    }
    
    for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
        game_state.enemy_bullets[i].active = FALSE;
        game_state.enemy_bullets[i].sprite = NULL;
    }
    
    for (i = 0; i < MAX_ENEMIES; i++) {
        game_state.enemies[i].active = FALSE;
        game_state.enemies[i].sprite = NULL;
    }
    for (i = 0; i < MAX_ENEMIES; i++) {
        game_state.explosions[i].active = FALSE;
        game_state.explosions[i].sprite = NULL;
        game_state.explosions[i].timer = 0;
    }    
    // Initialiser le joueur
    game_state.player.x = SCREEN_WIDTH / 2 - 8;
    game_state.player.y = SCREEN_HEIGHT - 40;
    game_state.player.lives = 3;
    game_state.player.active = TRUE;
    game_state.player.sprite = NULL;
    
    // Créer le sprite du joueur
    SPR_init();
    game_state.player.sprite = SPR_addSprite(&player_sprite, 
                                              game_state.player.x, 
                                              game_state.player.y, 
                                              TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
   // SPR_setFrame(game_state.player.sprite, 3);
    
    // Spawner la première vague
    enemies_spawn_wave();
}

void game_update() {
    if (game_state.game_over) {
        // Gérer le game over - retourner à l'écran de démarrage
        if (JOY_readJoypad(JOY_1) & BUTTON_START) {
            game_state.return_to_boot = TRUE;
        }
        return;
    }

    // Mettre à jour les composants
    player_update();
    enemies_update();
    bullets_update();
    enemy_bullets_update();
    updateExplosions();
    
    // Vérifier si tous les ennemis sont morts
    bool all_dead = TRUE;
    for (u16 i = 0; i < MAX_ENEMIES; i++) {
        if (game_state.enemies[i].active) {
            all_dead = FALSE;
            break;
        }
    }
    
    if (all_dead) {
        game_state.wave++;
        nextLevel();       
    }
}

void game_updateHUD()
{
    char text[20];
    
    // Afficher sur le plan Window
    sprintf(text, "SCORE:%06lu", game_state.score);
    VDP_clearTextBG(WINDOW, 1, 0, 15);
    VDP_drawTextBG(WINDOW, text, 1, 0);
    
    sprintf(text, "LIVES:%d", game_state.player.lives);
    VDP_clearTextBG(WINDOW, 25, 0, 10);
    VDP_drawTextBG(WINDOW, text, 25, 0);
}

void game_render() {
    // L'affichage est géré par les sprites
    // On pourrait ajouter ici l'affichage du score
    if (game_state.game_over) {
        VDP_drawTextBG(WINDOW,"GAME OVER", 14, 12);
        VDP_drawTextBG(WINDOW,"PRESS START", 13, 14);
    }
}

void game_reset() {
    // Supprimer tous les sprites
    SPR_reset();
    
    // Réinitialiser le jeu
    game_init();
}

bool check_collision(s16 x1, s16 y1, s16 w1, s16 h1, s16 x2, s16 y2, s16 w2, s16 h2) {
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

void clearScreen() {
    // Effacer tous les sprites
    SPR_reset();
    
    // Effacer tous les plans
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    
    // Optionnel: Fade out/in pour effet visuel
 //   PAL_fadeOutAll(10, TRUE);
    
    // Petit délai
    waitMs(500);
  //  PAL_fadeInAll(PAL0,10, TRUE);
    u16 tileIndex = TILE_USER_INDEX;
    VDP_loadTileSet(&tileset_stars, tileIndex, DMA);   
      u16 x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 64; x++) {
            u16 tile = (random() % 100 < 15) ? tileIndex + (random() % 8) : tileIndex;
            VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, tile), x, y);
        }
    }
    
    // Remplir le plan A
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 64; x++) {
            u16 tile = (random() % 100 < 10) ? tileIndex + 8 + (random() % 16) : 0;
            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, TRUE, FALSE, FALSE, tile), x, y);
        }
    }

  
}
void resetLevel() {

    // Effacer tout
    clearScreen();
      // Charger la palette

    // Réinitialiser les entités
    game_init();
    

    
    // Attendre 2 secondes
    u16 wait = 0;
    while(wait < 120) {
        SPR_update();
        SYS_doVBlankProcess();
        wait++;
    }
    
    
    // Mettre à jour le HUD
    game_updateHUD();
    
    // Fade in
  //  PAL_fadeInAll(PAL0,  10, TRUE);
    

}

void nextLevel() {
        // Effacer tout
    clearScreen();
    resetLevel();
    PAL_setPalette(PAL0, palette_main.data, DMA);      
}