#include "player.h"
#include "Network.h"

#include <thread>

using namespace std;

class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}

const char * InitError::what() const throw()
{
    return msg.c_str();
}

class SDL
{
public:
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
public:
    SDL( Uint32 flags = 0 );
    virtual ~SDL();
    void draw();
};

SDL::SDL( Uint32 flags )
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError();

    if ( SDL_CreateWindowAndRenderer( WIN_SIZE_X, WIN_SIZE_Y, SDL_WINDOW_SHOWN,
                                      &m_window, &m_renderer ) != 0 )
        throw InitError();
}

SDL::~SDL()
{
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    SDL_Quit();
}

void SDL::draw()
{
    // Clear the window with a black background
    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
    SDL_RenderClear( m_renderer );

    // Show the window
    SDL_RenderPresent( m_renderer );

    int rgb[] = { 203, 203, 203, // Gray
                  254, 254,  31, // Yellow
                    0, 255, 255, // Cyan
                    0, 254,  30, // Green
                  255,  16, 253, // Magenta
                  253,   3,   2, // Red
                   18,  14, 252, // Blue
                    0,   0,   0  // Black
                };

    SDL_Rect colorBar;
    colorBar.x = 0; colorBar.y = 0; colorBar.w = 90; colorBar.h = 480;

    // Render a new color bar every 0.5 seconds
    for ( int i = 0; i != sizeof rgb / sizeof *rgb; i += 3, colorBar.x += 90 )
    {
        SDL_SetRenderDrawColor( m_renderer, rgb[i], rgb[i + 1], rgb[i + 2], 255 );
        SDL_RenderFillRect( m_renderer, &colorBar );
        SDL_RenderPresent( m_renderer );
        SDL_Delay( 500 );
    }
}


//copy_Adatok_to_Players(Adatok jatekosok[8], int player_count,Player players[8]){}

void megjelenites(SDL_Renderer *renderer, Player* players, int &player_count, int &jatekos_id,int &counter,State &state){//, int &counter, bool &state){
    bool opt=true;
    cout<<"oks"<<endl;
    //cout<<CLOCKS_PER_SEC<<endl;
    clock_t t_start;
    clock_t t_lastDown;
    while (true){
        if (counter==0){
            t_start=clock();
            t_lastDown=clock();
            counter=1;
        }
        clock_t t1;
        clock_t delay=60;
        if (clock()>t1+CLOCKS_PER_SEC/delay){
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderClear(renderer);
            for (int i=0; i<player_count; i++){
                cout<<t_lastDown<<" "<<clock()<<endl;
                if (t_lastDown+1.9*CLOCKS_PER_SEC>clock())
                    players[i].down=true;
                else if (t_lastDown+2.2*CLOCKS_PER_SEC>clock())
                    players[i].down=false;
                else
                {
                    t_lastDown=clock();
                    players[i].down=true;
                }
                if (state==game && i==jatekos_id)
                    players[i].move_tick(opt, players, player_count,true);
                else if (state==game && i!=jatekos_id)
                    players[i].move_tick(opt, players, player_count,false);
                SDL_SetRenderDrawColor_Player(*renderer, i);
                players[i].drawPos(*renderer);
                players[i].drawObj(*renderer);

            }
            SDL_RenderPresent(renderer);
            t1=clock();
        } else {
            Sleep(1);
        }

    }
}

