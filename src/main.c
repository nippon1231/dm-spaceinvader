#include <genesis.h>
#include "game.h"
#include "resources.h"

// Variables pour le scrolling parallaxe
s16 scrollB = 0;  // Plan B (étoiles lointaines - lentes)
s16 scrollA = 0;  // Plan A (étoiles proches - rapides)

// Fonction d'affichage de l'écran de boot Sega
void show_sega_boot_screen() {
    // Effacer l'écran
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);

    // Charger la palette du logo Sega
    PAL_setPalette(PAL0, palette_sega.data, DMA);

    // Décompresser et afficher l'image du logo Sega
    VDP_drawImageEx(BG_B, &sega_logo, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USER_INDEX), 0, 6, FALSE, TRUE);

    // Attendre 2 secondes pour afficher le logo
    waitMs(2000);

    // Fondu au noir
    PAL_fadeOutAll(20, TRUE);

    // Effacer l'écran
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
}

int main() {
    // Initialiser le système
    JOY_init();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();

    // Afficher l'écran de boot Sega
    show_sega_boot_screen();

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
