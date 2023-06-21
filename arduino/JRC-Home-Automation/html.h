#ifndef _HTML_IDX
#define _HTML_IDX

const char *html = R"(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>JRC Home Automation</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
</head>

<body>
    <div class="container">
        <header class="text-center mt-4">
            <h1>JRC Home Automation</h1>
            <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTXGa0ts752Ug6-MwlJ0Jn8NqFsfe4U8MFy8S1xrpvtbA&s"
                alt="JRC Logo" width="100">
        </header>

        <div class="row mt-5">
            <div class="col-md-6 my-2" id="help">
                <div class="card">
                    <div class="card-body">
                        <h5 class="card-title">Control Panel</h5>
                        <div class="row">
                            <div class="col-md-6 my-2">
                                <div class="card">
                                    <div class="card-body">
                                        <h6 class="card-subtitle mb-2 text-muted">Light</h6>
                                        <p class="card-text">
                                            <img src="https://cdn-icons-png.flaticon.com/512/4021/4021717.png"
                                                alt="Light Logo" width="50">
                                        </p>
                                        <p class="card-text" id="lightState">State: On</p>
                                        <button class="btn btn-primary" id="lightBtn">Turn On</button>
                                        <button class="btn btn-success" id="lightAutomateBtn">Automatic</button>
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-6 my-2">
                                <div class="card">
                                    <div class="card-body">
                                        <h6 class="card-subtitle mb-2 text-muted">Fan</h6>
                                        <p class="card-text">
                                            <img src="https://cdn-icons-png.flaticon.com/512/2337/2337999.png"
                                                alt="Light Logo" width="50">
                                        </p>
                                        <p class="card-text" id="fanState">State: On</p>
                                        <button class="btn btn-primary" id="fanBtn">Turn On</button>
                                        <button class="btn btn-success" id="fanAutomateBtn">Automatic</button>
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-6 my-2">
                                <div class="card">
                                    <div class="card-body">
                                        <h6 class="card-subtitle mb-2 text-muted">Door</h6>
                                        <p class="card-text">
                                            <img src="https://cdn-icons-png.flaticon.com/512/279/279930.png"
                                                alt="Light Logo" width="50">
                                        </p>
                                        <p class="card-text" id="doorState">State: Open</p>
                                        <button class="btn btn-primary" id="doorBtn">Open</button>
                                        <button class="btn btn-success" id="doorAutomateBtn">Automatic</button>
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-6 my-2">
                                <div class="card">
                                    <div class="card-body">
                                        <h6 class="card-subtitle mb-2 text-muted">Window</h6>
                                        <p class="card-text">
                                            <img src="https://cdn-icons-png.flaticon.com/512/148/148261.png"
                                                alt="Light Logo" width="50">
                                        </p>
                                        <p class="card-text" id="windowState">State: Open</p>
                                        <button class="btn btn-primary" id="windowBtn">Open</button>
                                        <button class="btn btn-success" id="windowAutomateBtn">Automatic</button>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="col-md-6 my-2">
                <div class="card">
                    <div class="card-body">
                        <h5 class="card-title">Sensor Reading Panel</h5>

                        <div class="card my-2">
                            <div class="card-body">
                                <h6 class="card-subtitle mb-2 text-muted">Daylight Status</h6>

                                <p class="card-text" id="ldrState">Loading...</p>
                            </div>
                        </div>

                        <div class="card my-2">
                            <div class="card-body">
                                <h6 class="card-subtitle mb-2 text-muted">People Present</h6>

                                <p class="card-text" id="pirState">Loading...</p>
                            </div>
                        </div>

                        <div class="card my-2">
                            <div class="card-body">
                                <h6 class="card-subtitle mb-2 text-muted">Rain State</h6>
                                <p class="card-text"><img src="" alt="" id="rainImg" width="50"></p>
                                <p class="card-text" id="rainState">Loading...</p>
                            </div>
                        </div>

                        <div class="card my-2">
                            <div class="card-body">
                                <h6 class="card-subtitle mb-2 text-muted">Temperature</h6>
                                <p class="card-text" id="tempState">Loading...</p>
                            </div>
                        </div>

                        <div class="card my-2">
                            <div class="card-body">
                                <h6 class="card-subtitle mb-2 text-muted">Humidity</h6>

                                <p class="card-text" id="humidState">Loading...</p>
                            </div>
                        </div>

                    </div>
                </div>
            </div>
        </div>
    </div>

    <script>
        // Fetch light level and rain state from API and update the card text
        function fetchSensorData() {
            fetch('api/sensors')
                .then(response => response.json())
                .then(data => {

                    // PIR sensor State
                    document.getElementById('pirState').textContent = data.pirState == "on" ? "People Present" : "No People Present";

                    // Rain Sensor State
                    document.getElementById('rainState').textContent = data.rainState == "on" ? "Raining" : "Not Raining";

                    // LDR state
                    document.getElementById('ldrState').textContent = data.ldrState == "on" ? "Daylight" : "Night";

                    document.getElementById('rainImg').src = data.rainState == "on" ? "https://cdn-icons-png.flaticon.com/512/3217/3217120.png" : "https://cdn-icons-png.flaticon.com/512/2392/2392508.png";
                    document.getElementById('tempState').textContent = `${data.tempState}° Celcius`;
                    document.getElementById('humidState').textContent = `${data.humidState}%`;
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Get the light state and update the button text and state text
        function fetchLightState() {
            fetch('api/light/get-state')
                .then(response => response.json())
                .then(data => {
                    const lightBtn = document.getElementById('lightBtn');
                    const lightState = document.getElementById('lightState');
                    lightBtn.textContent = data.state == "on" ? 'Turn Off' : 'Turn On';
                    lightState.textContent = data.state == "on" ? 'State: On' : 'State: Off';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }


        // Toggle light state and update the button text and state text
        function toggleLightState() {
            fetch('api/light/toggle')
                .then(response => response.json())
                .then(data => {
                    const lightBtn = document.getElementById('lightBtn');
                    const lightState = document.getElementById('lightState');
                    lightBtn.textContent = data.state == "on" ? 'Turn Off' : 'Turn On';
                    lightState.textContent = data.state == "on" ? 'State: On' : 'State: Off';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Get light automate state from api/light/get-automate-state and update the button text (Automatic/Manual) and class (btn-success/btn-danger)
        function fetchLightAutomateState() {
            fetch('api/light/get-automate-state')
                .then(response => response.json())
                .then(data => {
                    const lightAutomateBtn = document.getElementById('lightAutomateBtn');
                    lightAutomateBtn.textContent = data.state == "on" ? 'Automatic' : 'Manual';
                    lightAutomateBtn.className = data.state == "on" ? 'btn btn-success' : 'btn btn-danger';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Toggle light automate state and update the button text (Automatic/Manual) and class (btn-success/btn-danger)
        function toggleLightAutomateState() {
            fetch('api/light/toggle-automate')
                .then(response => response.json())
                .then(data => {
                    const lightAutomateBtn = document.getElementById('lightAutomateBtn');
                    lightAutomateBtn.textContent = data.state == "on" ? 'Automatic' : 'Manual';
                    lightAutomateBtn.className = data.state == "on" ? 'btn btn-success' : 'btn btn-danger';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Get the fan state and update the button text and image
        function fetchFanState() {
            fetch('api/fan/get-state')
                .then(response => response.json())
                .then(data => {
                    const fanBtn = document.getElementById('fanBtn');
                    const fanState = document.getElementById('fanState');
                    fanBtn.textContent = data.state == "on" ? 'Turn Off' : 'Turn On';
                    fanState.textContent = data.state == "on" ? 'State: On' : 'State: Off';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Toggle fan state and update the button text and image
        function toggleFanState() {
            fetch('api/fan/toggle')
                .then(response => response.json())
                .then(data => {
                    const fanBtn = document.getElementById('fanBtn');
                    const fanState = document.getElementById('fanState');
                    fanBtn.textContent = data.state == "on" ? 'Turn Off' : 'Turn On';
                    fanState.textContent = data.state == "on" ? 'State: On' : 'State: Off';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Get fan automate state from api/light/get-automate-state and update the button text (Automatic/Manual) and class (btn-success/btn-danger)
        function fetchFanAutomateState() {
            fetch('api/fan/get-automate-state')
                .then(response => response.json())
                .then(data => {
                    const fanAutomateBtn = document.getElementById('fanAutomateBtn');
                    fanAutomateBtn.textContent = data.state == "on" ? 'Automatic' : 'Manual';
                    fanAutomateBtn.className = data.state == "on" ? 'btn btn-success' : 'btn btn-danger';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Toggle fan automate state and update the button text (Automatic/Manual) and class (btn-success/btn-danger)
        function toggleFanAutomateState() {
            fetch('api/fan/toggle-automate')
                .then(response => response.json())
                .then(data => {
                    const fanAutomateBtn = document.getElementById('fanAutomateBtn');
                    fanAutomateBtn.textContent = data.state == "on" ? 'Automatic' : 'Manual';
                    fanAutomateBtn.className = data.state == "on" ? 'btn btn-success' : 'btn btn-danger';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Get the door state and update the button text and image
        function fetchDoorState() {
            fetch('api/door/get-state')
                .then(response => response.json())
                .then(data => {
                    const doorBtn = document.getElementById('doorBtn');
                    const doorState = document.getElementById('doorState');
                    doorBtn.textContent = data.state == "open" ? 'Close' : 'Open';
                    doorState.textContent = data.state == "closed" ? 'State: Closed' : 'State: Open';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Toggle door state and update the button text and image
        function toggleDoorState() {
            fetch('api/door/toggle')
                .then(response => response.json())
                .then(data => {
                    const doorBtn = document.getElementById('doorBtn');
                    const doorState = document.getElementById('doorState');
                    doorBtn.textContent = data.state == "open" ? 'Close' : 'Open';
                    doorState.textContent = data.state == "closed" ? 'State: Closed' : 'State: Open';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Get door automate state from api/light/get-automate-state and update the button text (Automatic/Manual) and class (btn-success/btn-danger)
        function fetchDoorAutomateState() {
            fetch('api/door/get-automate-state')
                .then(response => response.json())
                .then(data => {
                    const doorAutomateBtn = document.getElementById('doorAutomateBtn');
                    doorAutomateBtn.textContent = data.state == "on" ? 'Automatic' : 'Manual';
                    doorAutomateBtn.className = data.state == "on" ? 'btn btn-success' : 'btn btn-danger';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Toggle door automate state and update the button text (Automatic/Manual) and class (btn-success/btn-danger)
        function toggleDoorAutomateState() {
            fetch('api/door/toggle-automate')
                .then(response => response.json())
                .then(data => {
                    const doorAutomateBtn = document.getElementById('doorAutomateBtn');
                    doorAutomateBtn.textContent = data.state == "on" ? 'Automatic' : 'Manual';
                    doorAutomateBtn.className = data.state == "on" ? 'btn btn-success' : 'btn btn-danger';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Get window state and update the button text and image
        function fetchWindowState() {
            fetch('api/window/get-state')
                .then(response => response.json())
                .then(data => {
                    const windowBtn = document.getElementById('windowBtn');
                    const windowState = document.getElementById('windowState');
                    windowBtn.textContent = data.state == "open" ? 'Close' : 'Open';
                    windowState.textContent = data.state == "closed" ? 'State: Closed' : 'State: Open';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Toggle window state and update the button text and image
        function toggleWindowState() {
            fetch('api/window/toggle')
                .then(response => response.json())
                .then(data => {
                    const windowBtn = document.getElementById('windowBtn');
                    const windowState = document.getElementById('windowState');
                    windowBtn.textContent = data.state == "open" ? 'Close' : 'Open';
                    windowState.textContent = data.state == "closed" ? 'State: Closed' : 'State: Open';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Get window automate state from api/window/get-automate-state and update the button text (Automatic/Manual) and class (btn-success/btn-danger)
        function fetchWindowAutomateState() {
            fetch('api/window/get-automate-state')
                .then(response => response.json())
                .then(data => {
                    const windowAutomateBtn = document.getElementById('windowAutomateBtn');
                    windowAutomateBtn.textContent = data.state == "on" ? 'Automatic' : 'Manual';
                    windowAutomateBtn.className = data.state == "on" ? 'btn btn-success' : 'btn btn-danger';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Toggle window automate state and update the button text (Automatic/Manual) and class (btn-success/btn-danger)
        function toggleWindowAutomateState() {
            fetch('api/window/toggle-automate')
                .then(response => response.json())
                .then(data => {
                    const windowAutomateBtn = document.getElementById('windowAutomateBtn');
                    windowAutomateBtn.textContent = data.state == "on" ? 'Automatic' : 'Manual';
                    windowAutomateBtn.className = data.state == "on" ? 'btn btn-success' : 'btn btn-danger';
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        }

        // Add event listeners to the buttons
        document.getElementById('lightBtn').addEventListener('click', toggleLightState);
        document.getElementById('fanBtn').addEventListener('click', toggleFanState);
        document.getElementById('doorBtn').addEventListener('click', toggleDoorState);
        document.getElementById('windowBtn').addEventListener('click', toggleWindowState);

        document.getElementById('lightAutomateBtn').addEventListener('click', toggleLightAutomateState);
        document.getElementById('fanAutomateBtn').addEventListener('click', toggleFanAutomateState);
        document.getElementById('doorAutomateBtn').addEventListener('click', toggleDoorAutomateState);
        document.getElementById('windowAutomateBtn').addEventListener('click', toggleWindowAutomateState);

        function update() {
            // Fetch initial sensor data
            fetchSensorData();

            // Fetch initial light state and automate state
            fetchLightState();
            fetchLightAutomateState();


            // Fetch initial fan state
            fetchFanState();
            fetchFanAutomateState();



            // Fetch initial door state
            fetchDoorState();
            fetchDoorAutomateState();

            // Fetch initial window state
            fetchWindowState();
            fetchWindowAutomateState();
        }
        update();

        // Update sensor data every 10 seconds
        setInterval(update, 10000);
    </script>
</body>

</html>
)";

#endif // _HTML_IDX