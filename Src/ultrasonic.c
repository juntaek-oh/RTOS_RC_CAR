
#include "ultrasonic.h"


static uint16_t IC_Value1 =0;  //에코핀이 하이엣지 일때
static uint16_t IC_Value2 =0;
static uint16_t IC_Value3 =0;  //에코핀이 하이엣지 일때
static uint16_t IC_Value4 =0;
static uint16_t IC_Value5 =0;  //에코핀이 하이엣지 일때
static uint16_t IC_Value6 =0;//에코핀이 로우엣지 일때
static uint16_t echoTime1  =0;
static uint16_t echoTime2  =0;
static uint16_t echoTime3  =0; //에코핀의 하이엣지와 로우엣지 사이의 시간(카운트 갯수)
uint8_t direction = 'S';
uint8_t distance_right = 0;
uint8_t distance_center = 0;
uint8_t distance_left = 0;// 거리

//uint8_t direction = 0;
uint8_t receivedByte =0;
// 센서별 플래그
uint8_t captureFlag1 = 0, captureFlag2 = 0, captureFlag3 = 0;

uint32_t lastCommandTime;
bool isMoving;
bool isAutoMode = false;
static uint8_t stop_count = 0;
static uint32_t last_stop_time = 0;
bool escape_mode = false;




void HAL_TIM1_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    if(captureFlag1 == 0)
    {
      IC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      captureFlag1 = 1;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }
    else if(captureFlag1 == 1)
    {
      IC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      __HAL_TIM_SET_COUNTER(htim, 0);

      if(IC_Value2 > IC_Value1)
      {
        echoTime1 = IC_Value2 - IC_Value1;
      }
      else if(IC_Value1 > IC_Value2)
      {
        echoTime1 = (0xFFFF - IC_Value1) + IC_Value2;
      }

      distance_right = echoTime1 / 58;
      captureFlag1 = 0;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
  }
}

// Timer 2 Input Capture Callback
void HAL_TIM2_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    if(captureFlag2 == 0)
    {
      IC_Value3 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      captureFlag2 = 1;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }
    else if(captureFlag2 == 1)
    {
      IC_Value4 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      __HAL_TIM_SET_COUNTER(htim, 0);

      if(IC_Value4 > IC_Value3)
      {
        echoTime2 = IC_Value4 - IC_Value3;
      }
      else if(IC_Value3 > IC_Value4)
      {
        echoTime2 = (0xFFFF - IC_Value3) + IC_Value4;
      }

      distance_center = echoTime2 / 58;
      captureFlag2 = 0;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
  }
}

// Timer 4 Input Capture Callback
void HAL_TIM4_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    if(captureFlag3 == 0)
    {
      IC_Value5 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      captureFlag3 = 1;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }
    else if(captureFlag3 == 1)
    {
      IC_Value6 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      __HAL_TIM_SET_COUNTER(htim, 0);

      if(IC_Value6 > IC_Value5)
      {
        echoTime3 = IC_Value6 - IC_Value5;
      }
      else if(IC_Value5 > IC_Value6)
      {
        echoTime3 = (0xFFFF - IC_Value5) + IC_Value6;
      }

      distance_left = echoTime3 / 58;
      captureFlag3 = 0;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
  }
}
void HCSR04_Trigger1(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
    delay_us1(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
      delay_us1(100);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
      __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
}
void HCSR04_Trigger2(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
   delay_us2(1);
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
     delay_us2(100);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
     __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC1);
}
void HCSR04_Trigger3(void)
{

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);//확실하게 동작할수 있도록 만들어준 코드 밑에꺼만 있어도 동작은 됨
  delay_us4(1);



  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
  delay_us4(100); // 10us 동안 1로 유지하는게 디폴트값

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);


  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1);// cc1 = 채널 1을 활성화 시키는거
}

void setLeftMotor(uint8_t forward, uint16_t pwm) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, forward);  // IN4
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, !forward); // IN3
    TIM3->CCR1 = pwm;

}

void setRightMotor(uint8_t forward, uint16_t pwm) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, forward);  // IN2
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, !forward); // IN1
    TIM3->CCR2 = pwm;
}



