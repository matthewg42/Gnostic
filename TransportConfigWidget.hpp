#ifndef TRANSPORTCONFIGWIDGET_HPP
#define TRANSPORTCONFIGWIDGET_HPP

#include <QWidget>

class TransportConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransportConfigWidget(QWidget *parent=0);
    virtual ~TransportConfigWidget() = 0;

signals:
    void configChanged();

protected slots:
    void emitChanged();

};

#endif // TRANSPORTCONFIGWIDGET_HPP
