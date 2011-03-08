#ifndef PASSWORDDIALOG_HPP
#define PASSWORDDIALOG_HPP

#include <QDialog>

namespace Ui {
    class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(const QString& message, QWidget *parent = 0);
    ~PasswordDialog();

    const QString& getPassword() {return pass;}

private slots:
    void setPassword();

private:
    Ui::PasswordDialog *ui;
    QString pass;
};

#endif // PASSWORDDIALOG_HPP
