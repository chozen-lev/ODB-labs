#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = 0);
    ~ImportDialog();

private:
    Ui::ImportDialog *ui;

signals:
    void importedData(QString cinemas, QString sessions, QString movies);
private slots:
    void on_buttonBox_accepted();
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();
};

#endif // IMPORTDIALOG_H
