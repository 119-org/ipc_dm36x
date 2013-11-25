/*************************************************
Copyright (C), 2001-2012, TMVIDEO. Co., Ltd.
File name: // �ļ���
Author: Version: Date: ���Ƹ� 2012-03-07 // ���ߡ��汾���������
Description: // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
// �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
Others: // �������ݵ�˵��
Function List: // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
History: // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸�
���ݼ���
Date:
Author:
Modification:
*************************************************/
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <linux/route.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>

#ifndef _TM_UPGRADE_H__
#define _TM_UPGRADE_H__



/***********Kernel Flash�����ռ��ļ�*********************/
#define MTD_KERNEL_UPDATE_DEV				"/dev/mtdblock1"

/***********Rootfs Flash�����ռ��ļ�*********************/
#define MTD_ROOTFS_UPDATE_DEV				"/dev/mtdblock2"

/***********Ӧ�ó��� Flash�����ռ��ļ�*********************/
#define MTD_APP_UPDATE_DEV				"/dev/mtdblock3"

/***********������ Flash�����ռ��ļ�*********************/
#define MTD_PARAMETER_UPDATE_DEV				"/dev/mtdblock4"



/******
�Ƿ��"д"FLASH���ܣ���������±���򿪣�
�ö�����Ҫ��������ģ�鹦�ܵ��ԡ�
********/
#define D_WRITE_FLASH_TASK			1

/******
�Ƿ��"��"FLASH���ܣ���������±���رգ�
�ö�����Ҫ��������ģ�鹦�ܵ��ԡ�
******/
#define D_READ_FLASH_TEST			0

/***
����򿪶�����ȡFLASH������ļ������壬
���ڵ���
***/
#define D_READ_FLASH_SAVE_FILENAME	"read_rootfs5.jffs2"
/***
����򿪶�����ȡFLASH������ļ��Ĵ�С��
���ڵ���
***/
#define D_READ_FLASH_FILE_SZIE      0x300000
/***
���������ļ���
���ڵ���
***/
#define UPDATE_FILE_NAME				"read_rootfs1_ok.jffs2"

/***
����TCP\UDP�˿ڣ��̶�Ϊ60001
***/
#define UPDATE_TCP_UDP_LISTEN_PORT			60001


#define		SOCK_SNDRCV_LEN		(1024*32)

#define     SOCK_TIME_OVER      10


#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef LPVOID
#define LPVOID void *
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef True
#define True	1
#endif

#ifndef TRUE
#define TRUE	1
#endif


#ifndef False
#define False	0
#endif

#ifndef FALSE
#define FALSE	0
#endif



/***
�������ݼ��
***/
typedef enum _file_upgrade_datacheck_enum
{
/***�����ļ���ͷЧ�����***/
		FILE_UPGRADE_DATACHECK_OK=0	,			//������ȷ
		FILE_UPGRADE_DATACHECK_BUFFER_ERR,		//bufferΪNULL����
		FILE_UPGRADE_DATACHECK_FLAG_ERR,		//FLAG����
		FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR,	//�������ʹ���
		FILE_UPGRADE_DATACHECK_CPU_TYPE_ERR ,	//CPU���ʹ���
		FILE_UPGRADE_DATACHECK_VIDEOCOUNT_ERR ,	//��Ƶͨ������
		FILE_UPGRADE_DATACHECK_VERSION_ERR ,	//�汾����
		FILE_UPGRADE_DATACHECK_DATALEN_ERR ,	//����ʵ�ʳ��ȴ���
		FILE_UPGRADE_DATACHECK_CRC_ERR ,		//CRC����Ч�����
/***�����ļ���д״̬���***/		
		FILE_UPGRADE_OPEN_FAILURE ,				//���ļ�ʧ��
		FILE_UPGRADE_COUNTLENGHT_FAILURE ,		//���ļ��ܳ���С��������ͷ160���ֽ�
		FILE_UPGRADE_MALLOC_MEMERY_FAILURE ,	//����ϵͳ�ڴ�ʧ��
		FILE_UPGRADE_USBDISK_DONTFIND ,			//��U����δ�ҵ������ļ�
/***��дFlash״̬���***/	
		FLASH_UPGRADE_OK	,				//FLASHд������,�����ɹ�
		FLASH_UPGRADE_OPEN_DEVICE_FAIL,		//��FLASH�豸ʧ��
		FLASH_UPGRADE_WRITE_DATA_FAIL,		//д�����ݵ�FLASH�豸ʧ��
		FLASH_UPGRADE_READ_DATA_FAIL,		//��ȡ���ݵ�FLASH�豸ʧ��
		FLASH_UPGRADE_DATA_SIZE_ERR	,		//����д�����ݴ�С����
/***��������״̬���***/
		NETWORK_STATUS_CHECK_OK ,					//��������״̬����
		NETWORK_STATUS_CHECK_BLOCKE_ERROR ,			//�����������ְ�����
		NETWORK_STATUS_CHECK_MALLOC_MEMERY_FAILURE , //������������ϵͳ�ڴ�ʧ��
		NETWORK_STATUS_RECV_ERROR                  //�����������ݽ����쳣


} FILE_UPGRADE_DATACHECK_E;









