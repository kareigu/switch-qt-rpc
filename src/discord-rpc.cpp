#include "discord-rpc.h"
#include <QApplication>
#include <thread>
#include <chrono>

namespace DC_RPC {

  void runDiscordCallbacks(Data* data) {
    QDebug(QtMsgType::QtInfoMsg) << "Running Discord callbacks";
    do {
      updateActivity(data);
      data->core->RunCallbacks();

      std::this_thread::sleep_for(std::chrono::milliseconds(300));
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
    if (
        data->prevState.gameName == data->gameName &&
        data->prevState.statusMsg == data->statusMsg
       )
      {
        auto now = std::chrono::system_clock::now();
        auto elapsedInSeconds = (now - data->startTime).count() / 10'000'000;
        auto elapsed_h = elapsedInSeconds / 3600;
        auto elapsed_m = (elapsedInSeconds % 3600) / 60;
        auto elapsed_s = ((elapsedInSeconds % 3600) % 60);
        QString formattedStatus = QString("%1 (%2%3:%4%5:%6%7)")
            .arg(QString::fromStdString(data->statusMsg),
                 elapsed_h < 10 ? "0" : "", QString::number(elapsed_h),
                 elapsed_m < 10 ? "0" : "", QString::number(elapsed_m),
                 elapsed_s < 10 ? "0" : "", QString::number(elapsed_s));
        activity.SetState(formattedStatus.toStdString().c_str());
      }
    else
      {
        data->startTime = std::chrono::system_clock::now();
        data->prevState = { data->gameName, data->statusMsg };
        QString formattedStatus = QString("%1 (00:00:00)").arg(QString::fromStdString(data->statusMsg));
        activity.SetState(formattedStatus.toStdString().c_str());
      }

    activity.GetAssets().SetLargeImage(data->image.c_str());
    activity.GetAssets().SetSmallImage(defaultImage());
    data->core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
      if (result == discord::Result::Ok)
        return;
      QDebug(QtMsgType::QtDebugMsg) << QString("Failed updating activity with code %1").arg(QString::number(static_cast<int>(result)));
    });
  }

}
