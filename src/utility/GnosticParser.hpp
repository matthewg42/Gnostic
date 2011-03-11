#ifndef GNOSTICPARSER_HPP
#define GNOSTICPARSER_HPP

#include <QObject>
#include <QRegExp>
#include <QString>

//! \class GnosticParser
//! An instance of this object is attached to a Transport and receives raw data
//! which the Transport spews out.  The GnosticParser object interprets
//! the incoming lines according to the Gnostic Data Protocol, setting internal
//! state in response to recognized header lines.
//! For all other input lines, the GnosticParser emits a spewLine with the
//! raw line data, and, if the incoming line matches the Gnostic Data Protocol
//! pattern for plottable data, also emits a spewDataItem with the data
//! in a format read for graphing.
class GnosticParser : public QObject
{
	Q_OBJECT
public:
	explicit GnosticParser(QObject *parent = 0);

signals:
	void spewLine(const QString line);
	void spewDataItem(const double timestamp, const double value, const QString label);

public slots:
	void takeLine(QString line);

protected:
	void setSplitRe();

protected:
	QString hostId;
	QString monitorId;
	QString invocationPath;
	QString invocationArgs;
	QChar delimiter;
	bool inHeader;
	QRegExp splitRe;

};

#endif // GNOSTICPARSER_HPP
