#ifndef SCANNER_H
#define SCANNER_H

#include <QMap>
#include <QSet>
#include <QString>


extern const QSet<QString> KEYWORD_SET;
extern const QSet<char> PUNCTUATION;

extern QMap<QString, QString> word_map;

bool isPunctuation(char c);
bool isKeyword(QString word);
char *doScan(char *str);

#endif // SCANNER_H
