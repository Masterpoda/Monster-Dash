#include "playfield.h"


/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Constructor for the playfield
 *
 * @param[in] board - the NxN board representing the input file
 * @param[in] size - the x (or y) dimension of the playfield
 *****************************************************************************/
Playfield::Playfield(char **board, int size)
{
    N = size;
    Room     = new bool*[N];
    Corridor = new bool*[N];

    for (int i = 0; i < N; i++)
    {
        Room[i] = new bool[N];
        Corridor[i] = new bool[N];
        for (int j = 0; j < N; j++)
        {
            Room[i][j] = false;
            Corridor[i][j] = false;
            if (board[i][j] == '.')
                Room[i][j] = true;
            else if (board[i][j] == '+')
                Corridor[i][j] = true;
            else if (board[i][j] == '@')
            {
                playerSite = new Site(i,j);
                Room[i][j] = '.';
            }
            else if (board[i][j] >= 'A' && board[i][j] <= 'Z')
            {
                monsterSite = new Site(i,j);
                Room[i][j] = '.';
            }
        }
    }
}

/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Check to see if a site is a corridor site
 *
 * @param[in] v - the site to query about corridorness
 *****************************************************************************/
bool Playfield::isCorridor(Site *v)
{
    int i = v->i();
    int j = v->j();
    if (i < 0 || j < 0 || i >= N || j >= N)
        return false;
    return Corridor[i][j];
}

/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Check to see if a site is a room site
 *
 * @param[in] v - the site to query about roomness
 *****************************************************************************/
bool Playfield::isRoom(Site* v)
{
    int i = v->i();
    int j = v->j();
    if (i < 0 || j < 0 || i >= N || j >= N)
        return false;
    return Room[i][j];
}

/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Check to see if a site is a wall site
 *
 * @param[in] v - the site to query about wallness
 *****************************************************************************/
bool Playfield::isWall(Site *v)
{
    return (!isRoom(v) && !isCorridor(v));
}

/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Check to see if a site is a wall site
 *
 * @param[in] v - the site to query about wallness
 *****************************************************************************/
bool Playfield::isLegalMove(Site* v, Site* w)
{
    int i1 = v->i();
    int j1 = v->j();
    int i2 = w->i();
    int j2 = w->j();

    if (i1 < 0 || j1 < 0 || i1 >= N || j1 >= N)
        return false;

    if (i2 < 0 || j2 < 0 || i2 >= N || j2 >= N)
        return false;

    if (isWall(v) || isWall(w))
        return false;

    if (abs(i1 - i2) > 1)
        return false;

    if (abs(j1 - j2) > 1)
        return false;

    if (isRoom(v) && isRoom(w))
        return true;

    if (i1 == i2)
        return true;

    if (j1 == j2)
        return true;

    return false;
}

/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Move the player to a new site
 *
 * @param[in] v - the site to the player wants to move
 *
 * Throws runtime_error if the player attempts to move to an illegal spot
 *****************************************************************************/
void Playfield::setPlayerSite(Site *v)
{
    if (isLegalMove(playerSite, v))
        playerSite = v;
    else
        throw std::runtime_error("Player caught cheating");
}

/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Move the monster to a new site
 *
 * @param[in] v - the site to the monster wants to move
 *
 * Throws runtime_error if the monster attempts to move to an illegal spot
 *****************************************************************************/
void Playfield::setMonsterSite(Site *v)
{
    if (isLegalMove(monsterSite, v))
        monsterSite = v;
    else
        throw std::runtime_error("Monster caught cheating");
}

/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Overloaded output for playfield
 *
 * @param[in] os - the output stream
 * @param[in] play - the playfield
 *****************************************************************************/
ostream& operator<<(ostream& os, Playfield *play)
{
    string line = "";
    Site *playerSite = play->playerSite;
    Site *monsterSite = play->monsterSite;

    for (int i = 0 ; i < play->N ; i++)
    {
        for (int j = 0 ; j < play->N ; j++)
        {
            Site *site = new Site(i, j);
            if (playerSite->equals(monsterSite) &&
                    playerSite->equals(site)) line += "* ";
            else if (playerSite->equals(site)) line += "@ ";
            else if (monsterSite->equals(site)) line += "M ";
            else if (play->isRoom(site)) line += ". ";
            else if (play->isCorridor(site)) line += "+ ";
            else if (play->isWall(site)) line += "  ";
        }
        line += "\n";
    }
    os << line;
    return os;
}

/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 *    Overloaded output for Site
 *
 * @param[in] os - the output stream
 * @param[in] site - the site to output
 *****************************************************************************/
ostream& operator<<(ostream& os, Site* site)
{
    os << "Site[" << site->i() << ", " << site->j() << "]" << endl;
    return os;
}
