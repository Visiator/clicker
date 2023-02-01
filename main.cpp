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
#define STRSAFE_NO_DEPRECATE

#include <sys/types.h>
#include <sys/stat.h>

#include <cstdlib>
#include <dirent.h>
#include <algorithm> 

#ifdef _WIN32
#include <windef.h>
#include "windows.h"
#include <combaseapi.h>
#include <dshow.h>
#include <qedit.h>
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

//#define _WIN32

#ifdef _WIN32

static
const
CLSID CLSID_SampleGrabber = { 0xC1F400A0, 0x3F08, 0x11d3, { 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37 } };


HRESULT hr;  
    AM_MEDIA_TYPE mt;
    ISampleGrabber *pGrabber = NULL;
    BYTE *pBuffer = NULL;

    IGraphBuilder *pGraph = NULL; //менеджер графа фильтров
    ICaptureGraphBuilder2 *pBuild = NULL; //граф фильтра аудио\видео захвата
    ICreateDevEnum *pDevEnum = NULL;
    IBaseFilter *pGrabberF = NULL;
    IEnumMoniker* pEnum = NULL;
    IMoniker* pMoniker = NULL;
    IBaseFilter *pCapture = NULL; //видеофильтр
    IMediaControl *pMediaControl(NULL);
    IMediaEventEx  *pEvent(NULL);
    
// Writes a bitmap file
//  pszFileName:  Output file name.
//  pBMI:         Bitmap format information (including pallete).
//  cbBMI:        Size of the BITMAPINFOHEADER, including palette, if present.
//  pData:        Pointer to the bitmap bits.
//  cbData        Size of the bitmap, in bytes.
 
HRESULT WriteBitmap(PCWSTR pszFileName, BITMAPINFOHEADER *pBMI, size_t cbBMI, BYTE *pData, size_t cbData) {
    
    printf("WriteBitmap...\n");
    
    HANDLE hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, 0, NULL);
    if (hFile == NULL)
    {
        printf("WriteBitmap err1\n");
        return HRESULT_FROM_WIN32(GetLastError());
    }
 
    BITMAPFILEHEADER bmf = { };
 
    bmf.bfType = 0x4d42;//'MB';
    bmf.bfSize = cbBMI+ cbData + sizeof(bmf);
    bmf.bfOffBits = sizeof(bmf) + cbBMI;
 
    DWORD cbWritten = 0;
    BOOL result = WriteFile(hFile, &bmf, sizeof(bmf), &cbWritten, NULL);
    if (result)
    {
        result = WriteFile(hFile, pBMI, cbBMI, &cbWritten, NULL);
    }
    if (result)
    {
        result = WriteFile(hFile, pData, cbData, &cbWritten, NULL);
    }
 
    HRESULT hr = result ? S_OK : HRESULT_FROM_WIN32(GetLastError());
 
    CloseHandle(hFile);
    printf("WriteBitmap end\n");
    return hr;
}

HINSTANCE _hInstance_ = 0;

char *p_GUID(char *ss, GUID g) {
    unsigned char *c;
    c = (unsigned char *)&g;
    sprintf(ss, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X", c[3]&0xff, c[2]&0xff, c[1]&0xff, c[0]&0xff, c[5]&0xff, c[4]&0xff, c[7]&0xff, c[6]&0xff
            , c[8]&0xff, c[9]&0xff, c[10]&0xff, c[11]&0xff, c[12]&0xff, c[13]&0xff, c[14]&0xff, c[15]&0xff );
    return ss;
}

