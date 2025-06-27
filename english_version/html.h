#ifndef HTML_H
#define HTML_H
// ENGLISH ver. 1.0.1
const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <meta charset="UTF-8">
  <title>Air quality measurement</title>
  <link rel="icon" type="image/x-icon" href="/favicon.ico">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://fonts.googleapis.com/css2?family=Roboto&display=swap" rel="stylesheet">
  <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
  
  <style>
    html {
    overflow-y: scroll;
    }
    body {
      font-family: 'Roboto', sans-serif;
      text-align: center;
      background: #f5f5f5;
      margin: 0;
      padding: 0 10px;
    }

    .headline {
      font-weight: 900;
      letter-spacing: 1px;
      font-size: 26px;
      color: #009688cc; 
      border: 1px solid #b2dfdb; 
      background-color: #e0f7fa; 
      padding: 12px 20px;
      display: inline-block;
      margin: 20px auto 0 auto;
      box-shadow: 2px 2px 6px rgba(0, 150, 136, 0.1); 
      border-radius: 4px;
    }


    #topMenu {
      margin: 20px 0;
      text-align: center;
      display: flex;
      justify-content: center;
      gap: 12px;
      flex-wrap: nowrap;
      overflow-x: auto;
      -webkit-overflow-scrolling: touch; 
    }
    
    #topMenu button {
      margin: 0; 
      padding: 10px 16px;
      width: 100px;
      flex-shrink: 0;
      background-color: #f5f5f5;
      border: 1px solid #ccc;
      border-radius: 4px;
      font-size: 1em;
      cursor: pointer;
      transition: background-color 0.2s ease, transform 0.1s ease;
      box-shadow: 0 2px 4px rgba(0,0,0,0.1);
      color: #333;

      display: inline-flex;
      align-items: center;
      justify-content: center;
      gap: 8px;
    }

    #topMenu button .material-icons {
      font-size: 20px;
    }

    #topMenu .btn-label {
      font-size: 1em;
      font-weight: 500;
    }

    #topMenu button:hover {
      background-color: #fff;
      //transform: translateY(-1px);
    }

    #topMenu button.active {
      background-color: #68b141;
      color: white;
      border-color: #4CAF50;
    }

    .container,
    #chart-container,
    #service-container {
      margin-top: 20px;
      max-width: 600px;
      margin-left: auto;
      margin-right: auto;
    }

    #chart-container {
      max-width: 600px;
      margin: 20px auto;
      height: auto;       
      max-height: 80vh;  
      position: relative;
      overflow: visible; 
    }

    .table-wrapper {
      width: 100%;
      overflow-x: auto;
      -webkit-overflow-scrolling: touch;
    }

    .note-text {
      color: #bbb; 
      font-size: 0.875em;
      margin-top: 10px;
    }



    .mainTable {
      margin: 2rem auto;
      border-collapse: separate;  
      border-spacing: 0;
      width: 100%;
      max-width: 600px;
      font-size: 1em;
      box-shadow: 0 6px 10px rgba(0, 0, 0, 0.15);
      border-radius: 6px;
      overflow: hidden; 
      table-layout: fixed;
      background-color: #fafdfa; 
    }

    .mainTable th, 
    .mainTable td {
      padding: 14px 16px;
      text-align: center;
      border-bottom: 1px solid #cfd8cd; 
      word-wrap: break-word;
      width: 33.33%;
      color: #2e4429; 
    }

    .mainTable th {
      background-color: #b7d4b7; 
      color: #1e341e;            
      text-transform: uppercase;
      letter-spacing: 0.07em;
      font-weight: 700;
      user-select: none;
      box-shadow: inset 0 -3px 5px -3px rgba(0, 0, 0, 0.15); 
      white-space: nowrap;  /* zabránit zalomení */
    }

    .mainTable tbody tr:nth-child(even) {
      background-color: #f7fef7; 
    }

    .mainTable tbody tr:hover {
      background-color: #e3f2e3; 
      transition: background-color 0.25s ease;
      cursor: pointer;
    }

    .mainTable td:last-child {
      text-align: center;
    }

    .mainTable td:nth-child(2) {
      color: black !important;
      font-weight: 600;
    }

    .mainTable td[data-label="Quantity"] {
      cursor: pointer;
}

    .value {
      padding: 4px 10px;
      border: 1px solid #ccc;
      border-radius: 8px;
      font-weight: 600;
      color: black;
      display: inline-block;
      min-width: 40px;
      text-align: center;
      background-color: transparent;
    }   

    .temp-light { background-color: #fff9e6; }    /* do 22°C */
    .temp-medium { background-color: #ffeba3; }   /* 22-26°C */
    .temp-high { background-color: #ffd6b3; }     /* 26-30°C */
    .temp-very-high { background-color: #ffb3b3; } /* nad 30°C */

    .rh-light { background-color: #e6f0ff; }  /* RH ≤ 30 */
    .rh-medium{ background-color: #cde1ff; }  /* 31–60 */
    .rh-high{ background-color: #b3d1ff; }  /* 61–80 */
    .rh-very-high { background-color: #9ac2ff; }  /* > 80 */

    .green { background-color: #e6f4e6; }
    .yellow { background-color: #fff9e6; }
    .orange { background-color: #ffd6b3; }
    .red { background-color: #ffb3b3; }

    .statusTable {
      width: 100%;
      max-width: 600px;
      margin: 20px auto;
      border-collapse: separate;
      border-spacing: 0;
      border-radius: 6px;
      overflow: hidden;
      box-shadow: 0 2px 6px rgba(0, 0, 0, 0.04);
      background-color: #fcfcfd; 
      border: 1px solid #e0e4ea; 
      table-layout: fixed;
    }

    .statusTable th,
    .statusTable td {
      padding: 12px 16px;
      text-align: left;
      border-bottom: 1px solid #eceff4; 
      color: #5e6b7a; 
    }

    .statusTable th {
      background-color: #f2f5fa; 
      font-weight: 600;
      color: #445566;
      text-transform: uppercase;
      letter-spacing: 0.04em;
      user-select: none;
      box-shadow: none;
    }

    .statusTable tbody tr:nth-child(even) {
      background-color: #f8f9fb; 
    }

    .statusTable tbody tr:hover {
      background-color: #f0f3f8; 
      transition: background-color 0.2s ease;
      cursor: default;
    }

    .statusTable td {
      font-weight: 400;
      color: #5a6c7a;
    }


    .infoTable {
      width: 100%;
      max-width: 600px;
      margin: 1.5rem auto;
      border-collapse: collapse;
      font-size: 0.95em;
      color: #444;
      background-color: #fafafa;
      border: 1px solid #ddd;
      border-radius: 6px;
      overflow: hidden;
      table-layout: fixed;
    }

    .infoTable th, 
    .infoTable td {
      padding: 10px 14px;
      text-align: left;
      border-bottom: 1px solid #eee;
    }

    .infoTable th {
      background-color: #f0f0f0;
      font-weight: 600;
      color: #555;
      user-select: none;
    }

    .infoTable tr:last-child td {
      border-bottom: none;
    }

    .infoTable tr:hover {
      background-color: #f9f9f9;
      transition: background-color 0.3s ease;
    }


    .soft-link {
      color: #3b82f6;
      text-decoration: none;
      font-weight: 500;
      transition: color 0.3s;
    }

    .soft-link:hover {
      color: #1e40af; 
    }

    #infoBox {
      max-width: 600px;
      width: 100%;
      margin: 20px auto 0 auto;
      padding: 0px;
      /* background: #f9f9f9; */
      font-size: 0.9em;
      border: none;
      box-sizing: border-box;
    }
    
    #alert {
      background-color: #e61919;  
      color: white;
      font-weight: 600;
      padding: 12px 20px;
      margin: 20px auto;
      width: 100%;
      max-width: 600px;
      border-radius: 4px;
      display: none;
      font-size: 1.1em;
      box-shadow: 0 3px 8px rgba(0,0,0,0.15);
      text-align: center;
      border: 1px solid #e06c10;
      font-family: 'Roboto', sans-serif;
      letter-spacing: 0.5px;
      user-select: none;
      transition: background-color 0.3s ease;
      box-sizing: border-box;
    }

    #alert strong {
      font-weight: 700;
    }

 

    @media (max-width: 480px) {

    .headline {
      font-size: 24px;
      padding: 8px 10px;
      margin: 15px auto 0 auto;
      //letter-spacing: 0.8px;
      display: block; 
      width: 90%; 
    }
  
      #topMenu button {
        width: 80px;
        padding: 8px 10px;
        font-size: 0.9em;
        gap: 6px;
      }

      .table-wrapper {
        width: 90%;
        margin: 0 auto 20px auto;
        padding: 0;
        overflow-x: auto;
      }

      .mainTable,
      .statusTable,
      .infoTable {
        width: 100%;
        table-layout: auto;
        border-radius: 0;
        box-shadow: none;
        margin: 0 auto;
      }

      .mainTable th, .mainTable td,
      .statusTable th, .statusTable td,
      .infoTable th, .infoTable td {
        font-size: 0.85em;
        padding: 10px 10px;
        white-space: normal;          
        word-break: break-word;       
        text-align: center;
      }

      html, body {
        margin: 0;
        padding: 0;
        overflow-x: hidden;
      }
    }
    
  </style>
</head>
<body>

  <h1 class="headline">
    <img src="/headericon.png" alt="ikona" style="width:32px; height:32px; vertical-align:middle; margin-right:8px;">
    AIR QUALITY MONITOR
  </h1>

  <div id="topMenu">
    <button onclick="showSection('table')" id="btnTable" title="Show table">
      <span class="btn-label">Values</span>
    </button>
    <button onclick="showSection('chart')" id="btnChart" title="Show charts">
      <span class="btn-label">Charts</span>
    </button>
    <button onclick="showSection('servis')" id="btnUpdate" title="Service">
      <span class="btn-label">Service</span>
    </button> 
  </div>

<div class="container">

  <div id="alert">⚠️ <strong>ATTENTION:</strong> High CO₂ concentration! Ventilation required.</div>

  <div class="table-wrapper">
    <table class="mainTable">
      <thead>
        <tr><th>Quantity</th><th>Value</th><th>Unit</th></tr>
      </thead>
      <tbody>
        <tr id="co2">
          <td data-label="Quantity">🌿 CO₂</td>
          <td data-label="Value" id="co2value"><span class="value">--</span></td>
          <td data-label="Unit">ppm</td>
        </tr>
        <tr id="temp">
          <td data-label="Quantity">🌡️ Temperature</td>
          <td data-label="Value"><span class="value">--</span></td>
          <td data-label="Unit">°C</td>
        </tr>
        <tr id="rh">
          <td data-label="Quantity">💧 Humidity</td>
          <td data-label="Value"><span class="value">--</span></td>
          <td data-label="Unit">%</td>
        </tr>
        <tr id="pm1">
          <td data-label="Quantity">🌫️ PM1.0</td>
          <td data-label="Value" id="pm"><span class="value">--</span></td>
          <td data-label="Unit">µg/m³</td>
        </tr>
        <tr id="pm2p5">
          <td data-label="Quantity">💨 PM2.5</td>
          <td data-label="Value"><span class="value">--</span></td>
          <td data-label="Unit">µg/m³</td>
        </tr>
        <tr id="pm4">
          <td data-label="Quantity">☁️ PM4.0</td>
          <td data-label="Value"><span class="value">--</span></td>
          <td data-label="Unit">µg/m³</td>
        </tr>
        <tr id="pm10">
          <td data-label="Quantity">🌪️ PM10</td>
          <td data-label="Value"><span class="value">--</span></td>
          <td data-label="Unit">µg/m³</td>
        </tr>
        <tr id="voc">
          <td data-label="Quantity">🧪 VOC Index</td>
          <td data-label="Value" id="voc"><span class="value">--</span></td>
          <td data-label="Unit"></td>
        </tr>
        <tr id="nox">
          <td data-label="Quantity">🚗 NOx Index</td>
          <td data-label="Value" id="nox"><span class="value">--</span></td>
          <td data-label="Unit"></td>
        </tr>
      </tbody>
    </table>

    <p class="note-text">Update interval {{UPDATE_INTERVAL_MIN}} min.</p>  
    <p class="note-text">Start time: {{START_DATETIME}}</p>
  </div>

</div>
  
  <div id="chart-container">
    <canvas id="co2Chart"></canvas>
    <canvas id="tempChart"></canvas>
    <canvas id="rhChart"></canvas>
  </div>  

  <div id="service-container">
  <div class="table-wrapper">  
    <table class="statusTable">
      <tr><th>Parameter</th><th>Value</th></tr>
      <tr><td>IP address:</td><td>{{IP_ADDRESS}}</td></tr>
      <tr><td>Name (SSID):</td><td>{{SSID}}</td></tr>
      <tr><td>Signal (RSSI):</td><td>{{RSSI}} dBm</td></tr>
      <tr><td>Firmware version:</td><td>1.0.1</td></tr>
      <tr><td>Firmware update:</td><td><a href="/update" class="soft-link">WiFi (ElegantOTA)</a></td></tr>
      <tr><td>Current values:</td><td><a href="/now" class="soft-link">JSON data</a></td></tr>
    </table>
  </div>

  <br>

  <div class="table-wrapper"> 
    <table class="infoTable">
      <tr><th>Value RSSI</th><th>WiFi signal strength</th></tr>
      <tr><td>RSSI > –30 dBm</td><td>Excellent</td></tr>
      <tr><td>RSSI &lt; –55 dBm</td><td>Very good</td></tr>
      <tr><td>RSSI &lt; –67 dBm</td><td>Quite good</td></tr>
      <tr><td>RSSI &lt; –70 dBm</td><td>Good</td></tr>
      <tr><td>RSSI &lt; –80 dBm</td><td>Bad</td></tr>
      <tr><td>RSSI &lt; –90 dBm</td><td>Weak (unusable)</td></tr>
    </table>
  </div>

    <p class="note-text">To correctly display the time in the graph, the monitor must be connected to the Internet.</p>       
  </div>   

  <div id="infoBox"></div>


  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  
  <script>

  const HISTORY_LENGTH = {{HISTORY_LENGTH}};
  UPDATE_INTERVAL_MIN = {{UPDATE_INTERVAL_MIN}};

  let co2Data = new Array(HISTORY_LENGTH).fill(0);
  let tempData = new Array(HISTORY_LENGTH).fill(0);
  let rhData = new Array(HISTORY_LENGTH).fill(0);
  let labels = new Array(HISTORY_LENGTH).fill(0);

  // CO₂ graf
  const co2Chart = new Chart(document.getElementById('co2Chart').getContext('2d'), {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'CO₂ (ppm)',
        data: co2Data,
        borderColor: 'rgb(75, 192, 192)',
        fill: false,
        tension: 0.3,
        pointRadius: 2
      }]
    },
    options: {
      responsive: true,
      animation: false,
      scales: {
        x: {
          ticks: {
            maxRotation: 0,
            minRotation: 0,
            autoSkip: true,
            maxTicksLimit: 8
          }
        },
        y: {
          beginAtZero: true,
          suggestedMax: 2000
        }
      }
    }
  });

  const tempChart = new Chart(document.getElementById('tempChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'Temperature (°C)',
        data: tempData,
        borderColor: 'rgb(255, 99, 132)',
        fill: false,
        tension: 0.3,
        pointRadius: 2
      }]
    },
    options: {
      responsive: true,
      animation: false,
      scales: {
        x: {
          ticks: {
            maxRotation: 0,
            minRotation: 0,
            autoSkip: true,
            maxTicksLimit: 8
          }
        },
        y: {
          beginAtZero: true,
          suggestedMax: 50
        }
      }
    }
  });

  const rhChart = new Chart(document.getElementById('rhChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'Relative humidity (%)',
        data: rhData,
        borderColor: 'rgb(54, 162, 235)',
        fill: false,
        tension: 0.3,
        pointRadius: 2
      }]
    },
    options: {
      responsive: true,
      animation: false,
      scales: {
        x: {
          ticks: {
            maxRotation: 0,
            minRotation: 0,
            autoSkip: true,
            maxTicksLimit: 8
          }
        },
        y: {
          beginAtZero: true,
          suggestedMax: 100
        }
      }
    }
  });
  </script>
 
  <script>

    function setInfoBoxWidth() {
      const table = document.querySelector('.mainTable');
      if (table && infoBox) {
        infoBox.style.width = table.offsetWidth + 'px';
      }
    }

    window.addEventListener('resize', setInfoBoxWidth);
    setInfoBoxWidth();  // inicializace při načtení

    const sensorConfig = {
      co2:   { thresholds: [1000, 2000, 3000], classes: ['green', 'yellow', 'orange', 'red'] },
      temp:  { thresholds: [22, 26, 30],       classes: ['temp-light', 'temp-medium', 'temp-high', 'temp-very-high']},
      rh:    { thresholds: [30, 60, 80],       classes: ['rh-light', 'rh-medium', 'rh-high', 'rh-very-high'] },
      pm1:   { thresholds: [12, 35.4, 55.4],   classes: ['green', 'yellow', 'orange', 'red'] },
      pm2p5: { thresholds: [12, 35.4, 55.4],   classes: ['green', 'yellow', 'orange', 'red'] },
      pm4:   { thresholds: [12, 35.4, 55.4],   classes: ['green', 'yellow', 'orange', 'red'] },
      pm10:  { thresholds: [54, 154, 254],     classes: ['green', 'yellow', 'orange', 'red'] },
      voc:   { thresholds: [150, 250, 400],    classes: ['green', 'yellow', 'orange', 'red'] },
      nox:   { thresholds: [20, 150, 300],     classes: ['green', 'yellow', 'orange', 'red'] }
    };

    function colorCellUnified(rowId, value) {
      const row = document.getElementById(rowId);
      if (!row) return;

      const cell = row.querySelector('td[data-label="Hodnota"]');
      if (!cell) return;

      const valueSpan = cell.querySelector('.value');
      if (!valueSpan) return;

      valueSpan.className = 'value'; // reset classes
      valueSpan.textContent = value;

      const config = sensorConfig[rowId];
      if (!config) return;

      const { thresholds, classes } = config;

      for (let i = 0; i <= thresholds.length; i++) {
        if (i === thresholds.length || value <= thresholds[i]) {
          valueSpan.classList.add(classes[i]);
          break;
        }
      }
    }

    function applyColors() {
      Object.keys(sensorConfig).forEach(id => {
        const row = document.getElementById(id);
        if (!row) return;

        const cell = row.querySelector('td[data-label="Hodnota"] .value');
        if (!cell) return;

        const value = parseFloat(cell.textContent);
        if (!isNaN(value)) {
          colorCellUnified(id, value);
        }
      });
    }

    applyColors();

    // Zobraz alert hned při načtení, pokud je CO₂ > 2000
    const initialCo2 = parseFloat(document.getElementById('co2value').innerText);
    if (!isNaN(initialCo2) && initialCo2 > 2000) {
      document.getElementById("alert").style.display = "block";
    }

    function updateRow(id, value) {
      const row = document.getElementById(id);
      if (!row) return;

      const cell = row.cells[1];
      const valueSpan = cell.querySelector('.value');
      if (!valueSpan) return;

      valueSpan.innerText = (typeof value === 'number' ? value.toFixed(1) : value);
      colorCellUnified(id, value);
    }


    const UPDATE_INTERVAL_MIN = {{UPDATE_INTERVAL_MIN}};

    async function fetchNow() {
      const res = await fetch("/now");
      const now = await res.json();

      updateRow("co2", now.co2);
      updateRow("temp", now.temp);
      updateRow("rh", now.rh);

      updateRow("pm1", now.pm1);
      updateRow("pm2p5", now.pm2p5);
      updateRow("pm4", now.pm4);
      updateRow("pm10", now.pm10);
      updateRow("voc", now.vocIndex);
      updateRow("nox", now.noxIndex);

      const alertDiv = document.getElementById("alert");
      alertDiv.style.display = now.co2 > 2000 ? "block" : "none";
    }


  fetchNow();

    async function fetchData() {
      //console.log("fetchData called");
      const res = await fetch("/data");
      //console.log("Response ok?", res.ok, "status:", res.status);
      const data = await res.json();
      //console.log("Data ⇢", data);

      // Aktuální hodnoty do datových polí
      co2Data.push(data.co2);
      tempData.push(data.temp);
      rhData.push(data.rh);

      if (co2Data.length > HISTORY_LENGTH) co2Data.shift();
      if (tempData.length > HISTORY_LENGTH) tempData.shift();
      if (rhData.length > HISTORY_LENGTH) rhData.shift();

      // Zkopíruj správně časová razítka
      labels = data.timestamps.slice();

      // Použij historii ze serveru
      co2Chart.data.labels = labels;
      co2Chart.data.datasets[0].data = data.co2History;
      co2Chart.update();

      tempChart.data.labels = labels;
      tempChart.data.datasets[0].data = data.tempHistory;
      tempChart.update();

      rhChart.data.labels = labels;
      rhChart.data.datasets[0].data = data.rhHistory;
      rhChart.update(); 
      }

  setInterval(fetchData, UPDATE_INTERVAL_MIN);
  fetchData();

  </script>

  <script>    
    
    const co2Text = `
    <strong>What is CO₂ (carbon dioxide)?</strong><br><br>
    CO₂ is a colorless gas that naturally occurs in the atmosphere and is produced by:<br>
    – breathing by humans and animals,<br>
    – burning of fossil fuels,<br>
    – fermentation and other biological processes.<br><br>

    Outdoor air (clean, natural environment):  
    ~400 to 420 ppm (parts per million) (typical CO₂ concentration in rural or natural areas – slightly increasing year by year due to emissions).<br><br>

    Indoors, its concentration often rises due to insufficient ventilation.<br><br>

    <strong>Why monitor CO₂?</strong><br>
    High levels of CO₂ do not pose a direct health risk, but:<br>
    – they can lead to fatigue, reduced concentration, and drowsiness,<br>
    – they indicate poor ventilation and the accumulation of exhaled air.<br><br>

    <strong>Recommended CO₂ levels</strong><br><br>
    <table style="width:100%; border-collapse: collapse; font-size: 0.85em;">
      <tr style="background: #333; color: white;">
        <th style="padding:4px; border:1px solid #ddd;">Concentration</th>
        <th style="padding:4px; border:1px solid #ddd;">Air Quality</th>
        <th style="padding:4px; border:1px solid #ddd;">Recommendation</th>
      </tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">0 ÷ 1000 ppm</td><td style="padding:4px; border:1px solid #ddd;">Good</td><td style="padding:4px; border:1px solid #ddd;">Optimal ventilation</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">1001 ÷ 2000 ppm</td><td style="padding:4px; border:1px solid #ddd;">Elevated</td><td style="padding:4px; border:1px solid #ddd;">Ventilation recommended</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">2001 ÷ 3000 ppm</td><td style="padding:4px; border:1px solid #ddd;">Poor</td><td style="padding:4px; border:1px solid #ddd;">Ventilation required</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">3000+ ppm</td><td style="padding:4px; border:1px solid #ddd;">Very poor</td><td style="padding:4px; border:1px solid #ddd;">Health discomfort</td></tr>
    </table><br><br>
    `;

    const pmText = `
    <strong>What are PM particles?</strong><br>
    PM (Particulate Matter) particles are microscopic solid particles or liquid droplets found in the air.<br><br>

    The most commonly measured sizes are:
    <ul style="margin-top: 8px; margin-bottom: 8px;">
      <li><strong>PM1.0</strong> – very fine particles (up to 1 micrometre), capable of penetrating deep into the lungs.</li>
      <li><strong>PM2.5</strong> – fine particles (up to 2.5 micrometres), associated with pollution from traffic, combustion, and industry.</li>
      <li><strong>PM4.0</strong> – medium fraction, less commonly used.</li>
      <li><strong>PM10</strong> – coarse dust particles (up to 10 micrometres), typically trapped in the upper respiratory tract.</li>
    </ul>

    <table style="width: 100%; border-collapse: collapse; font-size: 0.95em; margin-top: 12px;">
      <tr style="background: #333; color: white;">
        <th style="padding: 6px; border: 1px solid #ddd;">Label</th>
        <th style="padding: 6px; border: 1px solid #ddd;">Particle size</th>
        <th style="padding: 6px; border: 1px solid #ddd;">Example / Source</th>
      </tr>
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM1.0</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 1 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Smoke, exhaust fumes, ultrafine particles</td>
      </tr>
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM2.5</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 2.5 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Vehicle emissions, wood burning, dust</td>
      </tr>
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM4.0</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 4 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Bacteria, some pollen particles, industrial aerosols</td>
      </tr>         
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM10</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 10 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Pollen, dust, construction materials</td>
      </tr>
    </table><br>

    <strong>Why are PM particles important?</strong><br>
    Long-term exposure to elevated PM concentrations can cause respiratory issues, cardiovascular problems, and other health complications.<br><br>

    <em>The smaller the particles, the deeper they can penetrate into the human body.</em><br><br>
    `;
      
    const vocText = `
    <strong>What is the VOC index?</strong><br>
    The VOC index (Volatile Organic Compounds index) is a numerical indicator of air quality based on the concentration of volatile organic compounds (VOCs) in the air.<br><br>

    VOCs are various organic chemicals that easily evaporate and can affect health and comfort.<br><br>

    The VOC index serves as an indicator of indoor air quality, as high VOC levels may suggest pollution or unpleasant odours.<br><br>

    <strong>How does the VOC index work?</strong><br>
    Sensors measure the concentration of various VOCs in the air.  
    These readings are processed by an algorithm that calculates the VOC index from the measured values.<br><br>

    The VOC index value usually falls within a certain range (e.g. 0–500) and can be interpreted as follows:<br><br>

    <table style="width:100%; border-collapse: collapse; font-size: 0.85em;">
      <tr style="background: #333; color: white;">
        <th style="padding:4px; border:1px solid #ddd;">VOC Index</th>
        <th style="padding:4px; border:1px solid #ddd;">Air Quality</th>
        <th style="padding:4px; border:1px solid #ddd;">Meaning</th>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">1 – 150</td>
        <td style="padding:4px; border:1px solid #ddd;">Good</td>
        <td style="padding:4px; border:1px solid #ddd;">Low VOC concentration, healthy air</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">151 – 200</td>
        <td style="padding:4px; border:1px solid #ddd;">Acceptable</td>
        <td style="padding:4px; border:1px solid #ddd;">Slightly elevated VOC concentration</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">201 – 300</td>
        <td style="padding:4px; border:1px solid #ddd;">Moderate</td>
        <td style="padding:4px; border:1px solid #ddd;">Decreased air quality, may cause discomfort</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">301 – 500</td>
        <td style="padding:4px; border:1px solid #ddd;">Poor</td>
        <td style="padding:4px; border:1px solid #ddd;">High VOC concentration, ventilation recommended</td>
      </tr>
    </table><br><br>
    `;

    const noxText = `
    <strong>What is the NOx index?</strong><br>
    The NOx index (nitrogen oxides index) is a numerical indicator of air quality based on the concentration of nitrogen oxides (NO and NO₂) in the air.<br><br>

    Nitrogen oxides are primarily produced by the combustion of fossil fuels and can irritate the respiratory system and degrade air quality.<br><br>

    <strong>How does the NOx index work?</strong><br>
    Sensors measure the concentration of NO and NO₂, and based on these values, the NOx index is calculated.<br><br>

    <strong>NOx index values and their meaning:</strong><br><br>

    <table style="width:100%; border-collapse: collapse; font-size: 0.85em;">
      <tr style="background: #333; color: white;">
        <th style="padding:4px; border:1px solid #ddd;">NOx Index</th>
        <th style="padding:4px; border:1px solid #ddd;">Air Quality</th>
        <th style="padding:4px; border:1px solid #ddd;">Meaning</th>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">0 ÷ 1</td>
        <td style="padding:4px; border:1px solid #ddd;">Excellent</td>
        <td style="padding:4px; border:1px solid #ddd;">Minimal nitrogen oxides</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">2 ÷ 50</td>
        <td style="padding:4px; border:1px solid #ddd;">Good</td>
        <td style="padding:4px; border:1px solid #ddd;">Low NOx concentration</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">51 ÷ 100</td>
        <td style="padding:4px; border:1px solid #ddd;">Acceptable</td>
        <td style="padding:4px; border:1px solid #ddd;">Slightly elevated concentration</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">101 ÷ 200</td>
        <td style="padding:4px; border:1px solid #ddd;">Moderate</td>
        <td style="padding:4px; border:1px solid #ddd;">Increased concentration, potential health issues</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">201 ÷ 300</td>
        <td style="padding:4px; border:1px solid #ddd;">Poor</td>
        <td style="padding:4px; border:1px solid #ddd;">High concentration – ventilation recommended</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">301 ÷ 500</td>
        <td style="padding:4px; border:1px solid #ddd;">Very Poor</td>
        <td style="padding:4px; border:1px solid #ddd;">Very high concentration – harmful</td>
      </tr>
    </table><br><br>
    `;

    const table = document.querySelector('.mainTable');
    const infoBox = document.getElementById('infoBox');
    infoBox.style.width = table.offsetWidth + 'px';

    const tooltipTexts = {
      co2: co2Text,
      pm1: pmText,
      pm2p5: pmText,
      pm4: pmText,
      pm10: pmText,
      voc: vocText, 
      nox: noxText
    };

    let currentlyVisible = null; 

    function addTooltipListener(rowId, text) {
      const row = document.getElementById(rowId);
      if (!row) return;
      const labelCell = row.querySelector('td[data-label="Quantity"]');
      if (!labelCell) return;

      labelCell.addEventListener('click', () => {
        if (currentlyVisible === rowId) {
          infoBox.innerHTML = '';
          currentlyVisible = null;
        } else {
          infoBox.innerHTML = text;
          currentlyVisible = rowId;
        }
      });
    }

    Object.entries(tooltipTexts).forEach(([id, text]) => {
      addTooltipListener(id, text);
    });

    document.addEventListener('click', (event) => {
      // Pokud klik není uvnitř .mainTable nebo infoBox
      if (!table.contains(event.target) && !infoBox.contains(event.target)) {
        infoBox.innerHTML = '';
        currentlyVisible = null;
      }
    });


    function showSection(section) {
      const sections = ['table', 'chart', 'servis'];
      sections.forEach(s => {
        const el = s === 'table' ? document.querySelector('.container') 
                  : s === 'chart' ? document.getElementById('chart-container')
                  : document.getElementById('service-container');
        if(el) el.style.display = (s === section) ? 'block' : 'none';
      });

      // aktivní tlačítko
      sections.forEach(s => {
        const btn = document.getElementById('btn' + (s === 'table' ? 'Table' : s === 'chart' ? 'Chart' : 'Update'));
        if(btn) btn.classList.toggle('active', s === section);
      });
    }

    window.onload = () => showSection('table');

    showSection('table');

  </script>

</body>
</html>
  )rawliteral";

#endif