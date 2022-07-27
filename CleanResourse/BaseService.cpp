#include "BaseService.h"
#include "HttpService.h"
#include "xml\tinyxml2.h"

static void SchoolBaseInfoJ2S(const Json::Value& json_info,
	SchoolBaseInfo& struct_info) {
	if (json_info.isMember("SchoolID")) {
		struct_info.school_id = json_info["SchoolID"].asString();
	}
	else {
		struct_info.school_id = "";
	}

	if (json_info.isMember("SchoolName")) {
		struct_info.school_name = json_info["SchoolName"].asString();
	}
	else {
		struct_info.school_name = "";
	}

	if (json_info.isMember("SchoolCode")) {
		struct_info.school_code = json_info["SchoolCode"].asString();
	}
	else {
		struct_info.school_code = "";
	}

	if (json_info.isMember("SchoolLevel")) {
		struct_info.school_level =
			std::atoi(json_info["SchoolLevel"].asCString());
	}
	else {
		struct_info.school_level = 0;
	}

	if (json_info.isMember("SchoolType")) {
		struct_info.school_type =
			std::atoi(json_info["SchoolType"].asCString());
	}
	else {
		struct_info.school_type = 0;
	}

	if (json_info.isMember("SchoolState")) {
		struct_info.school_state =
			std::atoi(json_info["SchoolState"].asCString());
	}
	else {
		struct_info.school_state = 0;
	}

	if (json_info.isMember("CreateTime")) {
		struct_info.create_time = json_info["CreateTime"].asString();
	}
	else {
		struct_info.create_time = "";
	}
}

static void DeviceInfoJ2S(const Json::Value& json_info,
	DeviceInfo& struct_info) {

	if (json_info.isMember("AmplifierModelID")) {
		struct_info.amplifier_model_id =
			json_info["AmplifierModelID"].asString();
	}
	else {
		struct_info.amplifier_model_id = "";
	}

	if (json_info.isMember("DVDModelID")) {
		struct_info.dvd_model_id = json_info["DVDModelID"].asString();
	}
	else {
		struct_info.dvd_model_id = "";
	}

	if (json_info.isMember("ProjectorModelID")) {
		struct_info.projector_model_id =
			json_info["ProjectorModelID"].asString();
	}
	else {
		struct_info.projector_model_id = "";
	}

	if (json_info.isMember("LamplightModelID")) {
		struct_info.lamplight_model_id =
			json_info["LamplightModelID"].asString();
	}
	else {
		struct_info.lamplight_model_id = "";
	}

	if (json_info.isMember("CurtainModelID")) {
		struct_info.curtain_model_id = json_info["CurtainModelID"].asString();
	}
	else {
		struct_info.curtain_model_id = "";
	}
}

static void BigScreenmodelJ2S(const Json::Value& json_info,
	BigScreenmodel& struct_info) {
	if (json_info.isMember("ID")) {
		struct_info.id = std::atoi(json_info["ID"].asCString());
	}
	else {
		struct_info.id = 0;
	}

	if (json_info.isMember("PositionX")) {
		struct_info.position_x = std::atof(json_info["PositionX"].asCString());
	}
	else {
		struct_info.position_x = 0.0;
	}

	if (json_info.isMember("PositionY")) {
		struct_info.position_y = std::atof(json_info["PositionY"].asCString());
	}
	else {
		struct_info.position_y = 0.0;
	}

	if (json_info.isMember("Width")) {
		struct_info.width = std::atof(json_info["Width"].asCString());
	}
	else {
		struct_info.width = 0.0;
	}

	if (json_info.isMember("Height")) {
		struct_info.height = std::atof(json_info["Height"].asCString());
	}
	else {
		struct_info.height = 0.0;
	}

	if (json_info.isMember("ClassRoomId")) {
		struct_info.classroom_id =
			std::atoi(json_info["ClassRoomId"].asCString());
	}
	else {
		struct_info.classroom_id = 0;
	}

	if (json_info.isMember("UpdateTime")) {
		struct_info.update_time = json_info["UpdateTime"].asString();
	}
	else {
		struct_info.update_time = "";
	}

	if (json_info.isMember("ResolutionWidth")) {
		struct_info.resolution_width =
			std::atof(json_info["ResolutionWidth"].asCString());
	}
	else {
		struct_info.resolution_width = 0.0;
	}

	if (json_info.isMember("ResolutionHeight")) {
		struct_info.resolution_height =
			std::atof(json_info["ResolutionHeight"].asCString());
	}
	else {
		struct_info.resolution_height = 0.0;
	}
}
static void CampusSecurServersJ2S(const Json::Value& json_info,
    SecurityCamServers& struct_info)
{
    if (json_info.isMember("ID")) {
        struct_info.id = std::atoi(json_info["ID"].asCString());
    }
    else {
        struct_info.id = 0;
    }

    if (json_info.isMember("Type")) {
        struct_info.type = std::atoi(json_info["Type"].asCString());
    }
    else {
        struct_info.type = 0;
    }

    if (json_info.isMember("IP")) {
        struct_info.ip = json_info["IP"].asString();
    }
    else {
        struct_info.ip = "";
    }

    if (json_info.isMember("Port")) {
        struct_info.port = json_info["Port"].asString();
    }
    else {
        struct_info.port = "";
    }
}

