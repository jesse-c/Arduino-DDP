/*
  DDP library for Arduino

  http://www.arduino.cc/en/Hacking/LibraryTutorial
*/

#ifndef DDP_h
#define DDP_h

#include <Arduino.h>
#include <Ethernet.h>
#include "/Users/jesse/Dropbox/university/engg4802/src/examples/pwm/hardware/pwm/libs/Arduino-Websocket/WebSocketClient.h"
//#include "/Users/jesse/Dropbox/university/engg4802/src/examples/pwm/hardware/pwm/libs/ArduinoWebsocketClient/WebSocketClient.h"
#include "/Users/jesse/Dropbox/university/engg4802/src/examples/pwm/hardware/pwm/libs/ArduinoJson/ArduinoJson.h"

class DDP {
  public:
    DDP();
    
    // WebSocket
    bool setup(String host, String path = "/", int port = 80);
    bool handshake();

    void output();
    
    // DDP    
    //void connect(String session = "", int version = 1, int support[] = DDP_Versions);
    void connect();
    
  private:
    WebSocketClient _webSocketClient;
    EthernetClient _client;

    String _host;
    String _path;
    int _port;

    StaticJsonBuffer<200> jsonBuffer;

    int _value;
    void _doSomethingSecret(void);

    //const int DDP_Versions[] = {1};
};

#endif
