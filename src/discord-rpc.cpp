#include "discord-rpc.h"
#include <QApplication>
#include <thread>
#include <chrono>

namespace DC_RPC {

  void runDiscordCallbacks(Data* data) {
    QDebug(QtMsgType::QtInfoMsg) << "Running Discord callbacks";
    do {
      data->core->RunCallbacks();

      std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } while (!data->interrupt);
    QDebug(QtMsgType::QtInfoMsg) << "Stopping Discord callback thread";
  }

  Data* initDiscord() {
    Data* data = new Data();

    auto result = discord::Core::Create(data->clientId, (uint64_t)discord::CreateFlags::Default, &data->core);
    QDebug(QtMsgType::QtDebugMsg) << "Discord RPC Create result: " << (uint64_t)result;

    updateActivity(data);


    data->callbackRunner = QtConcurrent::run(runDiscordCallbacks, data);
    return data;
  }

  void updateActivity(Data* data) {
    discord::Activity activity{};
    activity.SetDetails(data->gameName.c_str());
    activity.SetState(data->statusMsg.c_str());
    activity.GetAssets().SetLargeImage("default");
    data->core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
      QDebug(QtMsgType::QtDebugMsg) << ((result == discord::Result::Ok) ? "Succeeded" : "Failed") << " updating activity!";
    });
  }

}
