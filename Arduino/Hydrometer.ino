#include <SoftwareSerial.h>
#define RX 6
#define TX 5

// PINS connected to the HY-SRF05 distance sensor
const unsigned int TRIG_PIN=9;
const unsigned int ECHO_PIN=10;

// Wi-Fi Configuration
String AP = ""; // Wifi Name (SSID)
String PASS = ""; // Wifi Pass
String API = ""; // ThingsSpeak Write API Key
String HOST = "api.thingspeak.com";
String PORT = "80"; // ThingsSpeak PORT
String field = "field1"; // Name of field on ThingsSpeak

// Variables used by sendCommand() function
// Used to communicate with Wi-Fi module ESP8266
int countTimeCommand; // Used by sendCommand function to define the # of retries for an specific command.
boolean executed = false; // Indicates if a command has been executed su
SoftwareSerial esp8266(RX,TX); 

// Used to store HY-SRF05 distance sensor data.
double valSensor;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Set up distance sensor PINs 
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // Configure serial connection.
  Serial.begin(9600);
  esp8266.begin(115200);
  // Connect to Wi-Fi
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

// Get sensor data, print it on screen and sent it to ThingsSpeak. 
void loop() {
  valSensor = getSensorData();
  Serial.println("Sending distance to ThingsSpeak...");
  String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor);
  Serial.println(getData);
  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  esp8266.println(getData);
  delay(1500);
  sendCommand("AT+CIPCLOSE=0",5,"OK");
}

double getSensorData(){
  
  double distance = 0;
  unsigned long duration;

  Serial.println("Waiting until next read...");
  
  // Turn LED ON and OFF to indicate that distance reading has started.
  delay(5000);                       // wait for 5 seconds
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off

  Serial.println("Reading distance...");
  
  // Take 10 reads from ultrasonic HY-SRF05 distance sensor
  // Add up all 10 measurements up and assign them to "distance" variable.
  for (int i=1;i<=10;i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = distance + duration;
    delayMicroseconds(100000);
  }
  
  distance = distance / 10; // Average of the last 10 lectures.
  distance = (distance * 0.0343)/2; // Convert to cm and devide by 2, so only one journey from the pulse is considered... 0.0344 makes reference to the speed of sound.
  
  if (distance==0){
    Serial.println("Warning: no pulse from sensor");
  } else {
    Serial.println("Hydrometer Distance: " + String(distance));
  }
  
  return distance;

}

void sendCommand(String command, int maxTime, char readReply[]) {

  Serial.print("Executing command => ");
  Serial.print(command); // Print Command on screen.

  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command); // Send command to Wi-Fi module.
    if(esp8266.find(readReply)) // Read response from Wi-fi module and check if its equal to expected response.
    {
      executed = true; // Command executed successfully
      break;
    }
    delayMicroseconds(10000);
    countTimeCommand++;
  }
  
  if(executed == true)
  {
    Serial.println(" ====> OK.");
    countTimeCommand = 0;
  }
  
  if(executed == false)
  {
    Serial.println(" ====> FAILED.");
    countTimeCommand = 0;
  }
  Serial.println("==================================");
  executed = false;

 }
