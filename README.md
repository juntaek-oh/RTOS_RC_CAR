# RTOS_RC_CAR – FreeRTOS 기반 STM32 자율주행 RC카

![RC Car Demo](demo.gif)

STM32CubeIDE와 FreeRTOS로 구현한 자율주행 RC카 프로젝트입니다. 멀티 Task 구조와 실시간 센서·모터 제어를 통해 장애물 회피 및 경로 추종을 실현합니다.

---

## 주요 특징

- **FreeRTOS 기반 Task 구조**
  - STM32 MCU에서 주행, 센서, 통신 기능을 개별 Task로 분리하여 운용
  - Task 우선순위와 스케줄링으로 빠르고 안정적인 제어 실현

- **실시간 주행/장애물 회피**
  - 초음파 센서 정보로 장애물 감지 및 회피 자동화
  - 라인트레이싱, PWM 모터제어 등 제어 알고리즘 적용

- **디버깅 및 모듈화**
  - STM32CubeIDE RTOS-aware 디버깅 지원
  - 구조화된 코드와 모듈화, 유지보수 용이

---

## 프로젝트 구조

RTOS_RC_CAR/
├── Core/
│ ├── Inc/ # 헤더 파일
│ └── Src/ # C 소스코드 (Task/주요 모듈)
├── Hardware/ # 센서·모터 등 하드웨어 드라이버
├── demo.gif # 동작 시연 GIF (루트에 추가)
└── README.md


---

## 빌드 및 실행 방법

1. **프로젝트 클론**
    ```
    git clone https://github.com/juntaek-oh/RTOS_RC_CAR.git
    ```
2. **STM32CubeIDE에서 Import**
    - File → Import → Existing Projects into Workspace → 프로젝트 선택

3. **빌드 및 다운로드**
    - 보드/MCU 연결 후 Build → Debug/Run 실행

4. **GIF 파일 교체 (선택)**
    - 동작 시연을 `demo.gif` 파일로 저장 후 루트 폴더에 추가

---

## 기술 스택

- **MCU/IDE**: STM32, STM32CubeIDE
- **OS/드라이버**: FreeRTOS, HAL/LL 드라이버
- **언어**: C
- **I/O**: PWM, UART, 초음파 센서 등

---

## 성과 및 경험

- 90% 이상 주행 정확도, 실시간 장애물 회피 성공
- Task 기반 실시간 임베디드 설계 및 디버깅 경험
- 코드 구조화 및 모듈화, 하드웨어 연동 능력 강화

---

## 데모 영상

- 동작 모습은 상단 GIF(`demo.gif`) 또는  
- [GitHub 저장소](https://github.com/juntaek-oh/RTOS_RC_CAR)에서 확인

---

**문의 및 협업**  
GitHub 이슈 및 PR 환영합니다.
