#include "ScriptedCommandListener.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <stdexcept>

const QString ScriptedCommandListener::SIZE_DELIMITER(":");

ScriptedCommandListener::ScriptedCommandListener(int port, QObject * parent)
    : QObject(parent)
{
    m_tcpServer = new QTcpServer( this);

    /// make sure we know when new connection is established
    connect( m_tcpServer, & QTcpServer::newConnection,
             this, & ScriptedCommandListener::newConnectionCB);

    /// now listen to the port
    if( ! m_tcpServer-> listen( QHostAddress::AnyIPv4, port)) {
        throw std::runtime_error("Coud not listen for scripted commands on given port");
    }
}

void ScriptedCommandListener::newConnectionCB()
{
    qDebug() << "New scripted client connection...";
    if( m_connection != nullptr) {
        qWarning() << "Another client trying to connect? Ignoring...";
        return;
    }
    m_connection = m_tcpServer->nextPendingConnection();
    connect( m_connection, & QTcpSocket::readyRead,
             this, & ScriptedCommandListener::socketDataCB);

}

void ScriptedCommandListener::socketDataCB()
{
    qDebug() << "scripted command listener: socket data ready";
    QString str;
    bool result = receiveTypedMessage( "whatever", str );
    if (result == true) {
        str = str.trimmed();
        emit command( str);
    }
    else {
        qDebug() << "something went wrong in ScriptedCommandListener::socketDataCB()";
    }
}

QString ScriptedCommandListener::dataTransporter( QString input )
{
    int inputSize = input.size();
    // Prepend the data with "size:"
    // Python can then partition the incoming data to determine how much data
    // it should actually be receiving and make a second attempt to get the
    // rest, if necessary.
    input = QString::number(inputSize) + ScriptedCommandListener::SIZE_DELIMITER + input;
    m_connection->write( input.toLocal8Bit() );
    return input;
}

bool ScriptedCommandListener::receiveNBytes( int n, QString& data )
{
    bool result = true;
    char buff[n];
    qint64 lineLength = m_connection-> readLine( buff, n );
    if( lineLength == -1) {
        qWarning() << "scripted command listener: something wrong with socket";
        result = false;
    }

    if( lineLength == 0) {
        qDebug() << "scripted command listener: not a full line yet...";
        result = false;
    }

    if (result == true) {
        data = buff;
    }
    return result;
}

bool ScriptedCommandListener::receiveMessage( QString& data )
{
    //format: 4, 6, or 8 bytes: the size of the following message
    //after receiving this, enter a loop to receive this number of bytes
    bool result = receiveNBytes( 1000000, data );
    return result;
}

bool ScriptedCommandListener::receiveTypedMessage( QString messageType, QString& data )
{
    bool result = receiveMessage( data );
    return result;
}
