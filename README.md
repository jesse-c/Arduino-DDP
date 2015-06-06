Arduino DDP library
==================

A simple implementation of [DDP (version 1)](https://github.com/meteor/meteor/blob/devel/packages/ddp/DDP.md) for Arduinos.

_Primarily created for use in my [thesis project](https://github.com/jesse-c/thesis-sensor) for [CEIT](http://ceit.uq.edu.au) at [The University of Queensland](http://www.uq.edu.au). At the moment it's specific to my project though that will change._


Quick start
----------
    DDP ddp = DDP();

    void setup() {
      if (ddp.setup("192.168.0.1", 3000)) {
        if (ddp.connect()) {
        }
      }
    }

    void loop() {
      ddp.sub();
      ddp.listen();
    }

[Being used in my thesis project](https://github.com/jesse-c/thesis-sensor)

Documentation
-------------
#### Connecting
    bool setup(String host, String path ="/", int port = 80)
Partialyl implemented. `host` params not currently used.

    bool connect()
Im

#### Listening
    void listen()
After a connection has successfully been made, `listen()` checks for any data on the line and if any is found, what type of message it was. It then calls the appropriate function.

#### Heartbeats
    void ping(String id = "")
Partially implemented—doesn't yet response with ID if one was included.

    void pong(String id = "")
Partially implemented—doesn't yet response with ID if one was included.

#### Managing data
#### Client → Server
    void sub()
Partiually implemented

    void unsub()
Not yet implemeneted

#### Server → Client
    void nosub()
Not yet implemeneted

    void added()
Partially implemented.

    void changed()
Partially implemented.

    void removed()
Not yet implemeneted

    void ready()
Partially implemented.

    void addedBefore()
Not yet implemeneted

    void movedBefore()
Not yet implemeneted

#### Remote procedure calls
#### Client → Server
    void method()
Partially implemented.

#### Server → Client
    void result()
Partially implemented.

    void updated()
Partially implemented.

Dependencies
-----------

[ArduinoJson](https://github.com/bblanchon/ArduinoJson)

[Arduino-Websocket](https://github.com/brandenhall/Arduino-Websocket)

At the moment these work by using absolute paths for them which is not ideal.

Notes
-----
n/a

Compatability
------------
Tested on:

- Arduino Mega 2560

TODO
----
- Add tests

Contributing
-------------
Contributions are welcome!
