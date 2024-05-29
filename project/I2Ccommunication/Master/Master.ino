// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include <Wire.h>
#include <iostream>

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Janusz";
const char* password = "11111111";

const int output = 5;

byte x = 0;

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 80px;
        margin: 25px;
        height: 60px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1>ESP Pushbutton Web Server</h1>
    <button class="button" id="btn1" onmousedown="toggleCheckbox('on');" ontouchstart="toggleCheckbox('on');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">Forward</button>
    <button class="button" id="btn2" onmousedown="toggleCheckbox('back');" ontouchstart="toggleCheckbox('back');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">Backward</button>

   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

// the setup function runs once when you press reset or power the board
void setup() {

  Wire.begin(4,5); // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  delay(100);
  Serial.println("Master Ready");


  //Serial.begin(115200);
  Serial.println("\n\nyo");

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP()); // 192.168.4.1
  
  
  //pinMode(output, OUTPUT); // led

  //digitalWrite(output, LOW);

  // initialize digital pin LED_BUILTIN as an output.
  

  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    //digitalWrite(output, HIGH);
    // analogWrite(motorAPWM, 127);
    // analogWrite(motorBPWM, 127);
    
    // analogWrite(motorBBPWM, 127);

    transmit('f');

    request->send(200, "text/plain", "ok");
  });

  server.on("/back", HTTP_GET, [] (AsyncWebServerRequest *request){
    // digitalWrite(motorA1, LOW);
    // digitalWrite(motorA2, HIGH);

    // digitalWrite(motorB1, LOW);
    // digitalWrite(motorB2, HIGH);

    // digitalWrite(motorBB1, LOW);
    // digitalWrite(motorBB2, HIGH);


    // analogWrite(motorAPWM, 127);
    // analogWrite(motorBPWM, 127);

    // analogWrite(motorBBPWM, 127);
    transmit('b');
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request
  server.on("/off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // digitalWrite(motorA1, HIGH);
    // digitalWrite(motorA2, LOW);

    // digitalWrite(motorB1, HIGH);
    // digitalWrite(motorB2, LOW);

    // digitalWrite(motorBB1, HIGH);
    // digitalWrite(motorBB2, LOW);


    // //digitalWrite(output, LOW);
    // analogWrite(motorAPWM, 0);
    // analogWrite(motorBPWM, 0);

    // analogWrite(motorBBPWM, 0);
    transmitEnd();
    request->send(200, "text/plain", "ok");
  });
  
  server.onNotFound(notFound);
  server.begin();
}


void loop() {
  // Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8

  // while (Wire.available()) { // slave may send less than requested
  //   char c = Wire.read(); // receive a byte as character
  //   Serial.print(c);         // print the character
  // }
  // Serial.println();
  // delay(500); 

}

void transmitDefault() {
   Wire.beginTransmission(8);
  Wire.write("x is ");
  Wire.write(x);
  Wire.endTransmission();

  x++;
  std::cout << "\n";
  
}

void transmit(char direction) {
  Wire.beginTransmission(8);
  Wire.write(direction);
  Wire.endTransmission();

  std::cout << "direction " << direction << "\n";
  
}

void transmitEnd() {
   Wire.beginTransmission(8);
  Wire.write('e');
  //Wire.write(x);
  Wire.endTransmission();
  std::cout << "\n";
  
}