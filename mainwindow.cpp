#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <Qdir>
#include <QFileInfoList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(660, 420);
    readFiles();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::readFiles(){
    // 讀取現有檔案放到list
    QDir folder("saveFiles");
    if(folder.exists()){
        QFileInfoList fileList = folder.entryInfoList();
        foreach (QFileInfo fileInfo, fileList) {
            if (fileInfo.isFile()) {
                ui->listWidget->addItem(fileInfo.baseName());
            }
        }
    }
}

void MainWindow::on_btn_save_clicked()
{
    // 如果不存在saveFiles資料夾 創建一個
    QDir dir("saveFiles");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file("saveFiles/"+ ui->firstText->text() +".txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << ui->secText->toPlainText();
        file.close();
        ui->listWidget->addItem(ui->firstText->text());
        qDebug() << "File saved successfully.";
    } else {
        qDebug() << "Failed to open file for writing.";
    }
}

void MainWindow::on_btn_delete_clicked()
{
    QListWidgetItem* selectedItem = ui->listWidget->currentItem();
    if(selectedItem){
        QFile file("saveFiles/"+ selectedItem->text() +".txt");
        if(file.exists()){
            if(file.remove()){
                ui->listWidget->takeItem(ui->listWidget->row(selectedItem));
                ui->firstText->setText("");
                ui->secText->setText("");
                qDebug() << "delete success";
            }
            else{
                qDebug() << "delete failed";
            }
        }
        else{
            qDebug() << "file not exist";
        }
    }
}

void MainWindow::on_btn_open_clicked()
{
    QListWidgetItem* selectedItem = ui->listWidget->currentItem();
    if(selectedItem){
        QFile inputFile("saveFiles/"+ selectedItem->text() +".txt");
        QString wordContent;

        // 讀取標題(檔名)
        ui->firstText->setText(selectedItem->text());

        // 讀取內容並將內容印在畫面上
        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&inputFile);
            while (!in.atEnd()) {
                QString line = in.readLine();
                wordContent.push_back(line + "\n");
            }

            ui->secText->setPlainText(wordContent);
            inputFile.close();

        }
        else{
            qDebug() << "Error opening file!";
        }
    }
}


