#pragma once
#include <list>
#include <vector>
#include "json\json.h"
#include "xml\tinyxml2.h"
#include <afxinet.h>
//#include <string>
//using namespace std;

// struct define degin
// 字符编码格式均为utf8
typedef struct tagServiceInfo {
	std::string ip;
	unsigned int port;
} ServiceInfo;

typedef struct tagSchoolBaseInfo {
	std::string school_id;
	std::string school_name;
	std::string school_code;
	int school_level;
	int school_type;
	int school_state;
	std::string create_time;  // "2017/4/19 14:27:35"
} SchoolBaseInfo;

typedef struct tagCameraInfo {
	std::string cam_id;
	std::string camName;
	int cam_type;  // 1-大华；2-海康；3-采集盒
				   // 如果 CamType 为 1或2:
				   // 格式为“IP|端口|访问账号|访问密码|通道号”；
				   // 如果 CamType 为3:
				   // 格式为“监控对象（1：学生 2：教室 3：屏幕）|教室号|录播服务器IP地址”
	std::string set_info;
	double resolution_width;
	double resolution_height;
	std::string error_flag;

	tagCameraInfo& operator=(const tagCameraInfo& other) {
		cam_id = other.cam_id;
		camName = other.camName;
		cam_type = other.cam_type;
		set_info = other.set_info;
		resolution_width = other.resolution_width;
		resolution_height = other.resolution_height;
		error_flag = other.error_flag;
		return *this;
	}
} CameraInfo;

typedef struct tagCamera{
	
	// 格式为“IP|端口|访问账号|访问密码|通道号”；
	std::string camera_ip;
	std::string camera_port;
	std::string camera_username;
	std::string camera_userpwd;
	std::string camera_ch;

	tagCamera& operator=(const tagCamera& other) {
		camera_ip = other.camera_ip;
		camera_port	= other.camera_port;
		camera_username = other.camera_username;
		camera_userpwd = other.camera_userpwd;
		camera_ch = other.camera_ch;
		return *this;
	}
} Camera;

typedef struct tagNoElcRoomCamInfo
{
    std::string cam_id;
    int cam_type;
    std::string camName;
    std::string camIP;
    std::string camPort;
    std::string camAccount;
    std::string camPwd;
    tagNoElcRoomCamInfo& operator=(const tagNoElcRoomCamInfo& other)
    {
        cam_id = other.cam_id;
        cam_type = other.cam_type;
        camName = other.camName;
        camIP = other.camIP;
        camPort = other.camPort;
        camAccount = other.camAccount;
        camPwd = other.camPwd;
        return *this;
    }
}NoElcRoomCamInfo;

typedef struct TagSecurityCamServers
{
    int id;
    int type;
    std::string ip;
    std::string port;
    TagSecurityCamServers& operator=(const TagSecurityCamServers& other)
    {
        id = other.id;
        type = other.type;
        ip = other.ip;
        port = other.port;
        return *this;
    }
}SecurityCamServers;
typedef struct TagSecurityCamInfo
{
    int cam_id;
    int cam_brand;
    std::string camName;
    std::string camIP;
    std::string camPort;
    std::string camAccount;
    std::string camPwd;
    std::string camPostion;
    std::list<SecurityCamServers> cam_servers;
    TagSecurityCamInfo& operator=(const TagSecurityCamInfo& other)
    {
        cam_id = other.cam_id;
        cam_brand = other.cam_brand;
        camName = other.camName;
        camIP = other.camIP;
        camPort = other.camPort;
        camAccount = other.camAccount;
        camPwd = other.camPwd;
        camPostion = other.camPostion;
        cam_servers = other.cam_servers;
        return *this;
    }
}SecurityCamInfo;

typedef struct tagIP {

	// 格式为“IP|端口|访问账号|访问密码|通道号”；
	std::string strRtmp_ip;
	std::string str480_ip;
	std::string str1080_ip;
	std::string strCmb_ip;

	tagIP& operator=(const tagIP& other) {
		strRtmp_ip = other.strRtmp_ip;
		str480_ip = other.str480_ip;
		str1080_ip = other.str1080_ip;
		strCmb_ip = other.strCmb_ip;
		return *this;
	}
} IP;

typedef struct tagDeviceInfo {
	std::string amplifier_model_id;  // 功放设备型号
	std::string dvd_model_id;        // DVD 设备型号
	std::string projector_model_id;  // 投影仪设备型号
	std::string lamplight_model_id;  // 灯光设备型号
	std::string curtain_model_id;    // 窗帘设备型号

	tagDeviceInfo& operator=(const tagDeviceInfo& other) {
		amplifier_model_id = other.amplifier_model_id;
		dvd_model_id = other.dvd_model_id;
		projector_model_id = other.projector_model_id;
		lamplight_model_id = other.lamplight_model_id;
		curtain_model_id = other.curtain_model_id;
		return *this;
	}
} DeviceInfo;

