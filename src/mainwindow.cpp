#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "discord-rpc.h"


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_DiscordData = DC_RPC::initDiscord();
}

MainWindow::~MainWindow() {
  m_DiscordData->interrupt = true;
  m_DiscordData->callbackRunner.waitForFinished();
  delete m_DiscordData;
  delete ui;
}

void MainWindow::on_setPlayingButton_clicked() {
  QDebug(QtMsgType::QtDebugMsg) << "Set Playing";
}
