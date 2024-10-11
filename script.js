// Remplacer l'adresse IP par celle de l'ESP32
const webSocket = new WebSocket('ws://192.168.1.193:81/');

let fanOn = false;
let humOn = false;

webSocket.onopen = function() {
    console.log('WebSocket Connected');
};

webSocket.onmessage = function(event) {
    const data = JSON.parse(event.data);
    document.getElementById('temperature').innerText = data.TEMP;
    document.getElementById('humidity').innerText = data.HUM;
    document.getElementById('switchStatus').innerText = data.SWonoff;

    fanOn = data.fanLEDonoff === "ON";
    humOn = data.humLEDonoff === "ON";
};

webSocket.onclose = function() {
    console.log('WebSocket Disconnected');
};

function toggleFan() {
    const message = `fanLEDonoff=${fanOn ? "OFF" : "ON"}`;
    webSocket.send(message);
    fanOn = !fanOn;
}

function toggleHum() {
    const message = `humLEDonoff=${humOn ? "OFF" : "ON"}`;
    webSocket.send(message);
    humOn = !humOn;
}
