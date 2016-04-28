#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T10:48:36
#
#-------------------------------------------------

win32 {
QMAKE_INCDIR_QT=$$(QTDIR)/include 
QMAKE_LIBDIR_QT=$$(QTDIR)/lib 
QMAKE_MOC=$$(QTDIR)/bin/moc.exe 
QMAKE_UIC=$$(QTDIR)/bin/uic.exe 
QMAKE_IDC=$$(QTDIR)/bin/idc.exe 
QMAKE_RCC=$$(QTDIR)/bin/rcc.exe
} else {
QMAKE_INCDIR_QT=$$(QTDIR)/include 
QMAKE_LIBDIR_QT=$$(QTDIR)/lib 
QMAKE_MOC=$$replace(QMAKE_MOC, $$dirname(QMAKE_MOC), $$(QTDIR)/bin)) 
QMAKE_UIC=$$replace(QMAKE_UIC, $$dirname(QMAKE_UIC), $$(QTDIR)/bin/) 
QMAKE_IDC=$$replace(QMAKE_IDC, $$dirname(QMAKE_IDC), $$(QTDIR)/bin/) 
QMAKE_RCC=$$replace(QMAKE_RCC, $$dirname(QMAKE_RCC), $$(QTDIR)/bin/)
}


QT += core gui svg widgets xml network

CONFIG += console

TARGET = PowerAm
TEMPLATE = app
RESOURCES = poweram.qrc

SOURCES += main.cpp\
        mainwindow.cpp \
    svgparser.cpp \
    svgrenderer.cpp \
	svggraph.cpp \
	svgitem.cpp \
	textitem.cpp \ 
    graphicsscene.cpp \
    graphicsview.cpp \
    netclient.cpp \
    navview.cpp \
    navdelegate.cpp \
    navmodel.cpp \
	ftputil.cpp


HEADERS  += mainwindow.h \
    structs.h \
    svgparser.h \
    svgrenderer.h \
	svggraph.h \
	svglayer.h \
	define.h \
	breaker.h \
	basedev.h \
	transformer.h \
	svgitem.h \
	textitem.h \
	textsvg.h \
    graphicsscene.h \
    graphicsview.h \
    netclient.h \
    navview.h \
    colordefines.h \
    navdelegate.h \
    navmodel.h \
	ftputil.h


