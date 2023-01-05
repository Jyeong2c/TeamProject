// Copyright (C) 2020 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef HTTPWINDOW_H
#define HTTPWINDOW_H

#include <QProgressDialog>
#include <QNetworkAccessManager>
#include <QUrl>

#include <memory>

QT_BEGIN_NAMESPACE
class QFile;
class QLabel;
class QLineEdit;
class QPushButton;
class QSslError;
class QAuthenticator;
class QNetworkReply;
class QCheckBox;

QT_END_NAMESPACE

class IdNameBody;

class ProgressDialog : public QProgressDialog {
    Q_OBJECT

public:
    explicit ProgressDialog(const QUrl &url, QWidget *parent = nullptr);

public slots:
   void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);
};

class HttpWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HttpWindow(QWidget *parent = nullptr);
    ~HttpWindow();

    void startRequest(const QUrl &requestedUrl);

    //char* defaultUrl;
    //int HttpServer();       //Http Server를 만들기 위한 함수 -> 실패



private slots:
    void downloadFile();
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    void enableDownloadButton();
    void slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator);
    //Urltext 그대로 가져오기
    //void Urltext();
    void on_fileDialogButton_clicked();


#if QT_CONFIG(ssl)
    void sslErrors(const QList<QSslError> &errors);
#endif

private:
    std::unique_ptr<QFile> openFileForWrite(const QString &fileName);

    QLabel *statusLabel;
    QLineEdit *urlLineEdit;
    QPushButton *downloadButton;
    QPushButton *fileDialogButton;
    QCheckBox *launchCheckBox;
    QLineEdit *defaultFileLineEdit;
    QLineEdit *downloadDirectoryLineEdit;

    QUrl url;
    QNetworkAccessManager qnam;
    QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> reply; //
    std::unique_ptr<QFile> file;
    bool httpRequestAborted = false;


};

#endif
