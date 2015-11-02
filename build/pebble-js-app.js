var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getLines() {
  var url = "http://busal.es/api/v0.1/lines";
  
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var api_lines = JSON.parse(responseText);
      
      recursive_send(api_lines);
      
    }      
  );
  
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('Getting busal lines');
    getLines();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    getLines();
  }                     
);

function recursive_send(api_lines) {
  console.log('Recursive...');  
  if (api_lines.length) {    
    var line = api_lines.shift(),
      dictionary;      

      dictionary = {
        'LINE_NAME'       : line.name.replace(/[^\x00-\x7F]/g, ""),
        'LINE_NUM'        : line.num.replace(/[^\x00-\x7F]/g, ""),
        'LINE_DIRECTION1' : line.direccion1,
        'LINE_DIRECTION2' : line.direccion2
      };
    
    console.log("Sending: " + line);
    
    try{
      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          recursive_send(api_lines);
          console.log('Line sent to Pebble successfully!');
        },
        function(e) {
          console.log('Error sending line info to Pebble!');
        }     
      );
    } catch(ex){
      console.log(ex);
    }
      
  } else {
    Pebble.sendAppMessage({
      'MSG_END': '1'
    },
    function(e) {
      console.log('End of lines sent to Pebble successfully!');
    },
    function(e) {
      console.log('Error sending end of lines info to Pebble!');
    });
  }
}