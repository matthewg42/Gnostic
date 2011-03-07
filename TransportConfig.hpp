#ifndef TRANSPORTCONFIG_HPP
#define TRANSPORTCONFIG_HPP

#include <QWidget>
#include <QModelIndex>
#include "Transport.hpp"

namespace Ui {
    class TransportConfig;
}

class QStandardItemModel;

class TransportConfig : public QWidget
{
    Q_OBJECT

public:
    explicit TransportConfig(QWidget *parent = 0);
    ~TransportConfig();

public slots:
    void createNewTransport();
    void saveCurrentTransport();

private slots:
    void doTest();
    void changeTransportType(const QString newType, Transport* newTransport=0);
    void populateTransportModel();
    void transportSelectionChanged(QModelIndex i);
    void loadTransportWithId(const QString& id);
    void configUpdated();

private:
    Ui::TransportConfig *ui;
    Transport* transport;
    QStandardItemModel* model;
    TransportConfigWidget* configWidget;

};

#endif // TRANSPORTCONFIG_HPP
