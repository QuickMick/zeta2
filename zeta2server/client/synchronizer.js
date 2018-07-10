'use strict';

const Ticks = require('./../core/ticks.json');
const Packages = require("./../core/com");
const UpdateQueue = require('./../core/updatequeue');

const EventEmitter = require('events');

const EVT_ON_CLIENT_CONNECTED = "onClientConnected";
const EVT_ON_CLIENT_DISCONNECTED = "onClientDisconnected";

const EVT_ON_SERVER_ERROR = "onServerError";

const EVT_ON_SERVER_UPDATE = "onServerUpdate";
const EVT_ON_SERVER_UPDATE_SEPERATOR = "_";

/**
 * Receives all data from the server and changed data from the client and distributes it.
 */
class Synchronizer extends EventEmitter {
  constructor(supportedMessages) {
    super();
    this.socket = null;

    /**
     * used to detect updates which were done by the client
     * @type {EntityUpdateQueue}
     */
    this.updateQueue = new UpdateQueue();

    /**
     * socket to connect to the server
     * @type {null}
     */
    this.socket = null;

    /**
     * contains all supported statUpdate
     * @type {Set<any>}
     * @private
     */
    this._suppotedMessages = new Set();

    this.addSupportedMessages(supportedMessages);
  }

  /**
   * pass "TO_CLIENT" object of one section of the PROTOCOL from com.js
   * @param msgObject TO_CLIENT{msgName:"msg"}
   */
  addSupportedMessages(msgObject) {
    let cur = [].concat(msgObject);
    for (let curObj of cur) {
      for (let k in curObj) {
        this._suppotedMessages.add(curObj[k]);
      }
    }
  }

  removeSupportedMessages(msgObject) {
    let cur = [].concat(msgObject);
    for (let curObj of cur) {
      for (let k in curObj) {
        this._suppotedMessages.delete(curObj[k]);
      }
    }
  }

  start() {
    if (this.socket) {
      console.warn("synchronizer already initialized!");
      return;
    }

    this.socket = require('socket.io-client').connect();

    this._initHandlers();
  }

  /**
   * sends game updates from client to server, if changes are detected,
   * is started as soon as client_info is received
   * @private
   */
  _startUpdating() {
    //this interval sends the entityupdates.
    this.updateQueue.flush();
    setInterval(function() {
      if (!this.updateQueue.updateRequired) return;

      this.sendPackage(Packages.PROTOCOL.GENERAL.TO_SERVER.SEND_STATE, Packages.createEvent(
        this.CLIENT_INFO.id,
        this.updateQueue.popUpdatedData(),
        this.CLIENT_INFO.token
      ));
    }.bind(this), Ticks.CLIENT_UPDATE_INTERVAL);
  }

  /**
   * init all socket handlers,
   * if data was sent by the server, this method (to be more exact, the handlers initialized in this method),
   * receives and processes/distributes it.
   * @private
   */
  _initHandlers() {


    this.socket.on('disconnect', this._onDisconnect.bind(this));

    this.socket.on(Packages.PROTOCOL.GENERAL.TO_CLIENT.ERROR, this._onServerError.bind(this));

    // apply listeners for all supported messages
    for (let msg of this._suppotedMessages) {
      this.socket.on(msg, (evt) => {
        this.emit("on" + msg, evt.payload);
      });
    }
  }

  _remmoveHandlers() {
    this.socket.removeAllListeners();
  }

  _onClientAccepted(evt) {
    if (this.connectedServerID) return; // another received package could be from another game, to which the client is connected

    this.connectedServerID = evt.payload.serverID;
    this.CLIENT_INFO = evt.payload.clientInfo;
    console.log("Clientdata received");

    this._startUpdating();
    this.emit(EVT_ON_CLIENT_ACCEPTED, evt.payload.clientInfo);

    window.hideLoadingDialog();
  }

  _onInitGame(evt) {
    if (!this._vertifyServer(evt.senderID)) {
      console.log("message is not from server");
      return;
    }
    this.lastGameStateUpdateEventTimeStamp = this.lastGameStateUpdateTimeStamp = new Date().getTime();
    this.emit(EVT_ON_INIT_GAME, evt.payload);
  }

  _onStateUpdate(evt) {
    if (!this._vertifyServer(evt.senderID)) {
      console.log("message is not from server");
      return;
    }
    if (evt.timeStamp < this.lastGameStateUpdateEventTimeStamp) return; // if update is old, do not apply it
    let currentTime = new Date().getTime();
    this.processServerUpdates(evt.payload, currentTime - this.lastGameStateUpdateTimeStamp);
    this.lastGameStateUpdateEventTimeStamp = evt.timeStamp;
    this.lastGameStateUpdateTimeStamp = currentTime;
  }

  _onClientConnected(evt) {
    if (!this._vertifyServer(evt.senderID)) {
      console.log("message is not from server");
      return;
    }
    this.emit(EVT_ON_CLIENT_CONNECTED, evt.payload);
  }

  _onClientDisconnected(evt) {
    if (!this._vertifyServer(evt.senderID)) {
      console.log("message is not from server");
      return;
    }
    this.emit(EVT_ON_CLIENT_DISCONNECTED, evt.payload);
  }

  _onClientValueUpdate(evt) {
    if (!this._vertifyServer(evt.senderID)) {
      console.log("message is not from server");
      return;
    }
    this.emit(EVT_ON_CLIENT_VALUE_UPDATE, evt.payload);
  }

  _onClientValueUpdateRejected(evt) {
    if (!this._vertifyServer(evt.senderID)) {
      console.log("message is not from server");
      return;
    }
    this._handleValueRejections(evt.payload);
  }

  _onDisconnect(evt) {
    console.log("DISCONNECT", evt);
    // disconnect kann nur ein error sein
    this._remmoveHandlers();
  }

  sendPackage(type, msg) {
    this.socket.emit(type, msg);
  }


  sendStateUpdate(type, data) {
    this.updateQueue.postUpdate(type,
      this.CLIENT_INFO.id,
      data
    );
  }


  /**
   * processes the batched updates, received from the server
   * @param updateData
   * @param timeSinceLastUpdate the time since the last update was received from the server
   */
  processServerUpdates(updateData, timeSinceLastUpdate) {
    this.emit(EVT_ON_SERVER_UPDATE, {
      updates: updateData,
      timeSinceLastUpdate: timeSinceLastUpdate
    });

    for (let type in updateData) {
      if (!updateData.hasOwnProperty(type)) continue;

      let updates = updateData[type];

      this.emit(EVT_ON_SERVER_UPDATE + EVT_ON_SERVER_UPDATE_SEPERATOR + type, {
        type: type,
        updates: updates,
        timeSinceLastUpdate: timeSinceLastUpdate
      });
    }
  }
}

module.exports = Synchronizer;