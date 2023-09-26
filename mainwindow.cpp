#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "scanner.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QString test = QString::number(KEYWORD_SET.size());
//    QString test = QString::number(isPunctuation('#'));
//    QString test = QString::number(isKeyword("if"));
    QString test;
    int cnt = 0;
    char str[] = "#include<iostream.h>\n"
            "main()\n"
            "{\n"
               "int i;\n"
               "cin>>i;\n"
               "if (i>3) cout<<\"ok\";\n"
            "};\n";
    char *p = str;
    while (*p != '\0') {
        for (auto iter = word_map.constBegin(); iter != word_map.constEnd(); iter++) {
            test += " ";
            test += "Key: ";
            test += iter.key();
            test += " || ";
            test += " Value: ";
            test += iter.value();
            if (cnt % 3 == 0)
                test += "\n";
        }
        word_map.clear();
        cnt++;
        p = doScan(p);
    }
//    test += QString::number(word_map.size());
    test += QString::number(cnt);
    test += "\n";
//    for (auto iter = word_map.constBegin(); iter != word_map.constEnd(); iter++) {
//        test += " ";
//        test += "Key: ";
//        test += iter.key();
//        test += " || ";
//        test += " Value: ";
//        test += iter.value();
//        test += "\n";
//    }
    label = new QLabel(test, this);
    label->resize(1000, 1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete label;
}

