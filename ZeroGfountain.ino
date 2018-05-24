//************************************* INCLUDES *************************************************
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FastLED.h>
#include <FS.h>
#include <time.h>
#include "Ticker.h"
#include <WebSocketsServer.h>
#include "index.h"
extern "C" {
  #include "user_interface.h"
}

//************************************* VARIABLE DECLARATION **************************************
bool debug = true;

const char* htmlfile = "/index.html";

// FastLED definitions (WS2812b)
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define DATA_PIN 4 // Pin for ws2812
#define NUM_LEDS 40
CRGB leds[NUM_LEDS];

//Timer variables
os_timer_t Timer1;         
int Counter = 0;           // Argument for the Callback-Function
bool TickOccured = false;  // Flag, which is set in the Callback-Funktion

// WiFi parameters and variables
const char *ssid = "Zero_G__Access_Point"; // The name of the Wi-Fi network that will be created
const char *password = "123456789";   // The password required to connect to it, leave blank for
const char* mdnsName = "zerog";
// Create an instance of the server
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WiFiMulti wifiMulti;       // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

// Variables used to send as JSON via Websocket
String modeActive = "modeInactive"; //options: sensorActive; sensorInactive
int Mode = 2; // 0 := Off, 1 := Light On; 2 := Zero G, 3 := RGB, 4 := Music, 5 := No Light
int currentSensorConfig1 = -1;
float currentHz = 0;
String uptime;
String ipadress ;

#define MAX_STRING_LENGTH 10

#define PUMP_MAX_COUNT 255

#define POSITIVE 1
#define NEGATIVE -1

#define ON true
#define OFF false
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Global variables
unsigned int base_loop_count = 400; // 20 = 200hz | 50 = 166hz| 400 = 50hz | 500 = 40hz | 1000 = 20 hz

unsigned int phase_offset = 10;

float red_duty = 8.0;
float green_duty = 8.0;
float blue_duty = 8.0;
float base_duty = 8.0;

unsigned int red_duty_value = (float)base_loop_count * red_duty / 100.0;//switching value to achieve the desired duty cycle
unsigned int green_duty_value = (float)base_loop_count * green_duty / 100.0;//switching value to achieve the desired duty cycle
unsigned int blue_duty_value = (float)base_loop_count * blue_duty / 100.0;//switching value to achieve the desired duty cycle

unsigned int  pump_duty_value = 0;

unsigned int red_max_count = base_loop_count;
unsigned int green_max_count = base_loop_count;
unsigned int blue_max_count = base_loop_count;

unsigned int base_count = 0;
unsigned int red_count = 0;
unsigned int green_count = 0;
unsigned int blue_count = 0;
unsigned int pump_count = 0;

int red_sin_direction = POSITIVE;
int green_sin_direction = POSITIVE;
int blue_sin_direction = POSITIVE;

String stringText = "";

short sin_amplitude = 180;

bool flag_pump_off = true;
bool flag_red_sin = false;
bool flag_green_sin = false;
bool flag_blue_sin = false;

short mode_status = 0;//just used for a status reprot
float base_loop_count_temp = base_loop_count * 0.00277777777777777777777;

float sin_count = 0;
float sin_count_increment = 0.0174533;

bool flag_ms_start = true;
bool flag_ms_stop = false;
unsigned long ms;