static void CampusSecurInfoJ2S(const Json::Value& json_info,
    SecurityCamInfo& struct_info)
{
    if (json_info.isMember("ID")) {
        struct_info.cam_id = std::atoi(json_info["ID"].asCString());
    }
    else {
        struct_info.cam_id = 0;

    }

    if (json_info.isMember("Name")) {
        struct_info.camName = json_info["Name"].asString();
    }
    else {
        struct_info.camName = "";
    }

    if (json_info.isMember("IP")) {
        struct_info.camIP = json_info["IP"].asString();
    }
    else {
        struct_info.camIP = "";
    }

    if (json_info.isMember("Port")) {
        struct_info.camPort = json_info["Port"].asString();
    }
    else {
        struct_info.camPort = "";
    }

    if (json_info.isMember("Account")) {
        struct_info.camAccount = json_info["Account"].asString();
    }
    else {
        struct_info.camAccount = "";
    }

    if (json_info.isMember("Pwd")) {
        struct_info.camPwd = json_info["Pwd"].asString();
    }
    else {
        struct_info.camPwd = "";
    }

    if (json_info.isMember("Brand")) {
        struct_info.cam_brand = std::atoi(json_info["Brand"].asCString());
    }
    else {
        struct_info.cam_brand = 0;
    }

    if (json_info.isMember("Position")) {
        struct_info.camPostion = json_info["Position"].asString();
    }
    else {
        struct_info.camPostion = "";
    }

    if (json_info.isMember("Servers") && json_info["Servers"].isMember("VideoServerInfo"))
    {
        Json::Value servers = json_info["Servers"]["VideoServerInfo"];
        if (servers.isArray())
        {
            for (unsigned int i = 0; i < servers.size(); ++i)
            {
                SecurityCamServers info;
                CampusSecurServersJ2S(servers[i], info);
                struct_info.cam_servers.push_back(info);
            }
        }
        else
        {
            SecurityCamServers info;
            CampusSecurServersJ2S(servers, info);
            struct_info.cam_servers.push_back(info);
        }
    }
}

static void NoElcRoomCamInfoJ2S(const Json::Value& json_info,
    NoElcRoomCamInfo& struct_info)
{
    if (json_info.isMember("camId")) {
        struct_info.cam_id = json_info["camId"].asString();
    }
    else {
        struct_info.cam_id = "";
    }

    if (json_info.isMember("camType")) {
        struct_info.cam_type = std::atoi(json_info["camType"].asCString());
    }
    else {
        struct_info.cam_type = 1;
    }

    if (json_info.isMember("camName")) {
        struct_info.camName = json_info["camName"].asString();
    }
    else {
        struct_info.camName = "";
    }

    if (json_info.isMember("camIP")) {
        struct_info.camIP = json_info["camIP"].asString();
    }
    else {
        struct_info.camIP = "";
    }

    if (json_info.isMember("camPort")) {
        struct_info.camPort = json_info["camPort"].asString();
    }
    else {
        struct_info.camPort = "";
    }

    if (json_info.isMember("camAccount")) {
        struct_info.camAccount = json_info["camAccount"].asString();
    }
    else {
        struct_info.camAccount = "";
    }

    if (json_info.isMember("camPwd")) {
        struct_info.camPwd = json_info["camPwd"].asString();
    }
    else {
        struct_info.camPwd = "";
    }
}

