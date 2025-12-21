#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "raylib.h"

// Constants
#define screenWidth 600
#define screenHeight 960
#define columns 4 // ERROR: Unimplemented with controls (Controls only work for 4 columns)
#define tileNumber 50 // For more optimized numbering, use (screenHeight / tileHeight) + 3 - max number of tiles in the screen + 1 above, 1 below it
#define tileSize (screenWidth / columns)
#define tileHeight tileSize * 1.5f // Height of tiles 1.5 * the width
#define baseSpeed 20.0f // The higher the value the slower the pace
#define EASY (float)(tileHeight / (baseSpeed + 10))
#define MEDIUM (float)(tileHeight / (baseSpeed + 5))
#define HARD (float)(tileHeight / baseSpeed)
#define HIGHSCORE_FILE "highscore.dat"

// TODO List
// README :IMPORTANT
// Sort scores :IMPORTANT
// Store more (10) scores :IMPORTANT
// Add songs in the background
// Pulse the letters when clicked
// Blow up tiles when clicked
// Add notes to tiles
// Avatar in the corner shouting when u score a point
// Song guided tile generation

// Structs
// Tile | column - 0, 1, 2, 3 | y - y coordinate | active - true/false
typedef struct Tile {
    int column;
    float y;
    bool active;
} Tile;

// Game State Enum | PLAYING = 0 | PAUSE = 1 | END = 2
typedef enum GameState {
    PLAYING, PAUSE, END
} GameState;

// Rounds | EASY = 50 | MEDIUM = 100 | HARD > 50
typedef enum Round {
    ROUND_1 = 50, ROUND_2 = 100
} Round;

// Pulse | life (float) | color (Color) | active (bool)
typedef struct Pulse {
    float life;
    Color color;
    bool active;
} Pulse;

// Game instance | score (int) | highscore (int) | theLine (float - y coord) | tiles (Tiles[]) | speed (float - EASY/MEDIUM/HARD) | generated (int) | scored (int) | gameState (PLAYING/PAUSE/END) | pulses (Pulse[5] - a, s, k, l, Notification)
typedef struct Game {
    int score;
    int highscore;
    float theLine;
    Tile tiles[tileNumber];
    float speed;
    int generated;
    int scored;
    Pulse pulses[5];
    GameState gameState;
} Game;

// Function prototypes
static void GameInit(Game *game); // Initialize game stats
static void GameEnd(Game *game); // End game
static void UpdateGame(Game *game); // Update game physics
static void DrawFrame(Game *game); // Update and draw one frame
static void GenerateRandomTile(Game *game, int prev); // Generate tile in one of four collumns
static void CheckTilesOutsideScreen(Game *game); // Check if each tile is outside of the screen or near the line
int loadHighscore(void); // Load highscore
void saveHighscore(int hightscore); // Save highscore into file
int updateHighscore(int score, int highscore); // Update the highscore

// Main entry point
int main() {
    // Window init (Perence baslat)
    InitWindow(screenWidth, screenHeight, "piano tiles");
    SetTargetFPS(60);
    Game game;
    // Init Game (Oyun Baslat)
    GameInit(&game);    
    // Game Loop (Oyun Dongusu)
    while (!WindowShouldClose()) {
        UpdateGame(&game);
        DrawFrame(&game);
    }
    CloseWindow();
    return 0;
}

// Fonksiyonlar
// Load highscore
int loadHighscore(void) {
    FILE *file;
    int highscore = 0;
    file = fopen(HIGHSCORE_FILE, "r");
    if(file != NULL){
        fscanf(file, "%d", &highscore);
    }
    fclose(file);
    return highscore;
}

// Save highscore
void saveHighscore(int highscore) {
    FILE *file;
    file = fopen (HIGHSCORE_FILE, "w");
    if(file != NULL){
        fprintf(file, "%d", highscore);
        fclose(file);
    }
}

// Update highscore
int updateHighscore(int score, int highscore) {
    if(score > highscore){
        highscore = score;
        saveHighscore(highscore);
    }
    return highscore;
}

// Initialize game stats
static void GameInit(Game *game) {
    game->score = 0;
    game->speed = EASY;
    game->theLine = screenHeight - tileHeight * 1.2;
    for(int i = 0; i < tileNumber; i++) {
        game->tiles[i] = (Tile){
            GetRandomValue(0, 3),
            -tileHeight,
            false
        };
    }
    game->gameState = PLAYING;
    for(int i = 0; i < 5; i++) game->pulses[i] = (Pulse){0.0f, BLACK, false};
    game->generated = 0;
    game->scored = 0;
    GenerateRandomTile(game, GetRandomValue(0, 3));
    game->highscore = loadHighscore();
}

