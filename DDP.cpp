/*
  DDP library for Arduino
*/

#include "DDP.h"

// Constructor /////////////////////////////////////////////////////////////////
DDP::DDP() {
}

// Public Methods //////////////////////////////////////////////////////////////
/*
 * setup
 */
 
bool DDP::setup(String host, String path /* = "/" */, int port /* = 80 */) {
  // Save args
  _host = host;
  _path = path;
  _port = port;
  
  bool connected = false;
    
  // Connect to the websocket server
  if (_client.connect("192.168.0.3", 3000)) {
    Serial.println("Connected to server");
    connected = true;
    
    // Handshake with server
    connected = handshake();
  } else {
    Serial.println("Connection to server failed.");
    connected = false;
  }
  
  return connected;
}

/*
 *
 */
bool DDP::handshake() {
  // Handshake with server
  _webSocketClient.path = "/websocket";
  _webSocketClient.host = "192.168.0.3";

  if (_webSocketClient.handshake(_client)) {
    Serial.println("Handshake successful");
    return true;
  } else {
    Serial.println("Handshake failed.");
    return false;
  }
}

/*
 *
 */
void DDP::output() {
  String data;
  _webSocketClient.getData(data);

  if (data.length() > 0) {
    Serial.print("Received data: ");
    Serial.println(data);
  }
}
 
/*
 * connect (client -> server)
 */
void DDP::connect() {
  // Prepare message
  JsonObject& root = jsonBuffer.createObject();
  root["msg"]     = "connect";
  root["version"] = "1";
  JsonArray& array = jsonBuffer.createArray();
  array.add("1");
  root["support"] = array;

  char buffer[256];
  //root.printTo(Serial);
  root.printTo(buffer, sizeof(buffer));

  // Send message
  _webSocketClient.sendData(buffer);
  delay(500);
  
  // Handle response
  /*
  connected (server->client)
    session: string (an identifier for the DDP session)
  OR
  failed (server->client)
    version: string (a suggested protocol version to connect with)
  */
  String response;
  _webSocketClient.getData(response);

  if (response.length() > 0) {
    Serial.print("Received response: ");
    Serial.println(response);
  }

  if (response.indexOf("failed") >= 0) {
    Serial.println("failed");
  } else if (response.indexOf("connected") >= 0) {
    Serial.println("connected");
  }

  /*
  root = jsonBuffer.parseObject(response);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  String msg = root["msg"];

  if (msg.equals("failed")) {
    Serial.println("failed");
  }
  */
}

/*
 * connect (client -> server)
 *
 * session: string (if trying to reconnect to an existing DDP session)
 * version: string (the proposed protocol version)
 * support: array of strings (protocol versions supported by the client, in order of preference)  
 */
/*
void DDP::connect(String session, int version, int support[]) {
  // Prepare message
  JsonObject& root = jsonBuffer.createObject();
  root["sensor"] = "gps";

  // Send message
  
  
  // Handle response
  connected (server->client)
    session: string (an identifier for the DDP session)
  OR
  failed (server->client)
    version: string (a suggested protocol version to connect with)
  
  
}
*/

// Private Methods /////////////////////////////////////////////////////////////

void DDP::_doSomethingSecret(void) {
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
}

