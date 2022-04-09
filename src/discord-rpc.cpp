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

    discord::Activity activity{};
    activity.SetState("testing");
    activity.SetDetails("test");
    activity.GetAssets().SetLargeImage("default");
    data->core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
      QDebug(QtMsgType::QtDebugMsg) << ((result == discord::Result::Ok) ? "Succeeded" : "Failed") << " updating activity!";
    });


    data->callbackRunner = QtConcurrent::run(runDiscordCallbacks, data);
    return data;
  }

}