void dir(uint8_t dir)
{
    direction = dir;

    switch (dir) {

      case 'F': // 전진
        setLeftMotor(1, 400);
        setRightMotor(1, 400);
//        sendData("'F'\r\n");
        break;

      case 'B': // 후진
        setLeftMotor(0, 400);
        setRightMotor(0, 400);
//        sendData("'B'\r\n");
        break;

      case 'L': // 좌회전
        setLeftMotor(0, 0);
        setRightMotor(1, 850);
//        sendData("'L'\r\n");
        break;

      case 'R': // 우회전
        setLeftMotor(1, 850);
        setRightMotor(0, 0);
//        sendData("'R'\r\n");
        break;
      case 'S': // 정지
        setLeftMotor(0, 0);
        setRightMotor(0, 0);
//        sendData("'S'\r\n");
        break;

      case 'Z': // 주행
        moving();
//        sendData("'Z'\r\n");
        break;



      default:
        setLeftMotor(0, 0);
        setRightMotor(0, 0);
//        sendData("'S'\r\n");
        break;
    }
}

bool checkEscapeCondition()
{
    bool is_trapped = (distance_center < 30 && distance_left < 15 && distance_right < 15);

    if (is_trapped)
    {
        if (HAL_GetTick() - last_stop_time > 200) // 너무 빠르게 누적되는 것 방지
        {
            stop_count++;
            last_stop_time = HAL_GetTick(); // 다음 누적 시각 기록
        }

        if (stop_count >= 3)
        {
            return true;
        }
    }
    else
    {
        stop_count = 0; // 조건이 만족되지 않으면 리셋
    }

    return false;
}

void escapeMode() {
    static uint8_t escape_state = 0;
    static uint32_t escape_start = 0;
    static char escape_dir = 'N'; // 탈출 방향 기억 ('N'은 미결정)

    if (escape_state == 0) {
        escape_mode = true;
        dir('S');
        escape_state = 1;
        escape_start = HAL_GetTick();
        sendData("Escape: Started\r\n");
    }
    else if (escape_state == 1 && HAL_GetTick() - escape_start >= 200) {
        // 더 나은 탈출 방향 결정 로직
        if (distance_left > distance_right + 5) {
            escape_dir = 'L';
        }
        else if (distance_right > distance_left + 5) {
            escape_dir = 'R';
        }
        else {
            // 이전 기록이 없거나 큰 차이가 없으면, 긴 벽을 따라 탈출
            if (distance_center < 30) { // 정면이 매우 가까우면
                escape_dir = (distance_left > distance_right) ? 'L' : 'R';
            } else {
                // 약간의 랜덤성 추가
                escape_dir = (HAL_GetTick() % 2) ? 'L' : 'R';
            }
        }

        dir(escape_dir);
        escape_state = 2;
        escape_start = HAL_GetTick();
        char debug[30];
        sprintf(debug, "Escape: Turn %c\r\n", escape_dir);
        sendData(debug);
    }
    else if (escape_state == 2) {
        // 조건 1: 센서 값이 충분히 넓어짐
        if (distance_center > 40 && distance_left > 19 && distance_right > 19) {
            dir('F');
            escape_state = 3;
            escape_start = HAL_GetTick();
            sendData("Escape: Stop by sensor\r\n");
        }
        // 조건 2: 회전 시간이 너무 길어짐 (최대 제한)
        else if (HAL_GetTick() - escape_start >= 500) {
            dir('S');
            escape_state = 3;
            escape_start = HAL_GetTick();
            sendData("Escape: Stop by timeout\r\n");
        }
    }
    else if (escape_state == 3 && HAL_GetTick() - escape_start >= 200) {
        escape_mode = false;
        stop_count = 0;
        escape_state = 0; // Reset the escape state for next time
        sendData("Escape: Complete\r\n");
    }
}

