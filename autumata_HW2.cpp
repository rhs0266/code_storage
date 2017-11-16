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
    void push_edge(int to, char inp, char top, char _push[]){
        edges.push_back(EDGE(to, inp, top, _push));
    }
};

struct DPA{
    vector<STATE> states;
    int init, waypoint, final;
    char vars[]="EeTtFA";
    char alphabets[]="0+-*/()#";
    char table[6][8][]={{"Te","","","","","Te","",""},
                        {"","+Te","-Te","","","","",eps},
                        {"Ft","","","","","Ft","",""},
                        {"A","","","","","(E)","",""},
                        {ID,"","","","","","",""}};
    
    DPA(){ init=6; final=7; }
    
    void makeDPA(){
        int nvars = strlen(vars);
        int nalphabets = strlen(alphabets);
        
        init = nalphabets; waypoint = nalphabets+1; final=nalphabets+2;
        
        states.resize(nalphabets+3);
        
        for (int i=0;i<nvars;i++){
            for (int j=0;j<nalphabets;j++){
               if (strcmp(table[i][j],"")==0){
                   continue;
               }
               states[j].push_edge(j, eps, vars[i], table[i][j]);
            }
        }
    }
    
    bool read(char* inp){
        return true;
    }
}dpa;    
