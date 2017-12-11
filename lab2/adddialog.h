#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDialog(QSqlQuery &cinemas, QSqlQuery &sessions, QSqlQuery &movies, QWidget *parent = 0);
    ~addDialog();

signals:
    void addedRecord(int cinemas_id, int sessions_id, int movies_id, int price);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::addDialog *ui;
};

#endif // ADDDIALOG_H