static void CameraInfoJ2S(const Json::Value& json_info,
	CameraInfo& struct_info) {
	if (json_info.isMember("camId")) {
		struct_info.cam_id = json_info["camId"].asString();
	}
	else {
		struct_info.cam_id = "";
	}

	if (json_info.isMember("camName")) {
		struct_info.camName = json_info["camName"].asString();
	}
	else {
		struct_info.camName = "";
	}

	if (json_info.isMember("camType")) {
		struct_info.cam_type = std::atoi(json_info["camType"].asCString());
	}
	else {
		struct_info.cam_type = 1;
	}

	if (json_info.isMember("camInfo")) {
		struct_info.set_info = json_info["camInfo"].asString();
	}
	else {
		struct_info.set_info = "";
	}

	if (json_info.isMember("resolutionWidth")) {
		struct_info.resolution_width =
			std::atof(json_info["resolutionWidth"].asCString());
	}
	else {
		struct_info.resolution_width = 0.0;
	}

	if (json_info.isMember("resolutionHeight")) {
		struct_info.resolution_height =
			atof(json_info["resolutionHeight"].asCString());
	}
	else {
		struct_info.resolution_height = 0.0;
	}

	if (json_info.isMember("errorFlag")) {
		struct_info.error_flag = json_info["ErrorFlag"].asString();
	}
	else {
		struct_info.error_flag = "";
	}
}
static void CampusSecurityJ2S(const Json::Value& json_info,
    CampusSecurityInfo& struct_info)
{
    if (json_info.isMember("AreaID")) {
        struct_info.id = std::atoi(json_info["AreaID"].asCString());
    }
    else {
        struct_info.id = 0;
    }

    if (json_info.isMember("AreaName")) {
        struct_info.name = json_info["AreaName"].asCString();
    }
    else {
        struct_info.name = "";
    }

    if (json_info.isMember("Cameras") && json_info["Cameras"].isMember("CameraInfo"))
    {
        Json::Value camera = json_info["Cameras"]["CameraInfo"];
        if (camera.isArray())
        {
            for (unsigned int i = 0; i < camera.size(); ++i)
            {
                SecurityCamInfo info;
                CampusSecurInfoJ2S(camera[i], info);
                struct_info.securCam_list.push_back(info);
            }
        }
        else
        {
            SecurityCamInfo info;
            CampusSecurInfoJ2S(camera, info);
            struct_info.securCam_list.push_back(info);
        }
    }
        
}

static void NoElectricClassroomJ2S(const Json::Value& json_info,
    NoElectricClassroomInfo& struct_info)
{
    if (json_info.isMember("roomId")) {
        struct_info.roomId = std::atoi(json_info["roomId"].asCString());
    }
    else {
        struct_info.roomId = 0;
    }

    if (json_info.isMember("roomType")) {
        struct_info.roomType = std::atoi(json_info["roomType"].asCString());
    }
    else {
        struct_info.roomType = 0;
    }

    if (json_info.isMember("roomName")) {
        struct_info.roomName = json_info["roomName"].asString();
    }
    else {
        struct_info.roomName = "";
    }

    if (json_info.isMember("serverIP")) {
        struct_info.serverIP = json_info["serverIP"].asString();
    }
    else {
        struct_info.serverIP = "";
    }

    if (json_info.isMember("cameras") && json_info["cameras"].isMember("CameraInfo"))
    {
        Json::Value camera = json_info["cameras"]["CameraInfo"];
        if (camera.isArray())
        {
            for (unsigned int i = 0; i < camera.size(); ++i) 
            {
                NoElcRoomCamInfo info;
                NoElcRoomCamInfoJ2S(camera[i], info);
                struct_info.noElcRoomCam_list.push_back(info);
            }
        }
    }
}

static void ElectricClassroomJ2S(const Json::Value& json_info,
	ElectricClassroomInfo& struct_info) {

	if (json_info.isMember("roomId")) {
		struct_info.roomId = std::atoi(json_info["roomId"].asCString());
	}
	else {
		struct_info.roomId = 0;
	}

	if (json_info.isMember("roomName")) {
		struct_info.roomName = json_info["roomName"].asString();
	}
	else {
		struct_info.roomName = "";
	}

	if (json_info.isMember("roomPwd")) {
		struct_info.roomPwd = json_info["roomPwd"].asString();
	}
	else {
		struct_info.roomPwd = "";
	}
	
	if (json_info.isMember("buildingName")) {
		struct_info.buildingName = json_info["buildingName"].asString();
	}
	else {
		struct_info.buildingName = "";
	}

	if (json_info.isMember("rPersonName")) {
		struct_info.rPersonName = json_info["rPersonName"].asString();
	}
	else {
		struct_info.rPersonName = "";
	}

	if (json_info.isMember("rPersonPhone")) {
		struct_info.rPersonPhone = json_info["rPersonPhone"].asString();
	}
	else {
		struct_info.rPersonPhone = "";
	}
	
	if (json_info.isMember("roomStatus")) {
		struct_info.roomStatus = json_info["roomStatus"].asString();
	}
	else {
		struct_info.roomStatus = "";
	}
	
	if (json_info.isMember("hostMac")) {
		struct_info.hostMac = json_info["hostMac"].asCString();
	}
	else {
		struct_info.hostMac = "";
	}
	
	if (json_info.isMember("electricType")) {
		struct_info.electricType = json_info["electricType"].asString();
	}
	else {
		struct_info.electricType = "";
	}
	
	if (json_info.isMember("roomVersion")) {
		struct_info.roomVersion =json_info["roomVersion"].asCString();
	}
	else {
		struct_info.roomVersion = "";
	}
	
	if (json_info.isMember("centralIp")) {
		struct_info.centralIp = json_info["centralIp"].asString();
	}
	else {
		struct_info.centralIp = "";
	}
	
	if (json_info.isMember("centralPort")) {
		struct_info.centralPort = std::atoi(json_info["centralPort"].asCString());
	}
	else {
		struct_info.centralPort = 0;
	}
	
	if (json_info.isMember("terminalIp")) {
		struct_info.terminalIp = json_info["terminalIp"].asString();
	}
	else {
		struct_info.terminalIp = "";
	}
	
	if (json_info.isMember("terminalPort")) {
		struct_info.terminalPort = std::atoi(json_info["terminalPort"].asCString());
	}
	else {
		struct_info.terminalPort = 0;
	}
	
	if (json_info.isMember("recordServerIp")) {
		struct_info.recordServerIp = json_info["recordServerIp"].asString();
	}
	else {
		struct_info.recordServerIp = "";
	}

	if (json_info.isMember("deviceEquip")) {
		struct_info.deviceEquip = json_info["deviceEquip"].asString();
	}
	else {
		struct_info.deviceEquip = "";
	}
	
	if (json_info.isMember("cameraInfo") &&
		json_info["cameraInfo"].isMember("Model_CameraInfo")) {
		Json::Value camera = json_info["cameraInfo"]["Model_CameraInfo"];
		if (camera.isArray()) {
			for (unsigned int i = 0; i < camera.size(); ++i) {
				CameraInfo info;
				CameraInfoJ2S(camera[i], info);
				struct_info.camera_info_list.push_back(info);
			}
		}
		else {
			CameraInfo info;
			CameraInfoJ2S(camera, info);
			struct_info.camera_info_list.push_back(info);
		}
	}
}