// Game over functions, update high score
static void GameEnd(Game *game) {
    game->gameState = END;
    game->highscore = updateHighscore(game->score, game->highscore);
}

// Generate tile in one of four collumns
static void GenerateRandomTile(Game *game, int prev) {
    int col = GetRandomValue(0, 3);
    if(col == prev) col = (col + 1) % 4; // Ensures they don't spawn on the same column
    Tile tile = {
        col,
        -tileHeight, // Spawn just outside the window
        true
    };
    for(int i = 0; i < tileNumber; i++) {
        if(!game->tiles[i].active) {
            if((game->generated < ROUND_1 && game->speed == EASY) || (game->generated < ROUND_2 && game->speed == MEDIUM) || game->speed == HARD) { // If the round & generated tiles align, spawn. Otherwise, it means a round is over -> wait until all tiles have been clicked before generating
                game->tiles[i] = tile;
                game->generated++;
                return;
            }
        }
    }
    // If there are no active tiles (and this function is called), increase the difficulty
    if(game->speed == EASY) game->speed = MEDIUM;
    else game->speed = HARD;
}

// Check if tile is close to theLine
static void CheckTile(Game* game, int col) {
    int bestIndex = -1;
    float bestDistance = 0;
    for (int i = 0; i < tileNumber; i++) {
        if(game->tiles[i].active) {
            if(game->tiles[i].column == col) {
                // Distance checking to theLine
                float dist = fabsf(game->tiles[i].y - game->theLine);
                if(dist < tileHeight) {
                    bestIndex = i;
                    bestDistance = dist;
                }
            }
        }
    }
    if(bestIndex != -1) {
        if(bestDistance < (float)(tileHeight) * 0.2f || bestDistance > (float)(tileHeight) * 0.8f) {
            game->pulses[4] = (Pulse){1.1f, GOLD, true};
            game->score += 3;
        } // Good: 3 pts if tile is [0, 20] & [80, 100]% close
        else if(bestDistance < (float)(tileHeight) * 0.4f || bestDistance > (float)(tileHeight) * 0.6f) {
            game->pulses[4] = (Pulse){1.1f, DARKBLUE, true};
            game->score += 6;
        } // Great: 6 pts if tile is [20, 40] & [60, 80]% close
        else {
            game->pulses[4] = (Pulse){1.1f, GREEN, true};
            game->score += 10;
        } // Perfect: 10 pts if tile is [40 - 60]% close

        game->tiles[bestIndex].active = false;
        game->tiles[bestIndex].y = -tileHeight;
        game->scored++;
    } else { // End game if wrong button clicked or timing is wrong
        GameEnd(game);
    }
}

// Check if each tile is outside of the screen
static void CheckTilesOutsideScreen(Game* game) {
    for (int i = 0; i < tileNumber; i++) {
        if(game->tiles[i].active) {
            // Game over state
            if(game->tiles[i].y > screenHeight) GameEnd(game);
        }
    }
}

// Update game physics
static void UpdateGame(Game *game) {
    if(game->gameState == END) {
        if(IsKeyPressed(KEY_R)) { // Restart Game
            GameInit(game);
        }
    } else if(game->gameState == PLAYING) {
        if(IsKeyPressed(KEY_A)) { // Column 0
            game->pulses[0] = (Pulse){1.0f, GREEN, true};
            CheckTile(game, 0);
        } else if(IsKeyPressed(KEY_S)) { // Column 1
            game->pulses[1] = (Pulse){1.0f, BLUE, true};
            CheckTile(game, 1);
        } else if(IsKeyPressed(KEY_K)) { // Column 2
            game->pulses[2] = (Pulse){1.0f, PINK, true};
            CheckTile(game, 2);
        } else if(IsKeyPressed(KEY_L)) { // Column 3
            game->pulses[3] = (Pulse){1.0f, ORANGE, true};
            CheckTile(game, 3);
        } else if(IsKeyPressed(KEY_SPACE)) { // Pause Game
            game->gameState = PAUSE;
        }

        // Update pulses
        for(int i = 0; i < 5; i++) if(game->pulses[i].active) {
            game->pulses[i].life -= GetFrameTime();
            if(game->pulses[i].life <= 0) {
                game->pulses[i].life = 0.0f;
                game->pulses[i].active = false;
            }
        }

        for(int i = 0; i < tileNumber; i++) {
            if(game->tiles[i].active)  game->tiles[i].y += game->speed;
            if(game->tiles[i].y == 0 || // Generate tiles consecutively
                (game->scored == ROUND_1 && game->generated == ROUND_1) || // Generate first tile when changing rounds and all tiles have been clicked
                (game->scored == ROUND_2 && game->generated == ROUND_2)) 
                GenerateRandomTile(game, game->tiles[i].column);
        }
        CheckTilesOutsideScreen(game);
    } else {
        if(IsKeyPressed(KEY_SPACE)) { // Resume Game
            game->gameState = PLAYING;
        }
    }
} 

