const char SITE_index[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" /> 
<link rel="stylesheet" href="http://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.css" />
<script src="http://code.jquery.com/jquery-1.11.1.min.js"></script>
<script src="http://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.js"></script>
<script type="text/javascript">
    if (typeof jQuery == 'undefined') {
        document.write(unescape('%3Clink rel="stylesheet" type="text/css" href="\\jquery.mobile-1.4.5.min.css" /%3E'));
        document.write(unescape('%3Cscript type="text/javascript" src="\\jquery-1.11.1.min.js" %3E%3C/script%3E'));
        document.write(unescape('%3Cscript type="text/javascript" src="\\jquery.mobile-1.4.5.min.js" %3E%3C/script%3E'));
    }
</script>

<body>
  <div data-role="page" data-theme="b">
    <div data-role="header">
      <h1>Zero-G Fountain</h1>
    </div>

    <div id="main" data-role="main" class="ui-content">

      <h1>Fountain:</h1>
      
      <div data-role="content">
        <div class="ui-grid-a">
            <div class="ui-block-a socket"><a href="#" data-role="button" data-icon="check">ON</a> </div>
            <div class="ui-block-b socket"> <a href="#" data-role="button" data-icon="delete">OFF</a> </div>
        </div>
      </div>

      <h1>Modes:</h1>
      <div data-role="collapsible" id="SensorCollapsible" style="margin-left: 20px; margin-right: 20px;">
        <h4>Fountain Modes</h4>
        <input class="btnmode" value="Off" type="button" 
          style="background-image: url(https://upload.wikimedia.org/wikipedia/commons/thumb/d/d4/Off_font_awesome.svg/512px-Off_font_awesome.svg.png);">
        <input class="btnmode" value="Light On" type="button" 
          style="background-image: url(https://upload.wikimedia.org/wikipedia/commons/thumb/a/a3/Light_bulb_%28yellow%29_icon.svg/2000px-Light_bulb_%28yellow%29_icon.svg.png);">
        <input class="btnmode" value="Zero-G Standard" type="button" 
          style="background-image: url(https://upload.wikimedia.org/wikipedia/commons/7/79/Water_Drop_Icon_Vector.png);">
        <input class="btnmode" value="RGB" type="button"
          style="background-image: url(https://cdn.pixabay.com/photo/2017/04/29/05/56/rgb-2270087_960_720.png);">
        <input class="btnmode" value="Music Visualizer" type="button"
          style="background-image: url(https://upload.wikimedia.org/wikipedia/commons/thumb/9/91/Antu_view-media-equalizer.svg/2000px-Antu_view-media-equalizer.svg.png); ">
        <input class="btnmode" value="No Light" type="button" 
          style="background-image: url(https://upload.wikimedia.org/wikipedia/commons/thumb/d/d0/Fountain_icon.svg/2000px-Fountain_icon.svg.png ); ">
      </div>

      <div id="Off" class="sensor" style="display: none;">
       <img src="https://cdn3.iconfinder.com/data/icons/pix-glyph-set/50/520753-not_allowed-512.png" alt="image" style="display: block; margin-left: auto; margin-right: auto; width: 20vh;"/>
      </div>
      
      <div id="Light On" class="sensor" style="display: none">
        <div class="ui-field-contain" style="padding-top: 1.33em">
          <label for="flip-2" style="width: auto"><h4 style="-webkit-margin-before: 0px;">Activate:</h4></label>
          <select name="flip-2" class="modeActiveButton" data-role="flipswitch" data-mini="true" data-theme="b"><option value="modeInactive">OFF</option><option value="modeActive">ON</option></select>
        </div>
        <div id="BrightnessSlider" class="ui-slider-slider" style="text-align: center; color:white; text-shadow: black 0.1em 0.1em 0.2em;""> 
          <label style="font-size: 130%;">Brightness :</label>
          <input class="SensorConfigSlider1" id="Temperature" type="range" min="0" max="100" value="50" data-theme="a" data-track-theme="b" />
        </div>
      </div>

      <div id="Zero-G Standard" class="sensor" style="display: none">
        <div class="ui-field-contain" style="padding-top: 1.33em">
          <label for="flip-2" style="width: auto"><h4 style="-webkit-margin-before: 0px;">Activate:</h4></label>
          <select name="flip-2" class="modeActiveButton" data-role="flipswitch" data-mini="true" data-theme="b"><option value="modeInactive">OFF</option><option value="modeActive">ON</option></select>
        </div>
        <div id="BrightnessSlider" class="ui-slider-slider" style="text-align: center; color:white; text-shadow: black 0.1em 0.1em 0.2em;""> 
          <label style="font-size: 130%;">Brightness(%) :</label>
          <input class="SensorConfigSlider1" id="Brightness" type="range" min="0" max="100" value="50" data-theme="a" data-track-theme="b" />
        </div>
        <div id="FrequencySlider" class="ui-slider-slider" style="text-align: center; color:white; text-shadow: black 0.1em 0.1em 0.2em;""> 
          <label style="font-size: 130%;">Frequency(Hz) :</label>
          <input class="SensorConfigSlider2" id="Frequency" type="range" min="1" max="200" value="50" data-theme="a" data-track-theme="b" />
        </div>
        <h4>Meassurement:</h4>
        <textarea name="textarea" disabled id="textarea-a" cols="1" rows="1" style="color: #FFFFFF; opacity: 0.8; padding-left: -100px;">
          Calculating Frequency ...
        </textarea>
      </div>

      <div id="RGB" class="sensor" style="display: none">
        <div class="ui-field-contain" style="padding-top: 1.33em">
          <label for="flip-2" style="width: auto"><h4 style="-webkit-margin-before: 0px;">Activate:</h4></label>
          <select name="flip-2" class="modeActiveButton" data-role="flipswitch" data-mini="true" data-theme="b"><option value="modeInactive">OFF</option><option value="modeActive">ON</option></select>
        </div>
      </div>

      <div id="Music Visualizer" class="sensor" style="display: none">
        <div class="ui-field-contain" style="padding-top: 1.33em">
          <label for="flip-2" style="width: auto"><h4 style="-webkit-margin-before: 0px;">Activate:</h4></label>
          <select name="flip-2" class="modeActiveButton" data-role="flipswitch" data-mini="true" data-theme="b"><option value="modeInactive">OFF</option><option value="modeActive">ON</option></select>
        </div>
        <div id="BrightnessSlider" class="ui-slider-slider" style="text-align: center; color:white; text-shadow: black 0.1em 0.1em 0.2em;""> 
          <label style="font-size: 130%;">Update Speed :</label>
          <input class="SensorConfigSlider1" id="Temperature" type="range" min="0" max="100" value="50" data-theme="a" data-track-theme="b" />
        </div>
      </div>
      
      <div id="No Light" class="sensor" style="display: none;">
       <img src="https://cdn3.iconfinder.com/data/icons/pix-glyph-set/50/520753-not_allowed-512.png" alt="image" style="display: block; margin-left: auto; margin-right: auto; width: 20vh;"/>
      </div>

      <br>
      <h1>Information</h1>
      <div data-role="collapsible" style="margin-left: 20px; margin-right: 20px"><h4>Device Info</h4>
        <ul id="info">
          <li><b>BootUp Time: </b></li>
          <li><b>Ip Adress: </b></li>
        </ul>
      </div>

      </div>
          <div data-role="footer" data-position="fixed">
          <h3 id="footer">(C) MIT License Nico Kuhn / Raphael Wirth 2018</h3>
      </div>  

  </div>
</body>

  <style>
      /*First rule hides the text box, the second one makes slider full width*/
      div.ui-slider-slider input.ui-input-text {
        display: none;
      }
      div.ui-slider-slider div.ui-slider-track {
        #margin: 0 15px 0 15px !important;
      }
      input[type=range]::-webkit-slider-thumb {
        -webkit-appearance: none;
        border: none;
        height: 16px;
        width: 16px;
        border-radius: 50%;
        background: goldenrod;
        margin-top: -4px;
      }
      .ui-collapsible-heading .ui-btn  {
          text-align: center;
      }
      span > img {
          color: white;
      }
      span > input {
          background: none;
          color: white;
          padding: 0;
          border: 0;
      }

      input.btnmode {
        opacity: 1; 
        filter: brightness(0) invert(1);
        background-repeat: no-repeat; 
        background-position: right; 
        background-size: 40px 40px;
      }
      .sensor {
        margin-left: 20px;
        margin-right: 20px;
      }
      
    </style>

  <script type="text/javascript">
    $( ".socket" ).bind( "click", function(e) {
      var command = e.target.innerHTML;
      console.log("Command: " + command);
      //Disable all Sensor Functions
      $('.modeActiveButton').each(function(i, btn) {
        btn.value = "sensorInactive";
      });
      $( ".modeActiveButton" ).flipswitch( "refresh" );
      connection.send(command);
    });

    $( ".btnmode" ).bind( "click", function(e) {
      var type = e.target.value;
      console.log("Mode: " + type);
      //Update Sensor Specific Menu
      var divsToHide = document.getElementsByClassName("sensor"); //divsToHide is an array
      for(var i = 0; i < divsToHide.length; i++){
          divsToHide[i].style.display = "none"; // depending on what you're doing
      }
      document.getElementById(type).style.display = "block";
      $('#SensorCollapsible').collapsible( "collapse" );

      //Update Collapsable Header
      document.getElementById("SensorCollapsible").firstChild.firstChild.innerHTML = type + '<img src=' + e.target.style.backgroundImage.slice(4, -1) + 'alt="US Flag" style="postion: absolute; float:right; filter: brightness(0) invert(1); width:40px; height:40px; margin-top: -10px; margin-bottom: -15px" />'
      //Disable all Sensor Functions
      $('.modeActiveButton').each(function(i, btn) {
        btn.value = "sensorInactive";
      });
      $( ".modeActiveButton" ).flipswitch( "refresh" );

      connection.send(document.getElementsByClassName("modeActiveButton")[0].value)
      connection.send(type);
    });

    $( ".modeActiveButton" ).change(function(e){
      connection.send(e.target.value)
    });

    //Sensor Configuration Slider
    $( ".ui-slider-slider" ).on( "slidestop", function( event, ui ) {
      connection.send("modeConfig1:"+ event.target.value);
    } );

    //Websocket
    var connection = new WebSocket('ws://' + location.host + ':81/', ['arduino']);
    connection.onopen = function () {
      connection.send('Connect ' + new Date());
    };
    connection.onerror = function (error) {
      console.log('WebSocket Error ', error);
    };
    connection.onmessage = function (e) {
      console.log('Server: ', e.data);
      parseWsData(e.data)
    };
    connection.onclose = function () {
      console.log('WebSocket connection closed');
    };

    function parseWsData(data) {
      var obj = JSON.parse(data);
      //update device info
      var items = document.getElementById("info").childNodes;
      obj.data.Mode != 0 ? document.getElementsByClassName("sensor")[obj.data.Mode].style.display = "block" : null;
      items[1].innerHTML = "<b>BootUp Time: "+"</b>"+obj.uptime; 
      items[3].innerHTML = "<b>IP Adress: "+"</b>"+obj.ip;
      //update Sensor Function Activation Buttons
      $('.modeActiveButton').each(function(i, btn) {
        btn.value = obj.data.modeActive;
      });
      $( ".modeActiveButton" ).flipswitch( "refresh" );
      //update Mode Menu
      obj.data.Mode != 0 ? document.getElementById("SensorCollapsible").firstChild.firstChild.innerHTML = $(".btnmode")[obj.data.Mode].value + '<img src=' + $(".btnmode")[obj.data.Mode].style.backgroundImage.slice(4, -1) + 'alt="US Flag" style="postion: absolute; float:right; filter: brightness(0) invert(1); width:40px; height:40px; margin-top: -10px; margin-bottom: -15px" />' : null
      //update Current Mode Value
      obj.data.Mode != 0 && obj.data.modevalue != undefined ? document.getElementsByClassName("sensor")[obj.data.Mode].querySelector('#textarea-a').innerHTML = obj.data.modevalue + " " + obj.data.unit : null
      //update Current Mode Config1 Slider
      obj.data.modeConfig1 != -1 && obj.data.modeConfig1 != undefined ? document.getElementsByClassName("sensor")[obj.data.Mode].getElementsByClassName("SensorConfigSlider1")[0].value = obj.data.modeConfig1 : null
      $( ".SensorConfigSlider1" ).slider( "refresh" );
    }
  </script>
)=====";
