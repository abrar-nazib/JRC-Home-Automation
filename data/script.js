let outputDevices = {
  light: {
    state: "OFF",
    automationState: "OFF"
  },
  fan: {
    state: "OFF",
    automationState: "OFF"
  },
  door: {
    state: "OFF",
    automationState: "OFF"
  },
  window: {
    state: "OFF",
    automationState: "OFF"
  }
};

let sensorReadings = {
  temp: "0.0",
  humid: "0.0",
  pir: "LOW",
  ldr: "LOW",
  rain: "LOW"
};




class OutputDevice {
  constructor(name) {
    this.name = name;
    this.switch = document.getElementById(`${name}Switch`);
    this.switchLabel = document.getElementById(`${name}SwitchLabel`);
    this.automationSwitch = document.getElementById(`${name}AutomationSwitch`);
    this.automationSwitchLabel = document.getElementById(`${name}AutomationSwitchLabel`);
    this.state = this.switch.checked ? "ON" : "OFF";
    this.automationState = this.automationSwitch.checked ? "ON" : "OFF";
    this.switch.addEventListener("click", this.setState.bind(this));
    this.automationSwitch.addEventListener("click", this.setAutomationState.bind(this));
  }

  setState() {
    if (this.automationSwitch.checked) {
      this.switch.checked = !this.switch.checked;
    }
    else {
      this.state = this.switch.checked ? "ON" : "OFF";
      this.switchLabel.textContent = this.switch.checked ? "State: On" : "State: Off";
      outputDevices[this.name].state = this.state;
      ws.sendData();
    }

  }

  updateState() {
    this.state = outputDevices[this.name].state;
    this.switch.checked = this.state == "ON" ? true : false;
    this.switchLabel.textContent = this.state == "ON" ? "State: On" : "State: Off";
  }

  setAutomationState() {
    this.automationState = this.automationSwitch.checked ? "ON" : "OFF";
    this.automationSwitchLabel.textContent = this.automationSwitch.checked ? "Automatic" : "Manual";
    outputDevices[this.name].automationState = this.automationState;
    ws.sendData();
  }
  updateAutomationState() {
    this.automationState = outputDevices[this.name].automationState;
    this.automationSwitch.checked = this.automationState == "ON" ? true : false;
    this.automationSwitchLabel.textContent = this.automationState == "ON" ? "Automatic" : "Manual";
  }
}

class Sensor {
  constructor(name, ext, img0URL, img1URL) {
    this.name = name;
    this.state = sensorReadings[this.name];
    this.stateLabel = document.getElementById(`${name}State`);
    this.img = document.getElementById(`${name}Img`);
    this.img0URL = img0URL;
    this.img1URL = img1URL;
    this.ext = ext;
  }

  setLabel() {
    this.state = sensorReadings[this.name];
    this.stateLabel.textContent = `${this.state} ${this.ext}`;
  }
}

class DigitalSensor extends Sensor {
  constructor(name, img0URL, img1URL, text0, text1) {
    super(name, '', img0URL, img1URL);
    this.text0 = text0;
    this.text1 = text1;
  }
  setLabel() {
    this.state = sensorReadings[this.name];
    this.stateLabel.textContent = this.state == "LOW" ? this.text0 : this.text1;
    this.img.src = this.state == "LOW" ? this.img0URL : this.img1URL;
  }
}


const _light = new OutputDevice("light");
const _fan = new OutputDevice("fan");
const _door = new OutputDevice("door");
const _window = new OutputDevice("window");

const tempSensor = new Sensor("temp", "°C", "https://cdn-icons-png.flaticon.com/512/136/136609.png", "https://cdn-icons-png.flaticon.com/512/136/136609.png");
const humidSensor = new Sensor("humid", "%", "https://cdn-icons-png.flaticon.com/512/1582/1582886.png", "https://cdn-icons-png.flaticon.com/512/1582/1582886.png");
const pirSensor = new DigitalSensor("pir", "https://cdn-icons-png.flaticon.com/512/2947/2947640.png", "https://cdn-icons-png.flaticon.com/512/2947/2947640.png", "No Motion", "Motion Detected");
const ldrSensor = new DigitalSensor("ldr", "https://cdn-icons-png.flaticon.com/512/1792/1792806.png", "https://cdn-icons-png.flaticon.com/512/1792/1792806.png", "Dark", "Bright");
const rainSensor = new DigitalSensor("rain", "https://cdn-icons-png.flaticon.com/512/2392/2392508.png", "https://cdn-icons-png.flaticon.com/512/2392/2392508.png", "No Rain", "Rain Detected");

const outputDeviceArray = [_light, _fan, _door, _window];
const sensorArray = [tempSensor, humidSensor, pirSensor, ldrSensor, rainSensor];

const updateOutputDevices = () => {
  outputDeviceArray.forEach(device => {
    device.updateState();
    device.updateAutomationState();
  });
}

const updateSensorValues = () => {
  sensorArray.forEach(sensor => {
    sensor.setLabel();
  });
}

class WebSocketHandler {
  constructor() {
    this.ws = null;
    this.startSocket();
  }

  sendData() {
    var msg = outputDevices;
    // Convert json object to string
    var msgString = JSON.stringify(msg);

    this.ws.send(msgString);
  }

  startSocket() {
    this.ws = new WebSocket('ws://' + document.location.host + '/ws', ['arduino']);
    this.ws.binaryType = "arraybuffer";
    this.ws.onopen = function (e) {
      console.log("Connected To the Websocket server");
    };
    this.ws.onclose = function (e) {
      console.log("Disconnected from the Websocket server");
    };
    this.ws.onerror = function (e) {
      console.log("ws error", e);
      console.log("Error");
    };
    this.ws.onmessage = function (e) {
      var msg = e.data;

      // Convert String to json object
      var obj = JSON.parse(msg);
      outputDevices = obj.outputDevices;
      sensorReadings = obj.sensorReadings;
      updateOutputDevices();
      updateSensorValues();
    };
  }

}
const ws = new WebSocketHandler();