void webcaminit() {
    

    //Memo1->Lines->Add("Выбран источник изображения - веб камера");
 
    //Panel1->Visible = true;
    //Button2->Enabled = false;
 
 
    printf("webcaminit...\n");
    
    ZeroMemory(&mt, sizeof(mt));
    mt.formattype = FORMAT_VideoInfo;
    mt.majortype = MEDIATYPE_Video;
    mt.subtype = MEDIASUBTYPE_RGB24;
 
    //Инициализируем библиотеку COM
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if(SUCCEEDED(hr))
    {
        printf("webcaminit 1\n");
        //Создание графа-фильтров - Create the Capture Graph Builder
        hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pBuild );
        if (SUCCEEDED(hr))
        {
            printf("webcaminit 2\n");
            //Создание менеджера графа фильтров - Create the Filter Graph Manager
            hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
            if (SUCCEEDED(hr))
            {
                printf("webcaminit 3\n");
                //Initialize the Capture Graph Builder
                pBuild->SetFiltergraph(pGraph);
 
 
                // Find system device enumerator to find a video capture device.
                hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (LPVOID*)&pDevEnum);
                hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&pGrabberF));
                if(SUCCEEDED(hr))
                {
 
                    printf("webcaminit 4\n");
                    
                    //создает счетчик для определенной категории устройств
                    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
 
                    if(SUCCEEDED(hr))
                    {
                        printf("webcaminit 5\n");
                        if( pEnum->Next(1, &pMoniker,0) == S_OK )
                        {
                            printf("webcaminit 6\n");
                            
                            hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&pCapture);
 
                            //hr = pGraph->AddFilter(pCapture, L"CaptureFilter");
                            //pGraph->RenderFile(L"D:\\Wildlife.wmv", NULL); //чтение файла - работает
 
                            if(SUCCEEDED(hr))
                            {
                                printf("webcaminit 7\n");
                                
                                pGraph->AddFilter(pCapture, L"Capture Filter");
                                hr = pGrabberF->QueryInterface(IID_PPV_ARGS(&pGrabber));
                                if(SUCCEEDED(hr)) {
                                    printf("webcaminit 8 ok\n");
                                } else {
                                    printf("webcaminit 8 err\n");
                                }
                                hr = pGraph->AddFilter(pGrabberF, L"Sample Grabber");
 
                                hr = pBuild->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pCapture, NULL, NULL);  //3,4,5 Параметры - фильтры, которые объединены в граф
                                hr = pBuild->RenderStream(NULL, NULL, pCapture, NULL, pGrabberF);
                                
                                hr = pGrabber->SetMediaType(&mt);
                                if(SUCCEEDED(hr)) {
                                    
                                    //GUID gg = mt.formattype;
                                    char ss[200];
                                    ss[0] = 0;
                                    printf("webcaminit 11 ok [%s]\n", p_GUID(ss, mt.formattype));
                                    
                                    if(mt.formattype == FORMAT_VideoInfo) {
                                        printf("mt.formattype == FORMAT_VideoInfo %d\n", mt.cbFormat );
                                    }
                                    
                                } else {
                                    printf("webcaminit 11 err\n");                                    
                                }
                                pDevEnum->Release();
                                pEnum->Release();
                                pMoniker->Release();
 
                                // Specify the owner window.
                                IVideoWindow *pVidWin = NULL;
                                OAHWND hWnd;// = Panel1->Handle;
 
                                pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
 
                                //2023 pVidWin->put_Owner((OAHWND)Panel1->Handle);
                                //2023 pVidWin->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS );
 
                                pVidWin->put_Left(0);
                                pVidWin->put_Top(0);
                                //pVidWin->put_Width(260);
                                //pVidWin->put_Height(180);
                                pVidWin->put_Width(800);
                                pVidWin->put_Height(600);
 
                                // Set the owner window to receive event notices.
                                pGraph->QueryInterface(IID_IMediaEventEx, (void **)&pEvent);
                                pEvent->SetNotifyWindow((OAHWND)hWnd, 0, 0);
 
                                hr = pGrabber->SetOneShot(true);
                                //hr = pGrabber->SetBufferSamples(true);
 
                                // Run the graph.
                                pGraph->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
                                pMediaControl->Run();
 
                                //Memo1->Lines->Add("Запуск веб камеры");
                            }
                            else
                                pBuild->Release();
 
 
 
                        }
                    }
                }
            }
        }
    }

    printf("webcaminit END\n");
}

