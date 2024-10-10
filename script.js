function InitWebSocket() {
    websock = new WebSocket('ws://' + window.location.hostname + ':80/');
    websock.onmessage = function(evt) {
        let JSONobj = JSON.parse(evt.data);
        document.getElementById('Tempvalue').innerHTML = JSONobj.TEMP;
        let temp = parseInt(JSONobj.TEMP * 3);
        document.getElementById("dynRectangle1").style.width = temp + "%";
        document.getElementById('Humvalue').innerHTML = JSONobj.HUM;
        let hum = parseInt(JSONobj.HUM * 3);
        document.getElementById("dynRectangle2").style.width = hum + "%";
        document.getElementById('FANbtn').innerHTML = JSONobj.fanLEDonoff;
        document.getElementById('HUMbtn').innerHTML = JSONobj.humLEDonoff;
        document.getElementById('msg').innerHTML = JSONobj.SWonoff;
        if (JSONobj.SWonoff === 'ON') {
            document.getElementById('msg').style.backgroundColor = "#ffcc00";
        } else {
            document.getElementById('msg').style.backgroundColor = "#ffff33";
        }
    }
}

function fanONOFF() {
    websock.send('fanLEDonoff=' + (document.getElementById('FANbtn').innerHTML === 'ON' ? 'OFF' : 'ON'));
}

function humONOFF() {
    websock.send('humLEDonoff=' + (document.getElementById('HUMbtn').innerHTML === 'ON' ? 'OFF' : 'ON'));
}

window.onload = function() {
    InitWebSocket();
};