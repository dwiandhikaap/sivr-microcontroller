const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <script>
        const clamp = (num, min, max) => Math.min(Math.max(num, min), max);

        function addNotif(message, duration, type){
            let color = '';

            switch(type){
                case 'error':{
                    color = 'rgba(208, 26, 66, 0.75)'; // Red
                    break;
                }
                case 'success':{
                    color = 'rgba(0, 162, 51, 0.75)'; // Green
                    break;
                }
                default :{
                    color = 'rgba(87, 98, 255, 0.75)'; // Red
                    break;
                }
            }

            var notif = document.createElement("p");
            notif.className = "notif-item";
            var notifText = document.createTextNode(message);
            var firstElement = document.getElementById("notif-container").firstChild;
            notif.style.setProperty("--duration", `${duration}ms`);
            notif.style.setProperty("--bgColor", `${color}`);
            
            notif.appendChild(notifText);

            document.getElementById("notif-container").insertBefore(notif, firstElement);
            setTimeout(() => {
                document.getElementById("notif-container").removeChild(notif)
            }, duration);
        }

        function strengthToHue(strength){

            return clamp(strength+100, 0, 80);
        }

        function compare( a, b ) {
            const aNum = a.strength;
            const bNum = b.strength;
            if ( aNum < bNum ){
                return 1;
            }
            if ( aNum > bNum ){
                return -1;
            }
            return 0;
        }

        function setSSID(ssidString){
            document.getElementById("current-ssid").textContent = ssidString;
        }

        async function getCurrentSSID(){
            const response = await fetch("/getSSID", {
                method: "GET",
                headers: {
                    "Content-Type": "text/plain"
                },
            })
            
            if(response.status != 200){
                return;
            }
            
            document.getElementById("current-ssid").textContent = await response.text();
        }

        async function searchWifi(){
            const generateMarkup = (wifi) => {                
                const {ssid, strength, secure} = wifi;

                return `
                <div class="item-strength">
                    <svg xmlns=\"http://www.w3.org/2000/svg\" width="24" height="24" viewBox="0 0 24 24" style="fill: hsl(${strengthToHue(strength)}, 100%, 50%);"><path d="M12 6c3.537 0 6.837 1.353 9.293 3.809l1.414-1.414C19.874 5.561 16.071 4 12 4c-4.071.001-7.874 1.561-10.707 4.395l1.414 1.414C5.163 7.353 8.463 6 12 6zm5.671 8.307c-3.074-3.074-8.268-3.074-11.342 0l1.414 1.414c2.307-2.307 6.207-2.307 8.514 0l1.414-1.414z"></path><path d="M20.437 11.293c-4.572-4.574-12.301-4.574-16.873 0l1.414 1.414c3.807-3.807 10.238-3.807 14.045 0l1.414-1.414z"></path><circle cx="12" cy="18" r="2"></circle></svg>
                    ${strength} dB
                </div>
                <h3>${ssid}${secure ? " 🔐" : ""}</h3>
                <button class="item-button" onclick="setSSID('${ssid}')">Choose</button>
            `}
            
            let wifiData = await fetch("/searchWifi", {
                method: "GET",
                headers: {
                    "Content-Type": "application/json"
                }
            })
            
            if(wifiData.status != 200){
                addNotif("Failed to fetch data!", 3000, "error");
                return;
            }

            wifiData = await wifiData.json();
            wifiData.sort(compare);

            const listContainer = document.getElementById("list-container");
            for(wifi of wifiData){
                const newItem = document.createElement("div");
                newItem.setAttribute("class", "list-item");
                newItem.innerHTML = generateMarkup(wifi);
                listContainer.appendChild(newItem);
            }
        }

        async function saveConfig(){
            const ssid = document.getElementById("current-ssid").textContent
            const password = document.getElementById("current-password").value

            const response = await fetch("/saveConfig", {
                method: "POST",
                headers: {
                    "Content-Type": "application/x-www-form-urlencoded"
                },
                body: new URLSearchParams({
                    ssid: ssid,
                    password: password
                })
            })

            if(response.status != 200){
                addNotif("Failed to save config!", 3000, "error");
                return;
            }

            addNotif("Config is successfully saved!", 3000, "success");
        }
    </script>
    <style>
        /* NOTIF CSS START */

        @keyframes notifAdd{
            0% {transform: translateY(0,0); opacity:0}
            20% {transform: translateY(0px,20px); opacity:1}
            80% {transform: translateY(0px,0px); opacity:1;transform: scale(1, 1);}
            100% {transform: translateY(0px,20px); opacity:0; transform: scaleY(0);}
        }

        #notif-container{
            width: 100%;
            margin-top: 2rem;
            max-height: 12rem;
            overflow-y: hidden;
            position: fixed;
            z-index: 9999;
        }

        .notif-item{
            margin: 1rem auto;
            padding: 4px 1rem;
            width: 40%;

            color: white;
            background-color: var(--bgColor);
            font-weight: bold;
            text-align: center;

            transition: all 1s linear;
            animation: notifAdd var(--duration) linear;
        }

        /* NOTIF CSS END */
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            color: #fff;
            background-color: #101010;
            padding: 2rem;
            
            font-family: Roboto, sans-serif;
        }

        h1 {
            text-align: center;
        }
        
        #current-config{
            display: flex;
            flex-direction: column;
            margin-top: 2rem;
            margin-bottom: 2rem;
            padding: 1rem;

            background-color: hsl(0, 0%, 8%);
            border: 1px dashed white;
            border-radius: 8px;

            max-width: 75vw;
        }

        #current-ssid {
            display: inline-block;
            font-weight: bold;
            text-overflow: ellipsis;
            overflow: hidden;
            white-space: nowrap;
            max-width: 17ch;
            vertical-align:middle;
        }

        #current-password{
            color: white;
            background-color: hsl(0, 0%, 15%);
            border-style: solid;
            border: 1px solid hsl(0, 0%, 100%);
            padding: 4px;
            max-width: 14ch;

            margin-top: 8px;
            margin-bottom: 16px;
        }

        #current-config-apply{
            color: white;
            background-color: hsl(300, 100%, 50%);
            font-weight: bolder;
            width: 4rem;
            height: 1.5rem;
            border: 0;
            align-self: center;
        }

        #current-config-apply:hover{
            background-color: hsl(300, 100%, 75%);
        }

        #current-config-apply:active{
            background-color: hsl(300, 100%, 40%);
        }

        #list-container {
            padding: 0 1rem;
            max-height: 18.5rem;
            overflow-y: scroll;
            overflow-x: hidden;
        }

        #list-container::-webkit-scrollbar{
            background-color: transparent;
            width: 8px;
        }
        
        #list-container::-webkit-scrollbar-thumb{
            border-radius: 4px;
            background-color: hsl(0, 0%, 25%);
        }

        #list-container > *:last-child{
            margin-bottom: 0;
        }


        .list-item{
            display: grid;
            grid-template-columns: 56px auto 96px;
            width: min(75vw, 32rem);
            align-items: center;
            background-color: hsl(0, 0%, 10%);
            margin-bottom: 1rem;
        }

        .list-item > *:first-child, 
        .list-item > *:last-child{
            justify-self: center;
        }

        .item-strength {
            display: flex;
            flex-direction: column;
            align-items: center;
            font-size: 8px;
        }

        .item-button {
            background-color: hsl(120, 100%, 30%);
            color: white;
            border: 0;
            border-radius: 4px;
            width: 4rem;
            height: 2rem;
            font-weight: bold;
        }

        .item-button:hover {
            background-color: hsl(120, 100%, 33%);
        }

        .item-button:active {
            background-color: hsl(120, 100%, 27%);
        }
    </style>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>sivr-esp8266</title>