//************************************* SETUP ***************************************************
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  
  ESP.eraseConfig();
  
  // Start Serial
  Serial.begin(9600);
  
  //Initialize File System
  SPIFFS.begin();
  Serial.println("File System Initialized");
  Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
    }

  //Initialize Communication Modules
  startWiFi();
  startWebSocket();            // Start a WebSocket server
  if(WiFi.softAPgetStationNum() != 0){ startMDNS(); }  // Start the mDNS responder if AP mode
  startServer();               // Start a HTTP server with a file read handler and an upload handler
  getTime();

  //Initialize WS2812
  FastLED.addLeds<WS2812B, DATA_PIN, BRG>(leds, NUM_LEDS);

  os_timer_setfn(&Timer1, timerCallback, &Counter);
  os_timer_arm(&Timer1, 1000, true);
}
//************************************* LOOP ***************************************************
int R = 0;
int G = 0;
int B = 0;
int Rtemp = 0;
int Gtemp = 0;
int Btemp = 0;
void loop() {
  server.handleClient();          //Handle client requests
  webSocket.loop();               // constantly check for websocket events  

    if(red_count < red_duty_value){//Switch LEDs on/off for the set duty/frequency
      R = 255;
    }
    else{
      R = 0;
    }
    
    if(green_count < green_duty_value){
      G = 255;
    }
    else {
      G = 0;
    }
    
    if(blue_count < blue_duty_value){
      B = 255;
    }
    else {
      B = 0;
    }
    
    if(Rtemp != R || Gtemp != G || Btemp != B){ //Only execute on new RGB values
      fill_solid( leds, NUM_LEDS, CRGB(R,G,B));
      FastLED.show();
      Rtemp = R;
      Gtemp = G;
      Btemp = B;
    }
    if(pump_count == pump_duty_value){
        //PORTD &= ~_BV(PUMP_PIN);  //write port B5 LOW
    }
    else if(pump_count >= PUMP_MAX_COUNT && !flag_pump_off){
        //PORTD |= _BV(PUMP_PIN);  //write port B5 HIGH
        pump_count = 0;
    }

    base_count++;//Increment counts  
    red_count++;
    green_count++;
    blue_count++;
    pump_count++;

    if(base_count >= base_loop_count){
        base_count = 0;
        if(flag_ms_stop && debug){
            ms = millis() - ms;
            flag_ms_stop = false;
            flag_ms_start = true;
            currentHz = 1/((float)ms/1000.0);
            Serial.print("Estimated frequency: ");
            Serial.print(currentHz, 3);
            Serial.print("Hz\n");
        }
        if(flag_ms_start && debug){
            ms = millis();
            flag_ms_stop = true;
        }
        if(flag_red_sin){
            if(red_sin_direction == POSITIVE){
                red_count = base_loop_count_temp * (sin_amplitude * cos(sin_count) + sin_amplitude);
            }
            else{
                red_count = base_loop_count_temp * (360 - (sin_amplitude * cos(sin_count) + sin_amplitude));
            }
        }
        if(flag_green_sin){
            if(green_sin_direction == POSITIVE){
                green_count = base_loop_count_temp * (sin_amplitude * cos(sin_count) + sin_amplitude);
            }
            else{
                green_count = base_loop_count_temp * (360 - (sin_amplitude * cos(sin_count) + sin_amplitude));
            }
            
        }
        if(flag_blue_sin){
            if(blue_sin_direction == POSITIVE){
                blue_count = base_loop_count_temp * (sin_amplitude * cos(sin_count) + sin_amplitude);
            }
            else{
                blue_count = base_loop_count_temp * (360 - (sin_amplitude * cos(sin_count) + sin_amplitude));
            }
        }
        
        sin_count += sin_count_increment;//0.0174533 rads = 1 deg
        if(sin_count >= 6.283188){//6.283188 radians in 360 degrees
        }
    }

    if(red_count >= red_max_count){
        red_count = 0;
    }
    if(green_count >= green_max_count){
        green_count = 0;
    }
    if(blue_count >= blue_max_count){
        blue_count = 0;
    } 
}
//************************************* Timer Callbackk (1HZ) **********************************
void timerCallback(void *pArg) { 
    TickOccured = true;
    webSocket.sendTXT(0, "{\"uptime\": \"" + uptime + "\", \"ip\": \"" + ipadress + " \", " + "\"data\": {\"Mode\": " + Mode + ", \"modevalue\": "+ currentHz + ", \"unit\": \"Hz\", \"modeActive\": \""+modeActive+"\",\"modeConfig1\": \""+(-1) +"\" }}");
    /*Serial.println();
    Serial.println("Tick Occurred");
    Serial.print("Millis: "); Serial.println(millis());
    Serial.print("Counter: "); Serial.println(Counter);*/
    *((int *) pArg) += 1;
} 
//**************************************INITIALIZATION FUNCTIONS (SETUP)************************
//===============================================================
// Wifi Initialization
// - Connects to defined APÂ´s or creates on AP if no AP available
//===============================================================
void startWiFi() { // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);  
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started\r\n");

  wifiMulti.addAP("DontTouchThis", "PASSWORD");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("teco5g", "password");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting");
  while (wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) {  // Wait for the Wi-Fi to connect
    delay(250);
    Serial.print('.');
  }
  Serial.println("\r\n");
  if(WiFi.softAPgetStationNum() == 0) {      // If the ESP is connected to an AP
    WiFi.softAPdisconnect();
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());             // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.print(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
    WiFi.mode(WIFI_STA);
  } else {
    // If a station is connected to the ESP SoftAP
    Serial.println("Station connected to ESP8266 AP");
    Serial.println(WiFi.softAPIP());
    WiFi.disconnect(); 
  }
  Serial.println("\r\n");
  (WiFi.softAPgetStationNum() == 0) ? ipadress = WiFi.localIP().toString() : ipadress = WiFi.softAPIP().toString();
}
//===============================================================
// Initializes MDNS service
//===============================================================
void startMDNS() { // Start the mDNS responder
  MDNS.begin(mdnsName);  // start the multicast domain name server
  Serial.print("mDNS responder started: http://");
  Serial.print(mdnsName);
  Serial.println(".local");
}
//===============================================================
// Server Initialization to handle Client Requests
// - serves main page (index.html) from index.h file (include) 
// - serves required javascripts and stylesheets from SPIFFS
//===============================================================
void startServer() { // Start a HTTP server with a file read handler and an upload handler
  server.on ( "/", []() { server.send ( 200, "text/html", SITE_index );  } );                                          // and check if the file exists
  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  server.begin(); 
  // start the HTTP server
  Serial.println("HTTP server started.");
}
//===============================================================
// Initializes Websocket Server
//===============================================================
void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

