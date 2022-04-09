#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "discord-rpc.h"


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_DiscordData = DC_RPC::initDiscord();
  ui->gameName->setText(m_DiscordData->gameName.c_str());
  ui->statusMsg->setText(m_DiscordData->statusMsg.c_str());
}

MainWindow::~MainWindow() {
  m_DiscordData->interrupt = true;
  m_DiscordData->callbackRunner.waitForFinished();
  delete m_DiscordData;
  delete ui;
}

void MainWindow::on_setPlayingButton_clicked() {
  m_DiscordData->gameName = ui->gameName->text().toStdString();
  m_DiscordData->statusMsg = ui->statusMsg->text().toStdString();
  QDebug(QtMsgType::QtDebugMsg) << "Set Playing";
  DC_RPC::updateActivity(m_DiscordData);
}