// Draw game frame
static void DrawFrame(Game *game) {
    BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw lines
        for(int i = 0; i < 4; i++) DrawRectangleLines(screenWidth / 4 * i, 0, screenWidth / 4, screenHeight, DARKGRAY);
        DrawLine(0, game->theLine, screenWidth, game->theLine, BLACK);

        // If playing
        if(game->gameState == PLAYING || game->gameState == PAUSE) {
            // Draw Tiles
            for(int i = 0; i < tileNumber; i++) {
                if(game->tiles[i].active) {
                    DrawRectangle(game->tiles[i].column * tileSize, game->tiles[i].y, tileSize, tileHeight, BLACK);
                }
            }

            // Draw Controls
            // Reappearing Letters: (game->pulses[0].active ? Fade(game->pulses[0].color, game->pulses[0].life + 0.2f) : Fade(BLACK, 0.7f))
            DrawText("a", tileSize / 4, screenHeight - tileSize, tileSize / 2, Fade(game->pulses[0].color, game->pulses[0].life + 0.2f));
            DrawText("s", tileSize + tileSize / 4, screenHeight - tileSize, tileSize / 2, Fade(game->pulses[1].color, game->pulses[1].life + 0.2f));
            DrawText("k", tileSize * 2 + tileSize / 4, screenHeight - tileSize, tileSize / 2, Fade(game->pulses[2].color, game->pulses[2].life + 0.2f));
            DrawText("l", tileSize * 3 + tileSize / 4, screenHeight - tileSize, tileSize / 2, Fade(game->pulses[3].color, game->pulses[3].life + 0.2f));
            
            // Draw score
            DrawText(TextFormat("Score: %d", game->score), 10, 10, 40, GREEN);

            // Draw achievement
            if(game->pulses[4].active) {
                if(ColorIsEqual(game->pulses[4].color, GOLD)) DrawText("Good!", 10, 60, 30, Fade(game->pulses[4].color, game->pulses[4].life));
                else if(ColorIsEqual(game->pulses[4].color, DARKBLUE)) DrawText("Great!", 10, 60, 35, Fade(game->pulses[4].color, game->pulses[4].life));
                else DrawText("Perfect!", 10, 60, 40, Fade(game->pulses[4].color, game->pulses[4].life));
            }
            

            if(game->gameState == PAUSE) {
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GRAY, 0.8f));
                DrawText("PAUSED!", screenWidth / 2 - MeasureText("PAUSED!", 40) / 2, screenHeight / 2 - 20, 40, BLACK);
                DrawText("Press SPACE to resume!", screenWidth / 2 - MeasureText("Press SPACE to resume!", 20) / 2, screenHeight / 2 + 30, 20, BLACK);
            }
        }
        else {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GRAY, 0.6f));
            DrawRectangle(0, screenHeight / 2 - 60, screenWidth, 190, Fade(WHITE, 0.9f));
            DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 40) / 2, screenHeight / 2 - 40, 40, RED);
            DrawText("Press R to restart", screenWidth / 2 - MeasureText("Press R to restart", 20) / 2, screenHeight / 2 + 10, 20, BLACK);
            DrawText(TextFormat("Score: %d", game->score), screenWidth / 2 - MeasureText(TextFormat("Score: %d", game->score), 30) / 2, screenHeight / 2 + 40, 30, ORANGE);
            DrawText(TextFormat("Highscore: %d", game->highscore), screenWidth / 2 - MeasureText(TextFormat("Highscore: %d", game->highscore), 30) / 2, screenHeight / 2 + 80, 30, ORANGE);
        }
    EndDrawing();
}