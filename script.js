let websock;

function InitWebSocket() {
    const ip = "192.168.1.xxx"; // Remplacez par l'adresse IP de votre ESP32
    websock = new WebSocket(`ws://${ip}:81/`);
    
    websock.onopen = function() {
        document.getElementById('msg').innerText = "WebSocket Connected!";
    };

    websock.onmessage = function(evt) {
        const data = JSON.parse(evt.data);
        document.getElementById('Tempvalue').innerText = data.TEMP;
        document.getElementById('Humvalue').innerText = data.HUM;
        document.getElementById('FANbtn').innerText = `Fan: ${data.fanLEDonoff}`;
        document.getElementById('HUMbtn').innerText = `Humidifier: ${data.humLEDonoff}`;
        document.getElementById('msg').innerText = `Switch Status: ${data.SWonoff}`;
        
        document.getElementById("dynRectangle1").style.width = `${data.TEMP * 2}px`;
        document.getElementById("dynRectangle2").style.width = `${data.HUM * 2}px`;
    };

    websock.onclose = function() {
        document.getElementById('msg').innerText = "WebSocket Disconnected!";
    };
}

function fanONOFF() {
    const btn = document.getElementById('FANbtn');
    const newState = btn.innerText.includes('ON') ? 'fanLEDonoff=OFF' : 'fanLEDonoff=ON';
    websock.send(newState);
}

function humONOFF() {
    const btn = document.getElementById('HUMbtn');
    const newState = btn.innerText.includes('ON') ? 'humLEDonoff=OFF' : 'humLEDonoff=ON';
    websock.send(newState);
}

InitWebSocket();
