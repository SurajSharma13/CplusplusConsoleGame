#include <fstream>
#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "engine.h"
#include "moving.h"
#include "babis.h"
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    initscr();
    moving move;
    babis babi;
    engine obj(move, babi);
    obj.CreateMap(argv[1]);
    obj.welcome();
    this_thread::sleep_for(chrono::seconds(4));
    obj.startGame();
    endwin();
    return 0;
}
