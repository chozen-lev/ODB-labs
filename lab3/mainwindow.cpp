#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "importdialog.h"
#include "adddialog.h"
#include "editdialog.h"
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QMYSQL");

    db.setDatabaseName("test");
    db.setUserName("root");
    db.setHostName("127.0.0.1");
    db.setPassword("parolik");

    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError();
    } else {
        qDebug() << "connection successful";

        pqueryModelCinemas = new QSqlQueryModel;
        pqueryModelSessions = new QSqlQueryModel;
        pqueryModelMovies = new QSqlQueryModel;
        pqueryModelPrices = new QSqlQueryModel;
        pqueryModelLogs = new QSqlQueryModel;
        pqueryModelResult = new QSqlQueryModel;
        ui->cinemasView->setModel(pqueryModelCinemas);
        ui->sessionsView->setModel(pqueryModelSessions);
        ui->moviesView->setModel(pqueryModelMovies);
        ui->pricesView->setModel(pqueryModelPrices);
        ui->logsView->setModel(pqueryModelLogs);
        ui->ResultView->setModel(pqueryModelResult);

        QSqlQuery query("SELECT id, name, city FROM cinemas", db);
        ui->cinemasBox->addItem("All", -1);
        while(query.next()) {
            ui->cinemasBox->addItem(query.value(1).toString() + ", " + query.value(2).toString(), query.value(0).toInt());
        }

        ui->sessionsMaxTime->setTime(QTime(23,59,59));
        ui->ratingMaxBox->setValue(10.0);
        ui->durationMaxTime->setTime(QTime(23,59,59));

        for (int i = 1970; i < 2018; i++) {
            ui->yearMinBox->addItem(QString::number(i));
            ui->yearMaxBox->addItem(QString::number(i));
        }
        ui->yearMaxBox->setCurrentText("2017");
        ui->priceMaxBox->setValue(99999);
        ui->check2DBox->setChecked(true);
        ui->check3DBox->setChecked(true);
    }

    ui->addButton->hide();
    ui->importButton->hide();
    ui->logsBox->hide();

    QShortcut *shortcut = new QShortcut(QKeySequence(QKeySequence::Delete), ui->pricesView);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteRow()));
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::on_findButton_clicked()
{
    QString query = QString("SELECT cinemas.name, city, start, finish, price, movies.name, 3d, released, rating, duration FROM prices "
                            "LEFT JOIN cinemas ON cinemas_id = cinemas.id "
                            "LEFT JOIN sessions ON sessions_id = sessions.id "
                            "LEFT JOIN movies ON movies_id = movies.id WHERE TRUE ");
    if (ui->cinemasBox->currentData() != -1) {
        query += QString("and cinemas.id = %1 ").arg(ui->cinemasBox->currentData().toString());
    }
    if (ui->check3DBox->checkState() == Qt::Unchecked && ui->check2DBox->checkState() == Qt::Unchecked) {
            query += QString("and FALSE ");
    }
    else if (ui->check3DBox->checkState() == Qt::Unchecked || ui->check2DBox->checkState() == Qt::Unchecked) {
        query += QString("and movies.3d = %1 ").arg(ui->check3DBox->checkState() == Qt::Unchecked ? "0" : "1");
    }

    query += QString("and prices.price >= %1 ").arg(ui->priceMinBox->value());
    query += QString("and prices.price <= %1 ").arg(ui->priceMaxBox->value());

    query += QString("and movies.released >= str_to_date('12-31-%1','%m-%d-%Y') ").arg(ui->yearMinBox->currentText().toInt());
    query += QString("and movies.released <= str_to_date('12-31-%1','%m-%d-%Y') ").arg(ui->yearMaxBox->currentText().toInt());

    query += QString("and TIME(sessions.start) >= TIME('%1') ").arg(ui->sessionsMinTime->time().toString("hh:mm:ss"));
    query += QString("and TIME(sessions.finish) <= TIME('%1') ").arg(ui->sessionsMaxTime->time().toString("hh:mm:ss"));

    query += QString("and TIME(movies.duration) >= TIME('%1') ").arg(ui->durationMinTime->time().toString("hh:mm:ss"));
    query += QString("and TIME(movies.duration) <= TIME('%1') ").arg(ui->durationMaxTime->time().toString("hh:mm:ss"));

    if (!ui->findField->text().isEmpty())
    {
        query += QString(QString("and ") + (ui->fullTextBox->checkState() == Qt::Unchecked ? "not" : "") + "(match(movies.name) against ('" + ui->findField->text() + "' in boolean mode) ");
        query += QString("or match(cinemas.name, cinemas.city) against ('" + ui->findField->text() + "' in boolean mode))");
    }

    pqueryModelResult->setQuery(query);
    if (pqueryModelResult->lastError().isValid()) {
        qDebug() << pqueryModelResult->lastError();
    }
}

