#ifndef COUNTERDISPLAYCONFIGWIDGET_HPP
#define COUNTERDISPLAYCONFIGWIDGET_HPP

#include "DataDisplayConfigWidget.hpp"

namespace Ui {
    class CounterDisplayConfigWidget;
}

class CounterDisplayConfigWidget : public DataDisplayConfigWidget
{
    Q_OBJECT

public:
    explicit CounterDisplayConfigWidget(DataDisplay* d, QWidget* parent = 0);
    ~CounterDisplayConfigWidget();

private:
    Ui::CounterDisplayConfigWidget *ui;
};

#endif // COUNTERDISPLAYCONFIGWIDGET_HPP
