#include "openthread.h"

OpenThread::OpenThread(GraphicsScene* scene)
{
	m_pscene = scene;
}

void OpenThread::open(QString fname)
{
	m_fname = fname;
	start();
}

void  OpenThread::run()
{
	SvgGraph* pgraph = m_pscene->parserSvg(m_fname);
	emit drawSvgGraph(pgraph);
}