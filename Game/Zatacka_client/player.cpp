#include "player.h"

#define PI 2*acos(0.0)

using namespace std;

void SDL_SetRenderDrawColor_Player(SDL_Renderer &renderer, int i){
    if (i==0)       SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);
    else if (i==1)  SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
    else if (i==2)  SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
    else if (i==3)  SDL_SetRenderDrawColor(&renderer, 255, 255, 0, 255);
    else if (i==4)  SDL_SetRenderDrawColor(&renderer, 255, 0, 255, 255);
    else if (i==5)  SDL_SetRenderDrawColor(&renderer, 0, 255, 255, 255);
    else if (i==6)  SDL_SetRenderDrawColor(&renderer, 100, 100, 200, 255);
    else if (i==7)  SDL_SetRenderDrawColor(&renderer, 200, 100, 100, 255);
}

bool in_way(SDL_Rect rect, double x, double y, int angle){
    if (fabs((double)rect.x-x)<4 && fabs((double)rect.y-y)<4){
        double temp_x=x-1+cos(angle/360.0*PI*2.0)*3.0;
        double temp_y=y-1+sin(angle/360.0*PI*2.0)*3.0;
        if (((double)rect.x-temp_x)*((double)rect.x-temp_x)+((double)rect.y-temp_y)*((double)rect.y-temp_y)<2.0)
            cout<<((double)rect.x-temp_x)*((double)rect.x-temp_x)+((double)rect.y-temp_y)*((double)rect.y-temp_y)<<endl;
        if (((double)rect.x-temp_x)*((double)rect.x-temp_x)+((double)rect.y-temp_y)*((double)rect.y-temp_y)<1.0){
            cout<<cos(angle/360.0*PI*2.0)<<' '<<sin(angle/360.0*PI*2.0)<<endl;

            cout<<x<<" "<<y<<endl;
            cout<<temp_x<<" "<<temp_y<<endl;
            cout<<rect.x<<" "<<rect.y<<endl;
            return true;
        }
    }
    return false;
}

bool Player::chk_obj(Player players[8], int player_count){
    for (int i=0; i<player_count; i++){
        for (int j=0; j<players[i].rect1; j++){
            for (int k=0; k<500; k++){
                if (in_way(players[i].rects[j][k],x,y,angle))
                    return true;
            }
        }
        for (int k=0; k<players[i].rect2; k++){
            if (in_way(players[i].rects[players[i].rect1][k],x,y,angle))
                return true;
        }
    }
    return false;
}


void Player::drawPos(SDL_Renderer &renderer){
    //if (alive){
        SDL_Rect temp;
        temp.h=4; temp.w=4; temp.x=x-1; temp.y=y-1;
        SDL_RenderFillRect(&renderer, &temp);
    //}
}

void Player::drawObj(SDL_Renderer &renderer){
    for (int i=0; i<rect1; i++)
        SDL_RenderFillRects(&renderer,rects[i],500);
    SDL_RenderFillRects(&renderer,rects[rect1],rect2);
}

void Player::move_tick(bool opt, Player players[8], int player_count, bool modf){
    if (modf){
        if (alive){
            if (E)
                angle+=2.0*LITE;
            if (W)
                angle-=2.0*LITE;
        }
        if (chk_obj(players, player_count))
            alive=false;
    }
    if (alive){

        if (modf){
            x+=cos(angle/360.0*PI*2.0)*LITE;
            y+=sin(angle/360.0*PI*2.0)*LITE;

            if (x<0)
                x+=WIN_SIZE_X;
            else if (x>WIN_SIZE_X-1)
                x-=WIN_SIZE_X;

            if (y<0)
                y+=WIN_SIZE_Y;
            else if (y>WIN_SIZE_Y-1)
                y-=WIN_SIZE_Y;
        }

        if (down && opt){
            rects[rect1][rect2].x=x-1;
            rects[rect1][rect2].y=y-1;
            rect2++;
            if (rect2>499){
                rect1++; rect2=0;
            }
        }
    }
    else{
        x=-10;
        y=-10;
    }
}
