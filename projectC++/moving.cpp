#include "moving.h"

using namespace std;

moving::moving()
{
}
/*This function returns the coordinates of the next position,IF POSSIBLE, for the updateMap funtion in the engine. */
pair<int, int> moving::DoMove(const int move, const pair<int, int> pos, const vector<vector<char>> map)
{

    int newRow = pos.first;
    int newCol = pos.second;
    switch (move)
    {
    case KEY_UP:
        if (!collisionDetector(move, pos, map) || winCheck(move, pos, map))
        {
            newRow--;
            return (make_pair(newRow, newCol));
        }
        break;
    case KEY_DOWN:
        if (!collisionDetector(move, pos, map) || winCheck(move, pos, map))
        {
            newRow++;
            return (make_pair(newRow, newCol));
        }
        break;
    case KEY_LEFT:
        if (!collisionDetector(move, pos, map) || winCheck(move, pos, map))
        {
            newCol--;
            return (make_pair(newRow, newCol));
        };
        break;
    case KEY_RIGHT:
        if (!collisionDetector(move, pos, map) || winCheck(move, pos, map))
        {
            newCol++;
            return (make_pair(newRow, newCol));
        }
        break;
    default:
        return (make_pair(newRow, newCol));
    }
    return (make_pair(newRow, newCol));
}

/*This prevents characters moving to any position unless its a valid path*/
bool moving::collisionDetector(const int move, const pair<int, int> pos, const vector<vector<char>> map)
{
    bool shouldMove = false;
    switch (move)
    {
    case KEY_UP:
        if (map.at(pos.first - 1).at(pos.second) != '.')
        {
            return !shouldMove;
        }
        break;
    case KEY_DOWN:
        if (map.at(pos.first + 1).at(pos.second) != '.')
        {
            return !shouldMove;
        }
        break;
    case KEY_LEFT:
        if (map.at(pos.first).at(pos.second - 1) != '.')
        {
            return !shouldMove;
        }
        break;
    case KEY_RIGHT:
        if (map.at(pos.first).at(pos.second + 1) != '.')
        {
            return !shouldMove;
        }
        break;
    default:
        return !shouldMove;
    }
    return shouldMove;
}

/*The only time characters can move to a position that is not a clear path is when its the petradi, so it wins the game*/
bool moving::winCheck(int move, pair<int, int> pos, vector<vector<char>> map)
{
    bool wonGame = true;
    switch (move)
    {
    case KEY_UP:
        if (map.at(pos.first - 1).at(pos.second) != 'P')
        {
            return !wonGame;
        }
        break;
    case KEY_DOWN:
        if (map.at(pos.first + 1).at(pos.second) != 'P')
        {
            return !wonGame;
        }
        break;
    case KEY_LEFT:
        if (map.at(pos.first).at(pos.second - 1) != 'P')
        {
            return !wonGame;
        }
        break;
    case KEY_RIGHT:
        if (map.at(pos.first).at(pos.second + 1) != 'P')
        {
            return !wonGame;
        }
        break;
    default:
        return !wonGame;
    }
    return wonGame;
}