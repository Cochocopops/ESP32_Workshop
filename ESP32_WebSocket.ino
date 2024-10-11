#include <WiFi.h>
#include <WebSocketsServer.h>
#include <DHT.h>

// Configuration des pins
#define DHTPIN 27          // Pin connecté au Data du DHT22
#define DHTTYPE DHT22      // Type de capteur DHT22
#define fanLED 2           // Pin connecté à la DEL rouge (ventilateur)
#define humLED 4           // Pin connecté à la DEL bleue (humidificateur)

// Informations de connexion Wi-Fi
const char* ssid = "Bbox-22754943";
const char* password = "69EFCE941127F192CAC17ED9311259";

DHT dht(DHTPIN, DHTTYPE);
WebSocketsServer webSocket = WebSocketsServer(81);

boolean fanLEDonoff = false; 
boolean humLEDonoff = false;
String JSONtxt;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Configuration des pins
  pinMode(fanLED, OUTPUT);
  pinMode(humLED, OUTPUT);

  // Connexion au Wi-Fi
  Serial.print("Connexion au réseau Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Démarrer le serveur WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Serveur WebSocket démarré.");
}

void loop() {
  webSocket.loop();

  // Lecture et envoi des données toutes les secondes
  static unsigned long l = 0;
  unsigned long t = millis();
  if ((t - l) > 1000) {
    l = t;

    // Contrôle des LEDs en fonction de leur état
    digitalWrite(fanLED, fanLEDonoff ? HIGH : LOW);
    digitalWrite(humLED, humLEDonoff ? HIGH : LOW);

    // Lecture de la température et de l'humidité
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    String TEMPvalString = isnan(temp) ? "Erreur" : String(temp, 1);
    String HUMvalString = isnan(hum) ? "Erreur" : String(hum, 1);

    // Création du message JSON à envoyer
    JSONtxt = "{\"TEMP\":\"" + TEMPvalString + "\"," +
               "\"HUM\":\"" + HUMvalString + "\"," +
               "\"fanLEDonoff\":\"" + (fanLEDonoff ? "ON" : "OFF") + "\"," +
               "\"humLEDonoff\":\"" + (humLEDonoff ? "ON" : "OFF") + "\"}";

    // Envoi des données via WebSocket
    webSocket.broadcastTXT(JSONtxt);
  }
}

// Fonction pour gérer les événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String payloadString = (const char *)payload;

    // Contrôle de la LED rouge (ventilateur)
    if (payloadString.indexOf("fanLEDonoff=ON") >= 0) {
      fanLEDonoff = true;
    } else if (payloadString.indexOf("fanLEDonoff=OFF") >= 0) {
      fanLEDonoff = false;
    }

    // Contrôle de la LED bleue (humidificateur)
    if (payloadString.indexOf("humLEDonoff=ON") >= 0) {
      humLEDonoff = true;
    } else if (payloadString.indexOf("humLEDonoff=OFF") >= 0) {
      humLEDonoff = false;
    }
  }
}
