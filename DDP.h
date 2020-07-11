/*
  DDP library for Arduino

  http://www.arduino.cc/en/Hacking/LibraryTutorial
*/

#ifndef DDP_h
#define DDP_h

#include <Arduino.h>
#include <Ethernet.h>
#include "./libs/Arduino-Websocket/WebSocketClient.h"
//#include "./libs/ArduinoWebsocketClient/WebSocketClient.h"
#include "./libs/ArduinoJson/ArduinoJson.h"

class DDP {
  public:
    DDP();
    
    /**************************************************************************
     * WebSocket    
     *************************************************************************/
    bool setup(String host, String path = "/", int port = 80);
    bool handshake();

    void output();
    void waitFor();
    
    /**************************************************************************
     * DDP    
     *************************************************************************/
    bool connect();
    //void connect(String session = "", int version = 1, int support[] = DDP_Versions);

    // Listen
    void listen();

    /* Heartbeats ************************************************************/
    void ping(String id = "");
    void pong(String id = "");

    /* Managing data *********************************************************/
    // client -> server
    void sub();
    void unsub();
    // server -> client
    void nosub();
    void added();
    void changed();
    void removed();
    void ready();
    // addedBefore();
    // movedBefore()

    /* Remote procedure calls ************************************************/
    // client -> server
    void method(int readR, int readG, int readB);
    // server-> client
    void result();
    void updated();

    /* Sub *******************************************************************/
    bool subsReady();
    /* RGB *******************************************************************/
    int getR();
    int getG();
    int getB();

  private:
    int _pause = 1000;
    int _timer = 1;

    WebSocketClient _webSocketClient;
    EthernetClient _client;

    String _host;
    String _path;
    int _port;

    String _session;

    StaticJsonBuffer<200> _jsonBuffer;
    JsonObject& _root = _jsonBuffer.createObject();

    /* Subscription */
    bool _readyR = false;
    bool _readyG = false;
    bool _readyB = false;
    bool _addedR = false;
    bool _addedG = false;
    bool _addedB = false;
    // R, G, B
    int _r = 0;
    int _g = 0;
    int _b = 0;


    //const int DDP_Versions[] = {1};

    //int _value;
    //void _doSomethingSecret(void);

};

#endif
