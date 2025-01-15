#ifdef ESP32
#include <WiFi.h> // Include WiFi library for ESP32 | Biblioteka WiFi dla ESP8266 
#include <AsyncTCP.h> // Include asynchronous TCP library for ESP32 | Asynchroniczna biblioteka AsyncTCP dla ESP32
#else
#include <ESP8266WiFi.h> // Include WiFi library for ESP8266 | Biblioteka WiFi dla ESP8266 
#include <ESPAsyncTCP.h> // Include asynchronous TCP library for ESP8266 | Asynchroniczna biblioteka AsyncTCP dla ESP8266
#endif
#include <ESPAsyncWebServer.h> // Include library for running a web server | Biblioteka do obsługi serwera WWW

#include <Wire.h> // Include library for I2C communication | Biblioteka do komunikacji I2C

// REPLACE WITH YOUR NETWORK CREDENTIALS | ZASTĄP SWOIMI DANYMI SIECI
const char *ssid = "Janusz"; // Your WiFi network name | Nazwa sieci WiFi
const char *password = "11111111"; // Your WiFi network password | Hasło do sieci WiFi

byte x = 0; // Buffer variable for transmissions | Zmienna buforowa dla komunikacji

// HTML page content stored in flash memory (PROGMEM) to save RAM | Strona HTML zapisana w pamięci flash (PROGMEM), aby oszczędzać RAM
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
<title>ESP Pushbutton Web Server</title>
<meta name=viewport content="width=device-width, initial-scale=1" charset=UTF-8>
<style>// Minified with Toptal CSS Minifier .button,.button-main{font-weight:700;font-size:24px;text-align:center;outline:0;color:#fff;background-color:#2f4468;box-shadow:0 6px #999;cursor:pointer;-webkit-touch-callout:none;-khtml-user-select:none;-webkit-tap-highlight-color:transparent}.button:active,.button:hover{background-color:#1f2e45}body{font-family:Arial;text-align:center;margin:0 auto;padding-top:30px}div.rows,div.wrapper{display:flex;flex-direction:column}.button{border:0;border-radius:5px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.button-main{width:20px;padding:15px 80px;margin:8px;height:160px;border:0;border-radius:5px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.button:active{box-shadow:0 4px #666;transform:translateY(2px)}button.top.edgeright{border-radius:5px 50px 5px 5px}button.top.edgeleft{border-radius:50px 5px 5px}button.bot.edgeleft{border-radius:5px 5px 5px 50px}button.bot.edgeright{border-radius:5px 5px 50px}div.movement{display:flex;justify-content:center}div.row{display:flex;justify-content:space-between;align-items:center}</style>
</head>
<body>
<div class=wrapper>
<div class=controllers>
<div class=movement>
<div class=rows>
<div class="top row">
<span style="padding:10px 80px;margin:8px"></span>
<button class="button-main top mid" id=btn2 onmousedown="toggleCheckbox('forw')" ontouchstart="toggleCheckbox('forw')" onmouseup="toggleCheckbox('off')" ontouchend="toggleCheckbox('off')">F</button>
<span style="padding:10px 80px;margin:8px"></span>
</div>
<div class="mid row">
<button class="button-main middle left" id=btn1 onmousedown="toggleCheckbox('left')" ontouchstart="toggleCheckbox('left')" onmouseup="toggleCheckbox('off')" ontouchend="toggleCheckbox('off')">L</button>
<div class="middle mid" style=display:flex;flex-direction:column;justify-content:space-between;height:160px;margin:8px;width:160px;border-radius:5px>
<button class="button middle left" style=background-color:green;height:50%;margin-bottom:15px id=header-btn2 onclick="toggleCheckbox('header-f')" ontouchstart="releaseButton('header-f')">HF</button>
<button class="button middle left" style=background-color:green;height:50% id=header-btn2 onclick="toggleCheckbox('header-b')" ontouchstart="releaseButton('header-b')">HB</button>
</div>
<button class="button-main middle right" id=btn3 onmousedown="toggleCheckbox('right')" ontouchstart="toggleCheckbox('right')" onmouseup="toggleCheckbox('off')" ontouchend="toggleCheckbox('off')">R</button>
</div>
<div class="bot row">
<span style="padding:10px 80px;margin:8px"></span>
<button class="button-main bot mid" id=btn2 onmousedown="toggleCheckbox('back')" ontouchstart="toggleCheckbox('back')" onmouseup="toggleCheckbox('off')" ontouchend="toggleCheckbox('off')">B</button>
<span style="padding:10px 80px;margin:8px"></span>
</div>
</div>
</div>
</div>
</div>
<h1>ESP Pushbutton Web Server</h1>
<script>function toggleCheckbox(x){var xhr=new XMLHttpRequest();xhr.open("GET","/"+x,true);xhr.send();};</script>
</body>
</html> 
)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  // Handle requests to unknown URLs and respond with 404 Not Found | Obsługa żądań do nieznanych adresów URL i odpowiedź "404 Not Found"
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80); // Create a web server object on port 80 | Tworzenie obiektu serwera WWW na porcie 80

