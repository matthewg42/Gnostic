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
    void deleteCurrentTransport();
    void saveCurrentTransport();
    void testCurrentTransport();

private slots:
    void doTest();

    //! the transportCombo value has changed so we need to update the
    //! type of TransportConfigWidget we embed.
    //! \param newType a string description of the new type
    //! \param newTransport if NULL, a new transport object will be created of the
    //!        required type, else the passed object will be used and attached to the
    //!        new config dialog.
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
