#ifndef TRANSPORTEDITORFORM_HPP
#define TRANSPORTEDITORFORM_HPP

#include <QWidget>

namespace Ui {
    class TransportEditorForm;
}

class TransportEditorForm : public QWidget
{
    Q_OBJECT

public:
    explicit TransportEditorForm(QWidget *parent = 0);
    ~TransportEditorForm();

private:
    Ui::TransportEditorForm *ui;
};

#endif // TRANSPORTEDITORFORM_HPP
