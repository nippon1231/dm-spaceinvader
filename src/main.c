#include <genesis.h>
#include "game.h"
#include "resources.h"

// Variables pour le scrolling parallaxe
s16 scrollB = 0;  // Plan B (étoiles lointaines - lentes)
s16 scrollA = 0;  // Plan A (étoiles proches - rapides)

// Fonction pour afficher l'écran de démarrage
void show_boot_screen() {
    // Effacer l'écran
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    VDP_clearPlane(WINDOW, TRUE);

    // Afficher le titre
    VDP_drawTextBG(BG_A, "================================", 4, 5);
    VDP_drawTextBG(BG_A, "                                ", 4, 6);
    VDP_drawTextBG(BG_A, "         G A L A G A           ", 4, 7);
    VDP_drawTextBG(BG_A, "                                ", 4, 8);
    VDP_drawTextBG(BG_A, "     MEGA DRIVE / GENESIS      ", 4, 9);
    VDP_drawTextBG(BG_A, "                                ", 4, 10);
    VDP_drawTextBG(BG_A, "================================", 4, 11);

    // Afficher les instructions
    VDP_drawTextBG(BG_A, "      HOW TO PLAY:", 4, 14);
    VDP_drawTextBG(BG_A, "                                ", 4, 15);
    VDP_drawTextBG(BG_A, "  LEFT/RIGHT - MOVE SHIP       ", 4, 16);
    VDP_drawTextBG(BG_A, "  BUTTON A   - SHOOT           ", 4, 17);
    VDP_drawTextBG(BG_A, "  START      - RESET SCREEN    ", 4, 18);

    // Variable pour le clignotement
    u16 blink_counter = 0;
    bool show_text = TRUE;

    // Boucle d'attente avec clignotement du texte
    while(1) {
        // Gérer le clignotement du texte "PRESS A"
        blink_counter++;
        if (blink_counter > 30) {  // Changer toutes les 30 frames (~0.5 sec)
            show_text = !show_text;
            blink_counter = 0;

            if (show_text) {
                VDP_drawTextBG(BG_A, "      PRESS A TO PLAY!         ", 4, 22);
            } else {
                VDP_drawTextBG(BG_A, "                                ", 4, 22);
            }
        }

        // Vérifier les boutons pressés
        u16 joy = JOY_readJoypad(JOY_1);

        // BUTTON_A pour démarrer le jeu
        if (joy & BUTTON_A) {
            // Petit délai pour éviter les pressions multiples
            waitMs(200);
            break;
        }

        // BUTTON_START pour réinitialiser l'écran de démarrage
        if (joy & BUTTON_START) {
            // Effacer l'écran
            VDP_clearPlane(BG_A, TRUE);
            VDP_clearPlane(BG_B, TRUE);
            VDP_clearPlane(WINDOW, TRUE);

            // Réafficher le titre
            VDP_drawTextBG(BG_A, "================================", 4, 5);
            VDP_drawTextBG(BG_A, "                                ", 4, 6);
            VDP_drawTextBG(BG_A, "         G A L A G A           ", 4, 7);
            VDP_drawTextBG(BG_A, "                                ", 4, 8);
            VDP_drawTextBG(BG_A, "     MEGA DRIVE / GENESIS      ", 4, 9);
            VDP_drawTextBG(BG_A, "                                ", 4, 10);
            VDP_drawTextBG(BG_A, "================================", 4, 11);

            // Réafficher les instructions
            VDP_drawTextBG(BG_A, "      HOW TO PLAY:", 4, 14);
            VDP_drawTextBG(BG_A, "                                ", 4, 15);
            VDP_drawTextBG(BG_A, "  LEFT/RIGHT - MOVE SHIP       ", 4, 16);
            VDP_drawTextBG(BG_A, "  BUTTON A   - SHOOT           ", 4, 17);
            VDP_drawTextBG(BG_A, "  START      - RESET SCREEN    ", 4, 18);

            // Réinitialiser le clignotement
            blink_counter = 0;
            show_text = TRUE;

            // Petit délai pour éviter les pressions multiples
            waitMs(200);
        }

        // Attendre la prochaine frame
        SYS_doVBlankProcess();
    }

    // Effacer l'écran avant de commencer le jeu
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
}

int main() {
    // Initialiser le système
    JOY_init();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();

    // Afficher l'écran de démarrage
    show_boot_screen();

    // === CONFIGURER LE PLAN WINDOW ===
    // Window couvre toute la largeur, seulement la première ligne
    VDP_setWindowHPos(FALSE, 0);
    VDP_setWindowVPos(FALSE, 1);


    // Attendre la stabilisation
    waitMs(100);
    u16 tileIndex = TILE_USER_INDEX;
    VDP_loadTileSet(&tileset_stars, tileIndex, DMA);    
    // Charger la palette
    PAL_setPalette(PAL0, palette_main.data, DMA);
  
  
    // Remplir le plan B
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
    

    // Initialiser le jeu
    game_init();
    
    // Afficher un message
    VDP_drawTextBG(WINDOW,"GALAGA - START!", 10, 2);
    
    // Boucle principale
    while(1) {
        // Scrolling parallaxe
        scrollB += 1;
        if (scrollB >= 512) scrollB = 0;
        
        scrollA += 3;
        if (scrollA >= 512) scrollA = 0;
        
        VDP_setVerticalScroll(BG_B, scrollB);
        VDP_setVerticalScroll(BG_A, scrollA); 
        if (!game_state.game_over) {
            game_update();
        }
        game_render();
        game_updateHUD();
        SPR_update();   
        SYS_doVBlankProcess();
    }
    
    return 0;
}
