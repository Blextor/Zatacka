#include "Network.h"

char state_to_char(State state){
    return (char)state+64;
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



void Network(int warmup_sec, State &state, UDPSocket &Socket, WSASession &Session,
              Adatok jatekosok[8], int &player_count, bool send){
    char inp[30];
    for (int i=0; i<30; i++)
        inp[i]='\0';
    char out[200];
    for (int i=0; i<200; i++)
        out[i]='\0';

    sockaddr_in add = Socket.RecvFrom(inp, 30);
    std::cout<<inp<<std::endl;

    /*
    if (state==waiting_players){
        bool ok=false;
        for (int i=0; i<8; i++){
            if (strstr(inet_ntoa(add.sin_addr), jatekosok[i].addr)){
                ok=true;
                //std::cout<<"alma"<<std::endl;
                i=9;
            }
            //std::cout<<"megvagy "<<inet_ntoa(add.sin_addr)<<" "<<jatekosok[8].addr<<std::endl;
        }
        for (int i=0; !ok && i<8; i++){
            //std::cout<<"ok";
            if (jatekosok[i].addr[0]=='a'){
                strcpy(jatekosok[i].addr, inet_ntoa(add.sin_addr));
                //std::cout<<jatekosok[i].addr<<" "<<inet_ntoa(add.sin_addr)<<std::endl;
                out_buffer[0]='A'; out_buffer[1]=' '; out_buffer[2]=48; out_buffer[2]=49+i;
                i=9;
            }
        }
    }
    */




    if (inp[0]=='A'){
        if (inp[1]=='c'){
            if (state==waiting_players){
                out[0]='A'; set_one_num(out, 1, player_count);
                jatekosok[player_count].x=(double)(rand()%600)+100;
                set_four_num(out, 2, jatekosok[player_count].x);
                jatekosok[player_count].y=(double)(rand()%400)+100;
                set_four_num(out, 6, jatekosok[player_count].y);
                jatekosok[player_count].angle=(double)(rand()%90)*4.0;
                set_three_num(out, 10, jatekosok[player_count].angle);
                std::cout<<"more"<<std::endl;
                player_count++;
            }
            else {
                out[0]='E';
                out[1]=state_to_char(state);
            }
        }
        else if (inp[1]=='v'){
            if (state==waiting_players){
                out[0]='A'; out[1]='w';
            }
            else if (state==warmup){
                out[0]='B'; set_one_num(out,1,warmup_sec);
                set_one_num(out,2,player_count);
                for (int i=0; i<player_count; i++){
                    set_four_num(out,3+i*11,jatekosok[i].x);
                    set_four_num(out,7+i*11,jatekosok[i].y);
                    set_four_num(out,11+i*11,jatekosok[i].angle);
                }
            }
            else{
                out[0]='E';
                out[1]=state_to_char(state);
            }
        }
        else {
            out[0]='E';
            out[1]=state_to_char(state);
        }
    }

    else if (inp[0]=='B'){
        if (state==warmup){
            out[0]='B'; set_one_num(out,1,warmup_sec);
            set_one_num(out,2,player_count);
            for (int i=0; i<player_count; i++){
                set_four_num(out,3+i*11,jatekosok[i].x);
                set_four_num(out,7+i*11,jatekosok[i].y);
                set_four_num(out,11+i*11,jatekosok[i].angle);
            }
        }
        else if (state==start){
            out[0]='C'; out[1]='0';
            for (int i=0; i<player_count; i++){
                set_four_num(out,3+i*11,jatekosok[i].x);
                set_four_num(out,7+i*11,jatekosok[i].y);
                set_four_num(out,11+i*11,jatekosok[i].angle);
            }
        }
        else {
            out[0]='E';
            out[1]=state_to_char(state);
        }
    }
    else if (inp[0]=='C'){
        if (state==game){
            out[0]='D'; set_one_num(out,1,player_count);
            for (int i=0; i<player_count; i++){
                set_bool(out,2+i*19,jatekosok[i].W);
                set_bool(out,3+i*19,jatekosok[i].E);
                set_bool(out,4+i*19,jatekosok[i].SPACE);
                set_bool(out,5+i*19,jatekosok[i].alive);
                set_one_num(out,6+i*19,jatekosok[i].bullet_type);
                set_four_num(out,7+i*19,jatekosok[i].x);
                set_four_num(out,11+i*19,jatekosok[i].y);
                set_three_num(out,15+i*19,jatekosok[i].angle);
                set_three_num(out,18+i*19,jatekosok[i].points);
            }
        }
    }
    else if (inp[0]=='D'){
        if (state==game){
            int jatekos_id=char_to_num(inp[1]);
            jatekosok[jatekos_id].W=get_bool(inp,2);
            jatekosok[jatekos_id].E=get_bool(inp,3);
            jatekosok[jatekos_id].SPACE=get_bool(inp,4);
            jatekosok[jatekos_id].alive=get_bool(inp,5);
            jatekosok[jatekos_id].x=get_four_num(inp,6);
            jatekosok[jatekos_id].y=get_four_num(inp,10);
            jatekosok[jatekos_id].angle=get_three_num(inp,14);
            jatekosok[jatekos_id].points=get_three_num(inp,17);
            jatekosok[jatekos_id].bullet_type=get_one_num(inp,20);
            bool b1=false, b2=false;
            for (int i=0; i<player_count; i++){
                if (jatekosok[i].alive==true && b1==false)
                    b1=true;
                else if (jatekosok[i].alive==true)
                    b2=true;
            }
            /*if (!b2){
                state=ended;
                out[0]='E';
                out[1]=state_to_char(state);
            }*/
            //else {
                out[0]='D'; set_one_num(out,1,player_count);
                for (int i=0; i<player_count; i++){
                    set_bool(out,2+i*19,jatekosok[i].W);
                    set_bool(out,3+i*19,jatekosok[i].E);
                    set_bool(out,4+i*19,jatekosok[i].SPACE);
                    set_bool(out,5+i*19,jatekosok[i].alive);
                    set_one_num(out,6+i*19,jatekosok[i].bullet_type);
                    set_four_num(out,7+i*19,jatekosok[i].x);
                    set_four_num(out,11+i*19,jatekosok[i].y);
                    set_three_num(out,15+i*19,jatekosok[i].angle);
                    set_three_num(out,18+i*19,jatekosok[i].points);
                }
            //}
        }

    }
    else {
        out[0]='E';
        out[1]=state_to_char(state);
    }

    std::cout<<out<<std::endl;
    Socket.SendTo(add, out, 200);

}
