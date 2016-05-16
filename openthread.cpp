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
	m_pscene->openSvgFile(m_fname);
}