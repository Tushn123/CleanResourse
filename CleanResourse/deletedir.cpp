// deletedir.cpp :  delete file or folder ! create by LIAL ANFOX
//bool DeleteDir(const char *pDir)
//pDir  filename or foldername
//If the function succeeds, the return value is true,fails=false
#undef UNICODE
#include <Windows.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include "deletedir.h"

bool DeleteFolder(char * lpPath) 
{     
	char szFind[MAX_PATH];     
	WIN32_FIND_DATA FindFileData; 
    strcpy(szFind,lpPath);     
	strcat(szFind,"\\*.*");
    HANDLE hFind=::FindFirstFile(szFind,&FindFileData);     
	if(INVALID_HANDLE_VALUE == hFind)//有趣的返回 
	{  
		return false;
	}         
	while(TRUE) {         
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)         
		{  //判断是否为文件夹        
			if(FindFileData.cFileName[0]!='.')             
			{   
				strcpy(szFind,lpPath);                 
				strcat(szFind,"\\");                 
				strcat(szFind,FindFileData.cFileName);                 
				if(!DeleteFolder(szFind))
					return false;
			}         
		}         
		else      
		{  
			char buf[500];
			memset(buf,0,500);
			strncpy(buf,lpPath,strlen(lpPath));
			strcat(buf,"\\");
			strcat(buf,FindFileData.cFileName);
			if(!DeleteFile(buf))//嵌套搜索删除
				return false;
		}         
		if(!FindNextFile(hFind,&FindFileData))    
			break;     
	}     
	FindClose(hFind);
	RemoveDirectory(lpPath);
	return true;
} 


bool DeleteDir(const char *pDir)
{
	if (pDir == NULL) 
		return false;
	//判断是文件还是文件夹
	struct _stat sfile;
    if(_stat( pDir, &sfile)==0)
	{
		if (_S_IFREG&sfile.st_mode)
		{
			if(DeleteFile(pDir))
				return true;
			else return false;
		}
		if (_S_IFDIR&sfile.st_mode)
		{
			
			if(DeleteFolder((char *)pDir))
				return true;
		}
	}
	return true;
}