static void CamInfoJ2S(const Json::Value& json_info, CamInfo& struct_info) {
	if (json_info.isMember("camId")) {
		struct_info.camId = json_info["camId"].asString();
	}
	else {
		struct_info.camId = "";
	}

	if (json_info.isMember("camType")) {
		struct_info.cam_type = std::atoi(json_info["camType"].asCString());
	}
	else {
		struct_info.cam_type = 0;
	}

	if (json_info.isMember("CamIP")) {
		struct_info.cam_ip = json_info["CamIP"].asString();
	}
	else {
		struct_info.cam_ip = "";
	}

	if (json_info.isMember("CamPort")) {
		struct_info.cam_port = std::atoi(json_info["CamPort"].asCString());
	}
	else {
		struct_info.cam_port = 0;
	}

	if (json_info.isMember("AccessAccount")) {
		struct_info.access_account = json_info["AccessAccount"].asString();
	}
	else {
		struct_info.access_account = "";
	}

	if (json_info.isMember("AccessPwd")) {
		struct_info.access_pwd = json_info["AccessPwd"].asString();
	}
	else {
		struct_info.access_pwd = "";
	}

	if (json_info.isMember("ResolutionWidth")) {
		struct_info.resolution_width =
			std::atoi(json_info["ResolutionWidth"].asCString());
	}
	else {
		struct_info.resolution_width = 0;
	}

	if (json_info.isMember("ResolutionHeight")) {
		struct_info.resolution_height =
			std::atoi(json_info["ResolutionHeight"].asCString());
	}
	else {
		struct_info.resolution_height = 0;
	}

	if (json_info.isMember("ErrorFlag")) {
		struct_info.error_flag = std::atoi(json_info["ErrorFlag"].asCString());
	}
	else {
		struct_info.error_flag = 0;
	}
}