int test_web_cam() {

    printf("test_web_cam()...\n");
    /*
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
                                printf("BindToStorage ok\n");
                            
				//Retrieve the filter`s friendly name.
				VARIANT varName,varDesc,varPath;
				VariantInit(&varName);
				VariantInit(&varDesc);
				VariantInit(&varPath);
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				
				if(SUCCEEDED(hr)){
                                    char nn[2000];
                                    wchar_to_char(varName.bstrVal, nn, 2000);
                                    printf("FriendlyName is: [%s]\n", nn);
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
    */    
        ////////////////////////////////////////////////////////////////////////
    #define SIZE_PREHEADER 48

    
    webcaminit();
    
    usleep(3000);
    
    printf("test_web_cam() 2\n");
    
    // Find the required buffer size.
    long cbBuffer;
    hr = pGrabber->GetCurrentBuffer(&cbBuffer, NULL);
    
    printf("test_web_cam() cbBuffer=%d\n", (DWORD)cbBuffer);
    
    
    pBuffer = (BYTE*)CoTaskMemAlloc(cbBuffer);
    hr = pGrabber->GetCurrentBuffer(&cbBuffer, (long*)pBuffer);
    hr = pGrabber->GetConnectedMediaType(&mt);
    // Examine the format block.
    
    if ((mt.formattype == FORMAT_VideoInfo)) {
        printf("test_web_cam() 2+ %d\n", mt.cbFormat);
    }
    
    if ((mt.formattype == FORMAT_VideoInfo) &&
        (mt.cbFormat >= sizeof(VIDEOINFOHEADER)) &&
        (mt.pbFormat != NULL))
    {
        printf("test_web_cam() 3\n");
        VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)mt.pbFormat;
 
        printf("INFO biSize=%d biWidth=%d biHeight=%d biBitCount=%d\n", pVih->bmiHeader.biSize , (DWORD)pVih->bmiHeader.biWidth, (DWORD)pVih->bmiHeader.biHeight, pVih->bmiHeader.biBitCount);
        
        hr = WriteBitmap(L"1.bmp", &pVih->bmiHeader,
            mt.cbFormat - SIZE_PREHEADER, pBuffer, cbBuffer);
            int e = SIZE_PREHEADER;
        for (int x = 0; x < pVih->bmiHeader.biHeight; x++)
        {
            for (int y = 0; y < pVih->bmiHeader.biWidth; y++)
            {
                //Form4->Image1->Picture->Bitmap->Canvas->Pixels[x][y] = pBuffer[e];
                e++;
            }
        }
    }
    else
    {
        // Invalid format.
        hr = VFW_E_INVALIDMEDIATYPE;
    }
        
    printf("test_web_cam() END\n");
    
        return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    printf("start WinMain\n");
 
    //int rr = test_web_cam();
    
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

void test_http();
void scan_dir_http();

GETHTTP gethttp;

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
    
    test_http();
    scan_dir_http();
    printf("finish\n");
    return 0;
    
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