#define upgrade_err()\
{             \
	printf(" %s(%d) - Line %d , %s \n", __FILE__, __LINE__, errno, strerror(errno));\
}

//������ͷ160���ֽ�
typedef struct tagTMUPDATAFILE_HEADER
{
	DWORD	dwFlag;			//1.������ʶͷ"tmup",
	DWORD	dwDataCRC;		//2.CRCͷ�ļ�Ч��
	DWORD   dwCPUType;	    //3.������CPU���ͣ���˼HI3515\HI3520\HI3530\HI3531
	DWORD	dwImageType;    //4.Image���ͣ������ںˡ�����rootfs�ļ�ϵͳ
	DWORD	dwChCount;	    //5.��������Ƶͨ������	
	DWORD	dwServerType;   //6.����������
	DWORD	dwMainVer;		//7.�����ļ����汾��,1-8
	DWORD	dwChildVer;		//8.�����ļ��Ӱ汾��,1-10
	DWORD	dwModityVer;	//9.�����ļ��޸ı�ʶ�汾��,��д�ֶ���д��1-99
	DWORD	dwRawWriteFileLen;   //10.����д���ļ�����
	char	ReserveBuf[120];  //��Ϊ����������Ϣ��Ԥ���ֽڣ�Ϊ�˷�����չ
	
}TMUPDATAFILE_HEADER,*PTMUPDATAFILE_HEADER;


//�������ṹ
typedef struct tagTMUPDATANET_SERVER_INFO
{
	int					hTcpListenSock;		//TCP SOCKET
	int					hUdpPortSock;		//UDP SOCKET
	struct sockaddr_in	Udpaddr;
	
	
}TMUPDATANET_SERVER_INFO,*PTMUPDATANET_SERVER_INFO;






//����SDK������Ϣ�ṹ,��־ǿ����2012��3��8��
#define		NET_FLAG 0x64766d74

#define		BLOCK_NETWORK_UPDATA_BLOCKSIZE	1024  //��������3G,MTUֵ��ƣ�����16�ֽ�ͷ��������������Ϊ1040���ֽ�

#define		BLOCK_NETWORK_UPDATA_COUNTBLOCKSIZE	1040  //��������3G,MTUֵ��ƣ�����16�ֽ�ͷ��������������Ϊ1040���ֽ�		

#define		NETCMD_USBUPDATA_START		0x00A0		//����UI-->������������
													//���������USB������U��·��Ϊ/mnt/usb_disk

#define		NETCMD_USBUPDATA_RETURN		0x00A1		//����������-->����UI��
													//�ظ�UI�������������ʼ��������

#define		NETCMD_USBUPDATA_FAILURE	0x00A2		//����������-->����UI��
													//�ظ�UI���������������ʧ��

#define		NETCMD_USBUPDATA_SUCCESS	0x00A3		//����������-->����UI��
													//�ظ�UI����������������ɹ�


#define		NETCMD_NETWORK_UPDATA_START	0x00B0	//����-->������������
												//�������������������������׼�������������ݰ�
												//��дdwReservedΪ�ְ�����
												//��дdwLengthΪ�����������ݳ���

#define		NETCMD_NETWORK_UPDATA_RETURN	0x00B1	//����������-->����
													//�ظ������������������������ʼ�����������ݰ�
													//dwReserved��дΪ�ְ�����
													//dwLength��дΪ���������ݴ�С

#define		NETCMD_NETWORK_UPDATA_PACKET	0x00B2	//����-->����������
													//�ظ������������������������ʼ�����������ݰ�
													//��дdwReservedΪ�ְ���ţ���0��ʼ����
													//��дdwLengthΪ�����ְ����ݳ���

#define		NETCMD_NETWORK_UPDATA_MESSAGE	0x00B3	//����������-->����
													//�ظ������������������������ʼ�����������ݰ�
													//dwReserved��дΪ��Ϣ
													//

#define		NETCMD_NETWORK_UPDATA_PROGRESS	0x00B4	//����������-->����
													//�ظ������������������������ʼ�����������ݰ�
													//��дdwReservedΪ0-100д�����,100�������дFLASH

typedef struct tagNET_MESSAGE
{
	DWORD dwNetFlag;
	DWORD dwCMD;
	DWORD dwReserved;
	DWORD dwLength;
	char  lpData[];
}NET_MESSAGE,*LPNET_MESSAGE;


//˽�ò���
#define UPDATE_TCP_BUFFER_MESSAGE_NUMBER	10


