#ifndef ENGINE_H
#define ENGINE_H

#include "moving.h"
#include "babis.h"
#include <string>
#include <atomic>
#include <iostream>
#include <filesystem>
#include <ncurses.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <thread>
#include <random>

/*this engine class is the whole brain of the game*/
using namespace std;

class engine
{
private:
    moving move;
    babis babi;
    int rowSize;
    int colSize;
    int mv;
    pair<bool, bool> gameWon;
    bool gameLost;
    vector<vector<char>> map;

public:
    engine(moving &mov, babis &bab);
    pair<int, int> RandomMapPlaceGenerator(int row, int col) const;
    void CreateMap(const string &userMap);
    void AddCharacters();
    void RemoveCharacters();
    void Petradi();
    pair<int, int> getPosition(char character) const;
    pair<bool, bool> setMove(int c);
    void startGame();
    bool moveBabis();
    bool updateMap(int row, int col, char Char);
    void ShowMap() const;
    void YouWon() const;
    void Teleporting() const;
    void YouLost() const;
    void key() const;
    void quitter() const;
    void welcome() const;
};

#endif