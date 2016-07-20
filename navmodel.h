#ifndef NAVMODEL_H
#define NAVMODEL_H

#include <QAbstractListModel>
#include <vector>
#include <QVector>
#include "structs.h"
using namespace std;
class NavModel : public QAbstractListModel
{
	Q_OBJECT


public:
	NavModel(QObject *parent);
	~NavModel();

	/*!
	\brief
		Read data from xml.
	*/
	void ReadDataFromConfig(QString path);

	void Refresh();

	void		setData(QVector<TreeNode*> datalist);

signals:
	void		openFile(QString fname,QString stationId,bool svgroot);

public:
	int rowCount(const QModelIndex &parent /* = QModelIndex */) const;
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;

public slots:
	void Collapse(const QModelIndex& index);
	
private:
	void RefreshList();

private:
	QVector<TreeNode*> m_nodeList;
	QVector<ListNode> m_list;
};

#endif // NAVMODEL_H
