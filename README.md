# TOS - Toy Operating System

> C로 구현한 경량 OS 시뮬레이터

---

## 📌 프로젝트 소개

TOS는 운영체제의 핵심 개념을 직접 구현한 시뮬레이터입니다.
FCFS / Round Robin 스케줄링, 프로세스 관리, I/O 처리, 메모리 시뮬레이션을
인터랙티브 미니 쉘 환경에서 직접 실행하고 확인할 수 있습니다.

---

## ⚙️ 주요 기능

- 🔄 **프로세스 스케줄링** — FCFS, Round Robin 지원
- 📋 **프로세스 관리** — 생성, 상태 추적, 종료
- 💾 **메모리 시뮬레이션** — 프로세스별 1024KB 할당/해제
- ⌨️ **I/O 버스트 처리** — I/O 대기 및 복귀 시뮬레이션
- 🖥️ **미니 쉘** — 명령어 기반 인터랙티브 환경

---

## 🚀 빌드 및 실행

```bash
# 빌드
make

# 실행 (TOS/ 디렉토리에서 실행)
./TOS
```

> 실행 전 `runtime/` 디렉토리가 존재해야 합니다.
>
> ```bash
> mkdir runtime
> ```

---

## 🖥️ 쉘 명령어

| 명령어                                        | 설명                   |
| --------------------------------------------- | ---------------------- |
| `create <name> <time>`                      | 프로세스 생성          |
| `create <name> <time> io <io_at> <io_wait>` | I/O 포함 프로세스 생성 |
| `set_sched fcfs`                            | FCFS 스케줄링 설정     |
| `set_sched rr <quantum>`                    | Round Robin 설정       |
| `ps`                                        | 프로세스 상태 출력     |
| `run`                                       | 전체 실행              |
| `step`                                      | 1 단계 실행            |
| `stats`                                     | 통계 출력              |
| `mem`                                       | 메모리 테이블 출력     |
| `reset`                                     | 커널 초기화            |

---

## 📁 프로젝트 구조

TOS/
├── common/
│   ├── types.h          # PCB, 스케줄링 타입 정의
│   ├── queue.h/.c       # 원형 큐 구현
├── core/
│   ├── kernel/          # 커널 전역 구조체
│   ├── process/         # 프로세스 생성 및 관리
│   ├── scheduler/       # 스케줄링 로직
│   └── memory/          # 메모리 할당/해제
├── user/
│   └── shell/           # 미니 쉘 (main)
└── makefile


---
## 👤 개발자

| 이름 | GitHub |
|------|--------|
| 김상연 | [@seankim030218](https://github.com/seankim030218) |
---
