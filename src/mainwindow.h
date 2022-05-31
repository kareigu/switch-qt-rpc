#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent/QtConcurrent>
#include "discord-rpc.h"
#include "aboutdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void setPlayingButtonPressed();
  void setPlayingButtonReleased();

  void idleButtonPressed();
  void idleButtonReleased();

  void defaultIconButtonPressed();
  void defaultIconButtonReleased();
  void bwIconButtonPressed();
  void bwIconButtonReleased();

private:
  void iconButtonReleased(QPushButton*, const char*, bool skipCheck = false);

  Ui::MainWindow* ui;
  AboutDialog* m_AboutDialog;

  QVector<QPushButton*> m_IconButtons;

  DC_RPC::Data* m_DiscordData;
};
#endif // MAINWINDOW_H
