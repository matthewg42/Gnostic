#ifndef PENSTYLEWIDGET_HPP
#define PENSTYLEWIDGET_HPP

#include <QWidget>
#include <QPen>
#include <QIcon>

class QGraphicsScene;
class QGraphicsPathItem;

namespace Ui {
	class PenStyleWidget;
}

class PenStyleWidget : public QWidget
{
	Q_OBJECT

public:
	explicit PenStyleWidget(QWidget *parent = 0);
	~PenStyleWidget();

	void setInitialPen(QPen p);

signals:
	void setPen(QPen p);

private slots:
	void chooseColor();
	void setWidth(int i);
	void setPenStyle(QString s);
	void saveStyle();
	void revertStyle();
        void updatePreview();

private:
	Ui::PenStyleWidget *ui;
	QPen workingPen;
	QPen oldPen;
        QIcon colorIcon;
};

#endif // PENSTYLEWIDGET_HPP
