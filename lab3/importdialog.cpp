#include "importdialog.h"
#include "ui_importdialog.h"
#include <QFileDialog>

ImportDialog::ImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
}

ImportDialog::~ImportDialog()
{
    delete ui;
}

void ImportDialog::on_buttonBox_accepted()
{
    emit importedData(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text());
}

void ImportDialog::on_toolButton_clicked()
{
    ui->lineEdit->setText(QFileDialog::getOpenFileName(this, tr("Open File")));
}

void ImportDialog::on_toolButton_2_clicked()
{
    ui->lineEdit_2->setText(QFileDialog::getOpenFileName(this, tr("Open File")));
}

void ImportDialog::on_toolButton_3_clicked()
{
    ui->lineEdit_3->setText(QFileDialog::getOpenFileName(this, tr("Open File")));
}
