#include "function.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QtDebug>
#include <iostream>
#include <QTextCodec>

//定义3个代表替换方式的全局变量，
ReplaceFullFile g_replaceFullFile;
ReplaceWithFlags g_replaceWithFlags;
ReplaceLine g_replaceLine;
//定义保存要做替换的文件的vector
QVector<QString> g_files;

//读取配置文件，包括被替换的内容，要替换成什么，以及开始结束替换的标志
bool readConfigFile()
{
    qDebug() << "read config file .......\n";//输出读取配置文件的调试信息

    QFile loadFile("config.json");//加载配置文件
    if(!loadFile.open(QIODevice::ReadOnly))//以只读方式打开文件
    {
         qDebug() << "could't open config.json \n";//如果打开配置文件失败，输出错误信息，返回false
         return false;
    }
    QByteArray allData = loadFile.readAll();//读取文件中所有信息
    loadFile.close();//关闭文件

    QTextCodec *tc = QTextCodec::codecForName("GBK");
    QString tmpQStr = tc->toUnicode(allData);
    allData = tmpQStr.toUtf8(); //转换以下，处理中文乱码问题

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));//转为jsondoc格式

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!json_error.error ="<<json_error.error<< "\n";//如果转为jsondoc格式失败，输出错误信息，返回false
        return false;
    }

    //解析各个字段
    QJsonObject obj = jsonDoc.object();
    QJsonArray arrayTemp;
    int looper=0;

    //解析replaceFullFileMode
    QJsonObject repFullFileObj=obj.value("replaceFullFileMode").toObject();

    qDebug() << "g_replaceFullFil.findWhat:";
    arrayTemp=repFullFileObj.value("find_waht").toArray();
    for(looper=0;looper<arrayTemp.size();looper++)
    {
        g_replaceFullFile.findWhat.append(arrayTemp.at(looper).toString());
        qDebug() << g_replaceFullFile.findWhat.at(looper);
    }

    qDebug() << "g_replaceFullFil.replaceWith:";
    arrayTemp=repFullFileObj.value("replace_with").toArray();
    for(looper=0;looper<arrayTemp.size();looper++)
    {
        g_replaceFullFile.replaceWith.append(arrayTemp.at(looper).toString());
        qDebug() << g_replaceFullFile.replaceWith.at(looper);
    }
    qDebug() << "\n";
    if(g_replaceFullFile.findWhat.size()!=g_replaceFullFile.replaceWith.size())
    {
        qDebug() << "Error! in replaceFullFileMode,findWhat.size()!=replaceWith.size().please check config.json file.";
        return false;
    }

    //解析replaceWithFlagsMode
    QJsonObject replaceWithFlagObj=obj.value("replaceWithFlagsMode").toObject();

    qDebug() << "g_replaceWithFlags.findWhat:";
    arrayTemp=replaceWithFlagObj.value("find_waht").toArray();
    for(looper=0;looper<arrayTemp.size();looper++)
    {
        g_replaceWithFlags.findWhat.append(arrayTemp.at(looper).toString());
        qDebug() << g_replaceWithFlags.findWhat.at(looper);
    }

    qDebug() << "g_replaceWithFlags.replaceWith:";
    arrayTemp=replaceWithFlagObj.value("replace_with").toArray();
    for(looper=0;looper<arrayTemp.size();looper++)
    {
        g_replaceWithFlags.replaceWith.append(arrayTemp.at(looper).toString());
        qDebug() << g_replaceWithFlags.replaceWith.at(looper);
    }
    g_replaceWithFlags.begReplaceFlag=replaceWithFlagObj.value("begReplace_flag").toString();
    g_replaceWithFlags.endReplaceFlag=replaceWithFlagObj.value("endReplace_flag").toString();

    qDebug() << "g_replaceWithFlags.begReplaceFlag:";
    qDebug() << g_replaceWithFlags.begReplaceFlag;
    qDebug() << " g_replaceWithFlags.endReplace_flag:";
    qDebug() << g_replaceWithFlags.endReplaceFlag;
    qDebug() << "\n";
    if(g_replaceWithFlags.findWhat.size()!=g_replaceWithFlags.replaceWith.size())
    {
        qDebug() << "Error! in replaceWithFlagsMode,findWhat.size()!=replaceWith.size().please check config.json file.";
        return false;
    }

    //解析replaceLineMode
    QJsonObject replaceLineObj=obj.value("replaceLineMode").toObject();

    qDebug() << "g_replaceLine.findWhat:";
    arrayTemp=replaceLineObj.value("find_waht").toArray();
    for(looper=0;looper<arrayTemp.size();looper++)
    {
        g_replaceLine.findWhat.append(arrayTemp.at(looper).toString());
        qDebug() <<g_replaceLine.findWhat.at(looper);
    }

    qDebug() << "g_replaceLine.replaceWith:";
    arrayTemp=replaceLineObj.value("replace_with").toArray();
    for(looper=0;looper<arrayTemp.size();looper++)
    {
        g_replaceLine.replaceWith.append(arrayTemp.at(looper).toString());
        qDebug() <<g_replaceLine.replaceWith.at(looper);
    }
    g_replaceLine.replaceWhen=replaceLineObj.value("replace_when").toString();
    qDebug() << "g_replaceLine.replaceWhen:";
    qDebug() << g_replaceLine.replaceWhen;
    qDebug() << "\n";
    if(g_replaceLine.findWhat.size()!=g_replaceLine.replaceWith.size())
    {
        qDebug() << "Error! in replaceLineMode,findWhat.size()!=replaceWith.size().please check config.json file.";
        return false;
    }

    //解析要处理的文件列表
    arrayTemp=obj.value("process_files").toArray();
    for(looper=0;looper<arrayTemp.size();looper++)
    {
        g_files.append(arrayTemp.at(looper).toString());
    }
    if(g_files.size()==0)
    {
       qDebug() << "Error! no process files,please check config.json file.";
    }
    return true;
}

