/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: root
 *
 * Created on August 30, 2022, 3:03 PM
 */


#include <cstdlib>
#include <dirent.h>
#include <algorithm> 

#ifdef _WIN32
#include <dshow.h>
#endif

#include "GUI/GUI.h"
#include "pcap.h"
#include "global.h"
#include "webcam.h"
#include "tools.h"
#include "sniffer.h"
#include "clicker.h"
#include "program.h"
#include "mikrotik.h"




bool GLOBAL_STOP = false;

GLOBAL global;
GUI gui;
PCAP pcap;
SNIFFER sniffer;
CLICKER clicker;
PROGRAMS programs_;
MIKROTIK mikrotik;
SCREEN grab_screen_buffer;
WEBCAMS webcams;

void Load_Folders(ELEMENT* list) {
    list->item.push_back({&gui, ELEMENT_item::ListItem, "Downloads"});
    list->item.push_back({&gui, ELEMENT_item::ListItem, "Pictures"});
    list->item.push_back({&gui, ELEMENT_item::ListItem, "Music"});
    list->item.push_back({&gui, ELEMENT_item::ListItem, "Videos"});
    list->item.push_back({&gui, ELEMENT_item::ListItem, "tmp"});
}

void Load_NetIfList(ELEMENT* list){
    std::string s;
    DIR *dir;
    int k;
    char c[500], n[10];
    dir = opendir("/sys/class/net");
    struct dirent *entry;
    
    while ( (entry = readdir(dir)) != NULL) {
        //printf("{%d} - (%s) [%d] %d\n", entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
        sprintf(c, "/sys/class/net/%s/ifindex", entry->d_name);
        FILE *f;
        f = fopen(c, "rb");
        if(f != NULL) {
            for(int i=0;i<10;i++) n[i] = 0;
            k = fread(n, 1, 10, f);
            if(k >= 1 && k <= 9 && n[k-1] == '\n') n[k-1] = 0;
            fclose(f);
            sprintf(c, "%s:%s", n, entry->d_name);
            
            list->item.push_back({&gui, ELEMENT_item::ListItem, c});
        }
    };    
   closedir(dir);
   
   std::sort( list->item.begin(), list->item.end() );
}

void testtt() {
    
        
}

#define _WIN32

#ifdef _WIN32
HINSTANCE _hInstance_ = 0;

int test_web_cam() {

    printf("test_web_cam()...");
    
        ICreateDevEnum *pSysDevEnum = NULL;
	HRESULT hr = CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_SystemDeviceEnum,NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void **)&pSysDevEnum);
	if(FAILED(hr)){
		printf("failed to init CoCreateInstance\n");
		//getchar();
		return hr;
	} else {
            printf("init CoCreateInstance ok\n");
        }
        
	IEnumMoniker * pEnumCat = NULL;
	//Obtain a class enumerator for the video input category.
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
		&pEnumCat, 0);
	if( hr == S_OK){
            printf("CreateClassEnumerator ok\n");
		//Enumerate the moniker
		IMoniker *pMoniker = NULL;
		ULONG cFetched;
                int ii = 0;
		while(pEnumCat->Next(1,&pMoniker,&cFetched) == S_OK){
                    
                    printf("111 [%d]\n", ii++);
                    
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0,0, IID_IPropertyBag,
				(void**)&pPropBag);
			if(SUCCEEDED(hr))
			{
                                printf("222 ok\n");
                            
				//Retrieve the filter`s friendly name.
				VARIANT varName,varDesc,varPath;
				VariantInit(&varName);
				VariantInit(&varDesc);
				VariantInit(&varPath);
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				
				if(SUCCEEDED(hr)){
                                    char nn[2000];
                                    wchar_to_char(varName.bstrVal, nn, 2000);
                                    printf("the dev name is: [%s]\n", nn);
				} else { 
                                    printf("fail 1\n"); 
                                };
				hr = pPropBag->Read(L"Description", &varDesc, 0);
				if(SUCCEEDED(hr)){
					printf("the dev desc is:\t[%s]\n",
						varDesc.bstrVal);
				} else { 
                                    printf("fail 2\n"); 
                                };
				hr = pPropBag->Read(L"DevicePath", &varPath, 0);
				if(SUCCEEDED(hr)){
					printf("the dev path is:\t[%s]\n",
						varPath.bstrVal);
				} else { 
                                    printf("fail 3\n"); 
                                }
				VariantClear(&varName);
				VariantClear(&varDesc);
				VariantClear(&varPath);
			} else {
                            printf("222 fail\n");
                        }
			pPropBag->Release();
		}
		pEnumCat->Release();
	} else {
            printf("CreateClassEnumerator fail\n");
        }
	pSysDevEnum->Release();
        
        return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    printf("start WinMain\n");
 
    int rr = test_web_cam();
    
    _hInstance_ = hInstance;
    
    programs_.init();
    
    gui.create_elements();
    global.set_gui(&gui);
    global.init();
    
    if(gui.low_level_init()==false) return -1;
    
    gui.wait_run();
    
    set_GLOBAL_STOP(L"main");
    programs_.wait_execute_close();   
    gui.wait_execute_close();
    global.wait_execute_close();
    webcams.wait_execute_close();
    
    gui.finish();
    
    return 0;
}
#endif

int main(int argc, char** argv) {
  
    printf("start\n");
  /*  
    pid_t p;
    int i;
    for(i = 0; i < 5; i++) {
        printf(".%d[%d]\n", i, getpid());
        p = fork();
    }
    
    return 0;
*/    
    std::vector<std::string> list;

    mikrotik.set_ip_login_pass("192.168.5.5", "admin", "Qq1233!!");
    
    programs_.init();
    
    gui.create_elements();
    global.set_gui(&gui);
    global.init();
    
    if(gui.low_level_init()==false) return -1;
    
    Load_NetIfList(gui.IfList);
    Load_Folders(gui.FoldersList);
    
    std::vector<std::string> lst;
    
    webcams.init(&lst, &grab_screen_buffer);
    
    clicker.init();
    
    
    //global.active_PROG_tab();
    
    gui.wait_run();
    
    set_GLOBAL_STOP(L"main");
    programs_.wait_execute_close();   
    gui.wait_execute_close();
    global.wait_execute_close();
    webcams.wait_execute_close();
    
    gui.finish();
    
    return 0;
}