static void NetWorkRoomInfoJ2S(const Json::Value& json_info,NetWorkRoomInfo& struct_info)
{
	if (json_info.isMember("RoomID")) {
		struct_info.room_id = std::atoi(json_info["RoomID"].asCString());
	}
	else {
		struct_info.room_id = 0;
	}

	if (json_info.isMember("RoomName")) {
		struct_info.room_name = json_info["RoomName"].asString();
	}
	else {
		struct_info.room_name = "";
	}

	if (json_info.isMember("TeacherIP")) {
		struct_info.teacher_ip = json_info["TeacherIP"].asString();
	}
	else {
		struct_info.teacher_ip = "";
	}

	if (json_info.isMember("MacAddr")) {
		struct_info.mac_addr = json_info["MacAddr"].asString();
	}
	else {
		struct_info.mac_addr = "";
	}

	if (json_info.isMember("CamInfo") && json_info["CamInfo"].isObject()) {
		Json::Value cam_info = json_info["CamInfo"];
		if (cam_info.isArray()) {
			for (unsigned int i = 0; i < cam_info.size(); ++i) {
				CamInfo info;
				CamInfoJ2S(cam_info[i], info);
				struct_info.cam_info_list.push_back(info);
			}
		}
		else {
			CamInfo info;
			CamInfoJ2S(cam_info, info);
			struct_info.cam_info_list.push_back(info);
		}
	}
}
void  ResolveIPLine(std::string sMsg, IP& stu_ip)
{
	int iCount = -1;

	iCount = sMsg.find("|");
	if (iCount < 0 || sMsg.empty())
		return;
	stu_ip.strRtmp_ip = sMsg.substr(0, iCount);
	sMsg = sMsg.substr(iCount + 1);

	iCount = sMsg.find("|");
	if (iCount < 0 || sMsg.empty())
		return;
	stu_ip.str480_ip = sMsg.substr(0, iCount);
	sMsg = sMsg.substr(iCount + 1);

	iCount = sMsg.find("|");
	if (iCount < 0 || sMsg.empty())
		return;
	stu_ip.str1080_ip = sMsg.substr(0, iCount);
	sMsg = sMsg.substr(iCount + 1);

	if (!sMsg.empty())
		stu_ip.strCmb_ip = sMsg;

	return;
}
void  ResolveLine(std::string sMsg, Camera& stu_camera)
{
	int iCount = -1;

	iCount = sMsg.find("|");
	if (iCount < 0 || sMsg.empty())
		return;
	stu_camera.camera_ip = sMsg.substr(0, iCount);
	sMsg = sMsg.substr(iCount + 1);

	iCount = sMsg.find("|");
	if (iCount < 0 || sMsg.empty())
		return;
	stu_camera.camera_port = sMsg.substr(0, iCount);
	sMsg = sMsg.substr(iCount + 1);
	
	iCount = sMsg.find("|");
	if (iCount < 0 || sMsg.empty())
		return;
	stu_camera.camera_username = sMsg.substr(0, iCount);
	sMsg = sMsg.substr(iCount + 1);
	
	iCount = sMsg.find("|");
	if (iCount < 0 || sMsg.empty())
		return;
	stu_camera.camera_userpwd = sMsg.substr(0, iCount);
	sMsg = sMsg.substr(iCount + 1);
	
	if (!sMsg.empty())
		stu_camera.camera_ch = sMsg;
	
	return;
}
void Remorvespace(std::string& str)
{
	int pos = str.find(" ", 0);
	while (pos != -1)
	{
		str.replace(pos, 1, "");
		pos = str.find(" ", pos);
	}
}


CBaseService::CBaseService()
{

}
CBaseService::~CBaseService(void)
{

}

int CBaseService::ParseResponse(char* buf, Json::Value& json_data)
{
	tinyxml2::XMLDocument doc;
	json_data = Json::Value::null;
	if (doc.Parse(buf) != tinyxml2::XML_SUCCESS) {
		return -1;
	}

	// 首节点不为数组
	ConvertElement(doc.RootElement(), json_data, false);

	return 0;
}