//************************************* Web Events ***************************************************
//===============================================================
// Handles Requests from the client except index.html
// - Normal usage is to serve .js and .css from SPIFFS
//===============================================================
void handleWebRequests(){
  Serial.println(server.uri());
  if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}
//===============================================================
// Handles Websocket requests
//===============================================================
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t msglength) { // When a WebSocket message is received
  if(msglength == 0) {
    //Arduino Crashes if lenght == 0
    return;
  }
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "{\"uptime\": \"" + uptime + "\", \"ip\": \"" + ipadress + " \", " + "\"data\": {\"Mode\": " + Mode + ", \"modevalue\": "+ 0 + ", \"unit\": \"??\", \"modeActive\": \""+modeActive+"\",\"modeConfig1\": \""+(-1) +"\" }}"); 
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      Serial.printf("[%u] WebSocketMessaget: %s\n", num, payload);
      parseWsCommands(payload);
      break;
  }
}

//************************************* WS commands Routing ***************************************************
//===============================================================
// Parse Websocket commands
//===============================================================
void parseWsCommands(uint8_t *payload){
  String command = String((char *)payload);
  if( command == "ON" ) {fountainOn(); }
  if( command == "OFF" ) {fountainOff(); }
}

//************************************* fountainOn Functions ***************************************************
//===============================================================
// Turns fountainOn ON
//===============================================================
void fountainOn() {
  //digitalWrite(ledPIN, HIGH); 
  //Serial.println("Socket ON");
}
//===============================================================
// Turns Socket OFF
//===============================================================
void fountainOff() {
  //digitalWrite(ledPIN, LOW); 
  //Serial.println("Socket OFF");
}
//**********************************HELPER FUNCTIONS ******************************
//===============================================================
// Gets current Time from the Internet 
//===============================================================
void getTime(){
  configTime(2 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  int count = 0;
  while (time(nullptr) < 954367200 && count < 5) { // 954367200 = year 2000 in unix time
    Serial.print(":");
    delay(1000);
    count++;
  }
  time_t now = time(nullptr);
  uptime = ctime(&now);
  uptime.remove(uptime.length()-1);
  Serial.println("Start time: " + uptime);
}
//===============================================================
// Split data by seperator and return the indexed data value
//===============================================================
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
//===============================================================
// Reads requested file from SPIFFS and streams it to the client
//===============================================================
bool loadFromSpiffs(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}

