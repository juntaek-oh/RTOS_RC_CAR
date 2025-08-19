# 🚗 FreeRTOS 기반 STM32 자율주행 RC카

<table>
  <tr>
    <td width="50%">
      하이브리드 블랙박스 영상<br>
      <img src="https://github.com/user-attachments/assets/55caa8fc-b178-4640-9e15-2ad3bab294ad"><br>
      <img src="https://github.com/user-attachments/assets/f832f5c1-f685-4ade-97a5-10bc61b4dd2e">
    </td>
    <td width="50%">
      웹캠 블랙박스 영상<br>
      <img src="https://github.com/user-attachments/assets/94f06c32-7f0b-455c-80cd-50aec259a48d"><br>
      <img src="https://github.com/user-attachments/assets/dd0ed0d7-2f6c-44e8-a775-a90fcd987fd3">
    </td>
  </tr>
</table>

<div align="center">
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Python 3.8+](https://img.shields.io/badge/python-3.8+-blue.svg)](https://www.python.org/downloads/)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.5+-green.svg)](https://opencv.org/)
[![YOLOv4](https://img.shields.io/badge/YOLOv4-Tiny-red.svg)](https://github.com/AlexeyAB/darknet)
**STM32CubeIDE와 FreeRTOS로 구현한 자율주행 RC카 프로젝트**  
*멀티 Task 구조와 실시간 센서·모터 제어, 블루투스 무선 제어로 장애물 회피 및 경로 추종을 실현합니다.*
</div>

---

## 📋 목차
- [🎯 주요 기능](#-주요-기능)
- [🏗️ 시스템 아키텍처](#-시스템-아키텍처)
- [📦 디렉토리 구조](#-디렉토리-구조)
- [⚙️ 설정](#️-설정)
- [🚀 설치 및 실행](#-설치-및-실행)
- [📖 사용법](#-사용법)
- [🎛️ 키 조작법](#️-키-조작법)
- [📊 성능 정보](#-성능-정보)
- [🔧 문제해결](#-문제해결)
- [🛠️ 개발 정보](#️-개발-정보)
- [🤝 기여하기](#-기여하기)
- [📞 연락처](#-연락처)

---

## 🎯 주요 기능

- **FreeRTOS 기반 멀티태스킹**: 초음파 센서, 모터제어 Task 분리
- **실시간 장애물 감지**: HC-SR04 센서 3개 활용(전방/좌/우)
- **블루투스 무선 제어**: HC-06/05 UART 통신, 수동/자동 모드 지원
- **PWM 기반 모터 구동**: 정밀 속도/방향 조절
- **지능형 탈출 알고리즘**: 막힘 감지→후진/회전 시퀀스
- **모듈화된 코드/RTOS-aware 디버깅**
- **사용자 인터페이스**: 실시간 주행 상태 및 로깅

---

## 🏗️ 시스템 아키텍처

- **메인 MCU**: STM32F411 (Cortex-M4)
- **센서 입력**: HC-SR04 ×3 (초음파 거리)
- **저수준 제어**: PWM (TIM3), Timer Input Capture (TIM1/2/4/11)
- **모터 구동**: L298N 드라이버 + DC 기어모터 ×2
- **통신**: HC-06 블루투스 (UART6)
- **RTOS Task 구조**:
  - Ultrasonic Task (센서측정, Normal Priority)
  - Moving Task (모터·명령처리, Low Priority)

---

## 📦 디렉토리 구조

RTOS_RC_CAR/
├── Inc/
│ ├── FreeRTOSConfig.h # FreeRTOS 설정
│ ├── main.h # 메인 헤더
│ ├── ultrasonic.h # 센서/모터 제어
│ └── ...
├── Src/
│ ├── main.c # 메인 함수, 하드웨어 초기화
│ ├── freertos.c # Task 정의 및 스케줄러
│ ├── ultrasonic.c # 센서/모터 제어 로직
│ └── ...
├── README.md
├── downloads/ # 자동 생성 (예: 녹화/로그 등)
├── webcam_output/
├── log/
└── model/
├── yolov4-tiny.weights
├── yolov4-tiny.cfg
└── coco.names

---

## ⚙️ 설정
#### 웹캠 설정 예시 (`webcam_blackbox_config.json`)
{ "camera": { "device_id": 0, "width": 1280, "height": 720, "fps": 30 }, "detection": { "detection_interval": 1, "confidence_threshold": 0.5 } }

#### 하이브리드 분석 설정 (`hybrid_blackbox_config.json`)
{ "youtube": { "url": "your_default_url", "quality": "720p" }, "tracking": { "movement_threshold": 1.5, "iou_threshold": 0.25 } }

#### 주요 파라미터 요약
- PWM: 400~850 사용, 모드별 속도/회전차별 조정
- 시스템 tick: 1ms (TIM11)
- 블루투스 명령: F, B, L, R, S, Z

---

## 🚀 설치 및 실행

### 필수 사양
| 구분    | 사양         |
|-----------|--------------|
| MCU       | STM32F411    |
| IDE       | STM32CubeIDE |
| RTOS      | FreeRTOS v10.3, CMSIS v2 |
| 기타      | HC-SR04, HC-06, L298N, DC모터, UART, PWM, Timer |

### 설치 과정
1. 저장소 복제
git clone https://github.com/juntaek-oh/RTOS_RC_CAR.git
cd RTOS_RC_CAR

3. CubeIDE에서 프로젝트 Import (기존 프로젝트)
4. 아키텍처 맞게 각 디바이스/선 연결 후 `Build → Debug/Run 실행`
5. 블루투스 연결 시 HC-06 페어링
6. 명령 전송(F/B/L/R/S/Z) 및 센서값 실시간 확인

---

## 📖 사용법

### 모드별 명령어
- F: 전진, B: 후진, L: 좌회전, R: 우회전, S: 정지, Z: 자동주행 모드
- 자동 모드 활성화 후 장애물/미로에 따라 자동 탈출/회전 제어

### 실시간 상태확인 & 로그
- UART 메시지로 센서값 및 모터상태 수신
- 주요 이벤트 별도 로그로 저장(향후 통계/분석 활용 가능)

---

## 🎛️ 키 조작법
| 키         | 기능         | 설명            |
|------------|--------------|-----------------|
| F/B/L/R/S  | 모터 동작/정지/회전 |
| Z          | 자동주행 전환   |
| RESET      | 시스템 리셋    |
| DEBUG      | 실시간 정보 확인 |


---

## 📊 성능 정보

| 실험환경    | 주행 정확도 | 실시간 반응성 | 장애물 회피 | 미로 탈출성공률 |
|-------------|-------------|---------------|-------------|-----------------|
| 실내(실험실) | 90% 이상    | 평균 0.1초    | 정상동작    | 95%             |

---

## 🔧 문제해결

- **센서 측정 오류**: 배선/타이밍 확인, Echo/Trig 자가테스트
- **모터 미동작**: L298N 연결/전원 확인, PWM 범위 재설정
- **블루투스 지연/미연결**: 페어링 초기화 또는 UART 설정값 확인
- **CubeIDE 빌드 오류**: 경로, 설정, 파일명 중복 확인
- **RTOS Task 충돌/멈춤**: 우선순위/동기화 논리 확인, Tick 주기 조정

---

## 🛠️ 개발 정보

- 멀티태스킹 분리 구조 (센서/제어/통신)
- 고성능 Timer + PWM + Input Capture 활용
- 전체 코드 모듈별 분리(확장성/유지보수 우수)
- 실시간 디버깅(CubeIDE RTOS-aware 기능)
- 버전관리 및 주요 업데이트, 향후 고급 알고리즘/클라우드 연동 계획

---

## 🤝 기여하기

- Fork 및 Branch 생성 (`git checkout -b feature/AmazingFeature`)
- 변경사항 커밋(`git commit -m 'Add some AmazingFeature'`)
- 원격 저장소 푸시(`git push origin feature/AmazingFeature`)
- Github Pull Request 생성
- 버그/기능 문의: Issues 탭에 OS, MCU/센서/IDE 버전, 오류상세, 재현단계 포함해 제출

---

## 📞 연락처

- 이메일: [ojt8416@gmail.com](mailto:ojt8416@gmail.com)
- Issues: [링크](https://github.com/juntaek-oh/RTOS_RC_CAR/issues)

---

<div align="center">
🚗 안전한 운전의 시작, AI 블랙박스/RC카 시스템과 함께!  
**실시간 차량 추적과 장애물 감지를 FreeRTOS 기반 STM32로 구현**  
⭐ 도움이 되셨다면 Star와 Issue 환영!  
🔄 Pull Requests 기여 대환영!
</div>