typedef struct tagBigScreenmodel {
	int id;
	double position_x;
	double position_y;
	double width;
	double height;
	int classroom_id;
	std::string update_time;  // "2017-04-26T14:24:00"
	double resolution_width;
	double resolution_height;

	tagBigScreenmodel& operator=(const tagBigScreenmodel& other) {
		id = other.id;
		position_x = other.position_x;
		position_y = other.position_y;
		width = other.width;
		height = other.height;
		classroom_id = other.classroom_id;
		update_time = other.update_time;
		resolution_width = other.resolution_width;
		resolution_height = other.resolution_height;
		return *this;
	}
} BigScreenmodel;

typedef struct tagElectricClassroomInfo {
	int roomId;
	std::string roomName;
	std::string roomPwd;
	std::string buildingName;
	std::string rPersonName;
	std::string rPersonPhone;
	std::string roomStatus;  //教室状态 0为正常，1为故障
	std::string hostMac;
	std::string electricType; //1为常态录播，2为精品录播
	std::string roomVersion;  //中控教室版本
	std::string centralIp;
	int centralPort;
	std::string terminalIp;
	int terminalPort;
	std::string recordServerIp; //如果为精品录播教室，录播服务器地址|精品录播服务器地址
	std::list<CameraInfo> camera_info_list;
	std::string deviceEquip; //设备配标识，格式为“灯光排数|窗帘组数|投影仪品牌 ”，例如  3|2| 默认品牌

	tagElectricClassroomInfo& operator=(const tagElectricClassroomInfo& other) {
		roomId = other.roomId;
		roomName = other.roomName;
		roomPwd = other.roomPwd;
		buildingName = other.buildingName;
		rPersonName = other.rPersonName;
		rPersonPhone = other.rPersonPhone;
		roomStatus = other.roomStatus;
		hostMac = other.hostMac;
		electricType = other.electricType;
		roomVersion = other.roomVersion;
		centralIp = other.centralIp;
		centralPort = other.centralPort;
		terminalIp = other.terminalIp;
		terminalPort = other.terminalPort;
		recordServerIp = other.recordServerIp;
		camera_info_list = other.camera_info_list;
		deviceEquip = other.deviceEquip;
		return *this;
	}
}ElectricClassroomInfo;

typedef struct tagNoElectricClassroomInfo
{
    int roomId;
    int roomType;
    std::string roomName;
    std::string serverIP;
    std::list<NoElcRoomCamInfo> noElcRoomCam_list;
    tagNoElectricClassroomInfo& operator=(const tagNoElectricClassroomInfo& other) 
    {
        roomId = other.roomId;
        roomType = other.roomType;
        roomName = other.roomName;
        serverIP = other.serverIP;
        noElcRoomCam_list = other.noElcRoomCam_list;
        return *this;
    }
}NoElectricClassroomInfo;

typedef struct TagCampusSecurityInfo
{
    int id;
    std::string name;
    std::list<SecurityCamInfo> securCam_list;
    TagCampusSecurityInfo& operator=(const TagCampusSecurityInfo& other)
    {
        id = other.id;
        name = other.name;
        securCam_list = other.securCam_list;
        return *this;
    }
}CampusSecurityInfo;


//typedef struct tagElectricClassroomInfo {
//	int classroom_id;
//	std::string classroom_name;
//	std::string host_mac_addr;
//	std::string central_ip;
//	int central_port;
//	std::string terminal_iP;
//	int terminal_port;
//	std::string room_version;
//	std::string update_time;  // "2017-04-26T14:24:00"
//							  // 格式为“功放配备标识|DVD配备标识|投影仪配备标识|灯光配备标识|窗帘配备标识”
//							  // 1 表示有配备，0 表示无配备，如：1|0|1|0|0。
//	std::string device_equip_flag;
//	// 格式为“功放故障标识|DVD故障标识|投影仪故障标识|灯光故障标识|窗帘故障标识”//
//	// 如：1|0|1|0|0。
//	std::string device_error_flag;
//	std::string record_server_ip; // 录播服务器ip, port写死9911
//	DeviceInfo device_info;
//	BigScreenmodel big_screenmodel_info;
//	std::list<CameraInfo> camera_info_list;
//
//	tagElectricClassroomInfo& operator=(const tagElectricClassroomInfo& other) {
//		classroom_id = other.classroom_id;
//		classroom_name = other.classroom_name;
//		host_mac_addr = other.host_mac_addr;
//		central_ip = other.central_ip;
//		central_port = other.central_port;
//		terminal_iP = other.terminal_iP;
//		terminal_port = other.terminal_port;
//		room_version = other.room_version;
//		update_time = other.update_time;
//		device_equip_flag = other.device_equip_flag;
//		device_error_flag = other.device_error_flag;
//		device_info = other.device_info;
//		big_screenmodel_info = other.big_screenmodel_info;
//		camera_info_list = other.camera_info_list;
//		return *this;
//	}
//} ElectricClassroomInfo;