void moving()
{
    if (escape_mode)
    {
        escapeMode(); // 현재 탈출 중이면 계속 진행
        return; // 일반 이동 로직은 실행하지 않음
    }

    if (checkEscapeCondition())
    {
        escape_mode = true; // 탈출 모드 진입
        escapeMode(); // 탈출 시퀀스 시작
        return;
    }

    if (distance_right == 0 || distance_center == 0 || distance_left == 0) // uint8_t 제한
           {
             dir('S'); // 정지

           }
         // 초음파 센서 기반 자율 제어
         else if (distance_center >= 40 && distance_right >= 20 && distance_left >= 20)
           {
             dir('F'); // 전진
           }
         else if (distance_center < 33)
           {
             dir('S'); // 정지
             osDelay(100);  // 모터 중립 시간
             dir('B'); // 후진
             osDelay(250);

             if (distance_right > distance_left)
               {
                 dir('R'); // 우회전
               }
             else
               {
                 dir('L'); // 좌회전
               }
           }
         else if (distance_center >= 33 && distance_center < 60)
           {
             if (distance_right > distance_left + 7)
               {
                 dir('R'); // 우회전
               }
             else if (distance_left > distance_right + 7)
               {
                 dir('L'); // 좌회전
               }
             else
               {
                 dir('F'); // 전진
               }
           }
         else if (distance_right < 20 && distance_left < 20)
           {
             dir('S'); // 정지


             if (distance_right > distance_left)
               {
                 dir('R'); // 우회전
               }
             else
               {
                 dir('L'); // 좌회전
               }
           }
         else if (distance_right < 22 && distance_left >= 22)
           {
             dir('L'); // 좌회전
           }
         else if (distance_left < 22 && distance_right >= 22)
           {
             dir('R'); // 우회전
           }
         else if (distance_center >= 37 && distance_right >= 24 && distance_right < 40 && distance_left >= 24 && distance_left < 40)
           {
             if (abs((int)distance_right - (int)distance_left) < 8)
               {
                 dir('F'); // 전진
               }
             else if (distance_right > distance_left)
               {
                 dir('R'); // 우회전
               }
             else
               {
                 dir('L'); // 좌회전
               }
           }


   }
//void moving()
//{
//  sendData("moving() 시작됨\r\n");
//
//
//  if (distance_right == 0 || distance_center == 0 || distance_left == 0 ||
//          distance_right > 255 || distance_center > 255 || distance_left > 255) // uint8_t 제한
//        {
//          dir('S'); // 정지
//
//        }
//      // 초음파 센서 기반 자율 제어
//      else if (distance_center >= 40 && distance_right >= 20 && distance_left >= 20)
//        {
//          dir('F'); // 전진
//        }
//      else if (distance_center < 25)
//        {
//          dir('S'); // 정지
//          HAL_Delay(50); // 모터 중립 시간
//          dir('B'); // 후진
//          HAL_Delay(100);; // 충분히 후진 후 다음 판단
//
//          if (distance_right > distance_left)
//            {
//              dir('R'); // 우회전
//            }
//          else
//            {
//              dir('L'); // 좌회전
//            }
//        }
//      else if (distance_center >= 25 && distance_center < 50)
//        {
//          if (distance_right > distance_left + 12)
//            {
//              dir('R'); // 우회전
//            }
//          else if (distance_left > distance_right + 12)
//            {
//              dir('L'); // 좌회전
//            }
//          else
//            {
//              dir('F'); // 전진
//            }
//        }
//      else if (distance_right < 25 && distance_left < 25)
//        {
//          dir('S'); // 정지
//          HAL_Delay(50); // 모터 중립 시간
//          dir('B'); // 후진
//          HAL_Delay(100); // 충분히 후진 후 다음 판단
//
//          if (distance_right > distance_left)
//            {
//              dir('R'); // 우회전
//            }
//          else
//            {
//              dir('L'); // 좌회전
//            }
//        }
//      else if (distance_right < 25 && distance_left >= 25)
//        {
//          dir('L'); // 좌회전
//        }
//      else if (distance_left < 25 && distance_right >= 25)
//        {
//          dir('R'); // 우회전
//        }
//      else if (distance_center >= 40 && distance_right >= 25 && distance_right < 35 && distance_left >= 25 && distance_left < 35)
//        {
//          if (abs((int)distance_right - (int)distance_left) < 8)
//            {
//              dir('F'); // 전진
//            }
//          else if (distance_right > distance_left)
//            {
//              dir('R'); // 우회전
//            }
//          else
//            {
//              dir('L'); // 좌회전
//            }
//        }
////      if (checkEscapeCondition())
////          {
////                 escapeRoutine();
////                 escaped = true;
////          }
//
//}





