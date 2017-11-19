#include <stdio.h>
#pragma warning(disable:4996)
FILE *in = fopen("input.txt", "r"), *out = fopen("output.txt", "w"), *out_temp = fopen("output_temp.txt","w");
//FILE *in = stdin, *out = stdout;
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
#define eps '.'
#define EPS ""

char ID[30][10] = { "2","3","4","5","6","7","a","b","c","d","x","y","z" };
char printStr[NM];
struct EDGE {
	int to;
	char inp;
	char top, push[10];
	EDGE() {}
	EDGE(int to, char inp, char top, char _push[]) :to(to), inp(inp), top(top) {
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
	int init, waypoint, fin, dead;
	char vars[9] = "EXTYFAPQ";
	char alphabets[50] = "234567abcdxyz+-*/()#";
	char table[8][23][10];
	char alphaString[105], st[505];
	int aCnt = 0, sCnt;
	bool exist[8][23];

	void edit_cell(char var, char alphabet, char info[]) {
		int row = -1, col = -1;
		for (int i = 0; i < strlen(vars); i++) if (vars[i] == var) row = i;
		for (int i = 0; i<strlen(alphabets); i++) if (alphabets[i] == alphabet) col = i;
		assert(row != -1); assert(col != -1);
		strcpy(table[row][col], info);
		exist[row][col] = true;
	}

	void setting_table() {
		// E -> TX
		for (int i = '2'; i <= '7'; i++) edit_cell('E', i, "TX");
		for (int i = 'a'; i <= 'd'; i++) edit_cell('E', i, "TX");
		for (int i = 'x'; i <= 'z'; i++) edit_cell('E', i, "TX");
		edit_cell('E', '(', "TX");

		// X -> +TX | -TX | eps
		edit_cell('X', '+', "+TX");
		edit_cell('X', '-', "-TX");
		edit_cell('X', '#', EPS);
		edit_cell('X', ')', EPS);

		// T -> FY
		for (int i = '2'; i <= '7'; i++) edit_cell('T', i, "FY");
		for (int i = 'a'; i <= 'd'; i++) edit_cell('T', i, "FY");
		for (int i = 'x'; i <= 'z'; i++) edit_cell('T', i, "FY");
		edit_cell('T', '(', "FY");

		// Y -> *FY | /FY | eps
		edit_cell('Y', '+', EPS);
		edit_cell('Y', '-', EPS);
		edit_cell('Y', '*', "*FY");
		edit_cell('Y', '/', "/FY");
		edit_cell('Y', '#', EPS);
		edit_cell('Y', ')', EPS);

		// F -> PEQ | A
		for (int i = '2'; i <= '7'; i++) edit_cell('F', i, "A");
		for (int i = 'a'; i <= 'd'; i++) edit_cell('F', i, "A");
		for (int i = 'x'; i <= 'z'; i++) edit_cell('F', i, "A");
		edit_cell('F', '(', "PEQ");

		// A -> 2 | 3 | .. | z
		for (int i = '2'; i <= '7'; i++) edit_cell('A', i, ID[i - '2']);
		for (int i = 'a'; i <= 'd'; i++) edit_cell('A', i, ID[i - 'a' + 6]);
		for (int i = 'x'; i <= 'z'; i++) edit_cell('A', i, ID[i - 'x' + 10]);

		// P -> (
		edit_cell('P', '(', "(");

		// Q -> )
		edit_cell('Q', ')', ")");
	}

	void setting_edge() {
		int nvars = strlen(vars);
		int nalphabets = strlen(alphabets);

		init = nalphabets; waypoint = nalphabets + 1; fin = nalphabets + 2; dead = nalphabets + 3;

		FOR(i, 0, nalphabets + 1) states.push_back(STATE(i, false, false));
		states.push_back(STATE(nalphabets + 2, true, false));
		states.push_back(STATE(nalphabets + 3, false, true));

		for (int i = 0; i<nvars; i++) {
			for (int j = 0; j<nalphabets; j++) {
				if (!exist[i][j]) {
					continue;
				}
				states[j].push_edge(j, eps, vars[i], table[i][j]);
			}
		}

		states[init].push_edge(waypoint, eps, '#', "E#");

		for (int i = 0; i < nalphabets; i++) {
			states[waypoint].push_edge(i, alphabets[i], eps, EPS);
			if (i < nalphabets - 1)
				states[i].push_edge(waypoint, eps, alphabets[i], EPS);
		}

		states[nalphabets - 1].push_edge(fin, eps, '#', "#");
	}

	void makeDPA() {
		setting_table();
		setting_edge();
	}

	void Pop() {
		st[sCnt--] = 0;
	}
	void Push(char *str) {
		for (int i = strlen(str) - 1; i >= 0; i--) {
			st[++sCnt] = str[i];
		}
		st[sCnt + 1] = 0;
	}

	bool read(char* inp) {
		int cur = init, idx = 0, printFlag = 0, loopIdx = 0;
		strcpy(st, "#");
		while (cur != fin) {
			printFlag = 0;
			loopIdx++;
			int nxt = -1;
			for (auto &edge : states[cur].edges) {
				if (edge.inp == eps) {
					if (sCnt >= 0) {
						if (st[sCnt] == edge.top) {
							nxt = edge.to;
							printFlag = 1;
							Pop();
							Push(edge.push);
							if ((nxt == waypoint || nxt == fin) && cur < strlen(alphabets)) {
								alphaString[aCnt++] = alphabets[cur];
								alphaString[aCnt] = 0;
								printFlag = 0;
							}
							break;
						}
					}
				}
				else if (idx < strlen(inp)) {
					if (edge.inp == inp[idx]) {
						nxt = edge.to;
						idx++;
						break;
					}
				}
			}
			if (nxt == -1) return false;
			cur = nxt;
			if (printFlag) {
				fprintf(out_temp, "=> %s", alphaString);
				for (int i = strlen(st) - 1; i >= 1; i--) {
					fprintf(out_temp, "%c", st[i]);
				}
				fprintf(out_temp, "\n");
			}
		}
		return true;
	}
}dpa;

char inp[105];
int main() {
	dpa.makeDPA();
	fscanf(in, "%s", inp);
	strcat(inp, "#");
	if (dpa.read(inp)) {
		fprintf(out,"Yes\n");
		fclose(out_temp);
		FILE *in_temp = fopen("output_temp.txt", "r");
		while (fgets(printStr,NM,in_temp) != NULL) {
			fprintf(out, "%s", printStr);
		}
	}
	else {
		fprintf(out, "No\n");
	}
	return 0;
}
