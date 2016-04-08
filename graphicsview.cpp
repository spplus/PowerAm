#include "graphicsview.h"

GraphicsView::GraphicsView(GraphicsScene* scene)

{
	m_scale = 1.0;
	m_interal = 0.1;
	setScene(scene);	
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::zoomOut()
{

	m_scale += m_interal;
	QMatrix oldMatrix = this->matrix();
	this->resetMatrix();
	this->translate(oldMatrix.dx(), oldMatrix.dy());
	this->scale(m_scale, m_scale);
}

void GraphicsView::zoomIn()
{
	m_scale -= m_interal;

	QMatrix oldMatrix = this->matrix();
	this->resetMatrix();
	this->translate(oldMatrix.dx(), oldMatrix.dy());
	this->scale(m_scale, m_scale);
}

void GraphicsView::zoomHome()
{
	m_scale = 1.0;

	QMatrix oldMatrix = this->matrix();
	this->resetMatrix();
	this->translate(oldMatrix.dx(), oldMatrix.dy());
	this->scale(m_scale, m_scale);

}