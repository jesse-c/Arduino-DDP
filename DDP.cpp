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

void DDP::waitFor() {
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
  
  // Wait up to ~2.5 seconds for sid msg
  // {"sid":"e82638f4-2122-47c9-aea2-12189b6866dd","msg":"streamy$sid"}
  int sid = 0;
  _webSocketClient.getData(response);
  while (response.indexOf("sid") == -1 && sid < 5) {
    Serial.println("Waiting for sid msg");
    sid++;
    delay(_pause);
  }
  if (sid == 5) {
    Serial.println("Stopped waiting for sid msg");
  } else {
    Serial.println("Got sid msg");
  }

  return status;

  /* TODO Use parseObject
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

    /* Heartbeats ************************************************************/
    // Ping
    if (data.indexOf("ping") >= 0) {
      Serial.println("ping");

      Serial.println("Pong-ing");

      // TODO Call pong with optional ID
      if (data.length() > 14) {
        //String id = data.substring();
        pong(/* id */);
      } else {
        pong();
      }

      continue;
    }

    // Pong
    if (data.indexOf("pong") >= 0) {
      Serial.println("pong");

      continue;
    }

    /* Managing data *********************************************************/
    /*
     * nosub
     */
    if (data.indexOf("nosub") >= 0) {
      Serial.println("nosub");

      continue;
    }
    /*
     * added
     *
     * Assume already 1 document with initial value
     *
     * Example:
     *
     * {"msg":"added","collection":"rgb","id":"LM8HndauPqxBHqJ2b","fields":{"r":0,"g":0,"b":0}}
     * {"msg":"added","collection":"#r","id":"yq2q2QtmjKn2oPLCv","fields":{"value":0}}
     * {"msg":"added","collection":"#r","id":"yq2q2QtmjKn2oPLCv","fields":{"value":"100"}}
     */
    if (data.indexOf("added") >= 0) {
      Serial.println("added");

      if (data.indexOf("#r") >= 0) {
        String value = data.substring(77, 80);
        Serial.println("value: " + value);
        _r = value.toInt();
        Serial.print("_r: ");
        Serial.println(_r);
      }
      if (data.indexOf("#g") >= 0) {
        String value = data.substring(77, 80);
        Serial.println("value: " + value);
        _g = value.toInt();
        Serial.print("_g: ");
        Serial.println(_g);
      }
      if (data.indexOf("#b") >= 0) {
        String value = data.substring(77, 80);
        Serial.println("value: " + value);
        _b = value.toInt();
        Serial.print("_b: ");
        Serial.println(_b);
      }

      continue;
    }
    /*
     * changed
     *
     * Example:
     *
     * {"msg":"changed","collection":"#g","id":"BwRE6F3fgkfEJgt7P","fields":{"value":"011"}}
     */
    if (data.indexOf("changed") >= 0) {
      Serial.println("changed");

      if (data.indexOf("#r") >= 0) {
        String value = data.substring(79, 82);
        Serial.println("value: " + value);
        _r = value.toInt();
        Serial.print("_r: ");
        Serial.println(_r);
      }
      if (data.indexOf("#g") >= 0) {
        String value = data.substring(79, 82);
        Serial.println("value: " + value);
        _g = value.toInt();
        Serial.print("_g: ");
        Serial.println(_g);
      }
      if (data.indexOf("#b") >= 0) {
        String value = data.substring(79, 82);
        Serial.println("value: " + value);
        _b = value.toInt();
        Serial.print("_b: ");
        Serial.println(_b);
      }

      continue;
    }
    /*
     * ready
     *
     * When one or more subscriptions have finished sending their initial batch of data, the server will send a ready message with their IDs.
     *
     * Example:
     *
     * {"msg":"ready","subs":["1"]}
     */
    if (data.indexOf("ready") >= 0) {
      Serial.println("ready");
      if (data.indexOf("1") >= 0) {
        Serial.println("R, ready");
        _readyR = true;
      }
      if (data.indexOf("2") >= 0) {
        Serial.println("G, ready");
        _readyG = true;
      }
      if (data.indexOf("3") >= 0) {
        Serial.println("B, ready");
        _readyB = true;
      }
      continue;
    }



    delay(_pause);
  }
}

/* Heartbeats ****************************************************************/
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
/* Managing data *************************************************************/
/*
 * sub
 *    @id       string (an arbitrary client-determined identifier for this subscription)
 *    @name     string (the name of the subscription)
 *    @params   optional array of EJSON items (parameters to the subscription)
 */
void DDP::sub() {
  // Subscribe to r, g ,b
  JsonObject& root = _jsonBuffer.createObject();
  root["msg"] = "sub";
  root["id"] = "1";
  root["name"] = "#r";

  char buffer[200];
  root.printTo(buffer, sizeof(buffer));

  _webSocketClient.sendData(buffer);

  delay(_pause);

  root["msg"] = "sub";
  root["id"] = "2";
  root["name"] = "#g";

  root.printTo(buffer, sizeof(buffer));

  _webSocketClient.sendData(buffer);

  delay(_pause);

  root["msg"] = "sub";
  root["id"] = "3";
  root["name"] = "#b";

  root.printTo(buffer, sizeof(buffer));

  _webSocketClient.sendData(buffer);
}

/* Remote procedure calls ****************************************************/
/*
 * method
 *
 *    @method       string (method name)
 *    @params       optional array of EJSON items (parameters to the method)
 *    @id           string (an arbitrary client-determined identifier for this method call)
 *    @randomSeed   optional JSON value (an arbitrary client-determined seed for pseudo-random generators)
 */
void DDP::method() {
  // Test call test
  JsonObject& root = _jsonBuffer.createObject();
  root["msg"] = "method";
  root["method"] = "test";
  JsonArray& params = _jsonBuffer.createArray();
  root["params"] = params;
  root["id"] = "1";

  char buffer[200];
  root.printTo(buffer, sizeof(buffer));

  _webSocketClient.sendData(buffer);

  // Don't move on till we've handled the expected result and updated msgs
  bool handledResult = false;
  bool handledUpdated = false;

  while (!handledResult || !handledUpdated) {
    String data;
    _webSocketClient.getData(data);

    // Examples
    // data: {"msg":"result","id":"1"}
    // {"msg":"result","id":"1","result":"test"}
    if (data.length() > 0 && data.indexOf("result") >= 0) {
      Serial.println("Handled method/result");

      // TODO Params


      /* TODO Errors
       *    @error      string
       *    @reason     optional string
       *    @details    optional string
       */


      handledResult = true;
    // data: {"msg":"updated","methods":["1"]}
    } else if (data.length() > 0 && data.indexOf("updated") >= 0) {
      Serial.println("Handled method/updated");

      handledUpdated = true;
    }

    if (!handledResult) {
      Serial.println("Waiting for method/result");
    }
    if (!handledUpdated) {
      Serial.println("Waiting for method/updated");
    }

    delay(_pause);
  }
}

/* Sub ***********************************************************************/
bool DDP::subsReady() {
  if (_readyR && _readyG && _readyB) {
    return true;
  } else {
    return false;
  }
}
/* RGB ***********************************************************************/
int DDP::getR() {
  return _r;
}
int DDP::getG() {
  return _g;
}
int DDP::getB() {
  return _b;
}

// Private Methods /////////////////////////////////////////////////////////////
