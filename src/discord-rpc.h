#ifndef DISCORDRPC_H
#define DISCORDRPC_H

#include <string>
#include <discord.h>
#include <chrono>
#include <QtConcurrent/QtConcurrent>


#define CLIENT_ID 962304930977312778

namespace DC_RPC {

  inline const char* defaultGame() { return "Home"; };
  inline const char* defaultStatus() { return "Idle"; };
  inline const char* defaultImage() { return "default"; };

  struct Data {
    uint64_t clientId = CLIENT_ID;

    std::string gameName = defaultGame();
    std::string statusMsg = defaultStatus();
    std::string image = defaultImage();

    struct PrevState {
      std::string gameName = defaultGame();
      std::string statusMsg = defaultStatus();
    };

    PrevState prevState;

    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();


    bool interrupt = false;
    QFuture<void> callbackRunner;

    discord::Core* core;
  };

  Data* initDiscord();
  void runDiscordCallbacks(Data* data);
  void updateActivity(Data* data);

}


#endif // DISCORDRPC_H
