#include <stdio.h>
#pragma warning(disable:4996)
FILE *in = stdin, *out = stdout;
using namespace std;
#define NM 10005
#define FOR(i,n,m) for (int i=(n);i<=(m);i++)

#include <string.h>
#include <assert.h>
char M[NM], w[NM], t[NM];
int len_M, len_w;
char tape[2][NM];

int alpha_to_zeros(char alpha) { // alphabet을 부호화할 때, 0이 몇개 쓰이는 지 알려주는 함수
    if (alpha == '0') return 1;
    if (alpha == '1') return 2;
    if (alpha == '#') return 3;
    if (alpha == 'a') return 4;
    if (alpha == 'b') return 5;
    assert(false);
    return 0;
}

char zeros_to_alpha(int cnt) { // 0의 개수를 보고 alphabet을 알려주는 함수
    if (cnt == 1) return '0';
    if (cnt == 2) return '1';
    if (cnt == 3) return '#';
    if (cnt == 4) return 'a';
    if (cnt == 5) return 'b';
    assert(false);
    return 0;
}

void coding_w() { // 입력 문자열의 양 끝에 '#' 을 추가해서 2번째 테이프에 쓰는 작업
    tape[1][0] = '#';
    strcpy(&tape[1][1], w);
    strcat(tape[1], "#");
}

void pro() {
    strcpy(tape[0], M); // 첫번째 테이프에 M을 쓰는 작업
    coding_w(); // 두번째 테이프에 w를 쓰는 작업
    int cur1 = 0, cur2 = 1;
    while (true) {
        { // 전이를 찾기 위해서 11(0..0)1(0..0) 을 만드는 작업
			int cnt = 0;
            t[cnt++] = '1'; t[cnt++] = '1';
            FOR(i, 1, cur2) t[cnt++] = '0';
            t[cnt++] = '1';
            FOR(i, 1, alpha_to_zeros(tape[1][cur1])) t[cnt++] = '0';
            t[cnt++] = '1';
            t[cnt++] = 0;

        }
        char *idx = strstr(tape[0], t); // 해당 전이를 M에서 찾는 작업
        if (idx == NULL) break; // 전이가 없다면 halting

        
		// 전이에 맞게 다음 state를 갱신해주는 작업
		int next1 = 0, next2 = 0, ptr = idx - tape[0] + strlen(t), dir = 0;
        for (; tape[0][ptr] != '1'; ptr++, next2++);
        ptr++;
        for (; tape[0][ptr] != '1'; ptr++, next1++);
        ptr++;
        for (; tape[0][ptr] != '1'; ptr++, dir++);
        cur2 = next2;
        tape[1][cur1] = zeros_to_alpha(next1);
        if (dir == 1) cur1--;
        if (dir == 2) cur1++;
        if (dir == 3) cur1;
    }
}

int main(int argc, char* argv[]) {

	// 입력
    fscanf(in, "%s", w);
    {
        FILE *in_TM = fopen(argv[1], "r");
        fscanf(in_TM, "%s", M);
        len_M = strlen(M);
        fclose(in_TM);
    }

	// TM 실행
    pro();

	// TM 결과 출력
    int ans = 0;
    for (int i = 1; tape[1][i] != '#'; i++, ans++);
    fprintf(out, "%d", ans);
    return 0;
}
