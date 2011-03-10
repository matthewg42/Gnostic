#include "GnosticParser.hpp"

#include <QVariant>
#include <QDebug>
#include <QStringList>
#include <QRegExp>

static QRegExp chompRe("[\\n\\r]+");

GnosticParser::GnosticParser(QObject *parent) :
    QObject(parent),
    hostId(),
    monitorId(),
    delimiter(';'),
    inHeader(false)
{
	setSplitRe();
}

void GnosticParser::setSplitRe()
{
	QString s = QString("^(\\d+(\\.\\d*)?)%1(-?\\d+(\\.\\d*)?)%2(.*)$").arg(delimiter).arg(delimiter);
	qDebug() << "GnosticParser::setSplitRe was set to" << s;
	splitRe = QRegExp(s);
}

void GnosticParser::takeLine(QString line)
{
	line.remove(chompRe);
	qDebug() << "GnosticParser::takeLine" << line;
	// First do header checking...
	if (line == "GNOSTIC-DATA-PROTOCOL-VERSION=1.0")
	{
		inHeader = true;
		qDebug() << "GnosticParser::takeLine we have a header" << line;
		return;
	}
	if (inHeader)
	{
		if (line.startsWith("HOST="))
		{
			hostId = line;
			hostId.remove(0, 5);
			qDebug() << "GnosticParser::takeLine in header, set HOST" << hostId;
		}
		else if (line.startsWith("MONITOR-NAME=")) {
			monitorId = line;
			monitorId.remove(0, 13);
			qDebug() << "GnosticParser::takeLine in header, set MONITOR-NAME" << monitorId;
		}
		else if (line.startsWith("DELIMITER=") && line.length() > 10) {
			delimiter = line.at(10);
			qDebug() << "GnosticParser::takeLine in header, set DELIMITER" << delimiter;
			setSplitRe();
		}
		else if (line == "END-HEADER")
		{
			inHeader = false;
		}
		else
		{
			qDebug() << "GnosticParser::takeLine unrecognized header line" << line << "SKIPPING";
		}
		return;
	}

	// right, if we get here we're not in a header...
	emit(spewLine(line));

	if (splitRe.exactMatch(line))
	{
		double timestamp, value;
		bool ok;
		timestamp = QVariant(splitRe.capturedTexts().at(1)).toDouble(&ok);
		if (!ok)
		{
			qWarning() << "GnosticParser::takeLine failed convert timestamp to double" << splitRe.capturedTexts().at(1);
			return;
		}
		value = QVariant(splitRe.capturedTexts().at(3)).toDouble(&ok);
		if (!ok)
		{
			qWarning() << "GnosticParser::takeLine failed convert value to double" << splitRe.capturedTexts().at(3);
			return;
		}

		qDebug() << "GnosticParser::takeLine emitting data item" << timestamp << value << splitRe.capturedTexts().at(5);
		emit(spewDataItem(timestamp, value, splitRe.capturedTexts().at(5)));
	}
}
