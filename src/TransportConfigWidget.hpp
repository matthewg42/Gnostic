#ifndef TRANSPORTCONFIGWIDGET_HPP
#define TRANSPORTCONFIGWIDGET_HPP

#include <QWidget>

class Transport;

//! \class TransportConfigWidget
//! An abstract class whose derivatives are used to configuure the derivatives of
//! Transport.
class TransportConfigWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TransportConfigWidget(Transport* t, QWidget *parent = 0);
	~TransportConfigWidget() = 0;

signals:
	void wasUpdated();

public slots:
	bool test();
	Transport* getTransport();

protected slots:
	void madeUpdate();

protected:
	Transport* transport;
};

#endif // TRANSPORTCONFIGWIDGET_HPP
