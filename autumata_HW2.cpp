#include <stdio.h>
#pragma warning(disable:4996)
//FILE *in = fopen("input.txt", "r"), *out = fopen("output.txt", "w");
FILE *in = stdin, *out = stdout;
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
#define INF 1e9
#define NM 100005
#define MOD 1000000007
#define FOR(i,n,m) for (int i=(n);i<=(m);i++)
#define si(n) fscanf(in,"%d",&n)
#define sd(n) fscanf(in,"%lf",&n)
typedef long long int ll;

#include <string.h>
#include <assert.h>
#define ID0 "0"
#define ID1 "1"
#define ID2 "2"
#define ID3 "3"
#define ID4 "4"
#define ID5 "5"
#define ID6 "6"
#define ID7 "7"
#define ID8 "8"
#define ID9 "9"
#define IDa "a"
#define IDb "b"
#define IDc "c"
#define IDd "d"
#define IDe "e"
#define IDf "f"
#define eps 'Z'
#define EPS "Z"

struct EDGE{
	int to;
	char inp;
	char top, push[];
	EDGE() {}
	EDGE(int to,char inp, char top, char _push[]) :to(to),inp(inp),top(top) {
		strcpy(push, _push);
	}
};

struct STATE {
	int id;
	bool final_state;
	bool dead_state;
	vector<EDGE> edges;
	STATE() {}
	STATE(int id, bool fs, bool ds) : id(id), final_state(fs), dead_state(ds) {}
	void push_edge(int to, char inp, char top, char _push[]) {
		edges.push_back(EDGE(to, inp, top, _push));
	}
};

struct DPA {
	vector<STATE> states;
	int init, waypoint, final;
	char vars[10] = "EXTYFA";
	char alphabets[50] = "0123456789abcdef+-*/()#";
	char table[6][23][10];

	void edit_cell(char var, char alphabet, char info[]) {
		int row = -1, col = -1;
		for (int i = 0; i < strlen(vars); i++) if (vars[i] == var) row = i;
		for (int i=0;i<strlen(alphabets);i++) if (alphabets[i] == alphabet) col = i;
		assert(row != -1); assert(col != -1);
		strcpy(table[row][col], info);
	}

	void setting_table() {
		// E -> TX
		for (int i = '0'; i <= '9'; i++) edit_cell('E', i, "Te");
		for (int i = 'a'; i <= 'f'; i++) edit_cell('E', i, "Te");
		edit_cell('E', '(', "Te");

		// X -> +TX | -TX | eps
		edit_cell('X', '+', "+TX");
		edit_cell('X', '-', "-TX");
		edit_cell('X', '#', EPS);

		// T -> FY
		for (int i = '0'; i <= '9'; i++) edit_cell('T', i, "FY");
		for (int i = 'a'; i <= 'f'; i++) edit_cell('T', i, "FY");
		edit_cell('T', '(', "FY");

		// Y -> *FY | /FY | eps
		edit_cell('Y', '*', "*FY");
		edit_cell('Y', '/', "/FY");
		edit_cell('Y', '#', EPS);

		// F -> (E) | A
		for (int i = '0'; i <= '9'; i++) edit_cell('T', i, "FY");
		for (int i = 'a'; i <= 'f'; i++) edit_cell('T', i, "FY");
		edit_cell('F', '(', "(E)");

		// A -> 0 | 1 | ... | f
		for (int i = '0'; i <= '9'; i++) edit_cell('A', i, "i");
		for (int i = 'a'; i <= 'f'; i++) edit_cell('A', i, "i");

	}

	void makeDPA() {
		int nvars = strlen(vars);
		int nalphabets = strlen(alphabets);

		init = nalphabets; waypoint = nalphabets + 1; final = nalphabets + 2;

		states.resize(nalphabets + 3);

		setting_table();

		for (int i = 0; i<nvars; i++) {
			for (int j = 0; j<nalphabets; j++) {
				if (strcmp(table[i][j], "") == 0) {
					continue;
				}
				states[j].push_edge(j, eps, vars[i], table[i][j]);
			}
		}
	}

	bool read(char* inp) {
		return true;
	}
}dpa;
