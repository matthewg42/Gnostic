#ifndef TAILERDISPLAYCONFIGWIDGET_HPP
#define TAILERDISPLAYCONFIGWIDGET_HPP

#include "DataDisplayConfigWidget.hpp"

#include <QIcon>

class TailerDisplay;
class QTableWidgetItem;
class QWidget;

namespace Ui {
	class TailerDisplayConfigWidget;
}

class TailerDisplayConfigWidget : public DataDisplayConfigWidget
{
	Q_OBJECT

public:
	explicit TailerDisplayConfigWidget(DataDisplay* d, QWidget* parent = 0);
	~TailerDisplayConfigWidget();

private slots:
	void selectNewColor();
	void patternSelectionChanged();
	void patternSelected(const QString& pattern);
	void addPattern();
	void removePattern();
	void patternChanged(int r, int c);
	void updatePattern(const QString& oldPattern, const QString& newPattern);

private:
	void populateTable();

	Ui::TailerDisplayConfigWidget *ui;
	TailerDisplay* myTailer;
	QIcon colorIcon;
	QString currentPattern;
	bool populating;

};

#endif // TAILERDISPLAYCONFIGWIDGET_HPP
