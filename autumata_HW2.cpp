// Create
#include <string.h>
#define ID "0"
#define eps "X"


strcut EDGE{
    int to;
    char inp;
    char top, push[];
    EDGE(){}
    EDGE(int to,char inp, char top, char _push[]):to(to),inp(inp),top(top){
        strcpy(push, _push);
    }
};

struct STATE{
    int id;
    bool final_state;
    bool dead_state;
    vector<EDGE> edges;
    STATE(){}
    STATE(int id, bool fs, bool ds): id(id),fs(fs),ds(ds){}
};

struct DPA{
    vector<STATE> states;
    int init, final;
    char vars[]="EeTtFA";
    char alphabets[]="0+-*/()#";
    char table[6][8][]={{"Te","","","","","Te","",""},
                        {"","+Te","-Te","","","","",eps},
                        {"Ft","","","","","Ft","",""},
                        {"A","","","","","(E)","",""},
                        {ID,"","","","","","",""}};
    
    DPA(){ init=6; final=7; }
    
    void makeDPA(){
    }
    
    bool read(char* inp){
        return true;
    }
}dpa;    
