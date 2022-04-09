#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "discord-rpc.h"
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_DiscordData = DC_RPC::initDiscord();

  ui->gameName->setText(m_DiscordData->gameName.c_str());
  ui->statusMsg->setText(m_DiscordData->statusMsg.c_str());



  connect(ui->setPlayingButton, &QPushButton::pressed, this, &MainWindow::setPlayingButtonPressed);
  connect(ui->setPlayingButton, &QPushButton::released, this, &MainWindow::setPlayingButtonReleased);


  connect(ui->idleButton, &QPushButton::pressed, this, &MainWindow::idleButtonPressed);
  connect(ui->idleButton, &QPushButton::released, this, &MainWindow::idleButtonReleased);


  connect(ui->defaultIconButton, &QPushButton::pressed, this, &MainWindow::defaultIconButtonPressed);
  connect(ui->defaultIconButton, &QPushButton::released, this, &MainWindow::defaultIconButtonReleased);
}

MainWindow::~MainWindow() {
  m_DiscordData->interrupt = true;
  m_DiscordData->callbackRunner.waitForFinished();
  delete m_DiscordData;
  delete ui;
}

void iconButtonPressed(QPushButton* btn) {
  btn->setStyleSheet("image: url(:/images/icon.png);\nbackground-color: rgb(252, 252, 252);\nborder: 2 solid #00c0e1;\nborder-radius: 3;");
}

void iconButtonReleased(QPushButton* btn) {
  btn->setStyleSheet("image: url(:/images/icon.png);\nbackground-color: rgb(252, 252, 252);\nborder: 2 solid #ff000e;\nborder-radius: 3;");
}

void primaryButtonPressed(QPushButton* btn) {
  btn->setStyleSheet("background-color: #00c0e1;\nborder-radius: 3;\ncolor: #FFFFFF; border: 2 solid #ffffff;");
}

void primaryButtonReleased(QPushButton* btn) {
  btn->setStyleSheet("background-color: #00c0e1;\nborder-radius: 3;\ncolor: #FFFFFF;");
}

void secondaryButtonPressed(QPushButton* btn) {
  btn->setStyleSheet("background-color: #fe6054;\nborder-radius: 3;\ncolor: #FFFFFF; border: 2 solid #ffffff;");
}

void secondaryButtonReleased(QPushButton* btn) {
  btn->setStyleSheet("background-color: #fe6054;\nborder-radius: 3;\ncolor: #FFFFFF;");
}

void MainWindow::defaultIconButtonPressed() {
  iconButtonPressed(ui->defaultIconButton);
  m_DiscordData->image = DC_RPC::defaultImage();
}

void MainWindow::defaultIconButtonReleased() {
  iconButtonReleased(ui->defaultIconButton);
}

void MainWindow::setPlayingButtonPressed() {
  primaryButtonPressed(ui->setPlayingButton);
}

void MainWindow::setPlayingButtonReleased() {
  m_DiscordData->gameName = ui->gameName->text().toStdString();
  auto status = ui->statusMsg->text();

  if(status == DC_RPC::defaultStatus()) {
    const char* onlineText = "Online";
    m_DiscordData->statusMsg =  onlineText;
    ui->statusMsg->setText(onlineText);
  } else {
    m_DiscordData->statusMsg = status.toStdString();
  }

  QDebug(QtMsgType::QtDebugMsg) << "Set Playing - " << m_DiscordData->gameName.c_str() << " - " << m_DiscordData->statusMsg.c_str();
  DC_RPC::updateActivity(m_DiscordData);
  primaryButtonReleased(ui->setPlayingButton);
}

void MainWindow::idleButtonPressed() {
  secondaryButtonPressed(ui->idleButton);
}

void MainWindow::idleButtonReleased() {
  m_DiscordData->gameName = DC_RPC::defaultGame();
  m_DiscordData->statusMsg = DC_RPC::defaultStatus();
  ui->statusMsg->setText(DC_RPC::defaultStatus());
  QDebug(QtMsgType::QtDebugMsg) << "Set Idle - " << m_DiscordData->gameName.c_str() << " - " << m_DiscordData->statusMsg.c_str();
  DC_RPC::updateActivity(m_DiscordData);
  secondaryButtonReleased(ui->idleButton);
}

