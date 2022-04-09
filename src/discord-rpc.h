#ifndef DISCORDRPC_H
#define DISCORDRPC_H

#include <discord.h>
#include <QtConcurrent/QtConcurrent>


#define CLIENT_ID 962304930977312778

namespace DC_RPC {

  struct Data {
    uint64_t clientId = CLIENT_ID;
    bool interrupt = false;
    QFuture<void> callbackRunner;

    discord::Core* core;
  };

  Data* initDiscord();
  void runDiscordCallbacks(Data* data);

}


#endif // DISCORDRPC_H