//void CBaseService::ConvertElement(tinyxml2::XMLElement* node, Json::Value& json_data, bool root_is_array)
//{
//	if (!node) {
//		json_data = Json::Value::null;
//		return;
//	}
//
//	const char* node_name = node->Name();
//	tinyxml2::XMLElement* child_node = node->FirstChildElement();
//	if (child_node) {
//		// 比较前两个节点名称，判断其子节点是否为数组
//		std::string child_node_name = child_node->Name();
//		tinyxml2::XMLElement* next_node = child_node->NextSiblingElement();
//		if (next_node) {
//			bool is_array = false;
//			if (child_node_name == next_node->Name()) {
//				is_array = true;
//			}
//
//			unsigned int index = 0;
//			tinyxml2::XMLElement* i_node = node->FirstChildElement();
//			for (; i_node != NULL; i_node = i_node->NextSiblingElement()) {
//				const char* i_node_name = i_node->Name();
//				if (is_array) {
//					ConvertElement(i_node,
//						json_data[node_name][i_node_name][index++],
//						true);
//				}
//				else {
//					if (root_is_array) {
//						ConvertElement(i_node, json_data, false);
//					}
//					else {
//						ConvertElement(i_node, json_data[node_name], false);
//					}
//				}
//			}
//		}
//		else {
//			// 该node只有一个子节点
//			if (root_is_array) {
//				ConvertElement(child_node, json_data, false);
//			}
//			else {
//				ConvertElement(child_node, json_data[node_name], false);
//			}
//		}
//	}
//	else {
//		// 独立节点，无子节点
//		if (root_is_array) {
//			json_data = (node->GetText() != NULL) ? node->GetText() : "";
//		}
//		else {
//			json_data[node_name] =
//				(node->GetText() != NULL) ? node->GetText() : "";
//		}
//	}
//}
void CBaseService::ConvertElement(tinyxml2::XMLElement* node, Json::Value& json_data,
	bool root_is_array) {
	if (!node) {
		json_data = Json::Value::null;
		return;
	}

	const char* node_name = node->Name();
	tinyxml2::XMLElement* child_node = node->FirstChildElement();
	if (child_node) {
		// 比较前两个节点名称，判断其子节点是否为数组
		std::string child_node_name = child_node->Name();
		tinyxml2::XMLElement* next_node = child_node->NextSiblingElement();
		if (next_node) {
			bool is_array = false;
			if (child_node_name == next_node->Name()) {
				is_array = true;
			}

			unsigned int index = 0;
			tinyxml2::XMLElement* i_node = node->FirstChildElement();
			for (; i_node != NULL; i_node = i_node->NextSiblingElement()) {
				const char* i_node_name = i_node->Name();
				if (is_array) {
					ConvertElement(i_node, json_data[node_name][i_node_name][index++],
						true);
				}
				else {
					if (root_is_array) {
						ConvertElement(i_node, json_data, false);
					}
					else {
						ConvertElement(i_node, json_data[node_name], false);
					}
				}
			}
		}
		else {
			// 该node只有一个子节点
			if (root_is_array) {
				ConvertElement(child_node, json_data, false);
			}
			else {
				ConvertElement(child_node, json_data[node_name], false);
			}
		}
	}
	else {  // 独立节点，无子节点
			// 仅转化有值的节点，丢弃空节点
		if (node->GetText()) {
			root_is_array ? json_data = node->GetText()
				: json_data[node_name] = node->GetText();
		}
	}
}
long CBaseService::GetSubSystemServerInfo(IN char* code, OUT char* service_ip_port)
{
	CHttpService cHttpService;
	int cnt = 0;

	// http://172.16.52.250:8002/Base/WS/Service_Basic.asmx/WS_G_GetSubSystemServerInfo?sysID=E00&subjectID=
	//char cReqData[1024 * 10];
	std::string strUrl;
	strUrl = "http://";
	strUrl += m_strServerIP;
	strUrl += "/Base/WS/Service_Basic.asmx/WS_G_GetSubSystemServerInfo?sysID=";
	strUrl += code;
	strUrl += "&subjectID=";

	
	TCHAR cUrl[1024 * 5] = L"";
	g_charToTCHAR(strUrl.c_str(), cUrl);
	m_sReqData = "";
	cHttpService.HttpGet(cUrl, m_sReqData);

	Json::Value out_param;
	if (ParseResponse((char*)m_sReqData.c_str(), out_param) != 0) {
		return -1;
	}

	if (!out_param.empty() && out_param.isMember("ArrayOfString") &&
		out_param["ArrayOfString"].isObject() &&
		out_param["ArrayOfString"].isMember("string") &&
		out_param["ArrayOfString"]["string"].isArray() &&
		out_param["ArrayOfString"]["string"].size() >= 4)
	{
		std::string str_server = out_param["ArrayOfString"]["string"][3].asString();
		std::string ip_port = str_server.substr(7, str_server.size() - 8);

		g_StringToChar(ip_port.c_str(), service_ip_port);
	}
	else {
		return -1;
	}

	return 0;
}

int CBaseService::GetServiceInfo(IN char* cIP, IN char* port, IN char* code, OUT char* service_ip, OUT char* service_port)
{
	CHttpService cHttpService;
	int cnt = 0;
	
	// http://192.168.2.55:8002/Base/WS/Service_Basic.asmx/WS_G_GetSubSystemServerInfo?sysID=E00&subjectID=
	char cReqData[1024 * 10];
	std::string strUrl;
	strUrl = "http://";
	strUrl += cIP;
	strUrl += ":";
	strUrl += port;
	strUrl += "/Base/WS/Service_Basic.asmx/WS_G_GetSubSystemServerInfo?sysID=";
	strUrl += code;
	strUrl += "&subjectID=";

	TCHAR cUrl[1024 * 5] = L"";
	g_charToTCHAR(strUrl.c_str(), cUrl);
	int iret = cHttpService.HttpGet(cUrl, cReqData);
	if (iret!=0)
	{
		return -1;
	}
	
	Json::Value out_param;
	if (ParseResponse(cReqData, out_param) != 0) {
		return -1;
	}

	if (!out_param.empty() && out_param.isMember("ArrayOfString") &&
		out_param["ArrayOfString"].isObject() &&
		out_param["ArrayOfString"].isMember("string") &&
		out_param["ArrayOfString"]["string"].isArray() &&
		out_param["ArrayOfString"]["string"].size() >= 4)
	{
		std::string str_server = out_param["ArrayOfString"]["string"][3].asString();
		std::string ip_port = str_server.substr(7, str_server.size() - 8);

		g_StringToChar(ip_port.substr(0, ip_port.find(":")).c_str(),service_ip);
		g_StringToChar(ip_port.substr(ip_port.find(":") + 1).c_str(), service_port);

	}
	else {
		return -1;
	}

	return 0;
}