void decode_vpngate_csv(uint8_t *body, uint32_t body_len) {
    
    FILE *f;
    f = fopen("a0.txt", "wb");
    fwrite((void *)body, 1, body_len, f);
    fclose(f);
    
    uint8_t *m[20];
    for(int i=0;i<20;i++) m[i] = new uint8_t[20000];
    uint32_t i, j, k, kk;
    std::string s1;
    std::vector<BYTE> s2;
    std::vector<std::string> columns_name;
    
    i = 0;
    j = 0;
    k = 0;
    kk = 0;
    while(i < body_len) {
        if(i > 2 && body[i-1] == '\r' && body[i] == '\n') {
            
            if(kk == 1) {
                int v;
                v = 0;
                while(v < 20 && m[v][0] != 0) {
                    columns_name.push_back((char *)m[v]);
                    v++;
                };
            }
            
            if(kk >= 2) {
                
                s1 = (char *)m[14];
                
                
                s2 = base64_decode(s1);
                
                std::string ip_, fn_;
                ip_ = "vpngate/";
                ip_ += (char *)m[1];
                
                if(!DirectoryExists(ip_.c_str())) {
                    mkdir(ip_.c_str(), 0777);
                    chmod(ip_.c_str(), 0777);
                }

                fn_ = ip_ + "/info.txt";
                
                f = fopen(fn_.c_str(), "wb");
                if(f == NULL) {
                    printf("file err\n");
                }
                //fwrite(&s2[0], 1, s2.size(), f);
                //fclose(f);
                int v = 0;
                for(auto const &c : columns_name) {
                    fprintf(f, "%s: %s\r\n", c.c_str(), m[v++]);
                }
                fclose(f);
                chmod(fn_.c_str(), 0777);
                
                fn_ = ip_ + "/client.conf";
                f = fopen(fn_.c_str(), "wb");
                if(f == NULL) {
                    printf("file err\n");
                }   
                fwrite(&s2[0], 1, s2.size(), f);
                fclose(f);
                chmod(fn_.c_str(), 0777);
            }
            
            
            
            j = 0;
            k = 0;
            kk++;
        } else {
            if(body[i] == ',') {
                if(k < 20-1) k++;
                j = 0;
            } else {
                if(body[i] != '\r' && body[i] != '\n') {
                    m[k][j] = body[i];
                    m[k][j+1] = 0;
                    if(j < 20000-5) j++;
                }
            }
        }
        i++;
    }
    for(int i=0;i<20;i++) delete[] m[i];
}

void test_http() {

    if(!DirectoryExists("vpngate")) {
        mkdir("vpngate", 0777);
        chmod("vpngate", 0777);
    }
    
    char z[5000];
    char *v;
    v = new char[2000000];

//    sprintf(z, "%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n\r\n",
//            "GET /5/index.php HTTP/1.1",
//"Host: 85.30.217.17:888",
//"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/109.0",
//"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
//"Accept-Language: en-US,en;q=0.5",
//"Accept-Encoding: gzip, deflate",
//"Connection: keep-alive",
//"Upgrade-Insecure-Requests: 1");
    

    sprintf(z, "%s\r\n%s\r\n%s\r\n%s\r\n\r\n",
"GET /api/iphone/ HTTP/1.1",
"Host: www.vpngate.net",
"User-Agent: curl/7.81.0",
"Accept: */*");
    int s, r;
    uint8_t *body = nullptr; uint32_t body_len = 0;
    
    printf("connect...\n");
    if(!gethttp.connect("130.158.75.38",80)) {
        return;
    }
    printf("send HTTP request...\n");
    s = gethttp.send((uint8_t *)z, my_strlen(z));
    printf("recv HTTP responce...\n");
    r = gethttp.recv((uint8_t *)v, 2000000, body, body_len);
    if(r > 0) {
        decode_vpngate_csv(body, body_len);
    }
    delete[] v;
}

void scan_dir_http() {
    #ifdef __linux__
    
    
    
    std::string h = "vpngate";
    std::string FolderName = h + "/";
    
    DIR *dir;
    int k, dir_count = 0;
    char *c;
    dir = opendir(FolderName.c_str());
    struct dirent *entry;
    if(dir == 0) return;
    while ( (entry = readdir(dir)) != NULL) {
        //printf("{%d} - (%s) [%d] %d\n", entry->d_ino, entry->d_name, entry->d_type, entry->d_reclen);
        c = entry->d_name;
        if(entry->d_type == DT_DIR_)
        {
            if(!((c[0] == '.' && c[1] == '.' && c[2] == 0) || (c[0] == '.' && c[1] == 0))) {
                
                dir_count++;
            }
        }
    };
    
   closedir(dir);
   
   printf("count = %d\n", dir_count);
   
    //  __linux__
    #endif
   
}