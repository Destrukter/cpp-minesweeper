#ifndef GAME_H
#define GAME_H
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

struct Highscore {
    string name;
    int score;
};

void menu();
void tutorial();
void printhighscores(vector<Highscore> currenths);
int fieldsizesettings(int current, string what);
int minesettings(const int c, const int r, int m);
void linetoint(string linetemp, int &result);
void linetolong(string linetemp, long &result);
void demoplay();
void play(vector <Highscore> &highscores);
#endif // GAME_H
