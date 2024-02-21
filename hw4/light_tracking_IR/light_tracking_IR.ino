//#include <ros.h>
//#include <std_msgs/Int32.h>

//ros::NodeHandle nh;
//std_msgs::Int32 light_data;
int state = 0;
bool Ltouch;
bool Rtouch;
bool Dtouch;
int light;

//touch sensor and light sensor and IR sensor
#define Ltouch_sensor A2
#define Rtouch_sensor A1
#define Dtouch_sensor A3
#define light_sensor A0
#define IR_sensor 7

//left wheel
#define encoder0pinA_L 12
#define encoder0pinB_L 3
#define ENA 5
#define IN1 8
#define IN2 9
byte encoder0PinALast_L;
int duration_L;
boolean Direction_L;

//riht wheel
#define encoder1pinA_R 4
#define encoder1pinB_R 2
#define ENB 6
#define IN3 10
#define IN4 11
byte encoder1PinALast_R;
int duration_R;
boolean Direction_R;

//IR sensor
int val;
float rate;


////Get the light data
//void callback(const std_msgs::Int32 &input) {
//  light_data.data = input.data;
//}
//ros::Publisher ros_pub("a2pc", &key);
//ros::Subscriber<std_msgs::Int32> ros_sub("pc2a", &callback );


void setup() {

  Serial.begin(9600);
  //ROS
//  nh.initNode();
//  nh.subscribe(ros_sub);

  //Left wheel
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  //Right wheel
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //touch sensor and light sensor
  pinMode(Ltouch_sensor, INPUT);
  pinMode(Rtouch_sensor, INPUT);
  pinMode(Dtouch_sensor, INPUT);
  pinMode(light_sensor, INPUT);

  //IR sensor
  pinMode(IR_sensor, INPUT);

  EncoderInit();

}

void loop() {

  /*IR count set to 0*/
  float count0=0, count1=0;

//  Serial.println(state);
  duration_L = 0;
  duration_R = 0;

  /*讀取光感數值*/
//  nh.spinOnce();  
  
  /*讀取力感測器*/
  Ltouch = digitalRead(Ltouch_sensor);
  Rtouch = digitalRead(Rtouch_sensor);
  Dtouch = digitalRead(Dtouch_sensor);
  light = analogRead(light_sensor);
  Serial.println(light);
  

  /*state=3時才會讀取IR ratio*/
  if(state == 3){
    for(int i=0;i<120;i++){
      val = digitalRead(IR_sensor);
      if(val) count1++;
      if(!val) count0++;
      delay(1);
    }
    rate = count0/(count0+count1);
    Serial.println(rate);

    /*尋找IR時的避障*/
    if (Ltouch == 0 || Rtouch == 0) {
      setMotor_L(0, -1);
      setMotor_R(0, -1);
      delay(300);
      setMotor_L(1, 110);
      setMotor_R(0, 120);
      delay(200);
    }
    delay(10);
  }

  /*當state=1、2時才會判斷光源和光源時的避障*/
  if (state == 1 || state == 2) {
    
    if(Dtouch == 0){
      setMotor_L(0, 0);
      setMotor_R(0, 0);
      delay(1000);
      state = 3;
      return;
    }
    /*判斷前方是否有光*/
//    if (light_data.data == 0) {
//      state = 2;
//    }
//    else if (light_data.data == 1) {
//      state = 1;
//    }
    if(light < 830){
      state = 2;
    }
    else if(light >= 830){
      state = 1;
    }

    /*尋找光源時的避障*/
    if (Ltouch == 0 || Rtouch == 0) {
      setMotor_L(0, -1);
      setMotor_R(0, -1);
      delay(600);
      state = 1;
    }
    delay(10);
  }

  /*state定義
  0:開始到避開圓環後
  1:尋找光源模式(旋轉)
  2:發現光源(直走)
  3:抵達光緣(停止)
  */
  if (state == 0) {
    /*
      直走+避開圓環+直走，
      完成後state=1
    */
    setMotor_L(1, 210);
    setMotor_R(1, 220);
    delay(100);

    if (Ltouch == 0) {
      setMotor_L(1, 0);
      setMotor_R(1, 0);
      delay(1000);
      setMotor_L(0, -1);
      setMotor_R(0, -1);
      delay(600);
      setMotor_L(1, 110);
      setMotor_R(0, 120);
      delay(300);
      setMotor_L(1, 190);
      setMotor_R(1, 200);
      delay(1500);
      setMotor_L(1, 0);
      setMotor_R(1, 0);
      delay(500);
      state = 1;
    }
    else if (Rtouch == 0) {
      setMotor_L(1, 0);
      setMotor_R(1, 0);
      delay(1000);
      setMotor_L(0, -1);
      setMotor_R(0, -1);
      delay(600);
      setMotor_L(0, 100);
      setMotor_R(1, 110);
      delay(300);
      setMotor_L(1, 190);
      setMotor_R(1, 200);
      delay(1500);
      setMotor_L(0, 0);
      setMotor_R(0, 0);
      delay(500);
      state = 1;
    }
  }
  else if (state == 1) {
    /*尋找光源時，逆時針原地旋轉*/
    setMotor_L(0, 210);
    setMotor_R(1, 220);
    delay(10);
    setMotor_L(0, 110);
    setMotor_R(1, 120);
    delay(150);
    setMotor_L(0, 0);
    setMotor_R(0, 0);
    delay(100);
  }
  else if (state == 2) {
    /*發現光源後，直走靠近光源*/
    setMotor_L(1, 210);
    setMotor_R(1, 220);
    delay(100);
  }
  else if (state == 3) {
    if(rate<0.32 && rate>0.27){ //600
      setMotor_L(1, 210);
      setMotor_R(1, 220);
      delay(100);
    }
    else if(rate<0.22 && rate>0.17){ //1500
      setMotor_L(1, 210);
      setMotor_R(1, 220);
      delay(100);
    }
    else{
      /*when searching IR, rotate clockwise*/
      setMotor_L(1, 210);
      setMotor_R(0, 220);
      delay(150);
      setMotor_L(0, 0);
      setMotor_R(0, 0);
      delay(100);
    }
    
    
  }

  delay(10);
}

