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
  if (api_lines.lenth) {
    var line = api_lines.shift(),
        dictionary = {
          'LINE_NAME': line.name,
          'LINE_NUM': line.num
        };
    
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
  }
}