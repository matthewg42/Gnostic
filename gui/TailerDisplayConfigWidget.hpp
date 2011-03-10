#ifndef TAILERDISPLAYCONFIGWIDGET_HPP
#define TAILERDISPLAYCONFIGWIDGET_HPP

#include "DataDisplayConfigWidget.hpp"

namespace Ui {
    class TailerDisplayConfigWidget;
}

class TailerDisplayConfigWidget : public DataDisplayConfigWidget
{
    Q_OBJECT

public:
    explicit TailerDisplayConfigWidget(DataDisplay* d, QWidget* parent = 0);
    ~TailerDisplayConfigWidget();

private:
    Ui::TailerDisplayConfigWidget *ui;
};

#endif // TAILERDISPLAYCONFIGWIDGET_HPP
