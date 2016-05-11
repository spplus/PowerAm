#ifndef NAVMODEL_H
#define NAVMODEL_H

#include <QAbstractListModel>
#include <vector>
#include "structs.h"

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

	void	setData(std::vector<TreeNode*>& datalist);

signals:
	void	openFile(QString fname);

public:
	int rowCount(const QModelIndex &parent /* = QModelIndex */) const;
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;

public slots:
	void Collapse(const QModelIndex& index);
	
private:
	void RefreshList();

private:
	std::vector<TreeNode*> m_nodeList;
	std::vector<ListNode> m_list;
};

#endif // NAVMODEL_H
