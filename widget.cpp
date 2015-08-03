#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->pbGoToStartUrl, SIGNAL(clicked(bool)), this, SLOT(goStartUrl()));
    connect(ui->pbPrintPDF, SIGNAL(clicked(bool)), this, SLOT(printPDF()));

    ui->pbPrintPDF->setEnabled(false);

    cookieJar = new MyCookieJar;
    //qDebug() << cookieJar->getAllCookies();

    printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A4);
    //printer->setPageOrientation(QPageLayout::Landscape);
    printer->setOrientation(QPrinter::Portrait);
    printer->setOutputFormat(QPrinter::PdfFormat);

    w = new QWebView();
    webpage = new QWebPage();

    connect(webpage, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::pbAuthClick(){
   ui->txtLog->append("Auth");

   QUrl serviceUrl = QUrl("http://developer.alexanderklimov.ru/blocks/testreg.php");
   QByteArray postData;

   postData.append(ui->leLogin->text()); //"login=denisgamza@yandex.ru");
   postData.append(ui->lePassword->text()); //"password=azmam");

   QNetworkRequest request(serviceUrl);
   request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
   request.setRawHeader("Accept-Encoding","identity"); // Отменяю сжатие что бы, можно было нормально просматривать контент снифером
   request.setRawHeader("Accept-Language","ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3");
   request.setRawHeader("Connection","keep-alive");
   request.setRawHeader("Host","developer.alexanderklimov.ru");
   //request.setRawHeader("Connection","keep-alive");
   request.setRawHeader("User-Agent","Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:39.0) Gecko/20100101 Firefox/39.0");

   QString param = "login=denisgamza@yandex.ru&password=azmam";
   QByteArray postz = param.toUtf8();

   networkManager = new QNetworkAccessManager(this);
   networkManager->setCookieJar(cookieJar);
   connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinish(QNetworkReply*)));
   networkManager->post(request, postz);

}

void Widget::replyFinish(QNetworkReply* reply){
    //qDebug() << reply;
    //qDebug() << cookieJar->getAllCookies();

    cookies = cookieJar->getAllCookies();

     ui->txtLog->append("Cookies");

    for(int i = 0; i < cookies.length(); i++){
        ui->txtLog->append(cookies[i].name() + "=" + cookies[i].value());
    }

    w->page()->setNetworkAccessManager(networkManager);

    w->page()->setViewportSize(QSize(8000,8000));
    ui->pbPrintPDF->setEnabled(true);
}

void Widget::loadPage(QUrl &url){
    w->load(url);

}

void Widget::printPDF(){
    printer->setOutputFileName(w->title() + ".pdf");

    printer->setFullPage(true);
    //printer->setPaperSize(QSize(ui->webView->contentsRect().height(), ui->webView->contentsRect().width()),QPrinter::DevicePixel);
    //w->print(printer);

    QSize contentsSize = webpage->mainFrame()->contentsSize();
    webpage->setViewportSize ( contentsSize );
    printer->setPaperSize ( QSizeF( contentsSize ), QPrinter::DevicePixel );

    printer->setOutputFileName(webpage->mainFrame()->title() + ".pdf");

    painter = new QPainter ( printer );
    painter->setRenderHint ( QPainter::Antialiasing );
    webpage->mainFrame()->render ( painter );
    painter->end();


    //webpage->mainFrame()->print(printer);

    ui->pbPrintPDF->setEnabled(false);
}

void Widget::goStartUrl(){
    //loadPage(QUrl(ui->leStartUrl->text()));
    //w->load(QUrl(ui->leStartUrl->text()));
    //w->show();

    webpage->mainFrame()->load(QUrl(ui->leStartUrl->text()));
}

void Widget::loadFinished(bool){
    //QSize size = webpage->mainFrame()->contentsSize();
}