void replaceFiles()
{
    for(int i=0;i<g_files.size();i++)
    {
        replaceFullFile(g_files.at(i));
        replaceWithFlag(g_files.at(i));
        replaceLine(g_files.at(i));
    }
}

bool replaceFullFile(QString file)
{
    //输出对文件做替换的提示信息
     qDebug() << "replace file full text:";
     qDebug() << file;
     qDebug() << "\n";

     QFile loadFile(file);//加载要处理的文件
     if(!loadFile.open(QIODevice::ReadWrite))
     {
         qDebug() << "could't open file:";//打开文件失败，输出错误信息，返回false
         qDebug() << file;
         qDebug() << "\n";

         return false;
     }
     QByteArray allData = loadFile.readAll();//读取文件中所有信息
     loadFile.close();//关闭文件

     QTextCodec *tc = QTextCodec::codecForName("GBK");
     QString  allDataStr= tc->toUnicode(allData);
     for(int i=0;i<g_replaceFullFile.findWhat.size();i++)
     {
         allDataStr.replace( g_replaceFullFile.findWhat.at(i),g_replaceFullFile.replaceWith.at(i));
     }

     //把处理后的内容重新写入到文件
     loadFile.open(QIODevice::WriteOnly);
     QTextStream in(&loadFile);
     in <<allDataStr;
     loadFile.close();
     return true;
}

