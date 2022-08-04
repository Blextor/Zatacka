#include "Network.h"

using namespace std;

#define IDOZITES 1
#define OUT_BUF_LEN 500

class Main_data{
public:
    WSASession Session;
    UDPSocket Socket;
    State state=waiting_players;
    int player_count;
    Adatok jatekosok[8];
    int warmup_sec;
    time_t t1;
    char kb_input[20];

public:
    //Main_data(){state=waiting_players; player_count=0; warmup_sec=4; t1=time(NULL);}

    void operator()(){
        state=waiting_players; player_count=0; warmup_sec=4; t1=time(NULL);
        cout<<"alma"<<endl;
        Socket.Bind(PORT);
        cout<<"almafa"<<endl;
        while (1){
        Network(warmup_sec,state,Socket,Session,jatekosok,player_count,false);
        Network(warmup_sec,state,Socket,Session,jatekosok,player_count,true);
        }
    }

    /*void Network(int warmup_sec, State &state, UDPSocket &Socket, WSASession &Session,
             Adatok jatekosok[8], int &player_count, bool send){


    }*/
};


void read_input(char input[20]){
    while (true)
        std::cin>>input;
}

void msg_control(Main_data data){}

int main()
{
    srand(time(NULL));
    WSASession Session;
    UDPSocket Socket;
    State state=waiting_players;
    int player_count=0;
    Adatok jatekosok[8];

    int warmup_sec=4;
    time_t t1=time(NULL);
    Socket.Bind(PORT);
    char kb_input[20]="cern";

    Main_data data();

    cout<<"Start"<<endl;
    std::thread first (read_input,kb_input);
    //std::thread second (Network,warmup_sec,state,Socket,Session,jatekosok,player_count,false);
    //first.join();
    //second.join();
    while (1)
    {

        if (0==strcmp(kb_input,"alma"))
            cout<<"Siker?"<<endl;
        //first.join();
        if (t1<time(NULL)-5 && state==waiting_players){
            state=warmup;
            t1=time(NULL);
        }
        if (state==warmup && t1<time(NULL) && warmup_sec!=0){
            t1=time(NULL);
            warmup_sec--;
            if (warmup_sec==0)
                state=start;
        }
        if (t1<time(NULL) && state==start){
            state=game;
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(IDOZITES));
        //std::cout<<"Start Recv:"<<std::endl;

        //std::cout<<buffer<<" "<< inet_ntoa(add.sin_addr) <<std::endl;
        //std::string input(buffer);
        ///std::reverse(std::begin(input), std::end(input));
        //std::cout<<"Start Send:"<<std::endl;
        Network(warmup_sec,state,Socket,Session,jatekosok,player_count,false);
        Network(warmup_sec,state,Socket,Session,jatekosok,player_count,true);



        //Socket.SendTo(add, out_buffer, OUT_BUF_LEN);






    }

}
