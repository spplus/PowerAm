/************************************************************************/
/* 
		DESC:		定义常量.
		DATE:		2016-03-24
		AUTHOR: YUANLS
*/
/************************************************************************/

#ifndef __DEFINE_H__
#define __DEFINE_H__
#include <QString>
#include "include/commands.h"

// 带电接地颜色
#define					POWERON_GROUND_COLOR		"#FF0000"

// 不带电接地
#define					POWEROFF_GROUND_COLOR	"#835858"

// 不带电颜色
#define					POWEROFF_COLOR			"#AAAAAA"

// 提示颜色
#define					TIPSHOW_COLOR			"#e634e0"

enum eRoleType
{
	eManager = 1,			//管理员
	eDispatcher = 2,		//调度员
	eAutomater = 3,			//自动化
	eMaintainers = 4		
};

enum eResult
{
	eSUCCESS=0,eERROR
};

enum eTransType
{
	eTWO,				// 两卷 变
	eTHREE				// 三卷 变
};

// 系统状态
enum		eSysState
{
	eREALTIME,
	eANALOG,
	eTICKET
};

// 挂牌状态
enum	eTagState{eTagOff,eTagOn};
// 开关状态
enum	eBreakerState{eOFF=0,eON};

// 层定义
const	QString		HEAD_LAYER				= "Head_Layer";
const	QString		BUS_LAYER				= "Bus_Layer";
const	QString		BUS_LAYER_JC			= "Busbarsection_Layer";	// 集成电子母线层
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
const	QString		MEASURE2_LAYER			= "MeasureValue_Layer";			// kedong图形的量测层
const	QString		MEASURE3_LAYER			= "Measurement_Layer";				// 积成电子
const	QString		PT_LAYER				= "PT_Layer";
const	QString		GZP_LAYER				= "GZP_Layer";
const	QString		GENERATER_LAYER			= "Generator_Layer";
const	QString		CONNECTNODE_LAYER = "ConnectNode_Layer";


// 属性定义

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


// 标签定义
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

// 配置文件名称
const	QString		CONFIG		=		"client.conf";
const	QString		CLIENT			=		"client.xml";

// 颜色规则配置文件名称
const	QString		COLOR		=		"color.xml";

// 样式定义
const	QString		STYLE		=		"style.qss";


// SVG文件保存的目录
const	QString		SVG_ROOT				= QString::fromLocal8Bit("F:\\Work\\SVG\\nanrui\\吉安SVG和CIM文件\\svg_output\\");

// 本地SVG文件保存目录
const	QString		SVG_PATH				= "F:\\QtProject\\SVG\\";

#define		PBNS	com::spplus::buff

// 首页站点每行显示列数
#define		COL		5


// 资源文件定义
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

// 定义提示信息
#define			MSG_TITLE				"系统提示"
#define			MSG_TIP_NETWORK_ON		"成功连接服务器"
#define			MSG_TIP_NETWORK_OFF		"服务器连接断开"
#define			MSG_TIP_CONTROL_ON		"允许控制"
#define			MSG_TIP_CONTROL_OFF		"禁止控制"
#define			MSG_TIP_ALARM_ON		"允许声音告警"
#define			MSG_TIP_ALARM_OFF		"禁止声音告警"
#define			MSG_TIP_CHECK_ON		"允许校验"
#define			MSG_TIP_CHECK_OFF		"禁止校验"
#define			MSG_TIP_HOME			"首页"
#define			MSG_TIP_OPEN			"打开"
#define			MSG_TIP_NEXT			"下一张"
#define			MSG_TIP_PREV			"前一张"
#define			MSG_TIP_ZOOMOUT			"放大"
#define			MSG_TIP_ZOOMIN			"缩小"
#define			MSG_TIP_REFRESH			"刷新"
#define			MSG_TIP_SAVE			"保存存档"
#define			MSG_TIP_READ			"打开存档"
#define			MSG_TIP_RULE			"规则设置"
#define			MSG_TIP_CIRCLE			"环路查询"
#define			MSG_TIP_TAG				"挂牌查询"
#define			MSG_TIP_GROUND			"接地查询"
#define			MSG_TIP_OPRATION		"操作查询"
#define			MSG_TIP_EVENT			"事件查询"
#define			MSG_TIP_RUN_REAL		"实时态"
#define			MSG_TIP_RUN_ANALOG		"模拟态"
#define			MSG_TIP_RUN_BILL		"拟票态"
#define			MSG_TIP_SETTING			"人工设置查询"
#define			MSG_TIP_SAVING			"存档查询"
#define			MSG_TIP_SWITCH_ON		"置合"
#define			MSG_TIP_SWITCH_OFF		"置分"
#define			MSG_TIP_TAG_ON			"挂牌"
#define			MSG_TIP_TAG_OFF			"摘牌"
#define			MSG_TIP_POINTER			"指针"
#define			MSG_TIP_ZOOM_ORG		"原始尺寸"
#define			MSG_TIP_TOPO			"拓扑查询"
#define			MSG_TIP_BAY				"间隔查询"
#define			MSG_TIP_LOG				"日志查询"
#define			MSG_TIP_POWERSET	"设置电源点"
#define			MSG_TIP_LINESET		"设置进出线"

#endif