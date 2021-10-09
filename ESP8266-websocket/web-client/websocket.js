var socket = new WebSocket("ws://192.168.100.69:4000");


socket.onmessage = (event) => {
    console.log(event.data);
}

let value = 255;
function sliderUpdater(){
    const slider = document.getElementById('brightnessSlider');
    if(value != 255-slider.value){
        socket.send(`${255-slider.value}`);
        value = 255-slider.value;
    }
}

setInterval(sliderUpdater, 100);