void Network_2(State &state, UDPSocket &Socket, WSASession &Session, Adatok jatekosok[8], int &jatekos_id, int& player_count, Player players[8]){
    auto start_time = std::chrono::steady_clock::now();
    while(true){

        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count()<16*LITE){
                //std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count()<<std::endl;
            SDL_Delay(state_to_delay(state));
        }
        else {
            //Sleep(state_to_delay(state));
            start_time = std::chrono::steady_clock::now();
            if (state==game){
                //counter+=1;
                jatekosok[jatekos_id].alive=players[jatekos_id].alive;
                jatekosok[jatekos_id].W=players[jatekos_id].W;
                jatekosok[jatekos_id].E=players[jatekos_id].E;
                jatekosok[jatekos_id].SPACE=players[jatekos_id].SPACE;
                jatekosok[jatekos_id].x=players[jatekos_id].x;
                jatekosok[jatekos_id].y=players[jatekos_id].y;
                jatekosok[jatekos_id].angle=players[jatekos_id].angle;
            }

            Network(state,Socket,Session,jatekosok,jatekos_id,player_count,true,players);

            Network(state,Socket,Session,jatekosok,jatekos_id,player_count,false,players);

            for (int i=0; i<player_count; i++){
                if (state!=warmup && i!=jatekos_id){
                    players[i].alive=jatekosok[i].alive;
                    players[i].W=jatekosok[i].W;
                    players[i].E=jatekosok[i].E;
                    players[i].SPACE=jatekosok[i].SPACE;
                    players[i].x=jatekosok[i].x;
                    players[i].y=jatekosok[i].y;
                    players[i].angle=jatekosok[i].angle;
                }
                if (state==warmup){
                    players[i].alive=jatekosok[i].alive;
                    players[i].W=jatekosok[i].W;
                    players[i].E=jatekosok[i].E;
                    players[i].SPACE=jatekosok[i].SPACE;
                    players[i].x=jatekosok[i].x;
                    players[i].y=jatekosok[i].y;
                    players[i].angle=jatekosok[i].angle;
                }
            }
        }
    }
}

