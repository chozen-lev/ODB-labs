#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_findButton_clicked();

    void on_importButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_addButton_clicked();

    void on_pricesView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQueryModel *pqueryModelCinemas;
    QSqlQueryModel *pqueryModelSessions;
    QSqlQueryModel *pqueryModelMovies;
    QSqlQueryModel *pqueryModelPrices;
    QSqlQueryModel *pqueryModelResult;

public slots:
    void addedRecord(int cinemas_id, int sessions_id, int movies_id, int price);
    void editedRecord(int cinemas_id, int sessions_id, int movies_id, int price);
    void deleteRow();
    void importedData(QString cinemas, QString sessions, QString movies);

};

#endif // MAINWINDOW_H
