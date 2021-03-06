#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "discord-rpc.h"
#include "stdio.h"
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_AboutDialog(new AboutDialog(this))
{
  ui->setupUi(this);
  m_DiscordData = DC_RPC::initDiscord();

  ui->gameName->setText(m_DiscordData->gameName.c_str());
  ui->statusMsg->setText(m_DiscordData->statusMsg.c_str());



  connect(ui->setPlayingButton, &QPushButton::pressed, this, &MainWindow::setPlayingButtonPressed);
  connect(ui->setPlayingButton, &QPushButton::released, this, &MainWindow::setPlayingButtonReleased);


  connect(ui->idleButton, &QPushButton::pressed, this, &MainWindow::idleButtonPressed);
  connect(ui->idleButton, &QPushButton::released, this, &MainWindow::idleButtonReleased);


  m_IconButtons = {
    ui->defaultIconButton,
    ui->bwIconButton
  };
  connect(ui->defaultIconButton, &QPushButton::pressed, this, &MainWindow::defaultIconButtonPressed);
  connect(ui->defaultIconButton, &QPushButton::released, this, &MainWindow::defaultIconButtonReleased);
  connect(ui->bwIconButton, &QPushButton::pressed, this, &MainWindow::bwIconButtonPressed);
  connect(ui->bwIconButton, &QPushButton::released, this, &MainWindow::bwIconButtonReleased);

  connect(ui->aboutButton, &QPushButton::released, this, [this]() { m_AboutDialog->show(); });
}

MainWindow::~MainWindow() {
  m_DiscordData->interrupt = true;
  m_DiscordData->callbackRunner.waitForFinished();
  delete m_DiscordData;
  delete ui;
}

void iconButtonPressed(QPushButton* btn, const char* iconName) {
  std::string styleSheet = "background-color: rgb(252, 252, 252);\nborder: 2 solid #00c0e1;\nborder-radius: 3;";
  styleSheet.append("\nimage: url(:/images/");
  styleSheet.append(iconName);
  styleSheet.append(".png);");
  btn->setStyleSheet(styleSheet.c_str());
}

void MainWindow::iconButtonReleased(QPushButton* btn, const char* iconName, bool skipCheck) {

  if (skipCheck) {
    std::string styleSheet = "background-color: rgb(252, 252, 252);\nborder: 2 solid #ff000e;\nborder-radius: 3;";
    styleSheet.append("\nimage: url(:/images/");
    styleSheet.append(iconName);
    styleSheet.append(".png);");
    btn->setStyleSheet(styleSheet.c_str());
  } else {
    std::string image(iconName);
    if (m_DiscordData->image != image) {
      iconButtonReleased(btn, iconName, true);
      return;
    }

    // FIXME: Create a proper data format to allow looping through the vector
    // and resetting all other buttons
    if (image == "bw")
      iconButtonReleased(m_IconButtons[0], DC_RPC::defaultImage(), true);

    if (image == DC_RPC::defaultImage())
      iconButtonReleased(m_IconButtons[1], "bw", true);
  }
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
  const char* iconName = DC_RPC::defaultImage();
  iconButtonPressed(ui->defaultIconButton, iconName);
}

void MainWindow::defaultIconButtonReleased() {
  const char* iconName = DC_RPC::defaultImage();
  m_DiscordData->image = iconName;
  iconButtonReleased(ui->defaultIconButton, iconName);
}

void MainWindow::bwIconButtonPressed() {
  const char* iconName = "bw";
  iconButtonPressed(ui->bwIconButton, iconName);
}

void MainWindow::bwIconButtonReleased() {
  const char* iconName = "bw";
  m_DiscordData->image = iconName;
  iconButtonReleased(ui->bwIconButton, iconName);
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

