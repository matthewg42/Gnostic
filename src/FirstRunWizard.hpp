#ifndef FIRSTRUNWIZARD_HPP
#define FIRSTRUNWIZARD_HPP

#include <QWizard>
#include <QString>

namespace Ui {
        class FirstRunWizard;
}

class FirstRunWizard : public QWizard
{
        Q_OBJECT

public:
        explicit FirstRunWizard(QWidget *parent = 0);
        ~FirstRunWizard();
        const QString getIniPath() { return iniPath; }

private slots:
        void setSample();
        void setEmpty();
        void setSelect();
        void pathEdited(QString);
        void browseForPath();

private:
        Ui::FirstRunWizard *ui;
        QString iniPath;
};

#endif // FIRSTRUNWIZARD_HPP
