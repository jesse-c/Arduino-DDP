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
 *
 *    @host
 *    @path
 *    @port
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
bool DDP::connect() {
  // Prepare message
  JsonObject& root = _jsonBuffer.createObject();
  root["msg"]     = "connect";
  root["version"] = "1";
  JsonArray& support = _jsonBuffer.createArray();
  support.add("1");
  root["support"] = support;

  char buffer[200];
  root.printTo(buffer, sizeof(buffer));

  // Send message
  _webSocketClient.sendData(buffer);
  delay(_pause);
  
  /*
   * Handle response
   *
   * connected (server->client)
   *    @session: string (an identifier for the DDP session)
   * failed (server->client)
   *    @version: string (a suggested protocol version to connect with)
   */
  String response;
  _webSocketClient.getData(response);

  if (response.length() > 0) {
    Serial.print("Received response: ");
    Serial.println(response);
  }

  bool status = false;

  if (response.indexOf("failed") >= 0) {
    status = false;
  } else if (response.indexOf("connected") >= 0) {
    status = true;
    // {"msg":"connected","session":"zwKbMXqs7jcKrke4Y"}
    _session = response.substring(30, 47);
  }

  return status;

  /*
  root = _jsonBuffer.parseObject(response);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  char* msg = root["msg"];
  String msgs(msg);

  if (msgs.equals("failed")) {
    Serial.println("failed");
  } else if (msgs.equals("connected")) {
    Serial.println("connected");
  }
  */
}

/*
 * listen
 */
void DDP::listen() {
  // TODO Check for Meteor connection too
  while(_client.connected()) {
    String data;
    _webSocketClient.getData(data);

    if (data.length() == 0) {
      Serial.println("No data...");
      delay(_pause);
      continue;
    }

    Serial.println("------------------------------");
    Serial.print("data: ");
    Serial.println(data);

    // Ping
    if (data.indexOf("ping") >= 0) {
      Serial.println("ping");

      Serial.println("Pong-ing");

      // TODO Call pong with optional ID
      if (data.length() > 14) {
        //String id = data.substring();
        void pong(/* id */);
      } else {
        void pong();
      }

      continue;
    }

    // Pong
    if (data.indexOf("pong") >= 0) {
      Serial.println("pong");

      continue;
    }

    delay(_pause);
  }
}

/*
 * ping
 *    @id   optional string (identifier used to correlate with response)
 */
void DDP::ping(String id /* = "" */) {
  JsonObject& root = _jsonBuffer.createObject();
  root["msg"] = "ping";

  if (id.length() > 0) {
    root["id"] = id;
  }

  char buffer[200];
  root.printTo(buffer, sizeof(buffer));

  _webSocketClient.sendData(buffer);
}

/*
 * pong
 *    @id   optional string (same as received in the ping message)
 */
void DDP::pong(String id /* = "" */) {
  JsonObject& root = _jsonBuffer.createObject();
  root["msg"] = "pong";

  if (id.length() > 0) {
    root["id"] = id;
  }

  char buffer[200];
  root.printTo(buffer, sizeof(buffer));

  _webSocketClient.sendData(buffer);
}

// Private Methods /////////////////////////////////////////////////////////////
