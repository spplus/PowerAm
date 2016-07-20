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
#include "include/commands.h"

// ����ӵ���ɫ
#define					POWERON_GROUND_COLOR		"#FF0000"

// ������ӵ�
#define					POWEROFF_GROUND_COLOR	"#835858"

// ��������ɫ
#define					POWEROFF_COLOR			"#AAAAAA"

// ��ʾ��ɫ
#define					TIPSHOW_COLOR			"#e634e0"

enum eRoleType
{
	eManager = 1,			//����Ա
	eDispatcher = 2,		//����Ա
	eAutomater = 3,			//�Զ���
	eMaintainers = 4		
};

enum eResult
{
	eSUCCESS=0,eERROR
};

enum eTransType
{
	eTWO,				// ���� ��
	eTHREE				// ���� ��
};

// ϵͳ״̬
enum		eSysState
{
	eREALTIME,
	eANALOG,
	eTICKET
};

// ����״̬
enum	eTagState{eTagOff,eTagOn};
// ����״̬
enum	eBreakerState{eOFF=0,eON};

// �㶨��
const	QString		HEAD_LAYER				= "Head_Layer";
const	QString		BUS_LAYER				= "Bus_Layer";
const	QString		BUS_LAYER_JC			= "Busbarsection_Layer";	// ���ɵ���ĸ�߲�
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
const	QString		MEASURE2_LAYER			= "MeasureValue_Layer";			// kedongͼ�ε������
const	QString		MEASURE3_LAYER			= "Measurement_Layer";				// ���ɵ���
const	QString		PT_LAYER				= "PT_Layer";
const	QString		GZP_LAYER				= "GZP_Layer";
const	QString		GENERATER_LAYER			= "Generator_Layer";
const	QString		CONNECTNODE_LAYER = "ConnectNode_Layer";


// ���Զ���

const	QString		ATTR_OBJECT_ID			= "ObjectID";
const	QString		ATTR_OBJECT_NAME		= "ObjectName";
const	QString		ATTR_XLINK				= "xlink:href";
const	QString		ATTR_CLASS				= "class";
const	QString		ATTR_STYLE					= "style";
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

// �����ļ�����
const	QString		CONFIG		=		"client.conf";
const	QString		CLIENT			=		"client.xml";

// ��ɫ���������ļ�����
const	QString		COLOR		=		"color.xml";

// ��ʽ����
const	QString		STYLE		=		"style.qss";


// SVG�ļ������Ŀ¼
const	QString		SVG_ROOT				= QString::fromLocal8Bit("F:\\Work\\SVG\\nanrui\\����SVG��CIM�ļ�\\svg_output\\");

// ����SVG�ļ�����Ŀ¼
const	QString		SVG_PATH				= "F:\\QtProject\\SVG\\";

#define		PBNS	com::spplus::buff

// ��ҳվ��ÿ����ʾ����
#define		COL		5


