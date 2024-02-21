#include <ros.h>
#include <std_msgs/Char.h>

ros::NodeHandle nh;
std_msgs::Char key;

//left wheel
#define encoder0pinA_L 12
#define encoder0pinB_L 3 //Interrupt pin
#define ENA 5
#define IN1 8
#define IN2 9
byte encoder0PinALast_L;
int duration_L; //the number of the pulses
boolean Direction_L; //the rotation direction

//riht wheel
#define encoder1pinA_R 4
#define encoder1pinB_R 2 //Interrupt pin
#define ENB 6
#define IN3 10
#define IN4 11
byte encoder1PinALast_R;
int duration_R; //the number of the pulses
boolean Direction_R; //the rotation direction


//get the key
void callback(const std_msgs::Char &input){ 
  key.data=input.data;
}
ros::Publisher ros_pub("a2pc", &key);
ros::Subscriber<std_msgs::Char> ros_sub("pc2a", &callback );


void setup() {
  
  //ROS
  nh.initNode();
  nh.advertise(ros_pub);
  nh.subscribe(ros_sub);

  //Left wheel
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);

  //Right wheel
  pinMode(ENB,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  
  EncoderInit();
}

void loop() {

  nh.spinOnce();
  delay(1);
  ros_pub.publish(&key);
  
  Serial.print("Left Speed:");
  Serial.println(duration_L);
  Serial.print("Right Speed:");
  Serial.println(duration_R);
  duration_L = 0;
  duration_R = 0;
  
  setMotor_L(key.data,80,ENA,IN1,IN2);
  setMotor_R(key.data,80,ENB,IN3,IN4);
  
  delay(100);

}

//Initialize the encoder
void EncoderInit()
{
  Direction_L = true;//default -> Forward
  Direction_R = true;//default -> Forward
  pinMode(encoder0pinA_L,INPUT);
  pinMode(encoder0pinB_L,INPUT);
  attachInterrupt(1, wheelSpeed_L, CHANGE); //1 represents pin3
  
  pinMode(encoder1pinA_R,INPUT);
  pinMode(encoder1pinB_R,INPUT);
  attachInterrupt(0, wheelSpeed_R, CHANGE); //0 represents pin2
}

//left wheel encoder
void wheelSpeed_L()
{
  int Lstate_L = digitalRead(encoder0pinA_L);
  if((encoder0PinALast_L == LOW) && Lstate_L==HIGH)
  {
    int val_L = digitalRead(encoder0pinB_L);
    if(val_L == LOW && Direction_L) //phase A is earlier than phase B
    {
      Direction_L = false; //CW Reverse
    }
    else if(val_L == HIGH && !Direction_L) //phase A is later than phase B
    {
      Direction_L = true;  //CCW Forward
    }
  }
  encoder0PinALast_L = Lstate_L;

  if(!Direction_L)
    duration_L++;
  else
    duration_L--;
}

//right wheel encoder
void wheelSpeed_R()
{
  int Lstate_R = digitalRead(encoder1pinA_R);
  if((encoder1PinALast_R == LOW) && Lstate_R==HIGH)
  {
    int val_R = digitalRead(encoder1pinB_R);
    if(val_R == LOW && Direction_R) //phase A is earlier than phase B
    {
      Direction_R = false; //CW Forward
    }
    else if(val_R == HIGH && !Direction_R) //phase A is later than phase B
    {
      Direction_R = true;  //CCW Reverse
    }
  }
  encoder1PinALast_R = Lstate_R;

  if(!Direction_R)
    duration_R++;
  else
    duration_R--;
}

//Left Wheel control
void setMotor_L(char key, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal);
  if(key == 'w'){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(key == 's'){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else if(key == 'a'){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else if(key == 'd'){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(key == 'x'){
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
}

//Right Wheel control
void setMotor_R(char key, int pwmVal, int pwm, int in3, int in4){
  analogWrite(pwm,pwmVal);
  if(key == 'w'){
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
  }
  else if(key == 's'){
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
  }
  else if(key =='a'){
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
  }
  else if(key == 'd'){
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
  }
  else if(key == 'x'){
    digitalWrite(in3,LOW);
    digitalWrite(in4,LOW);
  }
}
