#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QSqlQuery &cinemas, QSqlQuery &sessions, QSqlQuery &movies, int cinema, int session, int movie, int price, QWidget *parent = 0);
    ~EditDialog();

private slots:
    void on_buttonBox_accepted();

signals:
    void editedRecord(int cinemas_id, int sessions_id, int movies_id, int price);

private:
    Ui::EditDialog *ui;
};

#endif // EDITDIALOG_H
