int mr1 = 8;
int mr2 = 9;
int ml1 = 10;
int ml2 = 11;
int enr = 3;
int enl = 5;
int sr = 6;
int sl = 7;
int led = 13;
int svr = 0;
int svl = 0;
int vspeed = 100;
int tspeed = 255;
int tdelay = 20;

void setup() {
  pinMode(mr1, OUTPUT);
  pinMode(mr2, OUTPUT);
  pinMode(ml1, OUTPUT);
  pinMode(ml2, OUTPUT);
  pinMode(sr, INPUT);
  pinMode(sl, INPUT);
  pinMode(led, OUTPUT);
  delay(5000);
}

void loop() {
  svr = digitalRead(sr);
  svl = digitalRead(sl);

  if (svl == LOW && svr == LOW) {
    forward();
  } else if (svl == HIGH && svr == LOW) {
    left();
  } else if (svl == LOW && svr == HIGH) {
    right();
  } else if (svl == HIGH && svr == HIGH) {
    stopRobot();
  }
}

void forward() {
  digitalWrite(mr1, HIGH);
  digitalWrite(mr2, LOW);
  digitalWrite(ml1, HIGH);
  digitalWrite(ml2, LOW);
  analogWrite(enr, vspeed);
  analogWrite(enl, vspeed);
}

void backward() {
  digitalWrite(mr1, LOW);
  digitalWrite(mr2, HIGH);
  digitalWrite(ml1, LOW);
  digitalWrite(ml2, HIGH);
  analogWrite(enr, vspeed);
  analogWrite(enl, vspeed);
}

void right() {
  digitalWrite(mr1, LOW);
  digitalWrite(mr2, HIGH);
  digitalWrite(ml1, HIGH);
  digitalWrite(ml2, LOW);
  analogWrite(enr, tspeed);
  analogWrite(enl, tspeed);
  delay(tdelay);
}

void left() {
  digitalWrite(mr1, HIGH);
  digitalWrite(mr2, LOW);
  digitalWrite(ml1, LOW);
  digitalWrite(ml2, HIGH);
  analogWrite(enr, tspeed);
  analogWrite(enl, tspeed);
  delay(tdelay);
}

void stopRobot() {
  analogWrite(enr, 0);
  analogWrite(enl, 0);
  digitalWrite(led, HIGH);
}
