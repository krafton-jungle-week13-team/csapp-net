# CSAPP Network

CS:APP 11장(네트워크 프로그래밍)을 공부하며 Tiny 웹 서버를 직접 따라 쳐 보는 저장소예요.
이 README대로 하면 빈 뼈대에서 시작해 브라우저로 정적 페이지와 CGI 결과를 받아 보는 데까지 갈 수 있어요.

## 폴더 구성

```
tiny/
├── tiny.c          ← 직접 채울 파일 (함수 틀 + TODO만 있음)
├── cgi-bin/
│   ├── adder.c     ← 직접 채울 파일 (두 수를 더하는 CGI)
│   └── Makefile
├── csapp.c         ← 책의 도우미 함수 (그대로 사용)
├── csapp.h
├── home.html       ← 정적 콘텐츠 예시 (기본 페이지)
├── godzilla.gif    ← home.html에 들어가는 이미지
└── Makefile
```

`csapp.c`, `csapp.h`, `home.html`, `godzilla.gif`, Makefile은 책에서 제공하는 그대로예요.
직접 칠 파일은 `tiny.c`와 `cgi-bin/adder.c` 두 개예요.

## 따라 치는 순서

책의 그림 번호를 보며 아래 순서로 채우는 걸 추천해요. 각 함수의 TODO 주석에 할 일이 적혀 있어요.

1. `main` (그림 11.29): 듣기 소켓을 열고 연결을 하나씩 받는 반복문
2. `clienterror` (그림 11.31): 에러 응답. 먼저 만들어 두면 나머지를 테스트하기 쉬워요
3. `doit` (그림 11.30): 요청 줄 읽기와 분기
4. `read_requesthdrs` (그림 11.32)
5. `parse_uri` (그림 11.33)
6. `get_filetype`, `serve_static` (그림 11.34)
7. `cgi-bin/adder.c` (그림 11.27)
8. `serve_dynamic` (그림 11.35)

뼈대 상태에서도 `make`는 통과해요. 함수를 하나 채울 때마다 빌드하고 확인해 보세요.

## 빌드와 실행

Linux나 macOS, Windows는 WSL2에서 실행해요.

```bash
cd tiny
make            # tiny와 cgi-bin/adder를 함께 빌드
./tiny 8000     # 8000번 포트에서 서버 실행
```

## 확인하기

서버를 켠 채로 다른 터미널에서 확인해요.

| 단계 | 명령 | 기대 결과 |
|---|---|---|
| main, clienterror, doit | `curl -v -X POST http://localhost:8000/` | `501 Not Implemented` |
| parse_uri, 404 | `curl -v http://localhost:8000/nothing.html` | `404 Not found` |
| serve_static | `curl -v http://localhost:8000/` | `200 OK`, `Content-type: text/html`, home.html 내용 |
| get_filetype | `curl -s -o /dev/null -w "%{content_type}\n" http://localhost:8000/godzilla.gif` | `image/gif` |
| serve_dynamic, adder | `curl "http://localhost:8000/cgi-bin/adder?1&2"` | `The answer is: 1 + 2 = 3` |
| 브라우저 | `http://localhost:8000` 접속 | 고질라 이미지와 이름이 보임 |

HTTP 요청을 손으로 직접 보내 볼 수도 있어요. 아래처럼 접속한 뒤 요청 줄을 치고 엔터를 두 번 누르세요.

```bash
telnet localhost 8000
GET /home.html HTTP/1.0

```

서버 터미널에는 받은 요청 줄과 헤더가 찍히고, telnet 화면에는 응답 헤더와 본문이 그대로 보여요.

## 다 치고 나서

책의 연습문제 11.6~11.13으로 Tiny를 고쳐 볼 수 있어요.
특히 11.12(POST 지원)와 11.13(클라이언트가 먼저 끊었을 때 SIGPIPE 처리)은 웹 서버 과제와 바로 이어져요.
