#ifndef DISPLAYCONFIGWIDGET_HPP
#define DISPLAYCONFIGWIDGET_HPP

#include <QWidget>

class DataDisplay;

//! \class DataDisplayConfigWidget
//! An abstract class whose derivatives are used to configuure the derivatives of
//! DataDisplay.
class DataDisplayConfigWidget : public QWidget
{
	Q_OBJECT

public:
	explicit DataDisplayConfigWidget(DataDisplay* t, QWidget *parent = 0);
	~DataDisplayConfigWidget() = 0;

signals:
	void wasUpdated();

public slots:
	DataDisplay* getDataDisplay();

protected slots:
	void madeUpdate();

protected:
	DataDisplay* display;
};

#endif // DISPLAYCONFIGWIDGET_HPP
