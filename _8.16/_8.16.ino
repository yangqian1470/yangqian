#include <MsTimer2.h>
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
#define NUMFLAKES 10
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
Adafruit_SSD1306 display(OLED_RESET);

SoftwareSerial mySerial(A2, A0);
unsigned char Re_buf[8], counter = 0;
unsigned char sign = 0;
int i;

/*---------------------------------全局需要的变量-------*/
int angLAST;
int x_time1 = 0;
int y_time1 = 0;
int x_time0 = 0;
int y_time0 = 0;
int x_time = 0;
int y_time = 0;
int A;
int a=0;
float x5 = 0;
float y5 = 0;
int x = 0;
int y = 0;
int ang8=0;
float _x = 0;
float _y = 0;//刷新瞬间两个参数
float x2 = 0;
float y2 = 0;
int x_l = 0;
int x_r = 0;
int y_l = 0;
int y_r = 0;
int ang = 0;
int A_r = 0;
int A_l = 0; //角度
int x_step = 0;
int y_step = 0;
long int _millis = 0;
long int x_step_total = 0;
long int y_step_total = 0; //总参数

static const int INT0A = 3;
static const int INT0B = 8;
static const int INT1A = 2;
static const int INT1B = 7;//读取编码器

const double wheel_d = 38;//轮直径38mm
const int x_line = 1000;
const int y_line = 1000;//光栅线程

void renturnang(int angle8);
void POS_refresh(int x_step, int y_step, int ang);
void testscrolltext(int a);

void setup() {
  pinMode(13,OUTPUT);
  pinMode(11,INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);
  POS_begin();
  GY25begin();
}

void loop() {
  /*---------------关于陀螺仪---------------*/
while (mySerial.available()) {
    Re_buf[counter] = mySerial.read();
    if (counter == 0 && Re_buf[0] != 0xAA) return; // 检查帧头
    counter++;
    if (counter == 8) //接受完毕
    {
      counter = 0; //重新赋值，准备下一帧数据的接收
      sign = 1;
    }
  }
  if (sign)
  {
    sign = 0;
    if (Re_buf[0] == 0xAA && Re_buf[7] == 0x55)   //检查帧头，帧尾
    {
      ang = (Re_buf[1] << 8 | Re_buf[2]) / 100; //合成数据，去掉小数点后2位
    }
  };
 //串口事件结束 
  POS_refresh(x_step, y_step, ang);
  renturnang(ang8);
  if (millis() - _millis >= 10)
  {transmission();
  _millis=millis();}
}

void blinkX()
{
  i++;
  int xstate = digitalRead(INT0B);
  if (xstate == HIGH)
  {
    x_step++;
  }
  else
  {
    x_step--;
  }
}

void blinkY()
{
  int ystate = digitalRead(INT1B);
  if (ystate == HIGH)
  {
    y_step--;
  }
  else
  {
    y_step++;
  }
}

void POS_refresh(int x_step, int y_step, int ang)
{
  x_time1 = x_step;
  y_time1 = y_step;
  x_time = x_time1 - x_time0;
  y_time = y_time1 - y_time0;
  x_time0 = x_step;
  y_time0 = y_step;
  _x = double(x_time) / x_line * PI * wheel_d;
  _y = double(y_time) / y_line * PI * wheel_d; //行走的距离 单位mm
  if(ang-angLAST<=20 && ang-angLAST>=-20)
  {
  float a = (-ang - 45) / 180.00 * PI;
  angLAST=ang;
  y2 = (-_y) * sin(a) + (_x) * cos(a);
  x2 = (_x) * sin(a) +(_y) * cos(a);
  x -= x2;
  y += y2;
  }
  ang8=ang;
  x_l = x >> 8;
  x_r = x & 0XFF;
  y_l = y >> 8;
  y_r = y & 0XFF;
}

void renturnang(int angle8)
{
  if (-90 <= angle8 && angle8 <= 180)
    A = angle8 + 90;
  else
    A = angle8 + 450;
  A_r = A & 0XFF; //低八位
  A_l = A >> 8; //高八位
}

void POS_begin()
{
  pinMode(INT0A, INPUT_PULLUP);
  pinMode(INT0B, INPUT);
  pinMode(INT1A, INPUT_PULLUP);
  pinMode(INT1B, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT0A), blinkX, RISING);
  attachInterrupt(digitalPinToInterrupt(INT1A), blinkY, RISING);//中断过程
}

void GY25begin()
{
  mySerial.begin(115200);//所需软串口波特率
  mySerial.write(0XA5);
  delay(1);
  mySerial.write(0X55);
  delay(1000);
  mySerial.write(0XA5);
  delay(1);
  mySerial.write(0X52);    //初始化GY25,连续输出模式 //初始化GY25,连续输出模式
  delay(1000);
}


void transmission()
{
//    Serial.write(0xAA);
//    Serial.write(x_l);
//    Serial.write(x_r);
//    Serial.write(y_l);
//    Serial.write(y_r);
//    Serial.write(A_l);
//    Serial.write(A_r);
//    Serial.write(0x55);
    Serial.print("x:");
    Serial.print(x);    
    Serial.print("  ");
    Serial.print("y:");
    Serial.print(y);   
    Serial.print("  ");
    Serial.print("p:");
    Serial.print(ang);
    Serial.print("  ");
    Serial.print("ang8:");
    Serial.print(ang8); 
    Serial.print("  ");
    Serial.print("x_step:");
    Serial.print(x_step); 
    Serial.print("  ");
    Serial.print("y_step:");
    Serial.print(y_step); 
    Serial.print("  ");
    Serial.print("y2");
    Serial.print(y2); 
    Serial.print("  ");
    Serial.print("x2:");
    Serial.println(x2); 
    display.clearDisplay();   // clears the screen and buffer
    display.setTextSize(1); //选择字号
    display.setTextColor(WHITE);  //字体颜色
    display.setCursor(0,0);   //起点坐标
    display.print("x:"); 
    display.println(x);
    display.print("y:");
    display.println(y);
    display.setTextSize(1);
    display.print("ang:");
    display.println(ang); 
    display.print("ang8:");
    display.println(ang8); 
    display.print("x_step:");
    display.print(x_step); 
    display.print("0000");
    display.print("y_step:");
    display.print(y_step); 
    display.display();
    delay(1);
}
