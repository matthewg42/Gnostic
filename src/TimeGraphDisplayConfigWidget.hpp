#ifndef TAILERDISPLAYCONFIGWIDGET_HPP
#define TAILERDISPLAYCONFIGWIDGET_HPP

#include "DataDisplayConfigWidget.hpp"

namespace Ui {
    class TimeGraphDisplayConfigWidget;
}

class TimeGraphDisplayConfigWidget : public DataDisplayConfigWidget
{
    Q_OBJECT

public:
    explicit TimeGraphDisplayConfigWidget(DataDisplay* d, QWidget* parent = 0);
    ~TimeGraphDisplayConfigWidget();

private:
    Ui::TimeGraphDisplayConfigWidget *ui;
};

#endif // TAILERDISPLAYCONFIGWIDGET_HPP