typedef struct tagCamInfo {
	std::string camId;
	int cam_type;
	std::string cam_ip;
	int cam_port;
	std::string access_account;
	std::string access_pwd;
	int resolution_width;
	int resolution_height;
	int error_flag;

	tagCamInfo& operator=(const tagCamInfo& other) {
		camId = other.camId;
		cam_type = other.cam_type;
		cam_ip = other.cam_ip;
		cam_port = other.cam_port;
		access_account = other.access_account;
		access_pwd = other.access_pwd;
		resolution_width = other.resolution_width;
		resolution_height = other.resolution_height;
		error_flag = other.error_flag;
		return *this;
	}
} CamInfo;

typedef struct tagNetWorkRoomInfo {
	int room_id;
	std::string room_name;
	std::string teacher_ip;
	std::string mac_addr;
	std::list<CamInfo> cam_info_list;

	tagNetWorkRoomInfo& operator=(const tagNetWorkRoomInfo& other) {
		room_id = other.room_id;
		room_name = other.room_name;
		teacher_ip = other.teacher_ip;
		mac_addr = other.mac_addr;
		cam_info_list = other.cam_info_list;
		return *this;
	}
} NetWorkRoomInfo;

class CBaseService
{
public:
	CBaseService();
	virtual ~CBaseService(void);

public:
	int ParseResponse(char* buf, Json::Value& json_data);
	void ConvertElement(tinyxml2::XMLElement* node, Json::Value& json_data, bool root_is_array);
	
	/********************************************
	** 函数名称：Exp_GetSubSystemServerInfo
	** 函数作用：获取子服务器的ip&port(ip:port)
	**
	** 入参：
	**  tcSysID：			子服务器ID
	** 出参：
	**  tcIPandPort：		ip&port(ip:port)
	**
	** 返回值：
	**					   0 执行成功
	**					  -1 执行失败
	**
	********************************************/
	long GetSubSystemServerInfo(IN char* tcSysID, OUT char* tcIPandPort);
	// 获取服务器信息
	// @param ip 基础平台服务器IP
	// @param port 基础平台服务器端口
	// @param code 服务器代码
	//              "D00" 校园广播服务器
	//              "890" 大数据服务器
	//              "E00" 学校教室信息服务器
	// @param service_ip 获取的服务器IP
	// @param service_port 获取的服务器端口
	// @return 0:Succeeded 1:Failed
	int GetServiceInfo(IN char* cIP, IN char* port, IN char* code, OUT char* service_ip, OUT char* service_port);
	// 获取当前学校基本信息
	// @param ip 基础平台服务器IP
	// @param port 基础平台服务器端口
	// @param school_info 学校基本信息
	// @return 0:Succeeded 1:Failed
	//int GetCurrentSchoolInfo(IN char* ip, IN char* port,	OUT SchoolBaseInfo& school_info);

	// 获取当前学校多媒体电教室列表
	// @param ip 基础平台服务器IP
	// @param port 基础平台服务器端口
	// @param classroom_list 多媒体电教室列表
	// @return 0:Succeeded 1:Failed
	int GetAllElectricClassroomInfo(IN char* ip, IN char* port, OUT std::list<ElectricClassroomInfo>& classroom_list);

	// 通过教室id获取单个 多媒体电教室信息
	// @param classroom_list 多媒体电教室列表
	// @param classroom_id 教室id
	// @param classroom_info 得到的班级信息
	// @return 0:Succeeded 1:Failed
	int GetElectricClassroomInfoByClassID(IN std::list<ElectricClassroomInfo>& classroom_list, IN int classroom_id, OUT ElectricClassroomInfo& classroom_info);

	// 获取当前学校云网络教室列表
	// @param ip 基础平台服务器IP
	// @param port 基础平台服务器端口
	// @param classroom_list 云网络教室列表
	// @return 0:Succeeded 1:Failed
	int GetAllNetClassroomInfo(IN char* ip, IN char* port,OUT std::list<NetWorkRoomInfo>& classroom_list);

	// 通过教室id获取单个云网络教室信息
	// @param classroom_list 云网络教室列表
	// @param classroom_id 教室id
	// @param classroom_info 得到的班级信息
	// @return 0:Succeeded 1:Failed
	int GetNetClassroomInfoByClassID(IN std::list<NetWorkRoomInfo>& classroom_list, IN int classroom_id,OUT NetWorkRoomInfo& classroom_info);
    int GetAllNoElectricRoomInfo(IN char* ip, IN char* port, OUT std::list<NoElectricClassroomInfo>& classroom_list);
    int GetAllCampusSecurityInfo(IN char* ip, IN char* port, OUT std::list<CampusSecurityInfo>& campusSecur_list);
private:
	std::string m_sReqData;
public:
	std::string m_strServerIP;
};

void  ResolveLine(std::string sMsg, Camera& stu_camera);
void  ResolveIPLine(std::string sMsg, IP& stu_ip);
void  Remorvespace(std::string& str);
