#include "babis.h"

using namespace std;

/*The algorithm for the movement of Babis is Dijkstra's algorithm where the distances between positions on the map are considered as weights,
and the algorithm calculates the minimum distance from the start position to all other positions */
vector<vector<int>> babis::computeShortestPath(const vector<vector<char>> &map, const pair<int, int> &start)
{
    int numRows = map.size();
    int numCols = map[0].size();
    vector<vector<int>> distance(numRows, vector<int>(numCols, INF));
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
    distance[start.first][start.second] = 0;
    pq.push(make_pair(0, start));
    while (!pq.empty())
    {

        pair<int, int> currPos = pq.top().second;
        pq.pop();
        int currDist = distance[currPos.first][currPos.second];

        for (const auto &move : MOVES)
        {
            if (move.first == -1)
                check = KEY_UP;
            else if (move.first == 1)
                check = KEY_DOWN;
            else if (move.second == -1)
                check = KEY_LEFT;
            else if (move.second == 1)
                check = KEY_RIGHT;

            int newRow = currPos.first + move.first;
            int newCol = currPos.second + move.second;

            if (DoMove(check, currPos, map) != currPos)
            {

                int newDist = currDist + 1;

                if (newDist < distance[newRow][newCol])
                {
                    distance[newRow][newCol] = newDist;
                    pq.push(make_pair(newDist, make_pair(newRow, newCol)));
                }
            }
        }
    }

    return distance;
}

pair<int, int> babis::BabisMv(pair<int, int> currBabPos, pair<int, int> petPos, const vector<vector<char>> map)
{
    int numRows = map.size();
    int numCols = map[0].size();

    vector<vector<int>> babisShortestPath = computeShortestPath(map, currBabPos);

    vector<vector<int>> petradiShortestPath = computeShortestPath(map, petPos);

    int minDist = INF;
    pair<int, int> nextPos = currBabPos;

    for (const auto &move : MOVES)
    {
        if (move.first == -1)
            check = KEY_UP;
        else if (move.first == 1)
            check = KEY_DOWN;
        else if (move.second == -1)
            check = KEY_LEFT;
        else if (move.second == 1)
            check = KEY_RIGHT;

        int newRow = currBabPos.first + move.first;
        int newCol = currBabPos.second + move.second;

        if (DoMove(check, currBabPos, map) != currBabPos)
        {

            int babisToPetradiDist = babisShortestPath[newRow][newCol] + petradiShortestPath[newRow][newCol];

            if (babisToPetradiDist < minDist)
            {
                minDist = babisToPetradiDist;
                nextPos = make_pair(newRow, newCol);
            }
        }
    }

    return nextPos;
}
