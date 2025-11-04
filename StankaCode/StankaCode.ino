#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27,16,2);

//const char* ssid = "RoboClub";         /* Replace with your Wi-Fi SSID */
//const char* password = "roboclub9405"; /* Replace with your Wi-Fi password */
//const char* ssid = "RoboClub";         
//const char* password = "roboclub9405";

WiFiUDP udp;                            /* UDP instance */
unsigned int localUdpPort = 10000;       /* The same port number as in your Python code */
char incomingPacket[255];   /* Buffer for incoming packets */

int detFront = 35; /*pin za sensor*/
int detLeft = 34; /*pin za sensor*/
int detRight = 14; /*pin za sensor*/
int detBack = 27; /*pin za sensor*/
int in_1 = 33;/*napred dqsna veriga*/
int in_2 = 32;/*nazad*/
int in_3 = 26;/*napred lqva veriga*/
int in_4 = 25;/*nazad*/
int led1 = 2; /*pin za predeno LED*/
int led2 = 4; /*pin za predeno LED*/
int in_pwmA = 19; /* pwm za motorit A*/
int in_pwmB = 18; /* pwm za motirt B*/
int stby = 5;

void setup() {

  
  pinMode(detFront, INPUT);
  pinMode(detLeft, INPUT);
  pinMode(detRight, INPUT);
  pinMode(detBack, INPUT);
  pinMode(in_1, OUTPUT);
  pinMode(in_2, OUTPUT);
  pinMode(in_3, OUTPUT);
  pinMode(in_4, OUTPUT);  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(in_pwmA, OUTPUT);
  pinMode(in_pwmB, OUTPUT);
  pinMode(stby, OUTPUT);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  /* Set ESP32 to WiFi Station mode */
  WiFi.mode(WIFI_AP_STA);
  /* start SmartConfig */
  WiFi.beginSmartConfig();

  /* Wait for SmartConfig packet from mobile */
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("SmartConfig done.");

  /* Wait for WiFi to connect to AP */
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0,0);
  lcd.print(WiFi.localIP());

  // Start the UDP listener on the specified port
  udp.begin(localUdpPort);
  Serial.printf("Now listening on UDP port %d\n", localUdpPort);
}

void loop() {
  // Check if any UDP packets are available
  int packetSize = udp.parsePacket();
  if (packetSize) {
    // Receive the incoming UDP packet
    // expected 
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;  // Null-terminate the string
    }

    // Print the incoming packet
    Serial.printf("Received UDP packet: %s\n", incomingPacket);

    // Parse the incoming JSON data
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, incomingPacket);

    if (error) {
      Serial.print("Failed to parse JSON: ");
      Serial.println(error.c_str());
      return;
    }

    // Extract values from the JSON
    String gesture = doc["gesture"];// Get the 'gesture' string value
    

    // Print the extracted values to the Serial Monitor
    Serial.printf("gesture: %s\n",gesture);
    
      if(gesture.equals("Counter Clockwise")){         
            turnLeft(300);
        }else if(gesture.equals("Clockwise")){
            turnRight(300);
        }
       if(gesture.equals("Open")){
             goForward(750);
      }
      if(gesture.equals("OK")){
        danceBaby();
      }
      if(gesture.equals("Close")){
        goBackward(750);
      }
  }
}

void goForward (int amount){
  lcd.setCursor(0,0);
  lcd.print("Going forward!");
  Serial.printf("FORWARD\n");
  digitalWrite(stby, HIGH);
  analogWrite(in_pwmA, 280);
  digitalWrite(in_pwmB, HIGH);
  digitalWrite(in_1, HIGH);
  digitalWrite(in_4, HIGH);
  delay(amount);
  digitalWrite(in_pwmA, LOW);
  digitalWrite(in_pwmB, LOW);
  digitalWrite(in_1, LOW);
  digitalWrite(in_4, LOW);
  lcd.setCursor(0,0);
  lcd.print(WiFi.localIP());
}

void goBackward (int amount){
  lcd.setCursor(0,0);
  lcd.print("Going backwards!");
  analogWrite(in_pwmA, 220);
  digitalWrite(in_pwmB, HIGH);
  digitalWrite(stby, HIGH);
  digitalWrite(in_2, HIGH);
  digitalWrite(in_3, HIGH);
  delay(amount);
  digitalWrite(in_pwmA, LOW);
  digitalWrite(in_pwmB, LOW);
  digitalWrite(in_2, LOW);
  digitalWrite(in_3, LOW);
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.print(WiFi.localIP());
}

void turnLeft (int amount){ // 1250 za 90 gradusa
  lcd.setCursor(0,0);
  lcd.print("Turning left!");
  digitalWrite(in_pwmA, HIGH);
  digitalWrite(in_pwmB, HIGH);
  digitalWrite(stby, HIGH);
  digitalWrite(in_1, HIGH);
  digitalWrite(in_3, HIGH);
  delay(amount);
  digitalWrite(in_pwmA, LOW);
  digitalWrite(in_pwmB, LOW);
  digitalWrite(in_1, LOW);
  digitalWrite(in_3, LOW);
  lcd.setCursor(0,0);
  lcd.print("                      ");
  lcd.print(WiFi.localIP());
}

void turnRight (int amount){
  lcd.setCursor(0,0);
  lcd.print("Turning right!");
  digitalWrite(in_pwmA, HIGH);
  digitalWrite(in_pwmB, HIGH);
  digitalWrite(stby, HIGH);
  digitalWrite(in_2, HIGH);
  digitalWrite(in_4, HIGH);
  delay(amount);
  digitalWrite(in_pwmA, LOW);
  digitalWrite(in_pwmB, LOW);
  digitalWrite(in_2, LOW);
  digitalWrite(in_4, LOW);
  lcd.setCursor(0,0);
  lcd.print("                      ");
  lcd.print(WiFi.localIP());
}

void danceBaby(){
  lcd.setCursor(0,0);
  lcd.print("Dancing!");
  for(int i = 0; i < 10; i++){
          if( i%2 == 0){
            digitalWrite(led1, HIGH);
            digitalWrite(led2,LOW);
            delay(150);
          }else{
            digitalWrite(led1, LOW);
            digitalWrite(led2,HIGH);
            delay(150);
          }
        }
        digitalWrite(led1,LOW);
        digitalWrite(led2,LOW);
        delay(150);
        
        turnLeft(300);
        delay(150);
        turnRight(300);
        delay(100);
        lcd.setCursor(0,0);
        lcd.print(WiFi.localIP());
}