int main( int argc, char * argv[] )
{
    srand(time(NULL));
    bool opt=true;
    char inp[30];
    WSASession Session;
    UDPSocket Socket;
    State state=waiting_players;
    Adatok *jatekosok=new Adatok[8];

    int state_2=0;
    int jatekos_id=-2;
    int player_count=-1;
        //SDL sdl( SDL_INIT_VIDEO | SDL_INIT_TIMER );

            srand(time(NULL));
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_CreateWindowAndRenderer( SZELES, MAGAS, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer );
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear( renderer );
        SDL_RenderPresent( renderer );

        SDL_Event ev;


        Player *players=new Player[8];

        //cout<<"alma"<<players[0].rect1<<endl;

        time_t t1=time(NULL);
        auto start_time = std::chrono::steady_clock::now();
        //auto ended = std::chrono::steady_clock::now();
        //std::cout<<"CECIL: "<<std::chrono::duration_cast<std::chrono::milliseconds>(ended - start).count()<<std::endl;
        int counter=-1, temp_count=0;
        thread frame(megjelenites,renderer,ref(players),ref(player_count),ref(jatekos_id),ref(counter),ref(state));
        thread network(Network_2,ref(state),ref(Socket),ref(Session),ref(jatekosok),ref(jatekos_id),ref(player_count),ref(players));
        while (counter<10000){
            SDL_PollEvent(&ev);
            if (jatekos_id>-1){
                if (ev.key.type==SDL_KEYDOWN){
                    //std::cout<<"down"<<std::endl;
                    if (ev.key.keysym.sym==SDLK_SPACE)
                        players[jatekos_id].SPACE=true;
                    else if (ev.key.keysym.sym==SDLK_w)
                        players[jatekos_id].W=true;
                    else if (ev.key.keysym.sym==SDLK_e)
                        players[jatekos_id].E=true;
                }
                else if (ev.key.type==SDL_KEYUP){
                    if (ev.key.keysym.sym==SDLK_SPACE)
                        players[jatekos_id].SPACE=false;
                    else if (ev.key.keysym.sym==SDLK_w)
                        players[jatekos_id].W=false;
                    else if (ev.key.keysym.sym==SDLK_e)
                        players[jatekos_id].E=false;
                }
            }
            if (state_2==0 && state==waiting_players){
                cout<<"Waiting for players..."<<endl;
                state_2=1;
            }
            else if (state_2==1 && state==warmup){
                cout<<"Warming up..."<<endl;
                state_2=2;
            }
            else if (state_2==2 && state==start){
                cout<<"Start in 1 sec..."<<endl;
                //,ref(state),jatekos_id);
                cout<<"ok"<<endl;
                state_2=3;
            }
            else if (state_2==3 && state==game){
                counter=0;
                cout<<"Game..."<<endl;
                state_2=4;
            }
            if (t1<time(NULL)){
                if (state==game)
                    std::cout<<"Frame count: "<<counter<<"  Frame/sec: "<<counter-temp_count<<std::endl;
                temp_count=counter;
                t1=time(NULL);
            }
            //SDL_Delay(10);
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count()<16*LITE){
                //std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count()<<std::endl;
                SDL_Delay(state_to_delay(state));
            }
            else {

                start_time = std::chrono::steady_clock::now();
                /*
                if (state==game){
                    //counter+=1;
                    jatekosok[jatekos_id].alive=players[jatekos_id].alive;
                    jatekosok[jatekos_id].W=players[jatekos_id].W;
                    jatekosok[jatekos_id].E=players[jatekos_id].E;
                    jatekosok[jatekos_id].SPACE=players[jatekos_id].SPACE;
                    jatekosok[jatekos_id].x=players[jatekos_id].x;
                    jatekosok[jatekos_id].y=players[jatekos_id].y;
                    jatekosok[jatekos_id].angle=players[jatekos_id].angle;
                }

                //Network(state,Socket,Session,jatekosok,jatekos_id,player_count,true,players);

                //Network(state,Socket,Session,jatekosok,jatekos_id,player_count,false,players);

                for (int i=0; i<player_count; i++){
                    if (state!=warmup && i!=jatekos_id){
                        players[i].alive=jatekosok[i].alive;
                        players[i].W=jatekosok[i].W;
                        players[i].E=jatekosok[i].E;
                        players[i].SPACE=jatekosok[i].SPACE;
                        players[i].x=jatekosok[i].x;
                        players[i].y=jatekosok[i].y;
                        players[i].angle=jatekosok[i].angle;
                    }
                    if (state==warmup){
                        players[i].alive=jatekosok[i].alive;
                        players[i].W=jatekosok[i].W;
                        players[i].E=jatekosok[i].E;
                        players[i].SPACE=jatekosok[i].SPACE;
                        players[i].x=jatekosok[i].x;
                        players[i].y=jatekosok[i].y;
                        players[i].angle=jatekosok[i].angle;
                    }
                }
                */

                /*
                SDL_SetRenderDrawColor(sdl.m_renderer,0,0,0,255);
                SDL_RenderClear(sdl.m_renderer);
                for (int i=0; i<player_count; i++){
                    if (counter%130>115)
                        players[i].down=false;
                    else
                        players[i].down=true;
                    if (counter%2==0)
                        opt=true;
                    else
                        opt=false;
                    //std::cout<<"P3Counter: "<<players[jatekos_id].rect1<<" "<<players[jatekos_id].rect2<<std::endl;
                    if (state==game && i==jatekos_id)
                        players[i].move_tick(opt, players, player_count,true);
                    else if (state==game && i!=jatekos_id)
                        players[i].move_tick(opt, players, player_count,false);
                    //std::cout<<"PCounter: "<<players[jatekos_id].rect1<<std::endl;
                    SDL_SetRenderDrawColor_Player(*sdl.m_renderer, i);
                    players[i].drawPos(*sdl.m_renderer);
                    players[i].drawObj(*sdl.m_renderer);

                }
                SDL_RenderPresent(sdl.m_renderer);
                */
            }

        }
        SDL_Delay(1000);

        return 0;
    /*}
    catch ( const InitError & err )
    {
        std::cerr << "Error while initializing SDL:  "
                  << err.what()
                  << std::endl;
    }*/



    //return 1;
}
