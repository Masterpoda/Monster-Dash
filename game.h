#ifndef __GAME_H
#define __GAME_H
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "playfield.h"
#include "monster.h"
#include "player.h"

using namespace std;

class Game
{
    Playfield* playfield;     // the playfield
    char MONSTER;        // name of the monster (A - Z)
    char PLAYER = '@';    // name of the player
    int N;               // board dimension
    Monster* monster;     // the monster
    Player* player;         // the player
    int moveCount;
    bool gameOver;

public:

    Game();
    void readFile(const char *fileName);
    Site* getMonsterSite() {
        return playfield->getMonsterSite();
    }
    Site* getPlayerSite()   {
        return playfield->getPlayerSite();
    }
    Playfield* getPlayfield()  {
        return playfield;
    }
    void executeMove();
    bool isGameOver() {
        return gameOver ;
    }
    int getMoveCount() {
        return moveCount ;
    }
};

#endif
