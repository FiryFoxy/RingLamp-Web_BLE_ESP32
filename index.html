<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 LED Controller</title>
    <link
        href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css"
        rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>
    <style>
        body {
            padding: 20px;
        }
        .color-preview {
            width: 50px;
            height: 50px;
            border-radius: 50%;
            margin: 10px auto;
        }
    </style>
</head>
<body>
    <div class="container text-center">
        <h1>ESP32 LED Controller</h1>
        <p id="status" class="text-muted">Status: Disconnected</p>
        <button id="connect" class="btn btn-primary mb-3">Connect to ESP32</button>

        <div id="controls" style="display: none;">
            <div class="mb-3">
                <button id="ledOn" class="btn btn-success">Turn On LEDs</button>
                <button id="ledOff" class="btn btn-danger">Turn Off LEDs</button>
            </div>
            <div class="mb-3">
                <label for="colorPicker" class="form-label">Choose LED Color:</label>
                <input type="color" id="colorPicker" class="form-control" value="#ffffff">
                <div class="color-preview" id="colorPreview" style="background-color: #ffffff;"></div>
            </div>
            <div class="mb-3">
                <label for="brightness" class="form-label">Brightness:</label>
                <input type="range" id="brightness" class="form-range" min="1" max="100" value="50">
            </div>
        </div>
    </div>

    <script>
        let device, server, ledCharacteristic;
        const statusElement = document.getElementById("status");
        const connectButton = document.getElementById("connect");
        const controls = document.getElementById("controls");
        const ledOnButton = document.getElementById("ledOn");
        const ledOffButton = document.getElementById("ledOff");
        const colorPicker = document.getElementById("colorPicker");
        const colorPreview = document.getElementById("colorPreview");
        const brightnessSlider = document.getElementById("brightness");

        async function connectBLE() {
            try {
                statusElement.textContent = "Status: Connecting...";
                device = await navigator.bluetooth.requestDevice({
                    acceptAllDevices: true,
                    optionalServices: ["19b10000-e8f2-537e-4f6c-d104768a1214"]
                });
                server = await device.gatt.connect();
                const service = await server.getPrimaryService("19b10000-e8f2-537e-4f6c-d104768a1214");
                ledCharacteristic = await service.getCharacteristic("19b10002-e8f2-537e-4f6c-d104768a1214");
                statusElement.textContent = "Status: Connected";
                controls.style.display = "block";

                device.addEventListener("gattserverdisconnected", () => {
                    statusElement.textContent = "Status: Disconnected";
                    controls.style.display = "none";
                });
            } catch (error) {
                console.error("Error connecting to BLE device:", error);
                statusElement.textContent = "Status: Error Connecting";
            }
        }

        async function sendCommand(command) {
            if (!ledCharacteristic) return;
            await ledCharacteristic.writeValue(new Uint8Array(command));
        }

        connectButton.addEventListener("click", connectBLE);

        ledOnButton.addEventListener("click", () => sendCommand([1]));
        ledOffButton.addEventListener("click", () => sendCommand([0]));

        colorPicker.addEventListener("input", (event) => {
            const color = event.target.value;
            colorPreview.style.backgroundColor = color;
            const r = parseInt(color.substr(1, 2), 16);
            const g = parseInt(color.substr(3, 2), 16);
            const b = parseInt(color.substr(5, 2), 16);
            sendCommand([67, r, g, b]); // 'C' command followed by RGB values
        });

        brightnessSlider.addEventListener("input", (event) => {
            const brightness = parseInt(event.target.value);
            console.log(`Brightness set to: ${brightness}`);
            // Implement brightness adjustment on the ESP32 side if supported.
        });
    </script>
</body>
</html>
