/*
 * adder.c - 따라 치기용 뼈대 (CS:APP 그림 11.27)
 *
 * 두 수를 더하는 아주 작은 CGI 프로그램이에요.
 * Tiny가 QUERY_STRING 환경 변수에 "1&2" 같은 인자를 넣고 이 프로그램을 실행하면,
 * 이 프로그램이 표준 출력에 쓴 내용이 그대로 클라이언트에게 가요.
 */
#include "csapp.h"

int main(void)
{
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
    int n1=0, n2=0;

    /* TODO 1. getenv("QUERY_STRING")으로 인자 문자열을 얻는다 (NULL이면 0 + 0)
     *   - strchr로 '&'를 찾아 '\0'으로 바꿔 두 부분으로 나눈다
     *   - atoi로 두 수 n1, n2를 얻는다
     *   - 생각해 볼 점: '&'가 없으면(예: ?5) 어떻게 될까요? */
    if ((buf = getenv("QUERY_STRING")) != NULL) {
       p = strchr(buf, '&');
       *p = '\0';
       strcpy(arg1, buf);
       strcpy(arg2, p+1);
       n1 = atoi(arg1);
       n2 = atoi(arg2);
    }

    /* TODO 2. 응답 본문(content)을 만든다
     *   Welcome to add.com: THE Internet addition portal.\r\n<p>
     *   The answer is: n1 + n2 = 합\r\n<p>
     *   Thanks for visiting!\r\n
     *
     *   주의: 책처럼 sprintf(content, "%s...", content)로 이어 붙이면
     *         gcc -O2에서 앞부분이 사라질 수 있어요(같은 버퍼를 읽고 쓰기 때문).
     *         strcat을 쓰거나 sprintf(content + len, ...)처럼 이어 쓰세요. */
    sprintf(content, "QUERY_STRING=%s", buf);
    sprintf(content, "Welcome to add.com: ");
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>", content, n1, n2, n1 + n2);
    sprintf(content, "%sThanks for visiting!\r\n", content);

    /* TODO 3. 나머지 HTTP 헤더와 본문을 printf로 출력한다
     *   Connection: close
     *   Content-length: <본문 길이>
     *   Content-type: text/html
     *   (빈 줄)
     *   <본문>
     *   그리고 fflush(stdout) */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
    exit(0);
}
