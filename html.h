#ifndef HTML_H
#define HTML_H
// verze 1.0.1
const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="cs" xml:lang="cs">
<head>
  <meta charset="UTF-8">
  <title>Měření kvality vzduchu</title>
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

    .mainTable td[data-label="Veličina"] {
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

 

    /* mobilní layout */
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

        /* Mobilní úpravy všech tabulek */
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
    MONITOR KVALITY VZDUCHU
  </h1>

  <div id="topMenu">
    <button onclick="showSection('table')" id="btnTable" title="Zobrazit tabulku">
      <span class="btn-label">Hodnoty</span>
    </button>
    <button onclick="showSection('chart')" id="btnChart" title="Zobrazit grafy">
      <span class="btn-label">Grafy</span>
    </button>
    <button onclick="showSection('servis')" id="btnUpdate" title="Servis">
      <span class="btn-label">Servis</span>
    </button> 
  </div>

<div class="container">

  <div id="alert">⚠️ <strong>POZOR:</strong> Vysoká koncentrace CO₂! Nutné větrat.</div>

  <div class="table-wrapper">
    <table class="mainTable">
      <thead>
        <tr><th>Veličina</th><th>Hodnota</th><th>Jednotka</th></tr>
      </thead>
      <tbody>
        <tr id="co2">
          <td data-label="Veličina">🌿 CO₂</td>
          <td data-label="Hodnota" id="co2value"><span class="value">--</span></td>
          <td data-label="Jednotka">ppm</td>
        </tr>
        <tr id="temp">
          <td data-label="Veličina">🌡️ Teplota</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">°C</td>
        </tr>
        <tr id="rh">
          <td data-label="Veličina">💧 Vlhkost</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">%</td>
        </tr>
        <tr id="pm1">
          <td data-label="Veličina">🌫️ PM1.0</td>
          <td data-label="Hodnota" id="pm"><span class="value">--</span></td>
          <td data-label="Jednotka">µg/m³</td>
        </tr>
        <tr id="pm2p5">
          <td data-label="Veličina">💨 PM2.5</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">µg/m³</td>
        </tr>
        <tr id="pm4">
          <td data-label="Veličina">☁️ PM4.0</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">µg/m³</td>
        </tr>
        <tr id="pm10">
          <td data-label="Veličina">🌪️ PM10</td>
          <td data-label="Hodnota"><span class="value">--</span></td>
          <td data-label="Jednotka">µg/m³</td>
        </tr>
        <tr id="voc">
          <td data-label="Veličina">🧪 VOC Index</td>
          <td data-label="Hodnota" id="voc"><span class="value">--</span></td>
          <td data-label="Jednotka"></td>
        </tr>
        <tr id="nox">
          <td data-label="Veličina">🚗 NOx Index</td>
          <td data-label="Hodnota" id="nox"><span class="value">--</span></td>
          <td data-label="Jednotka"></td>
        </tr>
      </tbody>
    </table>

    <p class="note-text">Interval aktualizace {{UPDATE_INTERVAL_MIN}} minut.</p>  
    <p class="note-text">Čas spuštění: {{START_DATETIME}}</p>
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
      <tr><th>Parametr</th><th>Hodnota</th></tr>
      <tr><td>IP adresa:</td><td>{{IP_ADDRESS}}</td></tr>
      <tr><td>Název (SSID):</td><td>{{SSID}}</td></tr>
      <tr><td>Signál (RSSI):</td><td>{{RSSI}} dBm</td></tr>
      <tr><td>Verze firmwaru:</td><td>1.0.1</td></tr>
      <tr><td>Aktualizace firmwaru:</td><td><a href="/update" class="soft-link">WiFi (ElegantOTA)</a></td></tr>
      <tr><td>Aktuální hodnoty:</td><td><a href="/now" class="soft-link">JSON data</a></td></tr>
    </table>
  </div>

  <br>

  <div class="table-wrapper"> 
    <table class="infoTable">
      <tr><th>Hodnota RSSI</th><th>Síla signálu WiFi</th></tr>
      <tr><td>RSSI > –30 dBm</td><td>Vynikající</td></tr>
      <tr><td>RSSI &lt; –55 dBm</td><td>Velmi dobrý</td></tr>
      <tr><td>RSSI &lt; –67 dBm</td><td>Docela dobrý</td></tr>
      <tr><td>RSSI &lt; –70 dBm</td><td>Dobrý</td></tr>
      <tr><td>RSSI &lt; –80 dBm</td><td>Špatný</td></tr>
      <tr><td>RSSI &lt; –90 dBm</td><td>Slabý (nepoužitelný)</td></tr>
    </table>
  </div>

    <p class="note-text">Pro správné zobrazení času v grafu musí být monitor připojen na internet.</p>       
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

  // Teplota graf
  const tempChart = new Chart(document.getElementById('tempChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'Teplota (°C)',
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

  // Vlhkost graf
  const rhChart = new Chart(document.getElementById('rhChart').getContext('2d'), {
    type: 'line',
    data: {
      labels: labels,
      datasets: [{
        label: 'Relativní vlhkost (%)',
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

    // Zobraz alert hned při načtení, pokud je CO₂ > 2000
    const initialCo2 = parseFloat(document.getElementById('co2value').innerText);
    if (!isNaN(initialCo2) && initialCo2 > 2000) {
      document.getElementById("alert").style.display = "block";
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

    function colorCell(rowId, value) {
      const row = document.getElementById(rowId);
      if (!row) return;
      const cell = row.querySelector('td[data-label="Hodnota"]');
      if (!cell) return;

      const valueSpan = cell.querySelector('.value');
      if (!valueSpan) return;

      valueSpan.className = 'value'; // Reset

      const config = sensorConfig[rowId];
      if (!config || isNaN(value)) return;

      const { thresholds, classes } = config;

      let colorClass = classes[classes.length - 1]; // default = last = worst
      for (let i = 0; i < thresholds.length; i++) {
        if (value <= thresholds[i]) {
          colorClass = classes[i];
          break;
        }
      }

      valueSpan.classList.add(colorClass);
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

    function updateRow(id, value) {
      const row = document.getElementById(id);
      if (!row) return;
      const cell = row.cells[1];
      const valueSpan = cell.querySelector('.value');
      if (!valueSpan) return;
      valueSpan.innerText = value.toFixed(1);
      colorCell(id, value);
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
    <strong>Co je CO₂ (oxid uhličitý)?</strong><br><br>
    CO₂ je bezbarvý plyn přirozeně se vyskytující v atmosféře, který vzniká:<br>
    – dýcháním lidí a zvířat,<br>
    – spalováním fosilních paliv,<br>
    – fermentací a dalšími biologickými procesy.<br><br>

    Venkovní vzduch (čistý, přírodní prostředí):
    ~400 až 420 ppm (parts per million) (běžná koncentrace CO₂ na venkově či v přírodě – rok od roku mírně roste vlivem emisí).<br><br>
    
    Ve vnitřních prostorách jeho koncentrace často stoupá kvůli nedostatečnému větrání.<br><br>

    <strong>Proč CO₂ sledovat?</strong><br>
    Vysoké hodnoty CO₂ nesignalizují přímé zdravotní riziko, ale:<br>
    – mohou vést k únavě, snížené soustředěnosti a ospalosti,<br>
    – indikují nedostatečné větrání a akumulaci vydýchaného vzduchu.<br><br>

    <strong>Doporučené hodnoty CO₂</strong><br><br>
    <table style="width:100%; border-collapse: collapse; font-size: 0.85em;">
      <tr style="background: #333; color: white;">
        <th style="padding:4px; border:1px solid #ddd;">Koncentrace</th>
        <th style="padding:4px; border:1px solid #ddd;">Kvalita vzduchu</th>
        <th style="padding:4px; border:1px solid #ddd;">Doporučení</th>
      </tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">0 ÷ 1000 ppm</td><td style="padding:4px; border:1px solid #ddd;">Dobrá</td><td style="padding:4px; border:1px solid #ddd;">Optimální větrání</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">1001 ÷ 2000 ppm</td><td style="padding:4px; border:1px solid #ddd;">Zvýšená</td><td style="padding:4px; border:1px solid #ddd;">Doporučeno větrat</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">2001 ÷ 3000 ppm</td><td style="padding:4px; border:1px solid #ddd;">Špatná</td><td style="padding:4px; border:1px solid #ddd;">Nutné větrat</td></tr>
      <tr><td style="padding:4px; border:1px solid #ddd;">3000+ ppm</td><td style="padding:4px; border:1px solid #ddd;">Velmi špatná</td><td style="padding:4px; border:1px solid #ddd;">Zdravotní diskomfort</td></tr>
    </table><br><br>
    `;

    const pmText = `
    <strong>Co jsou částice PM?</strong><br>
    Částice PM (Particulate Matter) jsou mikroskopické pevné částice nebo kapky kapaliny, které se nacházejí ve vzduchu.<br><br>

    Nejčastěji měřené velikosti jsou:
    <ul style="margin-top: 8px; margin-bottom: 8px;">
      <li><strong>PM1.0</strong> – velmi jemné částice (do 1 mikrometru), mohou pronikat hluboko do plic.</li>
      <li><strong>PM2.5</strong> – jemné částice (do 2.5 mikrometru), spojené se znečištěním z dopravy, spalování a průmyslu.</li>
      <li><strong>PM4.0</strong> – střední frakce, méně běžně používaná.</li>
      <li><strong>PM10</strong> – hrubší prach (do 10 mikrometrů), zachycuje se v horních cestách dýchacích.</li>
    </ul>

    <table style="width: 100%; border-collapse: collapse; font-size: 0.95em; margin-top: 12px;">
      <tr style="background: #333; color: white;">
        <th style="padding: 6px; border: 1px solid #ddd;">Označení</th>
        <th style="padding: 6px; border: 1px solid #ddd;">Velikost částic</th>
        <th style="padding: 6px; border: 1px solid #ddd;">Příklad / Zdroj</th>
      </tr>
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM1.0</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 1 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Kouř, spaliny, ultrajemné částice</td>
      </tr>
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM2.5</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 2,5 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Výfukové plyny, spalování dřeva, prach</td>
      </tr>
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM4.0</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 4 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Bakterie, některé pylové částice, průmyslové aerosoly</td>
      </tr>         
      <tr>
        <td style="padding: 6px; border: 1px solid #ddd;">PM10</td>
        <td style="padding: 6px; border: 1px solid #ddd;">≤ 10 µm</td>
        <td style="padding: 6px; border: 1px solid #ddd;">Pyl, prach, stavební materiály</td>
      </tr>
    </table><br>
  
    <strong>Proč jsou PM částice důležité?</strong><br>
    Dlouhodobé vystavení zvýšeným koncentracím PM může způsobovat dýchací potíže, kardiovaskulární problémy a další zdravotní komplikace.<br><br>

    <em>Čím menší částice, tím hlouběji mohou proniknout do lidského těla.</em><br><br>
    `;
      
    const vocText = `
    <strong>Co je VOC index?</strong><br>
    VOC index (Volatile Organic Compounds index) je číselný ukazatel kvality vzduchu založený na koncentraci těkavých organických sloučenin (VOC) v ovzduší.<br><br>

    VOC jsou různé organické chemikálie, které se snadno odpařují a mohou mít vliv na zdraví a komfort.<br><br>

    VOC index slouží jako indikátor kvality vnitřního vzduchu, protože vysoké hodnoty VOC mohou naznačovat znečištění nebo nepříjemné pachy.<br><br>

    <strong>Jak VOC index funguje?</strong><br>
    Senzory měří koncentraci různých VOC v ovzduší.<br>
    Tyto měření se zpracují algoritmem, který z hodnot vypočítá VOC index.<br><br>

    Hodnota VOC indexu obvykle spadá do určité škály (např. 0–500) a lze ji interpretovat takto:<br><br>

    <table style="width:100%; border-collapse: collapse; font-size: 0.85em;">
      <tr style="background: #333; color: white;">
        <th style="padding:4px; border:1px solid #ddd;">VOC index</th>
        <th style="padding:4px; border:1px solid #ddd;">Kvalita vzduchu</th>
        <th style="padding:4px; border:1px solid #ddd;">Význam</th>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">1 – 150</td>
        <td style="padding:4px; border:1px solid #ddd;">Dobrá</td>
        <td style="padding:4px; border:1px solid #ddd;">Nízká koncentrace VOC, zdravý vzduch</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">151 – 200</td>
        <td style="padding:4px; border:1px solid #ddd;">Přijatelná</td>
        <td style="padding:4px; border:1px solid #ddd;">Mírně zvýšená koncentrace VOC</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">201 – 300</td>
        <td style="padding:4px; border:1px solid #ddd;">Zvýšená</td>
        <td style="padding:4px; border:1px solid #ddd;">Zhoršená kvalita, může způsobit nepohodlí</td>
      </tr>
      <tr>
        <td style="padding:4px; border:1px solid #ddd;">301 – 500</td>
        <td style="padding:4px; border:1px solid #ddd;">Velmi špatná</td>
        <td style="padding:4px; border:1px solid #ddd;">Vysoká koncentrace, doporučuje se větrat</td>
      </tr>
    </table><br><br>
    `;

    const noxText = `
    <strong>Co je NOx index?</strong><br>
    NOx index (oxidů dusíku) je číselný ukazatel kvality vzduchu založený na koncentraci oxidů dusíku (NO a NO₂) v ovzduší.<br><br>

    Oxidy dusíku vznikají hlavně spalováním fosilních paliv a mohou dráždit dýchací cesty a zhoršovat kvalitu ovzduší.<br><br>

    <strong>Jak NOx index funguje?</strong><br>
    Senzory měří koncentraci NO a NO₂ a podle hodnot vypočítají NOx index.<br><br>

    <strong>Hodnoty NOx indexu a jejich význam:</strong><br><br>

<table style="width:100%; border-collapse: collapse; font-size: 0.85em;">
  <tr style="background: #333; color: white;">
    <th style="padding:4px; border:1px solid #ddd;">NOx index</th>
    <th style="padding:4px; border:1px solid #ddd;">Kvalita vzduchu</th>
    <th style="padding:4px; border:1px solid #ddd;">Význam</th>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">0 ÷ 1</td>
    <td style="padding:4px; border:1px solid #ddd;">Výborná</td>
    <td style="padding:4px; border:1px solid #ddd;">Minimální oxidy dusíku</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">2 ÷ 50</td>
    <td style="padding:4px; border:1px solid #ddd;">Dobrá</td>
    <td style="padding:4px; border:1px solid #ddd;">Nízká koncentrace NOx</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">51 ÷ 100</td>
    <td style="padding:4px; border:1px solid #ddd;">Přijatelná</td>
    <td style="padding:4px; border:1px solid #ddd;">Mírně zvýšená koncentrace</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">101 ÷ 200</td>
    <td style="padding:4px; border:1px solid #ddd;">Střední</td>
    <td style="padding:4px; border:1px solid #ddd;">Zvýšená koncentrace, možné zdravotní obtíže</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">201 ÷ 300</td>
    <td style="padding:4px; border:1px solid #ddd;">Špatná</td>
    <td style="padding:4px; border:1px solid #ddd;">Vysoká koncentrace – větrat doporučeno</td>
  </tr>
  <tr>
    <td style="padding:4px; border:1px solid #ddd;">301 ÷ 500</td>
    <td style="padding:4px; border:1px solid #ddd;">Velmi špatná</td>
    <td style="padding:4px; border:1px solid #ddd;">Velmi vysoká koncentrace – škodlivé</td>
  </tr>
</table>
<br><br>
    `;

    const table = document.querySelector('.mainTable');
    const infoBox = document.getElementById('infoBox');
    infoBox.style.width = table.offsetWidth + 'px';

    // Přehled textů pro jednotlivé veličiny
    const tooltipTexts = {
      co2: co2Text,
      pm1: pmText,
      pm2p5: pmText,
      pm4: pmText,
      pm10: pmText,
      voc: vocText, 
      nox: noxText
    };

    let currentlyVisible = null; // sleduje, který text je právě zobrazený

    function addTooltipListener(rowId, text) {
      const row = document.getElementById(rowId);
      if (!row) return;
      const labelCell = row.querySelector('td[data-label="Veličina"]');
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

    // Aplikuj na všechny relevantní veličiny
    Object.entries(tooltipTexts).forEach(([id, text]) => {
      addTooltipListener(id, text);
    });

    // Kliknutí mimo tabulku schová infoBox
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

    // Po načtení stránky zobrazíme tabulku
    window.onload = () => showSection('table');

    // Nastavit výchozí zobrazení
    showSection('table');

  </script>

</body>
</html>
  )rawliteral";

#endif