// ��Դ�ļ�����
#define			ICON_NETWORK_ON		":images/network_on.png"
#define			ICON_NETWORK_OFF	":images/network_off.png"
#define			ICON_CHECK_ON		":images/check_on.png"
#define			ICON_CHECK_OFF		":images/check_off.png"
#define			ICON_OPEN			":images/open.png"
#define			ICON_HOME			":images/home.png"
#define			ICON_ZOOM_OUT		":images/zoom_out.png"
#define			ICON_ZOOM_IN		":images/zoom_in.png"
#define			ICON_NEXT			":images/next.png"
#define			ICON_PREV			":images/prev.png"
#define			ICON_REFRESH		":images/refresh.png"
#define			ICON_SAVE			":images/save.png"
#define			ICON_READ			":images/read.png"
#define			ICON_CONTROL_ON		":images/control_on.png"
#define			ICON_CONTROL_OFF	":images/control_off.png"
#define			ICON_ALARM_ON		":images/alarm_on.png"
#define			ICON_ALARM_OFF		":images/alarm_off.png"
#define			ICON_ROLE			":images/role.png"
#define			ICON_CIRLCE			":images/circle.png"
#define			ICON_TAG_ON			":images/tag_on.png"
#define			ICON_TAG_QUERY			":images/tag_query.png"
#define			ICON_TAG_OFF		":images/tag_off.png"
#define			ICON_OPRATION		":images/opration.png"
#define			ICON_EVENT			":images/envent.png"
#define			ICON_RUN			":images/run.png"
#define			ICON_USERMGR		":images/usermgr.png"
#define			ICON_SWITCH_ON		":images/on.png"
#define			ICON_SWITCH_OFF		":images/off.png"
#define			ICON_POINTER		":images/pointer.png"
#define			ICON_ZOOM_ORG		":images/zoom_original.png"
#define			ICON_TOPO			":images/topo.png"
#define			ICON_BAY			":images/bay.png"
#define			ICON_LOG			":images/log.png"
#define			ICON_GROUND			":images/ground.png"
#define			ICON_SETQUERY		":images/msetquery.png"
#define			ICON_CUTTING		":images/cutquery.png"
#define			ICON_HAND			":images/hand.png"
#define			ICON_POWERSET	":images/powerset.png"
#define			ICON_LINESET			":images/lineset.png"
#define			ICON_ANALOG		":images/analog.png"
#define			ICON_REALTIME		":images/realtime.png"
#define			ICON_TICKET			":images/ticket.png"

// ������ʾ��Ϣ
#define			MSG_TITLE				"ϵͳ��ʾ"
#define			MSG_TIP_NETWORK_ON		"�ɹ����ӷ�����"
#define			MSG_TIP_NETWORK_OFF		"���������ӶϿ�"
#define			MSG_TIP_CONTROL_ON		"�������"
#define			MSG_TIP_CONTROL_OFF		"��ֹ����"
#define			MSG_TIP_ALARM_ON		"���������澯"
#define			MSG_TIP_ALARM_OFF		"��ֹ�����澯"
#define			MSG_TIP_CHECK_ON		"����У��"
#define			MSG_TIP_CHECK_OFF		"��ֹУ��"
#define			MSG_TIP_HOME			"��ҳ"
#define			MSG_TIP_OPEN			"��"
#define			MSG_TIP_NEXT			"��һ��"
#define			MSG_TIP_PREV			"ǰһ��"
#define			MSG_TIP_ZOOMOUT			"�Ŵ�"
#define			MSG_TIP_ZOOMIN			"��С"
#define			MSG_TIP_REFRESH			"ˢ��"
#define			MSG_TIP_SAVE			"����浵"
#define			MSG_TIP_READ			"�򿪴浵"
#define			MSG_TIP_RULE			"��������"
#define			MSG_TIP_CIRCLE			"��·��ѯ"
#define			MSG_TIP_TAG				"���Ʋ�ѯ"
#define			MSG_TIP_GROUND			"�ӵز�ѯ"
#define			MSG_TIP_OPRATION		"������ѯ"
#define			MSG_TIP_EVENT			"�¼���ѯ"
#define			MSG_TIP_RUN_REAL		"ʵʱ̬"
#define			MSG_TIP_RUN_ANALOG		"ģ��̬"
#define			MSG_TIP_RUN_BILL		"��Ʊ̬"
#define			MSG_TIP_SETTING			"�˹����ò�ѯ"
#define			MSG_TIP_SAVING			"�浵��ѯ"
#define			MSG_TIP_SWITCH_ON		"�ú�"
#define			MSG_TIP_SWITCH_OFF		"�÷�"
#define			MSG_TIP_TAG_ON			"����"
#define			MSG_TIP_TAG_OFF			"ժ��"
#define			MSG_TIP_POINTER			"ָ��"
#define			MSG_TIP_ZOOM_ORG		"ԭʼ�ߴ�"
#define			MSG_TIP_TOPO			"���˲�ѯ"
#define			MSG_TIP_BAY				"�����ѯ"
#define			MSG_TIP_LOG				"��־��ѯ"
#define			MSG_TIP_POWERSET	"���õ�Դ��"
#define			MSG_TIP_LINESET		"���ý�����"

#endif