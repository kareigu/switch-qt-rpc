#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtConcurrent/QtConcurrent>
#include <discord.h>

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
  void on_setPlayingButton_clicked();

private:
  void initDiscord();
  Ui::MainWindow *ui;


  bool m_InterruptCallbackRunner = false;
  QFuture<void> m_DiscordCallbacksRunner;

  discord::Core* m_Discord;
};
#endif // MAINWINDOW_H
