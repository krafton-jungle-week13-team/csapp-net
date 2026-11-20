/*
 * tiny.c - 따라 치기용 뼈대 (CS:APP 11.6절)
 *
 * GET 메서드로 정적/동적 콘텐츠를 서빙하는 순차(iterative) HTTP/1.0 웹 서버예요.
 * 함수 선언과 할 일(TODO)만 남겨 두었어요. 책의 그림 11.29~11.35를 보며 직접 채워 보세요.
 * 다 채우기 전에도 `make`는 통과하도록 빈 함수로 두었어요.
 */
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);

/* 그림 11.29 */
int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];

    /* TODO 1. 인자 개수 확인: `./tiny <port>` 형식이 아니면 사용법을 출력하고 종료 */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    /* TODO 2. Open_listenfd(port)로 듣기 소켓(listenfd)을 연다 */
    listenfd = Open_listenfd(argv[1]);

    /* TODO 3. 무한 루프
     *   - Accept로 연결 소켓(connfd)을 받는다
     *   - Getnameinfo로 클라이언트 호스트/포트를 얻어
     *     "Accepted connection from (%s, %s)\n" 출력
     *   - doit(connfd)로 트랜잭션 하나를 처리한다
     *   - Close(connfd)
     */
     while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        doit(connfd);
        Close(connfd);
     }
    exit(0);
}

/* 그림 11.30 - HTTP 트랜잭션 하나를 처리한다 */
void doit(int fd)
{
    /* TODO 1. Rio_readinitb로 rio를 초기화하고 Rio_readlineb로 요청 줄을 읽는다
     *         (읽은 게 없으면 return). 읽은 줄을 printf로 출력한다 */
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, fd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %d bytes\n", (int)n);
        Rio_writen(fd, buf, n);
    }
    /* TODO 2. sscanf로 method, uri, version을 나눈다 */

    /* TODO 3. method가 GET이 아니면(strcasecmp) 501 Not Implemented 에러 후 return */

    /* TODO 4. read_requesthdrs로 나머지 요청 헤더를 읽어 버린다 */

    /* TODO 5. parse_uri로 정적/동적 여부, filename, cgiargs를 얻는다 */

    /* TODO 6. stat(filename, &sbuf)이 실패하면 404 Not found 에러 후 return */

    /* TODO 7. 정적이면
     *           - 일반 파일(S_ISREG)이 아니거나 읽기 권한(S_IRUSR)이 없으면 403
     *           - serve_static(fd, filename, sbuf.st_size)
     *         동적이면
     *           - 일반 파일이 아니거나 실행 권한(S_IXUSR)이 없으면 403
     *           - serve_dynamic(fd, filename, cgiargs)
     */
}

/* 그림 11.32 - 요청 헤더를 읽고 무시한다 */
void read_requesthdrs(rio_t *rp)
{
    /* TODO 빈 줄("\r\n")이 나올 때까지 한 줄씩 읽고 printf로 출력한다 */
}

/* 그림 11.33 - URI를 파일 이름과 CGI 인자로 나눈다
 * 반환값: 정적 콘텐츠면 1, 동적 콘텐츠면 0 */
int parse_uri(char *uri, char *filename, char *cgiargs)
{
    /* TODO 1. uri에 "cgi-bin"이 없으면 정적
     *   - cgiargs를 빈 문자열로
     *   - filename = "." + uri
     *   - uri가 '/'로 끝나면 filename 뒤에 "home.html"을 붙인다
     *   - return 1
     */

    /* TODO 2. 있으면 동적
     *   - uri에서 '?'를 찾아 그 뒤를 cgiargs로 복사하고, '?' 자리를 '\0'으로 끊는다
     *     ('?'가 없으면 cgiargs는 빈 문자열)
     *   - filename = "." + uri
     *   - return 0
     */
    return 1;
}

/* 그림 11.34 - 정적 파일을 클라이언트에 보낸다 */
void serve_static(int fd, char *filename, int filesize)
{
    /* TODO 1. get_filetype으로 Content-type을 정한다 */

    /* TODO 2. 응답 헤더를 보낸다 (각 줄 끝은 \r\n, 마지막엔 빈 줄 \r\n 하나 더)
     *   HTTP/1.0 200 OK
     *   Server: Tiny Web Server
     *   Content-length: <filesize>
     *   Content-type: <filetype>
     *
     *   주의: sprintf(buf, "%s...", buf)처럼 같은 버퍼를 읽고 쓰면 안 돼요.
     *         한 줄씩 sprintf → Rio_writen 하거나, 길이를 기억해 이어 쓰세요.
     */

    /* TODO 3. 응답 본문을 보낸다
     *   - Open으로 파일을 열고
     *   - Mmap으로 파일 내용을 메모리에 매핑한 뒤 Close
     *   - Rio_writen으로 매핑한 영역을 fd에 쓰고
     *   - Munmap으로 매핑 해제
     */
}

/* 파일 이름의 확장자로 MIME 타입을 정한다 */
void get_filetype(char *filename, char *filetype)
{
    /* TODO .html → text/html, .gif → image/gif, .png → image/png,
     *      .jpg → image/jpeg, 나머지 → text/plain */
    strcpy(filetype, "text/plain");
}

/* 그림 11.35 - CGI 프로그램을 실행해 동적 콘텐츠를 보낸다 */
void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    /* TODO 1. 응답의 앞부분만 보낸다 (나머지 헤더와 본문은 CGI 프로그램이 출력)
     *   HTTP/1.0 200 OK
     *   Server: Tiny Web Server
     */

    /* TODO 2. Fork로 자식 프로세스를 만들고, 자식에서
     *   - setenv("QUERY_STRING", cgiargs, 1)
     *   - Dup2(fd, STDOUT_FILENO)로 표준 출력을 소켓으로 바꾼다
     *   - Execve(filename, emptylist, environ)로 CGI 프로그램 실행
     *     (emptylist는 { NULL }인 char* 배열)
     */

    /* TODO 3. 부모는 Wait(NULL)로 자식을 회수한다 */
}

/* 그림 11.31 - 에러 응답을 보낸다 */
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    /* TODO 1. 응답 헤더
     *   HTTP/1.0 <errnum> <shortmsg>
     *   Content-type: text/html
     *   (빈 줄)
     */

    /* TODO 2. 응답 본문 (간단한 HTML)
     *   <html><title>Tiny Error</title><body bgcolor="ffffff">
     *   <errnum>: <shortmsg>
     *   <p><longmsg>: <cause>
     *   <hr><em>The Tiny Web server</em>
     */
}