</head>
<body onload="searchWifi();getCurrentSSID()">
    <div id="notif-container"></div>

    <h1>Setup Mode</h1>
    <div id="current-config">
        <span>
            <span>SSID :</span>
            <span id="current-ssid"></span>
        </span>
        <span>
            <label for="current-password">Password : </label>
            <input type="password" name="current-password" id="current-password" placeholder="Optional">
        </span>
        <button id="current-config-apply" onclick="saveConfig()">Save</button>
    </div>
    <div id="list-container">
    </div>
</body>
</html>
)rawliteral";

void serverInit(){  
  server.on("/", indexHandler);  
  server.on("/searchWifi", searchWifiHandler);
  server.on("/saveConfig", HTTP_POST, saveConfigHandler);
  server.on("/getSSID", getSSIDHandler);

  server.begin();
}

void indexHandler() {
  server.send(200, "text/html", index_html); 
  ledBlink(LED_BUILTIN);
}

void searchWifiHandler() {
  String networkFound = networkSearch();
  server.send(200, "application/json", networkFound); 
}

void saveConfigHandler() {
  //Serial.print(server.arg("ssid") + server.arg("password"));
  writeConfig(server.arg("ssid"), server.arg("password"));
  server.send(200);
}

void getSSIDHandler() {
  String ssid = readSSID();
  
  server.send(200, "text/plain", ssid);
}