#ifndef FUNCTION_H
#define FUNCTION_H
#include <QString>
#include <QVector>

//定义全文替换的结构体
typedef struct ReplaceFullFile
{
    QVector<QString> findWhat;
    QVector<QString> replaceWith;

}ReplaceFullFile;

//定义有起始和结束替换标志的结构体
typedef struct ReplaceWithFlags
{
    QVector<QString> findWhat;
    QVector<QString> replaceWith;
    QString begReplaceFlag;//替换起始标志
    QString endReplaceFlag;//替换结束标志

}ReplaceWithFlags;

//定义替换行的结构体
typedef struct ReplaceLine
{
    QVector<QString> findWhat;
    QVector<QString> replaceWith;
    QString replaceWhen;//当某行出现改内容时，把该行替换

}ReplaceLine;

bool readConfigFile();
bool replaceFullFile(QString file);
bool replaceLine(QString file);
bool replaceWithFlag(QString file);
void replaceFiles();

#endif // FUNCTION_H

