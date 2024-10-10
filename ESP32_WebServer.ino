#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <SPIFFS.h>

// Informations WiFi
const char* ssid = "Bbox-22754943";
const char* password = "69EFCE941127F192CAC17ED9311259";

// Créez une instance de serveur web sur le port 80
WebServer server(80);
WebSocketsServer webSocket(81);  // Serveur WebSocket sur le port 81

// Gestion des événements WebSocket
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connection from %s\n", num, ip.toString().c_str());
            webSocket.sendTXT(num, "Hello from ESP32");
            break;
        }
        case WStype_TEXT:
            Serial.printf("[%u] Text: %s\n", num, payload);
            // Répondez avec un écho du message reçu
            webSocket.sendTXT(num, (char*)payload);
            break;
        default:
            break;
    }
}

void setup() {
    Serial.begin(115200);
    
    // Connexion au Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Démarrage du serveur Web et du serveur WebSocket
    server.begin();
    webSocket.begin();
    webSocket.onEvent(onWebSocketEvent);
}

void loop() {
    // Gère les clients WebSocket
    webSocket.loop();
    
    // Gère les requêtes des clients HTTP
    server.handleClient();
}