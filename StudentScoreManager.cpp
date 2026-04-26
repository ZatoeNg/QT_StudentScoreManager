#include "StudentScoreManager.h"
#include <QFile>
#include <QTextStream>

#include <QSqlDatabase>
#include <QSqlquery>
#include <QSqlerror>
#include <QSqlRecord>

StudentScoreManager::StudentScoreManager(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::StudentScoreManagerClass())
{
	ui->setupUi(this);
	ui->TableWidget->setSelectionMode(QTableWidget::SelectionMode::ContiguousSelection);
	ui->TableWidget->setSelectionBehavior(QTableWidget::SelectionBehavior::SelectRows);
	ui->TableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

	iniDatabase();
	onSearch();
}

StudentScoreManager::~StudentScoreManager()
{
	delete ui;
}


void StudentScoreManager::on_InsertBtn_clicked()
{
	qDebug() << __FUNCTION__;
}

void StudentScoreManager::on_DeleteBtn_clicked()
{
	qDebug() << __FUNCTION__;
}

void StudentScoreManager::on_UpdateBtn_clicked()
{
	qDebug() << __FUNCTION__;
}

void StudentScoreManager::on_SearchBtn_clicked()
{
	onSearch();
	qDebug() << __FUNCTION__;
}

void StudentScoreManager::on_SearchEdit_editingFinished()
{
	onSearch();
	qDebug() << __FUNCTION__;
}

void StudentScoreManager::loadTestData(const QString& filename)
{
	//打开文件
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		qWarning() << file.fileName() << "open error:" << file.errorString();
		return;
	}

	//读取文件
	QTextStream stream(&file);

	//读取表头并丢弃
	stream.readLine().split(QRegularExpression("[ \t]"));

	QString sql = "INSERT INTO stu_score(stu_id,name,dept,Chinese,Math,English) VALUES";

	//单行循环读取										
	while (!stream.atEnd())
	{
		auto datas = stream.readLine();
		QStringList info = datas.split(QRegularExpression("[ \t]"));

		sql += "(";
		sql += QString("%1,").arg(info[0]);
		sql += QString("'%1',").arg(info[1]);
		sql += QString("'%1',").arg(info[2]);
		sql += QString("%1,").arg(info[3]);
		sql += QString("%1,").arg(info[4]);
		sql += QString("%1").arg(info[5]);
		sql += "),";
	}
	sql.chop(1);

	QSqlQuery query(sql);
	if (!query.isActive())
	{
		qWarning() << "add test data error:" << query.lastError().text();
		return;
	}

	qWarning() << "loading yes";
}

void StudentScoreManager::iniDatabase()
{
	//qDebug()<<QSqlDatabase::drivers();

   //add database
	auto db = QSqlDatabase::addDatabase("QSQLITE");

	//set databaseName
	db.setDatabaseName("studentScoreManager.db");

	//open database
	if (!db.open())
	{
		qDebug() << db.databaseName() << "open error:" << db.lastError().text();
		return;
	}

	//创建学生成绩管理表
	QSqlQuery query;
	query.exec(R"(CREATE TABLE IF NOT EXISTS stu_score(
    stu_id BIGINT PRIMARY KEY NOT NULL,
    name VARCHAR(20) NOT NULL,
    dept VARCHAR(32) DEFAULT NULL,
    Chinese REAL DEFAULT NULL,
    Math    REAL DEFAULT NULL,
    English REAL DEFAULT NULL
    ))");

	if (!query.isActive())
	{
		qDebug() << "CREATE DATABASE ERRPR:" << query.lastError().text();
		return;
	}

	if (!query.exec("SELECT COUNT(*) count FROM stu_score"))
	{
		qDebug() << "exec error:" << query.lastError().text();
		return;
	}

	if (query.next())
	{
		if (query.value("count").toInt() == 0)
		{
			qWarning() << "Data loading.....";
			loadTestData("A:/HOME/Repository/Project/QT_Based_StudentScoreManager/Data/student.txt");
		}
	}
	qWarning() << "Loading Completed";



}

void StudentScoreManager::onSearch()
{
	//stu_id, name, dept, Chinese, Math, English
	QString filter;
	auto str = ui->SearchEdit->text();
	if (!str.isEmpty())filter = QString("WHERE stu_id LIKE '%%1%' OR name LIKE '%%1%' OR dept LIKE '%%1%'").arg(str);


	QString selectSql = QString("SELECT * FROM stu_score %1").arg(filter);
	QSqlQuery query(selectSql);
	if (!query.isActive())
	{
		qWarning() << "onSearch error:" << query.lastError().text();
		return;
	}

	//ui->TableWidget->clear();
	ui->TableWidget->clearContents(); // 只清空单元格内容（保留表头设置）
	// 循环删除所有行，确保表格初始无行
	while (ui->TableWidget->rowCount() > 0)
	{
		ui->TableWidget->removeRow(0);
	}

	auto count = query.record().count();
	ui->TableWidget->setColumnCount(count);
	ui->TableWidget->setHorizontalHeaderLabels({ "ID","NAME","CLASS","CHINESE","MATH","ENGLISH" });

	for (int r = 0; query.next(); r++)
	{
		if(r>=ui->TableWidget->rowCount())ui->TableWidget->insertRow(r);

		qDebug() << "ss:" << r << "-----ss:" << ui->TableWidget->rowCount();
		for (size_t i = 0; i < count; i++)
		{

			auto item = new QTableWidgetItem(query.value(i).toString());
			item->setTextAlignment(Qt::AlignCenter);

			if (i == 0)item->setCheckState(Qt::Unchecked);

			ui->TableWidget->setItem(r, i, item);

		}
	}

}
