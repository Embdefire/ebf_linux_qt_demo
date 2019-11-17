/******************************************************************
 Copyright (C) 2019 - All Rights Reserved by
 文 件 名 : main.cpp ---
 作 者    : Niyh(lynnhua)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
  Niyh	   2019    	1.0.0 1     文件创建
*******************************************************************/
#include "mainwindow.h"
#include "splashscreen.h"
#include "unit.h"
#include "skin.h"
#include "appconfig.h"

#include <QApplication>
#include <QTextCodec>
#include <QDesktopWidget>
#include <QScreen>
#include <QDir>
#include <QTranslator>
#include <QDebug>

void CheckDir(const QString &path) {
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(path);
    }
}

void CheckDirs(const QString &path, const QStringList &dirs) {
    for (int i = 0; i < dirs.size(); i++) {
        CheckDir(path + dirs.at(i));
    }
}


int main(int argc, char *argv[])
{
    // 设置输入法
#if 1
    qputenv("QT_IM_MODULE", QByteArray("xyinput"));
#else
    qputenv("QT_IM_MODULE", QByteArray("qtkeyboard"));
#endif

    QApplication a(argc, argv);
    // 初始化皮肤文件
    Skin::InitSkin();

    // 检查目录
    CheckDirs(a.applicationDirPath() + "/",
              QStringList() << "conf" << "music" << "video" << "nes" << "translations"
              << "ebook" << "notepad" << "photos" << "record" << "download");

    // 设置程序中文编码
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    // 设置配置文件路径
    AppConfig::m_strSettingsFile = a.applicationDirPath() + "/conf/config.ini";

    // 安装翻译文件
    QString strTemp = qApp->applicationDirPath() + "/translations/";
    strTemp += AppConfig::ReadSetting("System", "language", "qt_zh.qm").toString();
    QTranslator translator;
    if (translator.load(strTemp)) a.installTranslator(&translator);

#if 0
    // 启动界面
    SplashScreen *splash = new SplashScreen();
    a.processEvents();

    // 启动主程序
    splash->SetMainWidget(new MainWindow);
    splash->Start();
#else
    MainWindow w;
    w.setWindowTitle(QStringLiteral("野火 @ Linux Qt Demo"));

#ifdef __arm__
    QSize size = a.primaryScreen()->availableGeometry().size();
    // w.resize(size.width(), size.height());
    // 分辨率读取不对，暂时先指定HDMI分辨率为1280*720
    w.resize(1280, 720);    
    qDebug() << "primaryScreen availableGeometry" << size;
    w.showFullScreen();
#else
    w.resize(800, 480);
//    w.resize(480, 272);
    w.show();
#endif

#endif

    return a.exec();
}
