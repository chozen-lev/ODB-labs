#include "adddialog.h"
#include "ui_adddialog.h"
#include <QDebug>

addDialog::addDialog(QSqlQuery &cinemas, QSqlQuery &sessions, QSqlQuery &movies, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDialog)
{
    ui->setupUi(this);

    while (cinemas.next()) {
        ui->cinemasBox->addItem(cinemas.value(1).toString() + ", " + cinemas.value(2).toString(), cinemas.value(0));
    }
    while (sessions.next()) {
        ui->sessionsBox->addItem(sessions.value(1).toString() + ", " + sessions.value(2).toString(), sessions.value(0));
    }
    while (movies.next()) {
        ui->moviesBox->addItem(movies.value(1).toString() + ", " + movies.value(2).toString(), movies.value(0));
    }
}

addDialog::~addDialog()
{
    delete ui;
}

void addDialog::on_buttonBox_accepted()
{
//    qDebug() << ui->cinemasBox->itemData(ui->cinemasBox->currentIndex()).toInt();
    emit addedRecord(ui->cinemasBox->itemData(ui->cinemasBox->currentIndex()).toInt(), ui->sessionsBox->itemData(ui->sessionsBox->currentIndex()).toInt(),\
                     ui->moviesBox->itemData(ui->moviesBox->currentIndex()).toInt(), ui->lineEdit->text().toInt());
}