void MainWindow::on_importButton_clicked()
{
    ImportDialog impdialog(this);
    connect(&impdialog, SIGNAL(importedData(QString,QString,QString)), this, SLOT(importedData(QString,QString,QString)));
    impdialog.setModal(true);
    impdialog.exec();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 4) {
        ui->logsBox->show();
    } else {
        ui->logsBox->hide();
    }

    if (index == 3) {
        ui->importButton->hide();
        ui->addButton->show();
    } else {
        ui->addButton->hide();
        ui->importButton->show();
    }

    switch (index) {
    case 0:
        pqueryModelCinemas->setQuery("SELECT * FROM cinemas");
        break;
    case 1:
        pqueryModelSessions->setQuery("SELECT * FROM sessions");
        break;
    case 2:
        pqueryModelMovies->setQuery("SELECT * FROM movies");
        break;
    case 3:
        pqueryModelPrices->setQuery("SELECT cinemas_id, sessions_id, movies_id, price FROM prices");
        break;
    case 4:
        pqueryModelLogs->setQuery("SELECT * FROM logs");
        break;
    default:
        ui->addButton->hide();
        ui->importButton->hide();
    }
}

void MainWindow::on_addButton_clicked()
{
    QSqlQuery cinemas("SELECT id, name, city FROM cinemas ORDER BY city, name", db),
            sessions("SELECT id, start, finish FROM sessions ORDER BY start", db),
            movies("SELECT id, name, released FROM movies ORDER BY name, released", db);
    addDialog dialog(cinemas, sessions, movies, this);
    connect(&dialog, SIGNAL(addedRecord(int,int,int,int)), this, SLOT(addedRecord(int,int,int,int)));
    dialog.setModal(true);
    dialog.exec();
    pqueryModelPrices->setQuery("SELECT cinemas_id, sessions_id, movies_id, price FROM prices");
}

void MainWindow::addedRecord(int cinemas_id, int sessions_id, int movies_id, int price)
{
//    qDebug() << cinemas_id << sessions_id << movies_id << price;
    QSqlQuery query(db);
    query.prepare("INSERT INTO prices (cinemas_id, sessions_id, movies_id, price) "
                  "VALUES (:cinemas_id, :sessions_id, :movies_id, :price)");
    query.bindValue(":cinemas_id", cinemas_id);
    query.bindValue(":sessions_id", sessions_id);
    query.bindValue(":movies_id", movies_id);
    query.bindValue(":price", price);
    query.exec();
    if (query.lastError().isValid())
        qDebug() << query.lastError();
}

void MainWindow::editedRecord(int cinemas_id, int sessions_id, int movies_id, int price)
{
    QModelIndex index = ui->pricesView->currentIndex();
//    pqueryModelPrices->setData(
    QSqlQuery query(db);
    query.prepare("UPDATE prices SET cinemas_id = :cinemas_id, sessions_id = :sessions_id, movies_id = :movies_id, price = :price "
                  "WHERE cinemas_id  = :cinemas_id2 and sessions_id = :sessions_id2 and movies_id = :movies_id2");
    query.bindValue(":cinemas_id", cinemas_id);
    query.bindValue(":sessions_id", sessions_id);
    query.bindValue(":movies_id", movies_id);
    query.bindValue(":price", price);
    query.bindValue(":cinemas_id2", pqueryModelPrices->record(index.row()).value("cinemas_id").toInt());
    query.bindValue(":sessions_id2", pqueryModelPrices->record(index.row()).value("sessions_id").toInt());
    query.bindValue(":movies_id2", pqueryModelPrices->record(index.row()).value("movies_id").toInt());
    query.exec();
    pqueryModelPrices->setQuery("SELECT cinemas_id, sessions_id, movies_id, price FROM prices");
}

void MainWindow::deleteRow()
{
    if (ui->tabWidget->currentIndex() != 3)
        return;

    QModelIndex index = ui->pricesView->currentIndex();
    if (index.isValid()) {
        qDebug() << "remove" << index.row() << index.row() << pqueryModelPrices->record(index.row()).value("movies_id").toInt();
        QSqlQuery query(db);
        query.prepare("delete from prices "
                      "where cinemas_id = :cinemas_id and sessions_id = :sessions_id and movies_id = :movies_id");
        query.bindValue(":cinemas_id", pqueryModelPrices->record(index.row()).value("cinemas_id").toInt());
        query.bindValue(":sessions_id", pqueryModelPrices->record(index.row()).value("sessions_id").toInt());
        query.bindValue(":movies_id", pqueryModelPrices->record(index.row()).value("movies_id").toInt());
        query.exec();
        pqueryModelPrices->setQuery("SELECT cinemas_id, sessions_id, movies_id, price FROM prices");

        if (query.lastError().isValid())
            qDebug() << query.lastError();
    }
}

