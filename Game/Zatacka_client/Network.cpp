#include "player.h"
#include "Network.h"

#define IPADDR "192.168.1.102" //"medvekialtas.ddns.net"

using namespace std;

int state_to_delay(State state){
    if (state==waiting_players)
        return 500;
    else if(state==warmup)
        return 100;
    else if(state==game || state==start)
        return 1;
    return 1000;
}

char char_to_state(char a){
    return (State)(a-64);
}

char state_to_char(State state){
    return (char)state+64;
}

int char_to_int(char a){
    return a-48;
}

int char_to_num(char a){
    return a-48;
}

int get_one_num(char *buff, int idx){
    return char_to_num(buff[idx]);
}

int get_three_num(char *buff, int idx){
    int ret=0;
    ret=char_to_num(buff[idx])*100+char_to_num(buff[idx+1])*10+char_to_num(buff[idx+2]);
    return ret;
}

int get_four_num(char *buff, int idx){
    int ret=0;
    ret=char_to_num(buff[idx])*1000+char_to_num(buff[idx+1])*100+char_to_num(buff[idx+2])*10+char_to_num(buff[idx+3]);
    return ret;
}

bool get_bool(char *buff, int idx){
    if (buff[idx]=='1')
        return true;
    return false;
}

char num_to_char(char a){
    return a+48;
}

void set_one_num(char *buff, int idx, int num){
    buff[idx]=num_to_char(num);
}

void set_three_num(char *buff, int idx, double num){
    buff[idx]=num_to_char((int)(num/100)%10);
    buff[idx+1]=num_to_char((int)(num/10)%10);
    buff[idx+2]=num_to_char((int)num%10);
}

void set_four_num(char *buff, int idx, double num){
    buff[idx]=num_to_char(((int)num/1000)%10); buff[idx+1]=num_to_char(((int)num/100)%10);
    buff[idx+2]=num_to_char(((int)num/10)%10); buff[idx+3]=num_to_char((int)num%10);
}

void set_bool(char *buff, int idx, bool a){
    if (a)
        buff[idx]='1';
    else
        buff[idx]='0';
}



void Network(State &state, UDPSocket &Socket, WSASession &Session, Adatok jatekosok[8], int &jatekos_id, int& player_count, bool send, Player players[8]){
    char inp[30];
    char out[200];
    if (send){
        for (int i=0; i<30; i++) inp[i]='\0';
        if (state==waiting_players && jatekos_id==-2){
            inp[0]='A'; inp[1]='c';
            jatekos_id=-1;
        }
        else if (state==waiting_players && jatekos_id>-1){
            inp[0]='A'; inp[1]='v';
        }
        else if (state==warmup){
            inp[0]='B';
        }
        else if (state==start){
            inp[0]='C';
        }
        else if (state==game){
            inp[0]='D'; set_one_num(inp,1,jatekos_id);
            set_bool(inp,2,jatekosok[jatekos_id].W);
            set_bool(inp,3,jatekosok[jatekos_id].E);
            set_bool(inp,4,jatekosok[jatekos_id].SPACE);
            set_bool(inp,5,jatekosok[jatekos_id].alive);
            set_four_num(inp,6,jatekosok[jatekos_id].x);
            set_four_num(inp,10,jatekosok[jatekos_id].y);
            set_four_num(inp,14,jatekosok[jatekos_id].angle);
            set_three_num(inp,17,jatekosok[jatekos_id].points);
            set_one_num(inp,20,jatekosok[jatekos_id].bullet_type);
        }
        else
            inp[0]='E';
        Socket.SendTo(IPADDR, PORT, inp, 30);
        //std::cout<<"Send: "<<inp<<endl;
    }

    else {

        try {
            struct timeval timeout={10,0}; //set timeout for 2 seconds
            setsockopt(Socket.sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
            Socket.RecvFrom(out, 200);
        }
        catch (std::exception& e){
            cout<<"G_E"<<endl;
            cout << e.what();
            return ;
        }
        //std::cout<<"Recv: "<<out<<endl;
        if (state==waiting_players && jatekos_id==-1){

            //cout<<out<<endl;
            //cout<<"CALMA"<<endl;
            if (out[0]==state_to_char(state)){
                jatekos_id=char_to_int(out[1]);
                jatekosok[jatekos_id].x=char_to_int(out[2])*100;
                jatekosok[jatekos_id].y=char_to_int(out[3])*100;
                jatekosok[jatekos_id].angle=(double)char_to_int(out[4])/3.0;
                jatekosok[jatekos_id].alive=true;
            }
            else
                return ;
        }
        else if (state==waiting_players || state==warmup && jatekos_id!=-1){

            if (warmup==char_to_state(out[0])){
                player_count=char_to_num(out[2]);
                for (int i=0; i<player_count; i++){
                    jatekosok[i].x=get_four_num(out,3+11*i);
                    jatekosok[i].y=get_four_num(out,7+11*i);
                    jatekosok[i].angle=get_four_num(out,11+11*i);
                }
                state=warmup;
            }
            if (start==char_to_state(out[0])){
                state=start;
            }
        }
        else if (state==start){
            if (game==char_to_state(out[0])){
                state=game;
            }
        }

        else if (state==game){
            for (int i=0; i<get_one_num(out,1); i++){
                jatekosok[i].W=get_bool(out,2+i*19);
                jatekosok[i].E=get_bool(out,3+i*19);
                jatekosok[i].SPACE=get_bool(out,4+i*19);
                jatekosok[i].alive=get_bool(out,5+i*19);
                jatekosok[i].bullet_type=get_one_num(out,6+i*19);
                jatekosok[i].x=get_four_num(out,7+i*19);
                jatekosok[i].y=get_four_num(out,11+i*19);
                jatekosok[i].angle=get_three_num(out,15+i*19);
                jatekosok[i].points=get_three_num(out,18+i*19);
            }
        }
    }
    return ;
}
