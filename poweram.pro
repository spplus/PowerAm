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

LIBS += -llibprotobufd -L$$(PB_ROOT)/lib
} else {
QMAKE_INCDIR_QT=$$(QTDIR)/include 
QMAKE_LIBDIR_QT=$$(QTDIR)/lib 
QMAKE_MOC=$$replace(QMAKE_MOC, $$dirname(QMAKE_MOC), $$(QTDIR)/bin)) 
QMAKE_UIC=$$replace(QMAKE_UIC, $$dirname(QMAKE_UIC), $$(QTDIR)/bin/) 
QMAKE_IDC=$$replace(QMAKE_IDC, $$dirname(QMAKE_IDC), $$(QTDIR)/bin/) 
QMAKE_RCC=$$replace(QMAKE_RCC, $$dirname(QMAKE_RCC), $$(QTDIR)/bin/)

LIBS += $$(PB_ROOT)/lib/protobufd
}



QT += core gui svg widgets xml network

CONFIG += console

TARGET = PowerAm
TEMPLATE = app
RESOURCES = poweram.qrc

INCLUDEPATH += $$(AM_ROOT)
INCLUDEPATH += $$(PB_ROOT)/include


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
	ftputil.cpp \
	homewindow.cpp \
	titlewidget.cpp \
	leftwidget.cpp \
	contentwidget.cpp \
	righttopwidget.cpp \
	msgrouter.cpp \
	comutil.cpp \
	waitwidget.cpp \
	openthread.cpp \
	userpasswdreset.cpp \
	stationmgr.cpp \
	userlogindlg.cpp \
	usermgrdlg.cpp \
	push_button.cpp \
	linesetwidget.cpp \
	savewidget.cpp \
	openwidget.cpp \
	rulemgrdlg.cpp \
	rulecheckwidget.cpp \
	ticketmgr.cpp \
	radiowidget.cpp \
	$$(AM_ROOT)/buff/msgbean.pb.cc \
	$$(AM_ROOT)/buff/msgbody.pb.cc


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
	ftputil.h \
	homewindow.h \
	titlewidget.h \
	leftwidget.h \
	contentwidget.h \
	righttopwidget.h \
	msgrouter.h \
	comutil.h \
	waitwidget.h \
	openthread.h \
	userpasswdreset.h \
	stationmgr.h \
	userlogindlg.h \
	usermgrdlg.h \
	push_button.h \
	linesetwidget.h \
	savewidget.h \
	openwidget.h \
	rulemgrdlg.h \
	rulecheckwidget.h \
	ticketmgr.h \
	radiowidget.h \
	$$(AM_ROOT)/buff/msgbean.pb.h \
	$$(AM_ROOT)/buff/msgbody.pb.h