bool replaceLine(QString file)
{
    //输出对文件做替换的提示信息
     qDebug() << "replace file line:";
     qDebug() << file;
     qDebug() << "\n";

     QFile loadFile(file);//加载要处理的文件
     if(!loadFile.open(QIODevice::ReadWrite))
     {
         qDebug() << "could't open file:";//打开文件失败，输出错误信息，返回false
         qDebug() << file;
         qDebug() << "\n";

         return false;
     }

    QString strLine;
    QTextStream NctextStream(&loadFile);
    QString afterReplaceStr="";
    while(!NctextStream.atEnd())//一行一行地读取文件内容
    {
       strLine = NctextStream.readLine();
       if(strLine.indexOf(g_replaceLine.replaceWhen)!=-1)
       {
           for(int i=0;i<g_replaceLine.findWhat.size();i++)
           {
              strLine.replace(g_replaceLine.findWhat.at(i),g_replaceLine.replaceWith.at(i));
           }
       }
      afterReplaceStr.append(strLine);
      afterReplaceStr.append("\r\n");
    }
    loadFile.close();
    //把处理后的内容重新写入到文件
    loadFile.open(QIODevice::WriteOnly);
    QTextStream in(&loadFile);
    in <<afterReplaceStr;
    loadFile.close();
    return true;

}

//按照配置内容对文件进行替换
bool replaceWithFlag(QString file)
{
    //输出对文件做替换的提示信息
     qDebug() << "replace file with begin and end flag:";
     qDebug() << file;
     qDebug() << "\n";

     QFile loadFile(file);//加载要处理的文件
     if(!loadFile.open(QIODevice::ReadWrite))
     {
         qDebug() << "could't open file:";//打开文件失败，输出错误信息，返回false
         qDebug() << file;
         qDebug() << "\n";

         return false;
     }
     QByteArray allData = loadFile.readAll();//读取文件中所有信息
     loadFile.close();//关闭文件

     QTextCodec *tc = QTextCodec::codecForName("GBK");
     QString allDataStr = tc->toUnicode(allData);//保存原始的内容
     QString afterDataStr="";//保存处理之后的字符串

     int startIndex=allDataStr.indexOf(g_replaceWithFlags.begReplaceFlag);
     if(startIndex==-1)
     {
         qDebug() << "not found begReplace_flag do nothing\n";//如果没有开始替换的标志，return
         return true;
     }

     QString beforeBegFlagStr=allDataStr.mid(0,startIndex);//取开始替换标志之前的字符串（不包括开始标志）
     QString afterBegFlagStr=allDataStr.mid(startIndex);//取开始替换标志之后的字符串（包括开始标志）
     int endIndex=afterBegFlagStr.indexOf(g_replaceWithFlags.endReplaceFlag);//找开始标志之后的结束标志
     if(endIndex==-1)
     {
         //如果开始标志往后没有结束标志。直接处理所有开始标志之后的字符串
         for(int i=0;i<g_replaceWithFlags.findWhat.size();i++)
         {
            afterBegFlagStr.replace(g_replaceWithFlags.findWhat.at(i),g_replaceWithFlags.replaceWith.at(i));
         }
         afterDataStr.append(beforeBegFlagStr);
         afterDataStr.append(afterBegFlagStr);
     }else
     {
         //如果开始标志往后找到了结束标志
         QString midStr=afterBegFlagStr.mid(0,endIndex+g_replaceWithFlags.endReplaceFlag.length());//处理开始和结束之间的字符串标志之后的字符串包括结束标志
         for(int i=0;i<g_replaceWithFlags.findWhat.size();i++)
         {
             midStr.replace(g_replaceWithFlags.findWhat.at(i),g_replaceWithFlags.replaceWith.at(i));
         }
         afterDataStr.append(beforeBegFlagStr);
         afterDataStr.append(midStr);
         if(endIndex+g_replaceWithFlags.endReplaceFlag.length()<afterBegFlagStr.length())//如果结束标志不在文件末尾处
         {
             QString afterEndFlageStr=afterBegFlagStr.mid(endIndex+g_replaceWithFlags.endReplaceFlag.length());
             afterDataStr.append(afterEndFlageStr);
         }
      }

     //把处理后的内容重新写入到文件
     loadFile.open(QIODevice::WriteOnly);
     QTextStream in(&loadFile);
     in <<afterDataStr;
     loadFile.close();
     return true;
}