//Initialize the encoder
void EncoderInit()
{
  Direction_L = true;//default -> Forward
  Direction_R = true;//default -> Forward
  pinMode(encoder0pinA_L, INPUT);
  pinMode(encoder0pinB_L, INPUT);
  attachInterrupt(1, wheelSpeed_L, CHANGE); //1 represents pin3

  pinMode(encoder1pinA_R, INPUT);
  pinMode(encoder1pinB_R, INPUT);
  attachInterrupt(0, wheelSpeed_R, CHANGE); //0 represents pin2
}

//left wheel encoder
void wheelSpeed_L()
{
  int Lstate_L = digitalRead(encoder0pinA_L);
  if ((encoder0PinALast_L == LOW) && Lstate_L == HIGH)
  {
    int val_L = digitalRead(encoder0pinB_L);
    if (val_L == LOW && Direction_L) //phase A is earlier than phase B
    {
      Direction_L = false; //CW Reverse
    }
    else if (val_L == HIGH && !Direction_L) //phase A is later than phase B
    {
      Direction_L = true;  //CCW Forward
    }
  }
  encoder0PinALast_L = Lstate_L;

  if (!Direction_L)
    duration_L++;
  else
    duration_L--;
}

//right wheel encoder
void wheelSpeed_R()
{
  int Lstate_R = digitalRead(encoder1pinA_R);
  if ((encoder1PinALast_R == LOW) && Lstate_R == HIGH)
  {
    int val_R = digitalRead(encoder1pinB_R);
    if (val_R == LOW && Direction_R) //phase A is earlier than phase B
    {
      Direction_R = false; //CW Forward
    }
    else if (val_R == HIGH && !Direction_R) //phase A is later than phase B
    {
      Direction_R = true;  //CCW Reverse
    }
  }
  encoder1PinALast_R = Lstate_R;

  if (!Direction_R)
    duration_R++;
  else
    duration_R--;
}


//Left Wheel control
void setMotor_L(bool dir, int pwmVal) {
  analogWrite(ENA, pwmVal);
  //dir=1直走，dir=0後退
  if (dir) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
}

//Right Wheel control
void setMotor_R(bool dir, int pwmVal) {
  analogWrite(ENB, pwmVal);
  //dir=1直走，dir=0後退
  if (dir) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
}
