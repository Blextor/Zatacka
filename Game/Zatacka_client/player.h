#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <exception>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <SDL.h>
//#include <SDL_image.h>

#define WIN_SIZE_X 800
#define WIN_SIZE_Y 600
#define SZELES 800
#define MAGAS 600
#define LITE 1


void SDL_SetRenderDrawColor_Player(SDL_Renderer &renderer, int i);


struct Player{

    bool W,E,SPACE,down,alive;
    double x, y, angle;
    SDL_Rect *rects[100];
    //int rect_count[100];
    int rect1, rect2;

    Player(){
        W=false; E=false; SPACE=false; down=true; alive=false; x=(rand())%800+100; y=(rand())%600+100; angle=(rand())%360;
        rect1=0; rect2=0;
        for (int i=0; i<100; i++){
            //rect_count[i]=0;
            rects[i] = new SDL_Rect[500];
            for (int j=0; j<500; j++){
                rects[i][j].h=4;
                rects[i][j].w=4;
                rects[i][j].y=-1;
                rects[i][j].x=-1;
            }
        }

    }

    bool chk_obj(Player players[8], int player_count);
    void drawPos(SDL_Renderer &renderer);
    void drawObj(SDL_Renderer &renderer);
    void move_tick(bool opt, Player players[8], int player_count, bool modf);

};

#endif // PLAYER_H_INCLUDED
