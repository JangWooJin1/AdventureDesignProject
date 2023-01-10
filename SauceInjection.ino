//무게센서
#include "HX711.h" //HX711로드셀 엠프 관련함수 호출
#define calibration_factor -4050.0 // 눈금 단위
#define DOUT  A2 //엠프 데이터 아웃 핀 넘버 선언
#define CLK  A4  //엠프 클락 핀 넘버 
HX711 scale(DOUT, CLK); //엠프 핀 선언 
//무게센서

//버튼
  //분사량
#define LESSVOLUME 2
#define MIDVOLUME 3
#define MOREVOLUME 4

float lessWeight = 1.0;
float midWeight = 3.0;
float moreWeight = 6.0;
  //분사량

   //분사속도
#define SPEED 5
bool highsp = true;
int sp = 255;
   //분사속도

   //분사면적
#define AREA 6
bool bigArea = false;
   //분사면적

//버튼

//LED
  //분사량
#define LESSVOLUME_LED 7
#define MIDVOLUME_LED 8
#define MOREVOLUME_LED 9
  //분사량

   //분사속도
#define LOWSPEED_LED 10
#define HIGHSPEED_LED 11
   //분사속도

   //분사면적
#define BIGAREA_LED 12
#define SMALLAREA_LED 13
   //분사면적
//LED

//펌프
#define in1 A1
#define in3 A3

#define ENPin A0
#define ENPin2 A5
//펌프

void playWaterPump(float targetWeight){
  
  //워터 펌프 활성화
  analogWrite(in1, sp);
  if(bigArea == true) { 
    analogWrite(in3, sp); 
  }
  //워터 펌프 활성화

  scale.tare(); //영점잡기
  float startWeight = scale.get_units(); //펌프 작동 전 무게
  float tempWeight = 0.0;                //펌프 작동 후 실시간 무게
  int countSize = 5;                     //흔들림 오차를 줄이기 위한 측정 횟수 설정  
  float nowWeight = startWeight;         //단위 시간(countSize)내 무게의 평균값 ( 현재 무게 )
  int errorCount = 0;                    //예외처리 ( 소스가 없을 경우 멈추기 위한 변수 )

 // 목표 무게를 달성할때까지 측정 반복
  while( nowWeight - startWeight < targetWeight ){
    tempWeight = 0.0;
    for(int i=0; i < countSize; i++){ 
      tempWeight += scale.get_units();
    }

    //예외처리 ( 무게가 줄어들지 않는 경우 = 소스통이 비어있는 경우 ) => 에러
    if ( tempWeight / countSize <= nowWeight ){ errorCount += 1; }
    if( errorCount >= 3 ) { break; }  => 에러 횟수가 3이상이면 강제 종료
  
    nowWeight = tempWeight / countSize;   //현재 무게 업데이트
   
  }

  //워터 펌프 비활성화
  analogWrite(in1, LOW);
  if(bigArea == true){
    analogWrite(in3, LOW);        
  }
  //워터 펌프 비활성화

}

void setup() {
  Serial.begin(9600);

  scale.set_scale(calibration_factor);  //스케일 지정 
  scale.tare();  //스케일 설정

  //버튼
  pinMode(LESSVOLUME, INPUT);
  pinMode(MIDVOLUME, INPUT);
  pinMode(MOREVOLUME, INPUT);
  pinMode(SPEED, INPUT);
  pinMode(AREA, INPUT);
  //버튼

  //LED
  pinMode(LESSVOLUME_LED, OUTPUT);
  pinMode(MIDVOLUME_LED, OUTPUT);
  pinMode(MOREVOLUME_LED, OUTPUT);

  pinMode(LOWSPEED_LED, OUTPUT);
  pinMode(HIGHSPEED_LED, OUTPUT);

  pinMode(BIGAREA_LED, OUTPUT);
  pinMode(SMALLAREA_LED, OUTPUT); 
  //LED

  //펌프
  pinMode(in1, OUTPUT);

  pinMode(in3, OUTPUT);

  pinMode(ENPin, OUTPUT);
  pinMode(ENPin2, OUTPUT);
 
  //펌프
  analogWrite(ENPin,sp);
  analogWrite(ENPin2,sp); 
  digitalWrite(HIGHSPEED_LED, HIGH);
  digitalWrite(SMALLAREA_LED, HIGH);
}

void loop() {
 
  if(digitalRead(SPEED) == HIGH){
    highsp = !highsp;
    if(highsp == true){
      digitalWrite(LOWSPEED_LED, LOW);
      digitalWrite(HIGHSPEED_LED, HIGH);
      sp = 255;
    }
    else{
      digitalWrite(HIGHSPEED_LED, LOW);
      digitalWrite(LOWSPEED_LED, HIGH);
      sp = 130;       
    }
    analogWrite(ENPin,sp);
    analogWrite(ENPin2,sp); 
    delay(500);
  } 


  if(digitalRead(AREA) == HIGH){
    bigArea = !bigArea;
    if(bigArea == true){
      digitalWrite(SMALLAREA_LED, LOW);
      digitalWrite(BIGAREA_LED, HIGH);
    } 
    else{
      digitalWrite(BIGAREA_LED, LOW);
      digitalWrite(SMALLAREA_LED, HIGH);
    }
    Serial.println(bigArea);
    delay(500);
    
  }

  if(digitalRead(LESSVOLUME) == HIGH){
    digitalWrite(LESSVOLUME_LED, HIGH);
    playWaterPump(lessWeight);
    digitalWrite(LESSVOLUME_LED, LOW);
  } 
  else if(digitalRead(MIDVOLUME) == HIGH){
    digitalWrite(MIDVOLUME_LED, HIGH);
    playWaterPump(midWeight);
    digitalWrite(MIDVOLUME_LED, LOW);
  } 
  else if(digitalRead(MOREVOLUME) == HIGH){
    digitalWrite(MOREVOLUME_LED, HIGH);
    playWaterPump(moreWeight);
    digitalWrite(MOREVOLUME_LED, LOW);
  }

}
