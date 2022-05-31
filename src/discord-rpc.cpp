#include "discord-rpc.h"
#include <QApplication>
#include <thread>
#include <chrono>

bool stateChanged(DC_RPC::Data* data) {
  return !(
      data->prevState.gameName == data->gameName &&
      data->prevState.statusMsg == data->statusMsg);
}

namespace DC_RPC {

  void runDiscordCallbacks(Data* data) {
    QDebug(QtMsgType::QtInfoMsg) << "Running Discord callbacks";
    do {
      data->core->RunCallbacks();

      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    } while (!data->interrupt);
    data->core->ActivityManager().ClearActivity([](discord::Result result) {
        if (result == discord::Result::Ok)
          return;
        QDebug(QtMsgType::QtDebugMsg) << QString("Failed clearing activity with code %1").arg(QString::number(static_cast<int>(result)));
      });
    data->core->RunCallbacks();
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
    if (stateChanged(data)) {
        data->startTime = std::chrono::system_clock::now().time_since_epoch().count();
        data->prevState = { data->gameName, data->statusMsg };
    }

    activity.SetState(data->statusMsg.c_str());
    activity.GetAssets().SetLargeImage(data->image.c_str());
    activity.GetAssets().SetSmallImage(defaultImage());
    activity.GetTimestamps().SetStart(data->startTime / 10'000'000);
    activity.SetType(discord::ActivityType::Playing);
    data->core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
      if (result == discord::Result::Ok)
        return;
      QDebug(QtMsgType::QtDebugMsg) << QString("Failed updating activity with code %1").arg(QString::number(static_cast<int>(result)));
    });
  }

}
