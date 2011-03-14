#ifndef PATHEDITORFORM_HPP
#define PATHEDITORFORM_HPP

#include <QWidget>

namespace Ui {
	class PathEditorForm;
}

class PathEditorForm : public QWidget
{
	Q_OBJECT

public:
	explicit PathEditorForm(QWidget *parent = 0);
	~PathEditorForm();

private slots:
	//! Set the path for a program.
	//! \param path the path to use
	//! \param program the name of the program whose path is to be set.  If this is QString(),
	//!        then the name of the program is taken from sender().objectName()
	//! \returns true of the sender had a known id, else false.
	bool setPath(const QString& path, const QString& program=QString());
	bool browseForPath(const QString& program=QString());
	void loadPathsFromConfig();
	void savePaths();
	void madeUpdate();

private:
	const QString getPath(const QString& program);
	Ui::PathEditorForm *ui;
};

#endif // PATHEDITORFORM_HPP
