#ifndef PORTWRAPPER_MA_H
#define PORTWRAPPER_MA_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QDebug>

class PortWrapper_ma : public QObject
{
    Q_OBJECT
public:
    explicit PortWrapper_ma(QObject *parent = nullptr);

    /**
     * @brief Liste der Ports
     * @return Liste
     *
     * fragt im System die Liste der Seriellen Ports ab und gibt sie zurück.
     */
    QList<QSerialPortInfo> getSerialPortList();

    /**
     * @brief Port öffnen
     * @param portNr Nr des Ports
     *
     * Öffnet den seriellen Port. Die nr ist aus SerialPortList (siehe getSerialPortList()).
     */
    void openPort(int portNr);

public slots:
    /**
     * @brief AsciiMode auswählen
     *
     * wählt zur Kommuniaktion den Modus ASCII (Terminal) aus.
     */
    void selectAsciiMode();

    /**
     * @brief ein Zeichen Senden
     * @param sendC Zeichen
     *
     * Die Methode sendet ein Zeichen an die serielle Schnittstelle.
     */
    void sendChar(char sendC);

    /**
     * @brief Inhalt des Empfangspuffers
     * @return String des Inhalts (mit \\n als Zeilenumbruch)
     */
    QString getRecString();

signals:

private slots:

    /**
     * @brief Daten lesen und in Puffer
     *
     * liest die ankommenden Datenautomatisch und speicher sie im Empfangspuffer ab.
     * Auslesen des Puffers ist mit getRecString() möglich
     */
    void readData();

private:
    /// Serieller Port
    QSerialPort serial;

    /// Liste der Ports
    QList<QSerialPortInfo> portList;

    /// interner Puffer
    QByteArray recByteArray;
};

#endif // PORTWRAPPER_MA_H
