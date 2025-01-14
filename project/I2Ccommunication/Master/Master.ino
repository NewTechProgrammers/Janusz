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
const char *ssid = "Janusz";
const char *password = "11111111";

const int output = 5;

byte x = 0;

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="UTF-8">
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin: 0px auto;
            padding-top: 30px;
        }

        div.wrapper {
            display: flex;
            flex-direction: column;
        }

        div.rows {
            display: flex;
            flex-direction: column;
        }

        .button {
            font-weight: bold;
            width: 20px;
            padding: 15px 80px;
            margin: 8px;
            height: 160px;
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
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
        }

        .button:hover {
            background-color: #1f2e45
        }

        .button:active {
            background-color: #1f2e45;
            box-shadow: 0 4px #666;
            transform: translateY(2px);
        }

        button.top.edgeright {
            border-radius: 5px 50px 5px 5px;
        }

        button.top.edgeleft {
            border-radius: 50px 5px 5px 5px;
        }

        button.bot.edgeleft {
            border-radius: 5px 5px 5px 50px;
        }

        button.bot.edgeright {
            border-radius: 5px 5px 50px 5px;
        }

        div.movement {
            display: flex;

            justify-content: center;
        }

        div.row {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
    </style>
</head>

<body>
    <div class="wrapper">
        <div class="controllers">
            <div class="movement">
                <div class="rows">
                    <div class="top row">
                        <span style="padding: 10px 80px; margin: 8px;"></span>
                        <button class="button-main top mid" id="btn2" onmousedown="toggleCheckbox('forw');"
                            ontouchstart="toggleCheckbox('forw');" onmouseup="toggleCheckbox('off');"
                            ontouchend="toggleCheckbox('off');">F</button>
                        <span style="padding: 10px 80px; margin: 8px;"></span>
                    </div>
                    <div class="mid row">
                        <button class="button-main middle left" id="btn1" onmousedown="toggleCheckbox('left');"
                            ontouchstart="toggleCheckbox('left');" onmouseup="toggleCheckbox('off');"
                            ontouchend="toggleCheckbox('off');">L</button>
                        <div class= "middle mid" style="display: flex; flex-direction: column; justify-content: space-between; height: 160px; margin: 8px; width: 160px; border-radius: 5px;">
                          <button class="button middle left" style="background-color: green; height: 50%; margin-bottom: 15px;" id="header-btn2" onclick="toggleCheckbox('header-f')" ontouchstart="releaseButton('header-f')">HF</button>
                          <button class="button middle left" style="background-color: green; height: 50%;" id="header-btn2" onclick="toggleCheckbox('header-b')" ontouchstart="releaseButton('header-b')">HB</button>
                                                  
                        </div>
                        
                        <button class="button-main middle right" id="btn3" onmousedown="toggleCheckbox('right');"
                            ontouchstart="toggleCheckbox('right');" onmouseup="toggleCheckbox('off');"
                            ontouchend="toggleCheckbox('off');">R</button>
                    </div>
                    <div class="bot row">
                        <span style="padding: 10px 80px; margin: 8px;"></span>
                        <button class="button-main bot mid" id="btn2" onmousedown="toggleCheckbox('back');"
                            ontouchstart="toggleCheckbox('back');" onmouseup="toggleCheckbox('off');"
                            ontouchend="toggleCheckbox('off');">B</button>
                        <span style="padding: 10px 80px; margin: 8px;"></span>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <h1>ESP Pushbutton Web Server</h1>
    <script>
        function toggleCheckbox(x) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/" + x, true);
            xhr.send();
        }
    </script>
</body>

</html>
)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);


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
// the setup function runs once when you press reset or power the board
void setup() {

  Wire.begin(4, 5);    // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  delay(100);
  Serial.println("Master Ready");

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());  // 192.168.4.1

  // initialize digital pin LED_BUILTIN as an output.


  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request

  server.on("/header-f", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('q');
    request->send(200, "text/plain", "ok");
  });

  server.on("/header-b", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('j');
    request->send(200, "text/plain", "ok");
  });

  server.on("/forw", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('f');
    request->send(200, "text/plain", "ok");
  });

  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('l');
    request->send(200, "text/plain", "ok");
  });

  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('r');
    request->send(200, "text/plain", "ok");
  });

  server.on("/back", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('b');
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmitEnd();
    request->send(200, "text/plain", "ok");
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {}