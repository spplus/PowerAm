#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T10:48:36
#
#-------------------------------------------------

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


