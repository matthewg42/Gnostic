#ifndef LOCALTRANSPORTCONFIGWIDGET_HPP
#define LOCALTRANSPORTCONFIGWIDGET_HPP

#include <QWidget>

namespace Ui
{
	class LocalTransportConfigWidget;
}

class LocalTransportConfigWidget : public QWidget
{
	Q_OBJECT

public:
	explicit LocalTransportConfigWidget(QWidget *parent = 0);
	~LocalTransportConfigWidget();

private:
	Ui::LocalTransportConfigWidget *ui;
};

#endif // LOCALTRANSPORTCONFIGWIDGET_HPP
