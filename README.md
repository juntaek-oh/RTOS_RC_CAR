#  FreeRTOS 기반 STM32 자율주행 RC카

![KakaoTalk_20250806_164222677 (2) (1)](https://github.com/user-attachments/assets/5129212e-a56d-4a65-9e91-40769a18b2fb)
---

![KakaoTalk_20250806_164222677 (3) (1)](https://github.com/user-attachments/assets/95b3667e-6938-45e2-a393-8460dc290677)


STM32CubeIDE와 FreeRTOS로 구현한 자율주행 RC카 프로젝트입니다. 멀티 Task 구조와 실시간 센서·모터 제어, 블루투스 무선 제어를 통해 장애물 회피 및 경로 추종을 실현합니다.

---

## 주요 특징

### FreeRTOS 기반 멀티태스킹 아키텍처
- **Ultrasonic Task**: 3개 초음파 센서(HC-SR04) 실시간 거리 측정
- **Moving Task**: 블루투스 명령 수신 및 자율주행 로직 처리
- Task 우선순위와 스케줄링으로 빠르고 안정적인 제어 실현

### 실시간 주행 제어 시스템
- 전방, 좌측, 우측 초음파 센서 기반 360도 장애물 감지
- 지능형 탈출 알고리즘으로 복잡한 미로 환경 대응
- PWM 모터 제어를 통한 정밀한 속도 및 방향 조절

### 블루투스 무선 제어
- UART6 기반 HC-06/05 블루투스 모듈 연동
- 수동 제어(F/B/L/R/S) 및 자동 모드(Z) 지원
- 실시간 명령 처리 및 상태 피드백

### 디버깅 및 모듈화
- STM32CubeIDE RTOS-aware 디버깅 지원
- 구조화된 코드와 모듈 분리로 유지보수 용이
- Timer Input Capture를 활용한 정밀한 거리 측정

---

## 하드웨어 구성

- **MCU**: STM32F411 (ARM Cortex-M4)
- **센서**: HC-SR04 초음파 센서 3개 (전방/좌/우)
- **모터**: L298N 모터 드라이버, DC 기어 모터 2개
- **통신**: HC-06 블루투스 모듈 (UART6)
- **타이머**: TIM1/2/4 (센서), TIM3 (PWM), TIM11 (시스템)

---

## 소프트웨어 아키텍처

### Task 구성
```c
// 초음파 센서 Task (Normal Priority)
void Ultrasonic_01(void *argument) {
    HCSR04_Trigger1();  // 우측 센서
    HCSR04_Trigger2();  // 중앙 센서  
    HCSR04_Trigger3();  // 좌측 센서
}

// 모터 제어 Task (Low Priority)
void Moving_01(void *argument) {
    // 블루투스 명령 처리
    // 자율주행 로직 실행
    if (isAutoMode) moving();
}
```

### 핵심 알고리즘
- **장애물 회피**: 3방향 센서 데이터 융합으로 최적 경로 결정
- **탈출 모드**: 막힌 상황 감지 시 지능형 후진-회전 시퀀스
- **PWM 제어**: 좌우 모터 독립 제어로 정밀한 조향 구현

---

## 사용 방법

### 1. 프로젝트 설정
```bash
git clone https://github.com/juntaek-oh/RTOS_RC_CAR.git
```
- STM32CubeIDE에서 Import → Existing Projects into Workspace

### 2. 빌드 및 플래시
- 보드 연결 후 Build → Debug/Run 실행
- FreeRTOS 설정: TIM11 시스템 타이머, 1ms tick

### 3. 블루투스 제어
**수동 모드 명령어:**
- `F`: 전진 (400 PWM)
- `B`: 후진 (400 PWM)  
- `L`: 좌회전 (850 PWM)
- `R`: 우회전 (850 PWM)
- `S`: 정지
- `Z`: 자동 모드 활성화

**자동 모드:**
- 센서 기반 자율주행 시작
- 장애물 감지 시 자동 회피
- 막힌 상황에서 탈출 알고리즘 실행

---

## 기술 스택

- **MCU/IDE**: STM32F411, STM32CubeIDE
- **RTOS**: FreeRTOS v10.3.1, CMSIS-RTOS v2
- **드라이버**: HAL/LL 드라이버
- **언어**: C
- **통신**: UART, PWM, GPIO, Timer Input Capture

---

## 성과 및 특징

### 성능 지표
- **90% 이상 주행 정확도** - 다양한 환경에서 안정적 자율주행
- **실시간 반응성** - Task 기반 병렬 처리로 센서-모터 지연 최소화
- **복잡한 미로 대응** - 막힌 상황 감지 및 지능형 탈출 알고리즘

### 기술적 성취
- FreeRTOS 멀티태스킹으로 센서와 제어 로직 분리
- Timer Input Capture 활용한 정밀한 거리 측정
- 블루투스 기반 실시간 원격 제어 구현
- 모듈화된 코드 구조로 확장성과 유지보수성 향상

---

## 파일 구조

```
RTOS_RC_CAR/
├── Inc/
│   ├── FreeRTOSConfig.h    # FreeRTOS 설정
│   ├── main.h              # 메인 헤더
│   ├── ultrasonic.h        # 센서/모터 제어
│   └── ...
├── Src/
│   ├── main.c              # 메인 함수, 하드웨어 초기화
│   ├── freertos.c          # Task 정의 및 스케줄러
│   ├── ultrasonic.c        # 센서 및 모터 제어 로직
│   └── ...
└── README.md
```

---

## 데모 영상

실제 동작 모습은 상단 GIF(`demo.gif`) 또는 [GitHub 저장소](https://github.com/juntaek-oh/RTOS_RC_CAR)에서 확인 가능합니다.

---

**문의 및 협업**  
GitHub 이슈 및 PR 환영합니다.
