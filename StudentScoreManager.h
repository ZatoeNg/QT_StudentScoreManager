#pragma once

#include <QtWidgets/QWidget>
#include "ui_StudentScoreManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StudentScoreManagerClass; };
QT_END_NAMESPACE

class StudentScoreManager : public QWidget
{
	Q_OBJECT
private:
	Ui::StudentScoreManagerClass* ui;
public:
	StudentScoreManager(QWidget* parent = nullptr);
	~StudentScoreManager();

public slots:
	void on_SearchBtn_clicked();
	void on_SearchEdit_editingFinished();
	void on_InsertBtn_clicked();
	void on_DeleteBtn_clicked();
	void on_UpdateBtn_clicked();


public:
	void loadTestData(const QString& filename);
	void iniDatabase();
	void onSearch();
};