void transmit(char direction) {
  // Send a single character via I2C to another device | Wysyłanie pojedynczego znaku za pomocą I2C do innego urządzenia
  Wire.beginTransmission(8);
  Wire.write(direction);
  Wire.endTransmission();
}

void transmitEnd() {
  // Send an end signal to stop any ongoing commands ([e] is 101 in ASCII) | Wysłanie sygnału zakończenia, aby zatrzymać trwające polecenia ([e] to 101 w ASCII)
  transmit('e');
}

// The setup function runs once when the microcontroller is powered on or reset | Funkcja setup uruchamia się raz po włączeniu mikrokontrolera lub resecie
void setup() {
  Wire.begin(4, 5); // Join i2c bus (address optional for master) 4, 5 are default | Inicjalizacja magistrali I2C na pinach 4 (SDA) i 5 (SCL)
  Serial.begin(115200); // Start serial communication for output | Rozpoczęcie komunikacji szeregowej do debugowania
  delay(400); // Short delay for setup | Krótkie opóźnienie na potrzeby ustawień
  Serial.println("Master Ready");

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password); // Create a WiFi access point | Utworzenie WiFi hotspota

  IPAddress IP = WiFi.softAPIP(); // Get the IP address of the access point | Pobranie adresu IP hotspota
  Serial.print("AP IP address: ");
  Serial.println(IP);

  Serial.println(WiFi.localIP()); // Print ESP8266 local IP address - 192.168.4.1 | Wyświetlenie lokalnego adresu IP - 192.168.4.1

  // Turn on the built-in LED initially after the setup | Włączenie wbudowanej diody po konfiguracji
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  // Define web server routes | Definicja tras serwera WWW
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html); // Serve the main HTML page
  });

  server.on("/header-f", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('q'); // Command to move header forward ([q] is 113 in ASCII) | Polecenie ruchu headera do przodu ([q] to 113 w ASCII)
    request->send(200, "text/plain", "ok");
  });

  server.on("/header-b", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('j'); // Command to move header backward ([j] is 106 in ASCII) | Polecenie ruchu headera do tyłu ([j] to 106 w ASCII)
    request->send(200, "text/plain", "ok");
  });

  server.on("/forw", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('f'); // Command to move forward ([f] is 102 in ASCII) | Polecenie jazdy do przodu ([f] to 102 w ASCII)
    request->send(200, "text/plain", "ok");
  });

  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('l'); // Command to move left ([l] is 108 in ASCII) | Polecenie jazdy w lewo ([l] to 108 w ASCII)
    request->send(200, "text/plain", "ok");
  });

  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('r'); // Command to move right ([r] is 114 in ASCII) | Polecenie jazdy w prawo ([r] to 114 w ASCII)
    request->send(200, "text/plain", "ok");
  });

  server.on("/back", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmit('b'); // Command to move backward ([b] is 98 in ASCII) | Polecenie jazdy do tyłu ([b] to 98 w ASCII)
    request->send(200, "text/plain", "ok");
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    transmitEnd(); // Command to stop movement | Polecenie zatrzymania ruchu
    request->send(200, "text/plain", "ok");
  });

  server.onNotFound(notFound); // Handle unknown URL requests | Obsługa requestów do nieznanych routów URL
  server.begin(); // Start the web server | Uruchomienie serwera WWW
}

void loop() {} // The loop function is empty because the web server operates asynchronously | Funkcja loop jest pusta, ponieważ serwer WWW działa asynchronicznie