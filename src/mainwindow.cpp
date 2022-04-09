#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "discord-rpc.h"

#define CLIENT_ID 962304930977312778


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  initDiscord();
}

MainWindow::~MainWindow()
{
  m_InterruptCallbackRunner = true;
  m_DiscordCallbacksRunner.waitForFinished();
  delete ui;
}

void MainWindow::initDiscord()
{
  auto result = discord::Core::Create(CLIENT_ID, (uint64_t)discord::CreateFlags::Default, &m_Discord);
  QDebug(QtMsgType::QtDebugMsg) << "Discord RPC Create result: " << (uint64_t)result;
  discord::Activity activity{};
  activity.SetState("testing");
  activity.SetDetails("test");
  activity.GetAssets().SetLargeImage("default");
  m_Discord->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
    QDebug(QtMsgType::QtDebugMsg) << ((result == discord::Result::Ok) ? "Succeeded" : "Failed") << " updating activity!";
  });


  m_DiscordCallbacksRunner = QtConcurrent::run(DC_RPC::runDiscordCallbacks, m_Discord, &m_InterruptCallbackRunner);
}

void MainWindow::on_setPlayingButton_clicked()
{
  QDebug(QtMsgType::QtDebugMsg) << "Set Playing";
}
