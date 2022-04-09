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

  connect(ui->setPlayingButton, &QPushButton::released, this, &MainWindow::setPlayingButtonClicked);
  connect(ui->idleButton, &QPushButton::released, this, &MainWindow::idleButtonClicked);
}

MainWindow::~MainWindow() {
  m_DiscordData->interrupt = true;
  m_DiscordData->callbackRunner.waitForFinished();
  delete m_DiscordData;
  delete ui;
}

void MainWindow::setPlayingButtonClicked() {
  m_DiscordData->gameName = ui->gameName->text().toStdString();
  m_DiscordData->statusMsg = ui->statusMsg->text().toStdString();
  QDebug(QtMsgType::QtDebugMsg) << "Set Playing - " << m_DiscordData->gameName.c_str() << " - " << m_DiscordData->statusMsg.c_str();
  DC_RPC::updateActivity(m_DiscordData);
}

void MainWindow::idleButtonClicked() {
  m_DiscordData->gameName = DC_RPC::defaultGame();
  m_DiscordData->statusMsg = DC_RPC::defaultStatus();
  QDebug(QtMsgType::QtDebugMsg) << "Set Idle - " << m_DiscordData->gameName.c_str() << " - " << m_DiscordData->statusMsg.c_str();
  DC_RPC::updateActivity(m_DiscordData);
}