void MainWindow::importedData(QString cinemas, QString sessions, QString movies)
{
//    qDebug() << cinemas << sessions << movies;
    QFile cinemasFile(cinemas), sessionsFile(sessions), moviesFile(movies);

    QSqlQuery query(db);
    query.exec("TRUNCATE TABLE prices;");
    QJsonDocument jsonDoc;
    QJsonArray jsonArray;

    if (!cinemas.isEmpty())
    {
        cinemasFile.open(QIODevice::ReadOnly | QIODevice::Text);
        cinemas = cinemasFile.readAll();
        cinemasFile.close();
        query.exec("SET FOREIGN_KEY_CHECKS = 0;TRUNCATE TABLE cinemas;SET FOREIGN_KEY_CHECKS = 1;");
        jsonDoc = QJsonDocument::fromJson(cinemas.toUtf8());
        jsonArray = jsonDoc.array();
        foreach (const QJsonValue &value, jsonArray) {
            QJsonObject obj = value.toObject();
            query.exec("INSERT INTO cinemas VALUES (" + obj["id"].toString() + ",'" + obj["name"].toString() + "','" + obj["city"].toString() + "');");

            if (query.lastError().isValid()) {
                qDebug() << query.lastError();
            }
        }
    }

    if (!sessions.isEmpty())
    {
        sessionsFile.open(QIODevice::ReadOnly | QIODevice::Text);
        sessions = sessionsFile.readAll();
        sessionsFile.close();
        query.exec("SET FOREIGN_KEY_CHECKS = 0;TRUNCATE TABLE sessions;SET FOREIGN_KEY_CHECKS = 1;");
        jsonDoc = QJsonDocument::fromJson(sessions.toUtf8());
        jsonArray = jsonDoc.array();
        foreach (const QJsonValue &value, jsonArray) {
            QJsonObject obj = value.toObject();
            query.exec("INSERT INTO sessions VALUES (" + obj["id"].toString() + ",'" + obj["start"].toString() + "','" + obj["finish"].toString() + "');");

            if (query.lastError().isValid()) {
                qDebug() << query.lastError();
            }
        }
    }

    if (!movies.isEmpty())
    {
        moviesFile.open(QIODevice::ReadOnly | QIODevice::Text);
        movies = moviesFile.readAll();
        moviesFile.close();
        query.exec("SET FOREIGN_KEY_CHECKS = 0;TRUNCATE TABLE movies;SET FOREIGN_KEY_CHECKS = 1;");
        jsonDoc = QJsonDocument::fromJson(movies.toUtf8());
        jsonArray = jsonDoc.array();
        foreach (const QJsonValue &value, jsonArray) {
            QJsonObject obj = value.toObject();
            query.exec("INSERT INTO movies VALUES (" + obj["id"].toString() + ",'" + obj["name"].toString() + "'," + QString::number(obj["3d"].toInt()) + ",str_to_date('" + obj["released"].toString() + "','%d.%m.%Y')," + QString::number(obj["rating"].toDouble()) + ",'" + obj["duration"].toString() + "');");
            if (query.lastError().isValid()) {
                qDebug() << query.lastError();
            }
        }
        qDebug() << query.lastQuery();
    }
}

void MainWindow::on_pricesView_doubleClicked(const QModelIndex &index)
{
    qDebug() << "edit" << index.row() << pqueryModelPrices->record(index.row()).value("movies_id").toInt();
    QSqlQuery cinemas("SELECT id, name, city FROM cinemas ORDER BY city, name", db),
            sessions("SELECT id, start, finish FROM sessions ORDER BY start", db),
            movies("SELECT id, name, released FROM movies ORDER BY name, released", db);
    EditDialog editdialog(cinemas, sessions, movies, pqueryModelPrices->record(index.row()).value("cinemas_id").toInt(), pqueryModelPrices->record(index.row()).value("sessions_id").toInt(),\
                          pqueryModelPrices->record(index.row()).value("movies_id").toInt(), pqueryModelPrices->record(index.row()).value("price").toInt(), this);
    connect(&editdialog, SIGNAL(editedRecord(int,int,int,int)), this, SLOT(editedRecord(int,int,int,int)));
    editdialog.setModal(true);
    editdialog.exec();
}

void MainWindow::on_logsBox_clicked()
{
    QSqlQuery query(db);
    if (ui->logsBox->checkState() == Qt::Unchecked) {
        query.exec("DROP TRIGGER trigger_price");
    } else {
        query.exec("CREATE TRIGGER trigger_price AFTER INSERT ON prices FOR EACH ROW INSERT INTO logs SET time = NOW(), movies_id = NEW.movies_id;");
    }

    if (query.lastError().isValid())
        qDebug() << query.lastError();
}
