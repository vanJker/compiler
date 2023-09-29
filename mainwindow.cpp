#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scanner.h"

#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->openFileButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->doScanButton, &QPushButton::clicked, this, &MainWindow::scanWord);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "打开文件");

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            fileContent = in.readAll();
            file.close();
            ui->textBrowser->setText(fileContent);
        }
    }
}

void MainWindow::scanWord()
{
    QByteArray byteArray = fileContent.toUtf8();
    char *str = byteArray.data();

    QString test;
    char *p = str;
    while (*p != '\0') {
        p = doScan(p);
        for (auto iter = word_map.constBegin(); iter != word_map.constEnd(); iter++) {
            test += iter.key();
            test += "\t";
            test += iter.value();
            test += "\n";
        }
        word_map.clear();
    }

    ui->textBrowser_2->setText(test);
}
