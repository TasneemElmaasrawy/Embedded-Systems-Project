 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1); //each router has gateway, make devoce connect to router
IPAddress subnet(255,255,255,0); //masking (up to 255 decice can connect to the router)
ESP8266WebServer server(80);  //default port number

void setup() {
  Serial.begin(9600);  //baudrate
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
 
  server.on("/", handle_OnConnect);
  server.on("/led_on", handle_led_on);
  server.on("/led_toggle", handle_led_toggle);
  server.on("/led_off", handle_led_off);
  server.on("/date_time", handle_date_time);
 
  server.onNotFound(handle_NotFound);
  server.begin();

}
void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  String date_t="";
  server.send(200, "text/html", SendHTML(date_t));
}

void handle_led_on() {
  Serial.print('n');
  String date_t="";
  server.send(200, "text/html", SendHTML(date_t));
 }

void handle_led_off() {
  Serial.print('f');
  String date_t="";
  server.send(200, "text/html", SendHTML(date_t));
 }

void handle_led_toggle() {
  while (1)
  Serial.print('t');
  String date_t="";
  server.send(200, "text/html", SendHTML(date_t)); 
  
}

void handle_date_time() {
 Serial.print('d');
  String date_t="";
  while(Serial.available()>0)
  {
    date_t+= char(Serial.read());
   }

  server.send(200, "text/html", SendHTML(date_t));

}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(String date_t){
 
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Project</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color:#ECF0F1}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +=".button {display: block;width: 160px;background-color: #212F3C;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";


  ptr +="<a class=\"button\" href=\"/led_on\">ON</a>\n";
  ptr +="<a class=\"button\" href=\"/led_off\">OFF</a>\n";
  ptr +="<a class=\"button\" href=\"/led_toggle\">Toggle</a>\n";

  String date_t2 = "<p>Date: " +date_t+ "</p>\n";
  ptr +=date_t2;
  ptr += "<a class=\"button\" href=\"/date_time\">Date</a>\n";


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
