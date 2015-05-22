/*
  DDP library for Arduino

  http://www.arduino.cc/en/Hacking/LibraryTutorial
*/

#ifndef DDP_h
#define DDP_h

#include <Arduino.h>
#include <Ethernet.h>
#include "/Users/jesse/Dropbox/university/engg4802/src/examples/pwm/hardware/pwm/libs/Arduino-Websocket/WebSocketClient.h"
#include "/Users/jesse/Dropbox/university/engg4802/src/examples/pwm/hardware/pwm/libs/ArduinoJson/ArduinoJson.h"

const int DDP_Versions[] = {1};

class DDP {
  public:
    DDP();
    
    // WebSocket
    bool setup(String host, String path = "/", int port = 80);
    
    // DDP    
    void connect(String session, String version, int support[]);
    
  private:
    WebSocketClient _webSocketClient;
    EthernetClient _client;
    String _host;
    String _path;
    int _port;
    int _value;
    void _doSomethingSecret(void);
    StaticJsonBuffer<200> jsonBuffer;
};

#endif
