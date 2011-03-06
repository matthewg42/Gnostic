#ifndef TRANSPORTCONFIG_HPP
#define TRANSPORTCONFIG_HPP

#include <QWidget>

namespace Ui {
    class TransportConfig;
}

class TransportConfig : public QWidget
{
    Q_OBJECT

public:
    explicit TransportConfig(QWidget *parent = 0);
    ~TransportConfig();

private:
    Ui::TransportConfig *ui;
};

#endif // TRANSPORTCONFIG_HPP
