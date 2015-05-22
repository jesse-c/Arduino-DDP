/*
  DDP library for Arduino
*/

#include "DDP.h"

// Constructor /////////////////////////////////////////////////////////////////
DDP::DDP() {
}

// Public Methods //////////////////////////////////////////////////////////////
/*
  setup
 */
 
boolean DDP::setup(String host, String path /* = "/" */, int port /* = 80 */) {
  // Save args
  _host = host;
  _path = path;
  _port = port;
  
  boolean connected = false;
    
  // Connect to the websocket server
  if (_client.connect("echo.websocket.org", _port)) {
    Serial.println("Connected to server");
    connected = true;
    
    // Handshake with server
    if (_webSocketClient.handshake(_client)) {
      Serial.println("Handshake successful");
      connected = true;
    } else {
      Serial.println("Handshake failed.");
      connected = false;
    }
  } else {
    Serial.println("Connection to server failed.");
    connected = false;
  }
  
  return connected;
}
 
/*
  connect (client -> server)
  
  session: string (if trying to reconnect to an existing DDP session)
  version: string (the proposed protocol version)
  support: array of strings (protocol versions supported by the client, in order of preference)  
 */
void DDP::connect(String session, String version, int support[]) {
  // Send message
  
  
  // Handle response
  /*
  connected (server->client)
    session: string (an identifier for the DDP session)
  OR
  failed (server->client)
    version: string (a suggested protocol version to connect with)
  */
  
  
}

// Private Methods /////////////////////////////////////////////////////////////

void DDP::_doSomethingSecret(void) {
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
}

