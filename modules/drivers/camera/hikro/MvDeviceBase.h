
#ifndef __MV_DEVICE_BASE_H__
#define __MV_DEVICE_BASE_H__

#include "MvInclude.h"

namespace MvCamCtrl
{

    interface   IMvDevice
    {

		    // en:Open Device
        virtual int     Open(unsigned int nAccessMode = MV_ACCESS_Exclusive, unsigned short nSwitchoverKey = 0)    = 0;


		    // en:Close Device
        virtual int     Close()                                 = 0;

		    // en:Determines the status of the device, which can be opened only if false is returned
        virtual bool    IsOpen()                                = 0;


		    // en:Start Grabbing
        virtual int     StartGrabbing()                         = 0;


		    // en:Stop Grabbing
        virtual int     StopGrabbing()                          = 0;


		    // en:Get Device Information
        virtual int     GetDeviceInfo(MV_CC_DEVICE_INFO&)       = 0;


         /** @fn     GetGenICamXML(unsigned char* pData, unsigned int nDataSize, unsigned int* pnDataLen)
	    	 *  @fn     GetGenICamXML(unsigned char* pData, unsigned int nDataSize, unsigned int* pnDataLen)
         *  @brief  Get the device XML file
         *  @param  pData           [IN][OUT]   - Cache Address to copy in
                    nDataSize       [IN]        - Cache Size
                    pnDataLen       [OUT]       - xml File Data Length
         *  
         *  @return Success, return MV_OK; Fail, return error code
         *  @note   When pData is NULL or nDataSize smaller than the actual xml file, do not copy the data, return xml file size from pnDataLen;
         *          When pData is a valid cache address, and the cache is large enough, the full data is copied and the xml file size is returned by pnDataLen.
         */
        virtual int     GetGenICamXML(unsigned char* pData, unsigned int nDataSize, unsigned int* pnDataLen)        = 0;


        /** @fn     GetOneFrame(unsigned char * pData , unsigned int nDataSize, MV_FRAME_OUT_INFO* pFrameInfo)
         *  @brief  ��ȡһ֡ͼ������
         *  @param  pData           [IN][OUT]   - ����ָ��
                    nDataLen        [IN]        - ���ݳ���
                    pFrameInfo      [OUT]       - �����֡��Ϣ
         *  
         *  @return �ɹ�������MV_OK��ʧ�ܣ����ش�����
		 
		 * @fn     GetOneFrame(unsigned char * pData , unsigned int nDataSize, MV_FRAME_OUT_INFO* pFrameInfo)
         *  @brief  Get one frame image data
         *  @param  pData           [IN][OUT]   - Data Pointer
                    nDataLen        [IN]        - Data Length
                    pFrameInfo      [OUT]       - Output Frame Information
         *  
         *  @return Success, return MV_OK; Fail, return error code
         */
        virtual int     GetOneFrame(unsigned char * pData , unsigned int nDataSize, MV_FRAME_OUT_INFO* pFrameInfo)  = 0;


        // ch:��ȡGenICamʹ�õĴ���������
		// en:Gets the transport layer agent class used by GenICam
        virtual TlProxy     GetTlProxy()                                                                            = 0;

        virtual ~IMvDevice( void ){};
    };


    interface   IDeviceFactory
    {

        // ch:ö�������ڣ�ָ���Ĵ���Э���Ӧ�������豸
		// en:Enumerate all devices within the subnet that correspond to the specified transport protocol
        virtual int EnumDevices( unsigned int nTLayerType , MV_CC_DEVICE_INFO_LIST& stDevList )     = 0;


        // ch:�����豸������
		// en:Create a device agent class
        virtual IMvDevice* CreateDevice( const MV_CC_DEVICE_INFO& device )                          = 0;


        // ch:����ָ���豸���ڲ���Դ
		// en:Destroy the internal resources of the specified device
        virtual int DestroyDevice( IMvDevice* )                                                     = 0;


        // ch:�ж�ָ�����豸�Ƿ���Է���
		// en:Determines whether the specified device is accessible
        virtual bool IsDeviceAccessible( const MV_CC_DEVICE_INFO& deviceInfo)                       = 0;
    };

    

}

#endif /* __MV_DEVICE_BASE_H__ */