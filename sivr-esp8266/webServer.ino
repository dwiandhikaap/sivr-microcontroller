const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <script>
        async function searchWifi(){
            const response = await fetch("/searchWifi", {
                method: "GET",
                headers: {
                    "Content-Type": "text/plain"
                },
            }).then(data => data.text());

            document.getElementById("wifi-container").innerHTML = response;
        }
    </script>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>sivr-esp8266</title>
</head>
<body>
    <button onclick="searchWifi()">poggers</button>
    <div id="wifi-container">
        lol
    </div>
</body>
</html>)rawliteral";

void serverInit(){  
  server.on("/", indexHandler);  
  server.on("/searchWifi", searchWifiHandler);

  server.begin();
}

void indexHandler() {
  server.send(200, "text/html", index_html); 
  ledBlink(LED_BUILTIN);
}

void searchWifiHandler() {
  String networkFound = networkSearch();
  server.send(200, "text/plain", networkFound); 
}