int CBaseService::GetAllElectricClassroomInfo(IN char* ip, IN char* port, OUT std::list<ElectricClassroomInfo>& classroom_list)
{
	CHttpService cHttpService;
	int cnt = 0;

	char server_ip[MAX_PATH];
	char server_port[MAX_PATH];
	if (GetServiceInfo(ip, port, "E00", server_ip, server_port) != 0)
	{
		return -1;
	}

	//SchoolBaseInfo school_info;
	/*if (GetCurrentSchoolInfo(ip, port, school_info) != 0) 
	{
		return -1;
	}*/

	//char cReqData[1024 * 10];
	std::string strUrl;
	strUrl = "http://";
	strUrl += server_ip;
	strUrl += ":";
	strUrl += server_port;
	strUrl += "/WebService/Mgr_Classroom_PC.asmx/WS_IDMC_Room_Detail_Electric_Get";

	

	TCHAR cUrl[1024 * 5] = L"";
	string sReqData = "";
	g_charToTCHAR(strUrl.c_str(), cUrl);
	int iret = cHttpService.HttpGet(cUrl, sReqData);
	if (iret!=0)
	{
		return -1;
	}

	Json::Value out_param;
	if (ParseResponse((char*)sReqData.c_str(), out_param) != 0) {
		return -1;
	}

	if (!out_param.empty() &&
		out_param.isMember("ArrayOfModel_Room_Detail_Electric_Get") &&
		out_param["ArrayOfModel_Room_Detail_Electric_Get"].isObject() &&
		out_param["ArrayOfModel_Room_Detail_Electric_Get"].isMember("Model_Room_Detail_Electric_Get")) 
	{
		Json::Value& classroom_info = out_param["ArrayOfModel_Room_Detail_Electric_Get"]["Model_Room_Detail_Electric_Get"];
		if (classroom_info.isArray()) 
		{
			for (unsigned int i = 0; i < classroom_info.size(); ++i) {
				ElectricClassroomInfo struct_info;
				ElectricClassroomJ2S(classroom_info[i], struct_info);
				classroom_list.push_back(struct_info);
			}
		}
		else {
			ElectricClassroomInfo struct_info;
			ElectricClassroomJ2S(classroom_info, struct_info);
			classroom_list.push_back(struct_info);
		}
	}
	return 0;
}

int CBaseService::GetElectricClassroomInfoByClassID(IN std::list<ElectricClassroomInfo>& classroom_list, IN int classroom_id, OUT ElectricClassroomInfo& classroom_info)
{
	std::list<ElectricClassroomInfo>::iterator it_classroom = classroom_list.begin();
	for (; it_classroom != classroom_list.end(); ++it_classroom) {
		if (classroom_id == it_classroom->roomId) {
			classroom_info = *it_classroom;
			return 0;
		}
	}

	return 0;
}

int CBaseService::GetAllNoElectricRoomInfo(IN char* ip, IN char* port, OUT std::list<NoElectricClassroomInfo>& classroom_list)
{
    CHttpService cHttpService;
    int cnt = 0;

    char server_ip[MAX_PATH];
    char server_port[MAX_PATH];
    if (GetServiceInfo(ip, port, "E00", server_ip, server_port) != 0)
    {
        return -1;
    }

    std::string strUrl;
    strUrl = "http://";
    strUrl += server_ip;
    strUrl += ":";
    strUrl += server_port;
    strUrl += "/api/GetNoElectricRoomCameras";

  

    TCHAR cUrl[1024 * 5] = L"";
    string sReqData = "";
    g_charToTCHAR(strUrl.c_str(), cUrl);
    cHttpService.HttpGet(cUrl, sReqData);

    Json::Value out_param;
    if (ParseResponse((char*)sReqData.c_str(), out_param) != 0) {
        return -1;
    }

    if (!out_param.empty()&&
        out_param.isMember("ArrayOfModel_TeachMonitor")&&
        out_param["ArrayOfModel_TeachMonitor"].isObject() && 
        out_param["ArrayOfModel_TeachMonitor"].isMember("Model_TeachMonitor"))
    {
        Json::Value& noElecRoominfo = out_param["ArrayOfModel_TeachMonitor"]["Model_TeachMonitor"];
        if (noElecRoominfo.isArray())
        {
            for (unsigned int i = 0; i < noElecRoominfo.size(); ++i)
            {
                NoElectricClassroomInfo struct_info;
                NoElectricClassroomJ2S(noElecRoominfo[i], struct_info);
                classroom_list.push_back(struct_info);
            }
        }
        else
        {
            NoElectricClassroomInfo struct_info;
            NoElectricClassroomJ2S(noElecRoominfo, struct_info);
            classroom_list.push_back(struct_info);
        }
    }
}

