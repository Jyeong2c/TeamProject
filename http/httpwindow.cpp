// Copyright (C) 2020 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "httpwindow.h"

#include "ui_authenticationdialog.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QUrl>

#include <algorithm>
#include <memory>

/*int형 포트번호를 8000번으로 설정*/
const int PORT_NUMBER = 8000;

#if QT_CONFIG(ssl)
/*오픈할 서버의 포트는 8001번 이므로 원래 포트번호에 +1을 하여 서버주소를 맞춤*/
QString defaultUrl = "http://127.0.0.1:" + QString::number(PORT_NUMBER + 1);
#else
const char defaultUrl[] = "http://www.qt.io/";
#endif
const char defaultFileName[] = "test.html";

/*프로그래스 다이얼로그를 사용하여 다운로드 과정을 출력*/
ProgressDialog::ProgressDialog(const QUrl &url, QWidget *parent)
    : QProgressDialog(parent)
{
    setWindowTitle(tr("Download Progress"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setLabelText(tr("Downloading %1.").arg(url.toDisplayString()));
    setMinimum(0);
    setValue(0);
    setMinimumDuration(0);
    setMinimumSize(QSize(400, 75));
}

void ProgressDialog::networkReplyProgress(qint64 bytesRead, qint64 totalBytes)
{
    setMaximum(totalBytes);
    setValue(bytesRead);
}

/*HttpWindow 클래스의 생성자 내부의 변수 초기화와 UI의 구성을 나열*/
HttpWindow::HttpWindow(QWidget *parent)
    : QDialog(parent)
    , statusLabel(new QLabel(tr("Please enter the URL of a file you want to download.\n\n"), this))
    , urlLineEdit(new QLineEdit(defaultUrl))
    , downloadButton(new QPushButton(tr("Download")))
    , fileDialogButton(new QPushButton(tr("fileDialog")))   //파일다이얼로그 버튼을 띄우기 위한 변수 초기화
    , launchCheckBox(new QCheckBox("Launch file"))
    , defaultFileLineEdit(new QLineEdit(defaultFileName))
    , downloadDirectoryLineEdit(new QLineEdit)
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("HTTP"));

    //! [qnam-auth-required-1]
    connect(&qnam, &QNetworkAccessManager::authenticationRequired,
            this, &HttpWindow::slotAuthenticationRequired);
    //! [qnam-auth-required-1]

    QFormLayout *formLayout = new QFormLayout;      //UI 구성을 위한 폼 레이아웃
    urlLineEdit->setClearButtonEnabled(true);
    connect(urlLineEdit, &QLineEdit::textChanged, this, &HttpWindow::enableDownloadButton);
    formLayout->addRow(tr("&URL:"), urlLineEdit);

    /*폴더에서 html을 다운로드 할 수 있는 경로*/
    QString downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if (downloadDirectory.isEmpty() || !QFileInfo(downloadDirectory).isDir())
        downloadDirectory = QDir::currentPath();

    /*수평 레이아웃으로 다운로드 경로 에디트랑 파일다이얼로그 버튼을 레이아웃*/
    QHBoxLayout* hlayout = new QHBoxLayout;
    /*다운로드할 폴더를 자동 저장, 이후 파일 다이얼로그를 호출*/
    downloadDirectoryLineEdit->setText(QDir::toNativeSeparators(downloadDirectory));
    fileDialogButton->setAutoDefault(false);
    connect(fileDialogButton, &QPushButton::clicked, this, &HttpWindow::on_fileDialogButton_clicked);
    hlayout->addWidget(downloadDirectoryLineEdit);
    hlayout->addWidget(fileDialogButton);

    /*폼 레이아웃에서 hlayout 했던 부분과 파일 경로 에디터를 레이아웃*/
    formLayout->addRow(tr("Download directort:"), hlayout);
    formLayout->addRow(tr("Default &file:"), defaultFileLineEdit);


    launchCheckBox->setChecked(true);
    formLayout->addRow(launchCheckBox);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);

    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

    statusLabel->setWordWrap(true);
    mainLayout->addWidget(statusLabel);

    /*파일다운로드 버튼을 연동하는 connect 함수*/
    downloadButton->setDefault(true);
    connect(downloadButton, &QAbstractButton::clicked, this, &HttpWindow::downloadFile);

    /*UI 우측하단의 두개의 버튼을 레이아웃을 표시*/
    QPushButton *quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttonBox);

    urlLineEdit->setFocus();
}
HttpWindow::~HttpWindow() = default;

void HttpWindow::startRequest(const QUrl &requestedUrl)
{
    url = requestedUrl;
    httpRequestAborted = false;

    //! [qnam-download]
    reply.reset(qnam.get(QNetworkRequest(url)));
    //! [qnam-download]
    //! [connecting-reply-to-slots]
    connect(reply.get(), &QNetworkReply::finished, this, &HttpWindow::httpFinished);
    //! [networkreply-readyread-1]
    connect(reply.get(), &QIODevice::readyRead, this, &HttpWindow::httpReadyRead);
    //! [networkreply-readyread-1]
#if QT_CONFIG(ssl)
    //! [sslerrors-1]
    connect(reply.get(), &QNetworkReply::sslErrors, this, &HttpWindow::sslErrors);
    //! [sslerrors-1]
#endif
    //! [connecting-reply-to-slots]

    //프로그래스바가 100%로 채워지면 다운로드를 완료하는 과정을 표시
    ProgressDialog *progressDialog = new ProgressDialog(url, this);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(progressDialog, &QProgressDialog::canceled, this, &HttpWindow::cancelDownload);
    connect(reply.get(), &QNetworkReply::downloadProgress,
            progressDialog, &ProgressDialog::networkReplyProgress);
    connect(reply.get(), &QNetworkReply::finished, progressDialog, &ProgressDialog::hide);
    progressDialog->show();

    statusLabel->setText(tr("Downloading %1...").arg(url.toString()));
}


