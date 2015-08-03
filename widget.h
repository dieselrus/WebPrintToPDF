#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QWebView>
#include <QList>
#include <QNetworkRequest>
#include <QPrinter>
#include <QtWebKit>
#include <QWebPage>
#include <QWebFrame>
#include <QPainter>

class MyCookieJar : public QNetworkCookieJar
{
    public:
        QList<QNetworkCookie> getAllCookies() { return allCookies(); }
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QList <QNetworkCookie > cookies;
    MyCookieJar *cookieJar;
    QNetworkAccessManager* networkManager;
    QWebView *w;
    QPrinter* printer;
    QWebPage *webpage;
    QPainter* painter;

private slots:
    void pbAuthClick();
    void replyFinish(QNetworkReply* reply);
    void loadPage(QUrl &url);
    void printPDF();
    void goStartUrl();
    void loadFinished(bool);

};

#endif // WIDGET_H