int CBaseService::GetAllCampusSecurityInfo(IN char* ip, IN char* port, OUT std::list<CampusSecurityInfo>& campusSecur_list)
{
    CHttpService cHttpService;
    int cnt = 0;

    char server_ip[MAX_PATH];
    char server_port[MAX_PATH];
    if (GetServiceInfo(ip, port, "M10", server_ip, server_port) != 0)
    {
        return -1;
    }

    std::string strUrl;
    strUrl = "http://";
    strUrl += server_ip;
    strUrl += ":";
    strUrl += server_port;
    strUrl += "/api/GetCameras";

	
	TCHAR cUrl[1024 * 5] = L"";
	string sReqData = "";
	g_charToTCHAR(strUrl.c_str(), cUrl);
	cHttpService.HttpGet(cUrl, sReqData);

    Json::Value out_param;
    if (ParseResponse((char*)sReqData.c_str(), out_param) != 0) {
        return -1;
    }


    if (!out_param.empty() &&
        out_param.isMember("ArrayOfAreaInfo") &&
        out_param["ArrayOfAreaInfo"].isObject() &&
        out_param["ArrayOfAreaInfo"].isMember("AreaInfo"))
    {
        Json::Value& areaInfo = out_param["ArrayOfAreaInfo"]["AreaInfo"];
        if (areaInfo.isArray())
        {
            for (unsigned int i = 0; i < areaInfo.size(); ++i)
            {
                CampusSecurityInfo struct_info;
                CampusSecurityJ2S(areaInfo[i], struct_info);
                campusSecur_list.push_back(struct_info);
            }
        }
        else
        {
            CampusSecurityInfo struct_info;
            CampusSecurityJ2S(areaInfo, struct_info);
            campusSecur_list.push_back(struct_info);
        }
    }
    return 0;
}

int CBaseService::GetAllNetClassroomInfo(IN char* ip, IN char* port, OUT std::list<NetWorkRoomInfo>& classroom_list)
{
	CHttpService cHttpService;
	int cnt = 0;

	char server_ip[MAX_PATH];
	char server_port[MAX_PATH];
	if (GetServiceInfo(ip, port, "E00", server_ip, server_port) != 0)
	{
		return -1;
	}

	SchoolBaseInfo school_info;
	

	// http://172.16.52.250:8011/WebService.asmx/WS_G_CRMgr_GetNetworkRoomBySchoolID?SchoolID=S21-888888-289A
	//char cReqData[1024 * 10];
	std::string strUrl;
	strUrl = "http://";
	strUrl += server_ip;
	strUrl += ":";
	strUrl += server_port;
	strUrl += "/WebService.asmx/WS_G_CRMgr_GetNetworkRoomBySchoolID?SchoolID=";
	strUrl += school_info.school_id.c_str();

	

	TCHAR cUrl[1024 * 5] = L"";
	string sReqData = "";
	g_charToTCHAR(strUrl.c_str(), cUrl);
	cHttpService.HttpGet(cUrl, sReqData);

	Json::Value out_param;
	if (ParseResponse((char*)sReqData.c_str(), out_param) != 0) {
		return -1;
	}

	if (!out_param.empty() &&
		out_param.isMember("ArrayOfNetWorkRoomInfo") &&
		out_param["ArrayOfNetWorkRoomInfo"].isObject() &&
		out_param["ArrayOfNetWorkRoomInfo"].isMember("NetWorkRoomInfo")) {
		Json::Value& classroom_info = out_param["ArrayOfNetWorkRoomInfo"]["NetWorkRoomInfo"];
		if (classroom_info.isArray()) 
		{
			for (unsigned int i = 0; i < classroom_info.size(); ++i) 
			{
				NetWorkRoomInfo struct_info;
				NetWorkRoomInfoJ2S(classroom_info[i], struct_info);
				classroom_list.push_back(struct_info);
			}
		}
		else {
			NetWorkRoomInfo struct_info;
			NetWorkRoomInfoJ2S(classroom_info, struct_info);
			classroom_list.push_back(struct_info);
		}
	}
	return 0;
}

int CBaseService::GetNetClassroomInfoByClassID(IN std::list<NetWorkRoomInfo>& classroom_list, IN int classroom_id, OUT NetWorkRoomInfo& classroom_info)
{
	std::list<NetWorkRoomInfo>::iterator it_classroom = classroom_list.begin();
	for (; it_classroom != classroom_list.end(); ++it_classroom) {
		if (classroom_id == it_classroom->room_id) {
			classroom_info = *it_classroom;
			return 0;
		}
	}
	return 0;
}