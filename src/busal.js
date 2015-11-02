var current_line,
  direction,
  cached_lines;

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getLines() {

  if (!cached_lines) {
    var url = "http://busal.es/api/v0.1/lines";
    
    xhrRequest(url, 'GET', 
      function(responseText) {
        // responseText contains a JSON object with weather info
        var api_lines = JSON.parse(responseText);
        cached_lines = api_lines;
        
        recursive_send(api_lines, process_line_data, {
          'MSG_END_LINE': '1'
        });
        
      }      
    );
  } else {
    console.log("Cached lines loaded");
    recursive_send(cached_lines, process_line_data, {
      'MSG_END_LINE': '1'
    });
  }
    
  
}

function getBusStops(line, direction) {  
  var url = "http://busal.es/api/v0.1/stops?idl=" + line;
  
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var api_stops = JSON.parse(responseText),
        stops_array;     

      if (direction === "1"){
        api_stops.direction1.sort(function(a, b) {
          if ( parseInt(a.id) > parseInt(b.id)){
              return 1;
          }
          if ( parseInt(a.id) < parseInt(b.id)){
              return -1;
          }
          return 0;
        }).reverse();

        stops_array = api_stops.direction1;
      } else {
        api_stops.direction2.sort(function(a, b) {
            if ( parseInt(a.id) > parseInt(b.id)){
                return 1;
            }
            if ( parseInt(a.id) < parseInt(b.id)){
                return -1;
            }
            return 0;
        });

        stops_array = api_stops.direction2;
      }
      
      recursive_send(stops_array, process_stop_data, {
        'MSG_END_BUS_STOP': '1'
      });
      
    }      
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('JS Ready');
    // getLines();
    sendReadyMessage();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    if (e.payload.JS_GET_LINES) {
      console.log('getLines()');
      getLines();  
    } else if (e.payload.JS_LINE_NUM) {
      console.log('save current line :' + e.payload.JS_LINE_NUM);
      var data = e.payload.JS_LINE_NUM.split(":");
      getBusStops(data[0], data[1]);
    }
    
  }                     
);

function sendReadyMessage() {
  try{
      // Send to Pebble
      Pebble.sendAppMessage({
        'JS_CONNECTION_READY': 1
      },
        function(e) {
          recursive_send(api_lines);
          console.log('ReadyMessage sent');
        },
        function(e) {
          console.log('Error sending ReadyMessage');
        }     
      );
    } catch(ex){
      console.log(ex);
    }
}

function recursive_send(data, processFunction, endKey) {
  console.log('Recursive...');  
  if (data.length) {    

    var dictionary = processFunction(data.shift());      
    
    try{
      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          recursive_send(data, processFunction, endKey);
          console.log('Data sent to Pebble successfully!');
        },
        function(e) {
          console.log('Error sending data info to Pebble!');
        }     
      );
    } catch(ex){
      console.log(ex);
    }
      
  } else {
    Pebble.sendAppMessage(endKey,
    function(e) {
      console.log('End of data sent to Pebble successfully!');
    },
    function(e) {
      console.log('Error sending end of data info to Pebble!');
    });
  }
}


function process_line_data(line) {  
  var dictionary = {
    'LINE_NAME'       : line.name.replace(/[^\x00-\x7F]/g, ""),
    'LINE_NUM'        : line.num.replace(/[^\x00-\x7F]/g, ""),
    'LINE_DIRECTION1' : line.direccion1,
    'LINE_DIRECTION2' : line.direccion2
  };
  return dictionary;
}

function process_stop_data(stop) {    
  console.log("Processing stop: " + stop.name);
  var dictionary = {
    'STOP_NAME'     : stop.name.replace(/[^\x00-\x7F]/g, ""),
    'STOP_NEXT_BUS' : stop.proxima
  };

  return dictionary;
}