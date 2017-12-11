#include "editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(QSqlQuery &cinemas, QSqlQuery &sessions, QSqlQuery &movies, int cinema, int session, int movie, int price, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
    while (cinemas.next()) {
            ui->comboBox->addItem(cinemas.value(1).toString() + ", " + cinemas.value(2).toString(), cinemas.value(0));
    }
    ui->comboBox->setCurrentIndex(ui->comboBox->findData(cinema));
    while (sessions.next()) {
            ui->comboBox_2->addItem(sessions.value(1).toString() + ", " + sessions.value(2).toString(), sessions.value(0));
    }
    ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findData(session));
    while (movies.next()) {
            ui->comboBox_3->addItem(movies.value(1).toString() + ", " + movies.value(2).toString(), movies.value(0));
    }
    ui->comboBox_3->setCurrentIndex(ui->comboBox_3->findData(movie));
    ui->lineEdit_4->setText(QString::number(price));
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::on_buttonBox_accepted()
{
    emit editedRecord(ui->comboBox->itemData(ui->comboBox->currentIndex()).toInt(), ui->comboBox_2->itemData(ui->comboBox_2->currentIndex()).toInt(),\
                     ui->comboBox_3->itemData(ui->comboBox_3->currentIndex()).toInt(), ui->lineEdit_4->text().toInt());
}