/*다운로드 파일 상태를 출력하는 함수*/
void HttpWindow::downloadFile()
{
    /*urlSpec 정보를 나타내는 변수*/
    const QString urlSpec = urlLineEdit->text().trimmed(); //시작과 끝에서 공백이 제거된 문자열을 반환합니다.
    if (urlSpec.isEmpty())      //데이터 정보가 없다면 downloadFile함수를 나옴
        return;

    const QUrl newUrl = QUrl::fromUserInput(urlSpec);
    if (!newUrl.isValid()) {
        QMessageBox::information(this, tr("Error"),
                                 tr("Invalid URL: %1: %2").arg(urlSpec, newUrl.errorString()));
        return;
    }

    QString fileName = newUrl.fileName();
    if (fileName.isEmpty())
        fileName = defaultFileLineEdit->text().trimmed();
    if (fileName.isEmpty())
        fileName = defaultFileName;
    QString downloadDirectory = QDir::cleanPath(downloadDirectoryLineEdit->text().trimmed());
    bool useDirectory = !downloadDirectory.isEmpty() && QFileInfo(downloadDirectory).isDir();
    if (useDirectory)
        fileName.prepend(downloadDirectory + '/');
    if (QFile::exists(fileName)) {
        if (QMessageBox::question(this, tr("Overwrite Existing File"),
                                  tr("There already exists a file called %1%2."
                                     " Overwrite?")
                                  .arg(fileName,
                                       useDirectory
                                       ? QString()
                                       : QStringLiteral(" in the current directory")),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No)
                == QMessageBox::No) {
            return;
        }
        QFile::remove(fileName);
    }

    file = openFileForWrite(fileName);
    if (!file)
        return;

    downloadButton->setEnabled(false);

    // schedule the request
    startRequest(newUrl);
}


//스마트 포인터를 이용하여 파일 쓰기위한 open함수 구현
std::unique_ptr<QFile> HttpWindow::openFileForWrite(const QString &fileName)
{
    std::unique_ptr<QFile> file = std::make_unique<QFile>(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Error"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg(QDir::toNativeSeparators(fileName),
                                      file->errorString()));
        return nullptr;
    }
    return file;
}

void HttpWindow::cancelDownload() //다운로드 중에 취소
{
    statusLabel->setText(tr("Download canceled."));
    httpRequestAborted = true;
    reply->abort();
    downloadButton->setEnabled(true);
}

void HttpWindow::httpFinished() //Http 다운로드 완료여부 메시지를 출력하는 함수
{
    QFileInfo fi;
    if (file) {
        fi.setFile(file->fileName());
        file->close();
        file.reset();
    }

    //! [networkreply-error-handling-1]
    QNetworkReply::NetworkError error = reply->error();
    const QString &errorString = reply->errorString();
    //! [networkreply-error-handling-1]
    reply.reset();
    //! [networkreply-error-handling-2]
    if (error != QNetworkReply::NoError) {
        QFile::remove(fi.absoluteFilePath());
        // For "request aborted" we handle the label and button in cancelDownload()
        if (!httpRequestAborted) {
            statusLabel->setText(tr("Download failed:\n%1.").arg(errorString));
            downloadButton->setEnabled(true);
        }
        return;
    }
    //! [networkreply-error-handling-2]

    statusLabel->setText(tr("Downloaded %1 bytes to %2\nin\n%3")
                         .arg(fi.size())
                         .arg(fi.fileName(), QDir::toNativeSeparators(fi.absolutePath())));
    if (launchCheckBox->isChecked())
        QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absoluteFilePath()));
    downloadButton->setEnabled(true);
}

//! [networkreply-readyread-2]
void HttpWindow::httpReadyRead()
{
    // This slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}
//! [networkreply-readyread-2]

void HttpWindow::enableDownloadButton()     //url 경로가 활성화 될때, 다운로드 활성화 반대일 경우 비활성화
{
    downloadButton->setEnabled(!urlLineEdit->text().isEmpty());
}

//! [qnam-auth-required-2]
void HttpWindow::slotAuthenticationRequired(QNetworkReply *, QAuthenticator *authenticator)
{
    QDialog authenticationDialog;
    Ui::Dialog ui;
    ui.setupUi(&authenticationDialog);
    authenticationDialog.adjustSize();
    ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm(), url.host()));

    // Did the URL have information? Fill the UI.
    // This is only relevant if the URL-supplied credentials were wrong
    ui.userEdit->setText(url.userName());
    ui.passwordEdit->setText(url.password());

    if (authenticationDialog.exec() == QDialog::Accepted) {
        authenticator->setUser(ui.userEdit->text());
        authenticator->setPassword(ui.passwordEdit->text());
    }
}
//! [qnam-auth-required-2]

#if QT_CONFIG(ssl)
//! [sslerrors-2]
void HttpWindow::sslErrors(const QList<QSslError> &errors)      //SSL Error?
{
    QString errorString;
    for (const QSslError &error : errors) {
        if (!errorString.isEmpty())
            errorString += '\n';
        errorString += error.errorString();
    }

    if (QMessageBox::warning(this, tr("SSL Errors"),
                             tr("One or more SSL errors has occurred:\n%1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        reply->ignoreSslErrors();
    }
}
//! [sslerrors-2]
#endif

/*파일 다이얼로그위젯을 호출하여 파일 경로를 에디터에 출력*/
void HttpWindow::on_fileDialogButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    "경로 선택",
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly);
    downloadDirectoryLineEdit->setText(dir);
}
