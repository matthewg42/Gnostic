#ifndef COUNTERDISPLAYCONFIGWIDGET_HPP
#define COUNTERDISPLAYCONFIGWIDGET_HPP

#include "DataDisplayConfigWidget.hpp"

namespace Ui {
    class LineCounterDisplayConfigWidget;
}

class LineCounterDisplayConfigWidget : public DataDisplayConfigWidget
{
    Q_OBJECT

public:
    explicit LineCounterDisplayConfigWidget(DataDisplay* d, QWidget* parent = 0);
    ~LineCounterDisplayConfigWidget();

private:
    Ui::LineCounterDisplayConfigWidget *ui;
};

#endif // COUNTERDISPLAYCONFIGWIDGET_HPP
