#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AboutDialog)
{
  ui->setupUi(this);

  connect(ui->closeButton, &QPushButton::released, this, [this]() { this->close(); });
}

AboutDialog::~AboutDialog()
{
  delete ui;
}
