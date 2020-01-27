#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <pass.h>

//using namespace std;
// Fill in your WiFi router SSID and password in pass.h

MDNSResponder mdns;

ESP8266WebServer server(80);

const int LED_PIN = 2;

const char * htmlMessage = " <!DOCTYPE html> "
"<html> "
"<body> "
"<p>This is a HTML only intro page. Please select a button bellow.</p>"
"<a href=\"/javascript\">Javascript code</a>"
"</br>"
"<a href=\"/cssButton\">CSS code</a>"
"</body> "
"</html> ";

const char * javascriptCode = " <!DOCTYPE html> "
"<html> "
"<body> "
"<p>Click the button to get a message from the ESP8266:</p> "
"<button onclick=\"buttonFunction()\">Message</button> "
"<script>"
"function buttonFunction() { "
" alert(\"Hello from the ESP8266!\"); "
" window.location.href= '/';"
"} "
"</script>"
"</body> "
"</html> ";

const char * cssButton ="<!DOCTYPE html>"
"<html>"
"<head>"
"<style>"
".button {"
"background-color: #990033;"
"border: none;"
"color: white;"
"padding: 7px 15px;"
"text-align: center;"
"cursor: pointer;"
"}"
"</style>"
"</head>"
"<body>"
"<input type=\"button\" class=\"button\" value=\"Input Button\">"
"</body>"
"</html>";

void handleRoot();
void handleJava();
void handleCss();
void handleMain();
void test(bool apa);
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
String processor(const String& var);

void setup(void)
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, 1);

    Serial.begin(115200);
    delay(10);
    Serial.print("\n");
    Serial.print("Connecting.");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to: ");
    Serial.println(ssid);
    Serial.print("On IP: ");
    Serial.println(WiFi.localIP());

    if (mdns.begin("nodemcuserver")){
        Serial.println("MDNS responder started");
    }
    else{
        Serial.println("Error setting up MDSN");
    }


    
    /*server.on("/", handleRoot);
    server.on("/javascript", handleJava);
    server.on("/cssButton", handleCss);
    server.on("/server", handleMain);

    server.begin();*/

    SPIFFS.begin();

    /*  // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false, processor);
    });
  
  // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });*/

    server.on("/on", HTTP_POST,  [](){
        digitalWrite(LED_PIN, 0);
        server.send(200, "text/plain", "ON");
    });

    server.on("/off", HTTP_POST,  [](){
        digitalWrite(LED_PIN, 1);
        //test(true);
        server.send(200, "text/plain", "OFF");
    });

    server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
    });

    server.begin();


}

void loop(void){
    server.handleClient();
}

void handleRoot(void){
    server.send(200, "text/html", htmlMessage);
}

void handleJava(void){
    server.send(200, "text/html", javascriptCode);
}

void handleCss(void){
    server.send(200, "text/html", cssButton);
}

void handleMain(void){
    server.send(200, "text/html", "../htmljs/test.html");
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  Serial.println(contentType);
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

void test(bool apa){
  int i = 1;
}