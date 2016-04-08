/************************************************************************/
/* 
		DESC:		���峣��.
		DATE:		2016-03-24
		AUTHOR: YUANLS
*/
/************************************************************************/

#ifndef __DEFINE_H__
#define __DEFINE_H__
#include <QString>

enum eDevType{eBREAKER,eLINE,eTRANS};

// �豸����
enum eDeviceType
{
	eSWITCH,			// ���ش��࣬�������ɿ��أ���բ���ӵص�բ
	eTRANSFORMER,		// ��ѹ��
	eWINDING,			// ��ѹ������
	eBUS,				// ĸ��
	eLINK,				// ������
	eACLINE,			// ������·
	eLOAD,				// ����
	eARRESTER,			// ������
	eTEXT,				// ����
	eDEFAULT			// ȱʡ����
};

enum eTransType
{
	eTWO,				// ���� ��
	eTHREE				// ���� ��
};
// ����״̬
enum	eBreakerState{eOFF=0,eON};

// �㶨��
const	QString		HEAD_LAYER				= "Head_Layer";
const	QString		BUS_LAYER				= "Bus_Layer";
const	QString		BREAKER_LAYER			= "Breaker_Layer";
const	QString		DISCONN_LAYER			= "Disconnector_Layer";
const	QString		GROUNDDISCONN_LAYER		= "GroundDisconnector_Layer";
const	QString		TRANS2_LAYER			= "Transformer2_Layer";
const	QString		TRANS3_LAYER			= "Transformer3_Layer";
const	QString		LINK_LAYER				= "Link_Layer";
const	QString		TEXT_LAYER				= "Text_Layer";
const	QString		COMPANSTOR_LAYER		= "Compensator_Layer";
const	QString		ACLINE_LAYER			= "ACLineSegment_Layer";
const	QString		LOAD_LAYER				= "Load_Layer";
const	QString		TERM_LAYER				= "Term_Layer";
const	QString		ARRESTER_LAYER			= "Arrester_Layer";
const	QString		MEASURE_LAYER			= "MeasurementValue_Layer";
const	QString		PT_LAYER				= "PT_Layer";
const	QString		GZP_LAYER				= "GZP_Layer";
const	QString		GENERATER_LAYER			= "Generator_Layer";


// ���Զ���

const	QString		ATTR_OBJECT_ID			= "ObjectID";
const	QString		ATTR_OBJECT_NAME		= "ObjectName";
const	QString		ATTR_XLINK				= "xlink:href";
const	QString		ATTR_CLASS				= "class";
const	QString		ATTR_ID					= "id";
const	QString		ATTR_X					= "x";
const	QString		ATTR_Y					= "y";
const	QString		ATTR_WIDTH				= "width";
const	QString		ATTR_HEIGHT				= "height";
const	QString		ATTR_FILL				= "fill";
const	QString		ATTR_D					= "d";
const	QString		ATTR_POINTS				= "points";
const	QString		ATTR_TRANSFORM			= "transform";			
const	QString		ATTR_FONTSIZE				= "font-size";
const	QString		ATTR_FONTW					= "font-width";
const	QString		ATTR_FONTH					= "font-height";
const	QString		ATTR_FONTFAMILY		= "font-family";
const	QString		ATTR_STROKE					= "stroke";


// ��ǩ����
const	QString		TAG_METADATA			= "metadata";
const	QString		TAG_USE					= "use";
const	QString		TAG_SVG					= "svg";
const	QString		TAG_G					= "g";
const	QString		TAG_DEFS				= "defs";
const	QString		TAG_PSR					= "cge:PSR_Ref";
const	QString		TAG_MEAS				= "cge:Meas_Ref";
const	QString		TAG_PATH				= "path";
const	QString		TAG_POLYLINE			= "polyline";
const	QString		TAG_STYLE				= "style";
const	QString		TAG_TEXT				= "text";


#endif