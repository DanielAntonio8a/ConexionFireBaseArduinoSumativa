//Inclusión de librerias
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>


const char* WIFI_SSID = "Casa Duran 5G 2do Piso";
const char* WIFI_PASSWORD = "fmla_DuranOchoa";

const int PinLuz = A0;

const char* API_KEY = "AIzaSyDpJC93zdDaUl-zCaW9E7glbvGmEarOMLo";
const char* DATABASE_URL = "https://bddlive-7e449-default-rtdb.firebaseio.com/";
const char* USER_EMAIL = "danielanto@uets.edu.ec";
const char* USER_PASSWORD = "danielanto@uets.edu.ec";

//Objetos necesarios para la comunicación con el Firebase
FirebaseData fbdo; 
FirebaseAuth auth; 
FirebaseConfig config; 
int luz = 0;
int luzMap=0;

void setup_WIFI() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

//Configura Firebase con las credenciales y la URL de la base de datos
void setupFirebase() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  config.signer.tokens.legacy_token = "<database secret>";
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
}

void setup() {
  Serial.begin(115200); 
    setup_WIFI();
    setupFirebase(); 
    pinMode(PinLuz, INPUT);
}

void loop() {
  int luz = analogRead(PinLuz); 
  luzMap = map(luz, 0, 3800, 0, 100);

  if (Firebase.ready()) { //Verifica si la conexión a Firebase está lista 
      Firebase.RTDB.setInt(&fbdo, "/sensor/luz", luzMap); //Envía el valor a la base de datos
      Serial.println(luzMap); //Imprime el valor
      delay(2000);
  }
}