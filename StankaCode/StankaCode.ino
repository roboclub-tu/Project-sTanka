#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27,16,2);

//const char* ssid = "RoboClub";         // Replace with your Wi-Fi SSID
//const char* password = "roboclub9405"; // Replace with your Wi-Fi password
//const char* ssid = "RoboClub";         
//const char* password = "roboclub9405";

WiFiUDP udp;                            // UDP instance
unsigned int localUdpPort = 10000;       // The same port number as in your Python code
char incomingPacket[255];   // Buffer for incoming packets

int detFront = 35;
int detLeft = 34;
int detRight = 14;
int detBack = 27;
int in_1 = 33;//napred dqsna veriga
int in_2 = 32;//nazad
int in_3 = 26;//napred lqva veriga
int in_4 = 25;//nazad
int led1 = 2;
int led2 = 4;


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
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

                                            // Connect to Wi-Fi
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Connecting to WiFi...");
//  }
//  Serial.println("Connected to WiFi");
//  
//  // Print the ESP32 IP address
//  Serial.print("ESP32 IP address: ");
//  Serial.println(WiFi.localIP());
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
//    int x = doc["x"];  // Get the 'x' value
//    int y = doc["y"];  // Get the 'y' value
    String gesture = doc["gesture"];// Get the 'gesture' string value
    

    // Print the extracted values to the Serial Monitor
    Serial.printf("gesture: %s\n",gesture);
//&& digitalRead(detLeft) == 0 && digitalRead(detRight) == 0 && digitalRead(detBack) == 0
      if(gesture.equals("Counter Clockwise")){  
          if(digitalRead(detLeft) == 1){       
            turnLeft(300);
          }
        }else if(gesture.equals("Clockwise")){
          if( digitalRead(detRight) == 1){
            turnRight(300);
          }
        }
       if(gesture.equals("Open")){
        if(digitalRead(detFront) == 1 || digitalRead(detRight) == 1 || digitalRead(detLeft) == 1){
          goForward(750);
        }
      }
      if(gesture.equals("OK")){
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
      }
      if(gesture.equals("Close")){
        if(digitalRead(detBack) == 1){
        goBackward(750);
        }
      }
//   }
  }
    
  
  
}

void goForward (int amount){
  digitalWrite(in_1, HIGH);
  digitalWrite(in_3, HIGH);
  delay(amount);
  digitalWrite(in_1, LOW);
  digitalWrite(in_3, LOW);
}

void goBackward (int amount){
  digitalWrite(in_2, HIGH);
  digitalWrite(in_4, HIGH);
  delay(amount);
  digitalWrite(in_2, LOW);
  digitalWrite(in_4, LOW);
}

void turnLeft (int amount){ // 1250 za 90 gradusa
  digitalWrite(in_1, HIGH);
  digitalWrite(in_4, HIGH);
  delay(amount);
  digitalWrite(in_1, LOW);
  digitalWrite(in_4, LOW);
}

void turnRight (int amount){
  digitalWrite(in_2, HIGH);
  digitalWrite(in_3, HIGH);
  delay(amount);
  digitalWrite(in_2, LOW);
  digitalWrite(in_3, LOW);
}
