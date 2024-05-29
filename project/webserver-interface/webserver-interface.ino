
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Janusz";
const char* password = "11111111";

const int output = 5;

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
    <button class="button" id="btn1" onmousedown="toggleCheckbox('on');" ontouchstart="toggleCheckbox('on');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">LED PUSHBUTTON</button>
    <button class="button" id="btn2" onmousedown="toggleCheckbox('back');" ontouchstart="toggleCheckbox('back');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">LED PUSHBUTTON</button>

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


// Left Part | Forward part
int motorA1 = 16;
int motorA2 = 5;
int motorAPWM = 15;

int motorB1 = 4;
int motorB2 = 0;
int motorBPWM = 2;

// Right Part || Backward part
int motorBB1 = 14;
int motorBB2 = 12;
int motorBBPWM = 13 ;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
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
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorAPWM, OUTPUT);

  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorBPWM, OUTPUT);

  pinMode(motorBB1, OUTPUT);
  pinMode(motorBB2, OUTPUT);
  pinMode(motorBBPWM, OUTPUT);

  // Left Part || Forward part
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);

  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);

  // Right Part || Backward part
  digitalWrite(motorBB1, HIGH);
  digitalWrite(motorBB2, LOW);


  // Actions

  // Serial.println("GO!");
  // digitalWrite(motorA1, HIGH);
  // digitalWrite(motorA2, LOW);
  // analogWrite(motorAPWM, 127);

  // delay(1000);

  // Serial.println("Stop!");

  // analogWrite(motorAPWM, 0);

  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    //digitalWrite(output, HIGH);
    analogWrite(motorAPWM, 127);
    analogWrite(motorBPWM, 127);
    
    analogWrite(motorBBPWM, 127);

    request->send(200, "text/plain", "ok");
  });

  server.on("/back", HTTP_GET, [] (AsyncWebServerRequest *request){
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);

    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);

    digitalWrite(motorBB1, LOW);
    digitalWrite(motorBB2, HIGH);


    analogWrite(motorAPWM, 127);
    analogWrite(motorBPWM, 127);

    analogWrite(motorBBPWM, 127);
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request
  server.on("/off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);

    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);

    digitalWrite(motorBB1, HIGH);
    digitalWrite(motorBB2, LOW);


    //digitalWrite(output, LOW);
    analogWrite(motorAPWM, 0);
    analogWrite(motorBPWM, 0);

    analogWrite(motorBBPWM, 0);
    request->send(200, "text/plain", "ok");
  });
  
  server.onNotFound(notFound);
  server.begin();
}



// the loop function runs over and over again forever
void loop() {
  //digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  // delay(1000);                      // wait for a second
  // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  // delay(1000);                      // wait for a second
}

