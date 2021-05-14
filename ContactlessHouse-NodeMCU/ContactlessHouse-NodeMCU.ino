#define D0 16
#define LED D0

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
}

char msg[100];
int val = 0;
int light = HIGH;
String inString = "";

void loop() {
  String data = String(val);
  Serial.print(data);
  val++;
  delay(500);
}
