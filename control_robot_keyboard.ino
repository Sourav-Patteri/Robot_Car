//Sourav Patteri 
//Final Project
char data;
//bool state = LOW;
String command;
#define RECV_PIN  12
//define channel enable output pins
#define LWS 5  // Left  wheel speed
#define RWS 6  // Right wheel speed
//define logic control output pins
#define LWF 7  // Left  wheel forward
#define LWR 8  // Left  wheel reverse
#define RWR 9  // Right wheel reverse
#define RWF 11 // Right wheel forward
#define carSpeed 150 // initial speed of car >=0 to <=255


 void forward(){//function to move the robot forward 
  digitalWrite(LWS,HIGH);
  digitalWrite(RWS,HIGH);
  digitalWrite(LWF,HIGH);
  digitalWrite(LWR,LOW);
  digitalWrite(RWR,LOW);
  digitalWrite(RWF,HIGH);
} 
void back(){//function to move the robot backwards
  digitalWrite(LWS,HIGH);
  digitalWrite(RWS,HIGH);
  digitalWrite(LWF,LOW);
  digitalWrite(LWR,HIGH);
  digitalWrite(RWR,HIGH);
  digitalWrite(RWF,LOW);
}
void left(){//function to turn the robot left
  analogWrite(LWS,carSpeed);
  analogWrite(RWS,carSpeed);
  digitalWrite(LWF,LOW);
  digitalWrite(LWR,HIGH);
  digitalWrite(RWR,LOW);
  digitalWrite(RWF,HIGH); 
}
void right(){//function to turn the robot right
  analogWrite(LWS,carSpeed);
  analogWrite(RWS,carSpeed);
  digitalWrite(LWF,HIGH);
  digitalWrite(LWR,LOW);
  digitalWrite(RWR,HIGH);
  digitalWrite(RWF,LOW);
}
void stop(){//function to stop the robot 
  digitalWrite(LWS, LOW);
  digitalWrite(RWS, LOW);
}

void setup() {
  // setup code here runs once:
  Serial.begin(9600); // Begins the Serial Communication with the Baud rate 9600.
  // Sets the Pins LWF, LWR, RWR, RWF, LWS, RWS as OUTPUT Pins.
  pinMode(LWF,OUTPUT);
  pinMode(LWR,OUTPUT);
  pinMode(RWR,OUTPUT);
  pinMode(RWF,OUTPUT);
  pinMode(LWS,OUTPUT);
  pinMode(RWS,OUTPUT);
  stop();
}
//A function is a way for programmers to reuse code without having to rewrite the code.
void moveRobot(String motion) {
 //checking value of motion send from the camera using python, depending on the position of cursor on the grid.
  if (motion == "f") { // RW - Fwd(10 - Pos, 11 - Neg); LW - Fwd(12 - Pos, 13 - Neg)
   forward();
    Serial.println("Forward");
  }
//if the value returned is b the robot moves backwards.
  if (motion == "b") { // RW - Bck(10 - Neg, 11 - Pos); LW - Bck(12 - Neg, 13 - Pos)
    back();
    Serial.println("Backward");
  }
//if its l it moves towards right
  if (motion == "l") { // RW - Fwd(10 - Pos, 11 - Neg); LW - Bck(12 - Neg, 13 - Pos)
   left();
   Serial.println("Left");
  }

  if (motion == "r") { // RW - Bck(10 - Neg, 11 - Pos); LW - Fwd(12 - Pos, 13 - Neg)
    right();
    Serial.println("Right");
  }

  if (motion == "s") { // RW - Stop(10 - Neg, 11 - Neg); LW - Stop(12 - Neg, 13 - Neg)
    stop();
    Serial.println("Stop");
  }

}

void loop() {
  //main code runs repeatedly:
  //when serial data is available runs
  while (Serial.available()) {
    data = Serial.read();
    command = String(data);
    moveRobot(command);
  }

  delay(1000);

}
