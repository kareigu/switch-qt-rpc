#include "discord-rpc.h"
#include <QApplication>
#include <thread>
#include <chrono>

namespace DC_RPC {

  void runDiscordCallbacks(discord::Core* core, bool* interrupted)
  {
    QDebug(QtMsgType::QtInfoMsg) << "Running Discord callbacks";
    do {
      core->RunCallbacks();

      std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } while (!(*interrupted));
    QDebug(QtMsgType::QtInfoMsg) << "Stopping Discord callback thread";
  }

}