typedef struct tagTCP_SEND_NET_MESSAGE
{
	BOOL bUse;
	int hConSock;
	NET_MESSAGE szMsg;
}TCP_SEND_NET_MESSAGE,*LTCP_SEND_NET_MESSAGE;


typedef struct tagNET_listen_ret_t
{
	struct sockaddr_in			client_addr;
	int					accept_sock;
	int					resverd;
}listen_ret_t, *plisten_ret_t;

/***********������ʶͷ"tmup"**************/
#define UPDATA_D_FLAG  0x746d7570

/***********������CPU����**************/
#define UPDATA_D_HI3515  0x00001001
#define UPDATA_D_HI3520  0x00001002
#define UPDATA_D_HI3530  0x00001003
#define UPDATA_D_HI3531  0x00001004
#define UPDATA_D_CPUUNKOWN  0x00001010

/***********����Image����**************/
#define UPDATA_D_ROOTFS  0x00001101    //�����ļ�ϵͳ
#define UPDATA_D_KERNEL  0x00001102	   //�����ں�ϵͳ
#define UPDATA_D_APPAREA  0x00001103	   //Ӧ�ó���ϵͳ
#define UPDATA_D_IMAGEUNKOWN  0x00001110	   //δ֪�ļ�

/***********��������Ƶͨ������**************/
#define UPDATA_D_4CHANNEL  4      //4·
#define UPDATA_D_5CHANNEL  5	   //5·
#define UPDATA_D_6CHANNEL  6	   //6·
#define UPDATA_D_7CHANNEL  7	   //7·
#define UPDATA_D_8CHANNEL  8	   //8·
#define UPDATA_D_CHANNELUNKOWN  40	   //δ֪ͨ��

/***********����������**************/
#define UPDATA_D_BASETYPE  0x00001301      //�����汾






/*************************************************
Function: Rootfs_UpdateFlash(int size, char *buffer)
Description: ��FLASH�ļ�д�����ݡ�
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����12M Rootfs�ռ����
author: ���Ƹ� 2012-03-07
*************************************************/

int Rootfs_UpdateFlash(int size, char *buffer);



/*************************************************
Function: Kernel_UpdateFlash(int size, char *buffer)
Description: ��Kernel ���� FLASH�ļ�д�����ݡ�
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����2M Kernel�ռ����
author: ���Ƹ� 2012-03-07
*************************************************/
int Kernel_UpdateFlash(int size, char *buffer);


/*************************************************
Function: App_UpdateFlash(int size, char *buffer)
Description: ��Kernel ���� FLASH�ļ�д�����ݡ�
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����2M Kernel�ռ����
author: ���Ƹ� 2012-03-07
*************************************************/
int App_UpdateFlash(int size, char *buffer);


/*************************************************
Function: Read_Flash_Data(int nReadsize , char *buffer , char *dev)
Description: ��ȡ FLASH�ļ�д�����ݡ�
Input: int nReadsize Ϊ��ȡ�ļ���С, char *buffer ��ȡ�ļ����� ��
char *dev ��ȡ�豸 ,char *savename �����ļ���
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����Զ�̵������
author: ���Ƹ� 2012-03-07
*************************************************/
int Read_Flash_Data(int nReadsize , char *buffer , char *dev , char *savename);

/*************************************************
Function: mount_mtdblock4_Parameter()
Description: ����Ӧ�ó��򡢲�����
Input: 
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: 
author: ���Ƹ� 2012-03-07
*************************************************/
//���ú������غ����������ڶ�ȡ�����ļ�ֱ�ӵ���
void mount_mtdblock_Parameter();

/*************************************************
Function: Check_Upgrade_Data_Whether_Correct(TMUPDATAFILE_HEADER *updatakitheader , 
unsigned char *UpgradeBuf ,int  Upgradelen )   
Description: ��ѡ�������ļ�������Ч��
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int Check_Upgrade_Data_Whether_Correct(TMUPDATAFILE_HEADER *updatakitheader , \
									   unsigned char *UpgradeBuf ,	\
									   int  Upgradelen);



/*************************************************
Function: int USBDisk_Auto_UpdateFlash()
Description: carDVR���ؽ�����򣬷����Զ�������Ϣ���Զ�Ѱ�������ļ���������
Input: char *UpgradePath �����ļ�����·�� /mnt/usb_disk/*.hibom.auto
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: 
author: ���Ƹ� 2012-03-07
*************************************************/
int USBDisk_Auto_UpdateFlash();


/*************************************************
Function: SendWriteFlashMsg(DWORD nWirteLen , DWORD nCorrentLen)
Description: ����дFLASH��Ϣ����,
DWORD nWirteLen дFLASH���� DWORD nCorrentLen ��ǰд��������
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void SendWriteFlashMsg(DWORD nWirteLen , DWORD nCorrentLen);


#endif



