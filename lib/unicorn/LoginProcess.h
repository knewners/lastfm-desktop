#ifndef LOGIN_PROCESS_H
#define LOGIN_PROCESS_H

#include "UnicornSession.h"

#include "lib/DllExportMacro.h"

#include <QObject>
#include <QPointer>

class QHostAddress;
class QTcpServer;
class QTcpSocket;

namespace unicorn
{


/**
* This class is used during the web authentication process in order to be able
* to return to the application after the authentication is completed.
*/
class UNICORN_DLLEXPORT TinyWebServer: public QObject
{
    Q_OBJECT
public:
    TinyWebServer( QObject* parent = 0 );

    int serverPort() const;
    QHostAddress serverAddress() const;

signals:
    void gotToken( QString token );

private:
    void processRequest();
    void sendRedirect();

private slots:
    void onNewConnection();
    void readFromSocket();

private:
    QPointer<QTcpServer> m_tcpServer;
    QPointer<QTcpSocket> m_clientSocket;
    QString     m_header;
    QString     m_token;
};

/**
 * This class encapsulates the whole login process.
 *
 * Call the authenticate function to start the login process
 * and connect to the gotSession signal to be notified when
 * the process finishes.
 */
class UNICORN_DLLEXPORT LoginProcess : public QObject
{
    Q_OBJECT
public:
    LoginProcess( QObject* parent = 0 );
    ~LoginProcess();
    QString token() const;
    QUrl authUrl() const;
    void showError() const;

public slots:
    void authenticate();
    void getSession( QString token );
    void cancel();

signals:
    void gotSession( unicorn::Session* s );

private slots:
    void onGotSession();
    void onGotDesktopToken();

private: 
    QPointer<TinyWebServer> m_webServer;
    QString m_token;
    lastfm::ws::ParseError m_lastError;
    QNetworkReply::NetworkError m_lastNetworkError;
    QUrl m_authUrl;
};

}// namespace unicorn

#endif // LOGIN_PROCESS_H
