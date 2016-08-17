#include "game.h"

/***************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Constructor
 ******************************************************************************/
Game::Game() : moveCount(0), gameOver(false)
{ }

/***************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Member function that reads in the playfield datafile.  It is expected
 * that the file starts with a single integer (N) that describes the dimensions
 * of the playfield (N x N).  Room spaces are denoted by '.', corridor spaces
 * '+', the player as '@', the monster as a capital letter from A-Z, and all
 * others as wall.
 *    While the playfield is NxN, the data file should be of the form N rows
 * of
 ******************************************************************************/
void Game::readFile(const char *fileName)
{
    int N;
    ifstream fin(fileName);
    fin >> N;
    char **board = new char*[N];
    char chRow[256];

    fin.ignore();
    for (int i = 0; i < N; i++)
    {
        fin.getline(chRow, 256);
        board[i] = new char[N];
        for (int j = 0; j < N; j++)
        {
            board[i][j] = chRow[2*j];
        }
    }
    fin.close();
    playfield = new Playfield(board, N);
    monster = new Monster(playfield);
    player   = new Player(playfield);
}

/***************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    This function attempts to execute on turn of the chase.  The monster
 * moves first checking to see if it has intercepted the player.  The player
 * moves second and it checks to see if the player steps into the same
 * square as the monster.
 *
 * If either case is true, the gameOver member data is set to true so that
 * the event loop can be exited in utilityCentral.
 *
 * A side-effect of this function is that it increments the member data
 * moveCount to keep track of the number of turns that have passed.
 ******************************************************************************/
void Game::executeMove()
{
    // monster moves
    if (getMonsterSite()->equals(getPlayerSite()))
        gameOver = true;
    else
        playfield->setMonsterSite(monster->move());

    // player moves
    if (getMonsterSite()->equals(getPlayerSite()))
        gameOver = true;
    else
        playfield->setPlayerSite(player->move());

    moveCount++;
}
