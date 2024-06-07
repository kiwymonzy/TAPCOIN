#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

void sendPostRequest(QNetworkAccessManager *manager) {
    QUrl url("https://api.tapcoins.xyz/coin/collect");
    QNetworkRequest request(url);

    QString  bearerToken = "U_22wCDZcZsQY8MsyRe5aQMWpjJZmKKJ8t.tAwwsDHZm58maAYYSybtSjJ57aayXr6ZZbD66Q6K8nWjFs6ekRwAHxCmSXEyPnP3";

    request.setRawHeader("Sec-Fetch-Dest", "empty");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Sec-Fetch-Site", "same-site");
    request.setRawHeader("Origin", "https://tapcoins.xyz");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 17_5_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148");
    request.setRawHeader("token", "Bearer "+bearerToken.toUtf8());
    request.setRawHeader("Sec-Fetch-Mode", "cors");
    request.setRawHeader("Referer", "https://tapcoins.xyz/");
    request.setRawHeader("Host", "api.tapcoins.xyz");
    request.setRawHeader("Accept-Language", "en-GB,en;q=0.9");
    request.setRawHeader("Accept", "application/json, text/plain, */*");

    QUrlQuery params;
    params.addQueryItem("coin", "1000");
    params.addQueryItem("power", "2000");
    params.addQueryItem("turbo", "3");
    params.addQueryItem("_token", bearerToken.toUtf8());

    QByteArray data = params.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkReply *reply = manager->post(request, data);
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            //qDebug() << "Response:" << response;

            // Check for code:0 and handle balance
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject["code"].toInt() == 0) {
                QJsonObject dataObject = jsonObject["data"].toObject();
                QJsonObject userInfoObject = dataObject["userInfo"].toObject();
                int balance = userInfoObject["coin"].toInt();

                QDateTime currentTime = QDateTime::currentDateTime();
                qDebug() << "__________________________________________";
                qDebug() << "Current Time:" << currentTime.toString("HH:mm:ss");
                qDebug() << "Balance:" << balance;
                qDebug() << "------------------------------------------";
                qDebug() << " ";
            }
        } else {
            //qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QNetworkAccessManager manager;

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&manager]() {
        sendPostRequest(&manager);
    });
    timer.start(500); // Send request every 500ms (0.5 second)

    return a.exec();
}
