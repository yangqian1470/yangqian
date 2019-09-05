#include<Servo.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define pi 3.1415926
#define mylength 2400
#define mymistake 100
Servo myservo;
void  textbujin();
void  runStepper();
void BuJinsetup();
float JG_R1[5];
float JG_R2[5];
const int dirPin   =  4;   // Direction
const int stepPin  = 5;   // Step
//const int enPin    = 6;   // Enable
int ButtonState;
int myspeed;

char my_X;
double POS_ang1;
double POS_ang;
int motorSpeed = 800;
int data_L = 1;
int ButtonState_l = 1;

void setup()
{
  mocalun();
  Serial.begin(9600);
  Serial1.begin(19200);
  Serial2.begin(19200);
  BuJinsetup();
}

void loop()
{
  begintime();
  ButtonState = digitalRead(11);//限位开关
  while (ButtonState != ButtonState_l)//ButtonState == LOW
  { digitalWrite(dirPin, LOW);
    ButtonState_l = 0;
    int data = digitalRead(6); //接受主机靠边指令
    Serial.println(data);
    if (data == data_L)
      /*------开始复位---*/
    { data_L = 0;
      POS_ang = 0;
      jiguang();
      textbujin();
      delay(10);
      runStepper(POS_ang);
      myservo.writeMicroseconds(1175);
      //    ButtonState_l=ButtonState;
    }
  }
  int aa = digitalRead(7);
  if (aa == 0)
    myservo.writeMicroseconds(1225);
  if (aa == 1)
    myservo.writeMicroseconds(1175);

}

/*--------------步进电机转动-----------*/
void runStepper (double circle)
{
  for (int t = 0; t < circle; t++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(motorSpeed);
  }
}

/*------用射击中间桶来验证步进电机-----*/
void  textbujin()
{
  float my_x = (JG_R1[0] - JG_R2[0]) / 2;
  POS_ang1 = atan2(2400 , my_x);
  POS_ang = POS_ang1 * double(180) / pi * 440 / 9;
  int my_long = sqrt(40.00000 + my_x * my_x);
  myspeed = 0.000000001387 * my_long * my_long * my_long - 0.000010265 * my_long * my_long + 0.0791 * my_long + 1149.3;

}
/*
  void targetangle()
  {
    switch( A[1]-A[2] )
    {
      case 2: case 6:
        {
          dy=4800-mymistake;
          dx=2400+my_X;}
        break;
      case 1: case 5:
        {
          dy=4800-mymistake;
          dx=2400-my_X;}
        break;
      case 3: case 7:
        {
          dy=0;
          dx=2400+my_X;}
        break;
      case 4:
        {
          dy=0;
          dx=2400-my_X;}
        break;
    }
  }    */


/*------------------步进电机初始化-----------------------*/
void BuJinsetup()
{ pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT);
  pinMode(13, OUTPUT);
  pinMode(11, INPUT_PULLUP);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin , OUTPUT);
  digitalWrite(dirPin, HIGH);
}

void  begintime()
{
  for (int t = 0; t < 50; t++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
  }
}

void mocalun()
{ myservo.attach(3);
  pinMode(3, OUTPUT);
  myservo.writeMicroseconds(2000);
  delay(3500);
  myservo.writeMicroseconds(1000);
  delay(2000);
}
/*----谁也别动我的激光------*/
void jiguang() {
  Serial1.print('D');
  for (int i = 0; i < 2; i++)
    JG_R1[i] = Serial1.parseFloat();
  Serial.println(JG_R1[0]);
  Serial2.print('D');
  for (int i = 0; i < 2; i++)
    JG_R2[i] = Serial2.parseFloat();
  Serial.println(JG_R2[0]);
}

