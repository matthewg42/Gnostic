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

public slots:
    bool testTransport();
    Transport* getTransport();

protected:
    Transport* transport;
};

#endif // TRANSPORTCONFIGWIDGET_HPP
