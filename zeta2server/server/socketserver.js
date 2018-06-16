'use strict';
const SocketIO = require('socket.io');

class SocketServer {
  constructor(server) {
    this.io = SocketIO(server);

    // register socket io events
    this.io.on('connection', this._onConnection.bind(this));
  }

  _onConnection(socket) {
    if (socket.forceDisconnected) return;
    console.log("connection received from:" + socket.handshake.address);


    if (game.isServerFull) {
      socket.emit(Packages.PROTOCOL.GENERAL.TO_CLIENT.ERROR, {
        payload: {
          reason: Packages.PROTOCOL.GENERAL.ERRORS.NO_FREE_SLOT_AVAILABLE_ON_SERVER
        }
      });
      console.log(socket.handshake.address, "wants to connect to a full server:", gameID);
      socket.disconnect();
      return;
    }

    socket.join(gameID);

    /*  var onevent = socket.onevent;
      socket.onevent = function (packet) {
          onevent.call (this, packet);
          if(socket.request.session) {
              socket.request.session.touch();
          }
      };*/


    game.onConnectionReceived(socket);
  }
}

module.exports = SocketServer;