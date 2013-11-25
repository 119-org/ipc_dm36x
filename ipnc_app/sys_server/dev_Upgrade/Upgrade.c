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
//�������Զ�����U��·��Ϊ/mnt/usb_disk/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <termios.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <pthread.h>
#include <linux/sem.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include   <netinet/tcp.h>   //   for   TCP_NODELAY 
#include <netdb.h>
#include <arpa/inet.h>
#include <linux/route.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <dirent.h>
#include <mntent.h>
#include <signal.h>

#include "dev_Upgrade.h"

//��������������ȫ�ֱ����ṹ
TMUPDATANET_SERVER_INFO	g_Upgrade_ServerInfo;
//TCP��Ϣ����
TCP_SEND_NET_MESSAGE    g_Tcp_Send_Msg[UPDATE_TCP_BUFFER_MESSAGE_NUMBER];
//TCPдFlash����
TCP_SEND_NET_MESSAGE    g_FlashWrit_Progress_Msg;
//TCP��Ϣ�����߳���
pthread_mutex_t hMsgBufferMutex;
//ʹ��ȫ�ֱ�����¼�����ļ�����
DWORD g_Check_Updata_Type;

int main()
{
	int ret;
	printf("******************************************\n");
	printf("*********LiuWenGao 2012-03-08-Write*******\n");
	printf("*****IPC Upgrade Server******\n");
	printf("******************************************\n\n");

	g_Check_Updata_Type = 0;
	
	printf("2-Upgrade Server Start ....\n");
	//��ʼ��ȫ�ֱ���
	memset(&g_Upgrade_ServerInfo , 0 , sizeof(TMUPDATANET_SERVER_INFO));
	memset(g_Tcp_Send_Msg , 0 , (sizeof(TCP_SEND_NET_MESSAGE)*10));
	memset(&g_FlashWrit_Progress_Msg , 0 , sizeof(TCP_SEND_NET_MESSAGE));
	//�ȴ�15�룬ϵͳ����ȫ�����
	sleep(15);
	//����1˵�����������ļ�������������,��Ҫ����ϵͳ������0˵��û�з��������ļ�
	ret = Auto_Temp_Updata_File();
	if (ret==1)
	{
		//��������
		Restart_Upgrader_System();
	}
	printf("No Find Updata ....\n");
	
	//��ʼ����Ϣ�����߳���
	ret=pthread_mutex_init(&hMsgBufferMutex , NULL);
	if(ret!=0)
	{
		upgrade_err();
		return 0;
	}
	//����Ӧ�ó�������������
	//mount_mtdblock_Parameter();

	//����������
	//system("/app/./carDVR &");

	printf("3-Upgrade Server Listhing ....\n");
	Upgrade_Net_ServerStart();
	printf("4-Upgrade Server Create Complete ....\n");
	


	while(1)
	{
		printf("Upgrade Server Running ....\n");
		sleep(120);
	}


#if 0
//���Դ���
/**********
	int filecountlenght=0;
	int fpwrlen=0;
	FILE *fp=NULL;
	fp=fopen(UPDATE_FILE_NAME,"rb");
	if (fp==NULL)
	{
		printf("open Upgrad file fail [%s] \n" , UPDATE_FILE_NAME);
		goto finish;
	}
	//��ȡ�����ļ�����	
	//SEEK_SET�� �ļ���ͷ ����SEEK_CUR�� ��ǰλ�� ����SEEK_END�� �ļ���β 
	fseek(fp , 0L, SEEK_END);
	filecountlenght=ftell(fp);
	fseek(fp , 0L, SEEK_SET);
	printf("open file lenght [%d]\n" , filecountlenght);
	//�����ڴ�
	unsigned char *buffer=NULL;
	buffer = (unsigned char *)malloc(filecountlenght+1);
	if (buffer == NULL)
	{
		printf("get memery fail\n");
		goto finish;
	}
	memset(buffer , 0xFF , filecountlenght);
	//��ȡ�ļ����������ݵ��ڴ���
	fpwrlen=fread(buffer,filecountlenght,1,fp);//���ַ������ݶ�ȡ���ļ� 
	if (fpwrlen!=1)
	{
		printf("fread error fpwrlen [%d]\n" , fpwrlen);
		goto finish;
	}
	printf("fread success fpwrlen [%d]\n" , fpwrlen);
	//д��FLASH����
	Rootfs_UpdateFlash(filecountlenght , buffer);
	
finish :

	printf("program complete .....\n");
	if (buffer != NULL)
	{
		free(buffer);
		buffer=NULL;
	}
	if (fp!=NULL)
	{
		fclose(fp);
	}
*******/
#endif
	return 0;
}


/*************************************************
Function: void Check_Block_Flash_Eraseall( ) 
Description: ���������ʽ�������������Ϊ�ڴ汻ռ�ã�����system������Ч�����
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: 
author: ���Ƹ� 2013-01-07
*************************************************/
void Check_Block_Flash_Eraseall( )  
{ 

	sync();
	system("killall -9 ipcam");
	system("killall -9 av_server.out");

	if (g_Check_Updata_Type==UPDATA_D_MTD_FACTORY)
	{
		sync();
		system("/usr/sbin/flash_eraseall /dev/mtd4 ");
	}
	else if (g_Check_Updata_Type==UPDATA_D_APPAREA)
	{
		sync();
		system("/usr/sbin/flash_eraseall /dev/mtd5 ");
	}
	else if (g_Check_Updata_Type==UPDATA_D_MTD_DEV)
	{
		sync();
		system("/usr/sbin/flash_eraseall /dev/mtd6 ");
	}
	else if (g_Check_Updata_Type==UPDATA_D_MTD_LIB)
	{
		sync();
		system("/usr/sbin/flash_eraseall /dev/mtd7 ");
	}
	else if (g_Check_Updata_Type==UPDATA_D_MTD_ALL_THREE)
	{
		sync();
		system("/usr/sbin/flash_eraseall /dev/mtd5 ");
		sync();
		system("/usr/sbin/flash_eraseall /dev/mtd6 ");
		sync();
		system("/usr/sbin/flash_eraseall /dev/mtd7 ");
		sync();
	}
	else
	{
		printf("Error g_Check_Updata_Type \n");
	}

    
}  




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
void mount_mtdblock_Parameter()
{
	int err_msg;	
	unsigned char szCheck_mtdblock[64*1024];  //���ٶ�ȡ64K
	/******************����mtdblock3******************/
	memset(szCheck_mtdblock , 0 , sizeof(szCheck_mtdblock));
	//��ȡ������ԭʼFLASH����
	err_msg=Read_Flash_Data(64*1024 , szCheck_mtdblock , "/dev/mtdblock3", NULL);
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock3 error err_msg [%d]\n" , err_msg);
		return ;
	}
	//����Ӧ�ó����ļ���
	system("mkdir /app");
	system("sync");
	//���������Ƿ��Ѿ�����jffs2����
	//0x85 0x19
	//����ǰ��2������ͷ
	if (szCheck_mtdblock[0]==0x85&&	\
		szCheck_mtdblock[1]==0x19)
	{
		//�Ѿ�������ֱ�ӽ��й���
		system("mount -t jffs2 /dev/mtdblock3 /app");
		system("sync");
	}
	else
	{
		//û�з��ֱ���ʽ�������ȸ�ʽ��
		system("flash_eraseall -j /dev/mtd3");
		system("sync");
		system("mount -t jffs2 /dev/mtdblock3 /app");
		system("sync");
	}	
		
	/******************����mtdblock4******************/
	memset(szCheck_mtdblock , 0 , sizeof(szCheck_mtdblock));
	//��ȡ������ԭʼFLASH����
	err_msg=Read_Flash_Data(64*1024 , szCheck_mtdblock , "/dev/mtdblock4", NULL);
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock4 error err_msg [%d]\n" , err_msg);
		return ;
	}
	//���������ļ���
	system("mkdir /param");
	system("sync");
	//���������Ƿ��Ѿ�����jffs2����
	//0x85 0x19
	//����ǰ��2������ͷ
	if (szCheck_mtdblock[0]==0x85&&	\
		szCheck_mtdblock[1]==0x19)
	{
		//�Ѿ�������ֱ�ӽ��й���
		system("mount -t jffs2 /dev/mtdblock4 /param");
		system("sync");
	}
	else
	{
		//û�з��ֱ���ʽ�������ȸ�ʽ��
		system("flash_eraseall -j /dev/mtd4");
		system("sync");
		system("mount -t jffs2 /dev/mtdblock4 /param");
		system("sync");
	}
	return ;
}
	


/*************************************************
Function: Network_UpdateFile_Crc(char *UpgradeBuffer , int nSize)
Description: ��������CRCЧ��
Input: char *UpgradeBuffer Ϊ��������������, int nSize ���������ݳ��Ⱥ���ͷ����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: 
author: ���Ƹ� 2012-03-07
*************************************************/
int Network_UpdateFile_Crc(char *UpgradeBuffer , int nSize)
{
	int UsbUpdataErr;
	int fileUpgradelenght;
	TMUPDATAFILE_HEADER *updatakitheader=NULL;
	fileUpgradelenght = nSize - sizeof(TMUPDATAFILE_HEADER);
	/********����������ͷ********/
	updatakitheader=(TMUPDATAFILE_HEADER *)UpgradeBuffer;
	if (updatakitheader==NULL)
	{
		printf("Malloc Memery failure !\n");
		UsbUpdataErr = FILE_UPGRADE_MALLOC_MEMERY_FAILURE;
		goto ErrDispose;
	}
	//����ļ������Ƿ�����
	if (nSize < sizeof(TMUPDATAFILE_HEADER))
	{
		printf("open upgrade failure !\n");
		UsbUpdataErr = FILE_UPGRADE_COUNTLENGHT_FAILURE;
		goto ErrDispose;
	}
	
	//���������Ƿ�������CRCЧ���Ƿ���ȷ
	UsbUpdataErr=Check_Upgrade_Data_Whether_Correct(updatakitheader , UpgradeBuffer+sizeof(TMUPDATAFILE_HEADER) , fileUpgradelenght);
	if (UsbUpdataErr!=FILE_UPGRADE_DATACHECK_OK)
	{
		printf("Upgrade Data Check Error !\n");
		goto ErrDispose;
	}

ErrDispose :

	return UsbUpdataErr;

}





/*************************************************
Function: Network_UpdateFlash(char *UpgradeBuffer , int nSize)
Description: �����������
Input: char *UpgradeBuffer Ϊ��������������, int nSize ���������ݳ��Ⱥ���ͷ����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: 
author: ���Ƹ� 2012-03-07
*************************************************/
int Network_UpdateFlash(char *UpgradeBuffer , int nSize)
{
	int UsbUpdataErr;
	int fileUpgradelenght;
	TMUPDATAFILE_HEADER *updatakitheader=NULL;
	fileUpgradelenght = nSize - sizeof(TMUPDATAFILE_HEADER);
	/********����������ͷ********/
	updatakitheader=(TMUPDATAFILE_HEADER *)UpgradeBuffer;
	if (updatakitheader==NULL)
	{
		printf("Malloc Memery failure !\n");
		UsbUpdataErr = FILE_UPGRADE_MALLOC_MEMERY_FAILURE;
		goto ErrDispose;
	}
	//�����������̣���Ҫ֪ͨ������ֹͣ���Ź���Ȼ��KILL������

	//��¼�����ļ�����
	g_Check_Updata_Type = updatakitheader->dwImageType;
	//APP\DEV\LIB������ͳһ����
	if (updatakitheader->dwImageType==UPDATA_D_MTD_ALL_THREE)
	{
		UsbUpdataErr = SYS_All_Three_block(fileUpgradelenght ,  UpgradeBuffer+sizeof(TMUPDATAFILE_HEADER));
	}
	


	//�ں�
	if (updatakitheader->dwImageType==UPDATA_D_KERNEL)
	{
		UsbUpdataErr = Kernel_UpdateFlash(fileUpgradelenght ,  UpgradeBuffer+sizeof(TMUPDATAFILE_HEADER));
	}


	//�ļ�ϵͳ
	if (updatakitheader->dwImageType==UPDATA_D_ROOTFS)
	{
		UsbUpdataErr = Rootfs_UpdateFlash(fileUpgradelenght ,  UpgradeBuffer+sizeof(TMUPDATAFILE_HEADER));
	}
	
	//�ں�
	if (updatakitheader->dwImageType==UPDATA_D_KERNEL)
	{
		UsbUpdataErr = Kernel_UpdateFlash(fileUpgradelenght ,  UpgradeBuffer+sizeof(TMUPDATAFILE_HEADER));
	}

	//Ӧ����
	if (updatakitheader->dwImageType==UPDATA_D_APPAREA)
	{
		UsbUpdataErr = App_UpdateFlash(fileUpgradelenght ,  UpgradeBuffer+sizeof(TMUPDATAFILE_HEADER));
	}

	//�豸������

	if (updatakitheader->dwImageType==UPDATA_D_MTD_DEV)
	{
		UsbUpdataErr = SYS_Dev_Flash(fileUpgradelenght ,  UpgradeBuffer+sizeof(TMUPDATAFILE_HEADER));
	}
	
	//���п���
	if (updatakitheader->dwImageType==UPDATA_D_MTD_LIB)
	{
		UsbUpdataErr = SYS_Dev_LIB(fileUpgradelenght ,  UpgradeBuffer+sizeof(TMUPDATAFILE_HEADER));
	}

	//��������������
	if (updatakitheader->dwImageType==UPDATA_D_MTD_FACTORY)
	{
		UsbUpdataErr = SYS_Dev_PARAMETER(fileUpgradelenght ,  UpgradeBuffer+sizeof(TMUPDATAFILE_HEADER));
	}


	
	
ErrDispose :
	
	return UsbUpdataErr;
	
}









/*************************************************
Function: USBDisk_Manual_UpdateFlash(char *UpgradePath)
Description: carDVR���ؽ�����򣬷���U�������ļ�����·������������
Input: char *UpgradePath �����ļ�����·�� /mnt/usb_disk/*.hibom
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: 
author: ���Ƹ� 2012-03-07
*************************************************/
int USBDisk_Manual_UpdateFlash(char *UpgradePath)
{
	int UsbUpdataErr;
	unsigned char *UpgradeBuf = NULL;
	TMUPDATAFILE_HEADER *updatakitheader=NULL;
	/********����������ͷ********/
	updatakitheader=(TMUPDATAFILE_HEADER *)malloc(sizeof(TMUPDATAFILE_HEADER));
	if (updatakitheader==NULL)
	{
		printf("Malloc Memery failure !\n");
		UsbUpdataErr = FILE_UPGRADE_MALLOC_MEMERY_FAILURE;
		goto ErrDispose;
	}
	memset(updatakitheader , 0 , sizeof(TMUPDATAFILE_HEADER));
	/********���ļ�********/
	FILE *fp = NULL;
	int filecountlenght=0 , fileUpgradelenght=0;
	fp = fopen(UpgradePath , "rb");
	if (fp == NULL)
	{
		printf("open upgrade failure ! [%s]\n" , UpgradePath);
		UsbUpdataErr = FILE_UPGRADE_OPEN_FAILURE;
		goto ErrDispose;
	}
	//��ȡ�����ļ�����
	/*
	SEEK_SET�� �ļ���ͷ ����SEEK_CUR�� ��ǰλ�� ����SEEK_END�� �ļ���β 
	*/
	fseek(fp , 0L, SEEK_END);
	filecountlenght=ftell(fp);
	fseek(fp , 0L, SEEK_SET);
	printf("open upgrade file lenght [%d]\n" , filecountlenght);
	//����ļ������Ƿ�����
	if (filecountlenght < sizeof(TMUPDATAFILE_HEADER))
	{
		printf("open upgrade failure !\n");
		UsbUpdataErr = FILE_UPGRADE_COUNTLENGHT_FAILURE;
		goto ErrDispose;
	}
	//�����������ݳ���,���ҷ����ڴ�
	fileUpgradelenght = filecountlenght - sizeof(TMUPDATAFILE_HEADER);
	UpgradeBuf=(unsigned char *)malloc(fileUpgradelenght+1);
	if (UpgradeBuf==NULL)
	{
		printf("Malloc Memery failure !\n");
		UsbUpdataErr = FILE_UPGRADE_MALLOC_MEMERY_FAILURE;
		goto ErrDispose;
	}

	//��ȡ����ͷ
	fread(updatakitheader ,  sizeof(TMUPDATAFILE_HEADER) , 1 , fp);
	//��ȡ��������
	fread(UpgradeBuf , fileUpgradelenght , 1 , fp);

	//���������Ƿ�������CRCЧ���Ƿ���ȷ

	UsbUpdataErr=Check_Upgrade_Data_Whether_Correct(updatakitheader , UpgradeBuf , fileUpgradelenght);
	if (UsbUpdataErr!=FILE_UPGRADE_DATACHECK_OK)
	{
		printf("Upgrade Data Check Error !\n");
		goto ErrDispose;
	}

	//�����������̣���Ҫ֪ͨ������ֹͣ���Ź���Ȼ��KILL������
	if (updatakitheader->dwImageType==UPDATA_D_ROOTFS)
	{
		UsbUpdataErr = Rootfs_UpdateFlash(fileUpgradelenght , UpgradeBuf);
	}

	if (updatakitheader->dwImageType==UPDATA_D_KERNEL)
	{
		UsbUpdataErr = Kernel_UpdateFlash(fileUpgradelenght , UpgradeBuf);
	}

	
	if (updatakitheader->dwImageType==UPDATA_D_APPAREA)
	{
		UsbUpdataErr = App_UpdateFlash(fileUpgradelenght ,  UpgradeBuf);
	}
	

	//�豸������
	
	if (updatakitheader->dwImageType==UPDATA_D_MTD_DEV)
	{
		UsbUpdataErr = SYS_Dev_Flash(fileUpgradelenght ,  UpgradeBuf);
	}
	
	//���п���
	if (updatakitheader->dwImageType==UPDATA_D_MTD_LIB)
	{
		UsbUpdataErr = SYS_Dev_LIB(fileUpgradelenght ,  UpgradeBuf);
	}
	
	//��������������
	if (updatakitheader->dwImageType==UPDATA_D_MTD_FACTORY)
	{
		UsbUpdataErr = SYS_Dev_PARAMETER(fileUpgradelenght ,  UpgradeBuf);
	}

	

ErrDispose :

	//�ر��ļ�
	if (fp!=NULL)
	{
		fclose(fp);
	}
	//�ͷ�ͷ�ڴ�
	if (updatakitheader!=NULL)
	{
		free(updatakitheader);
	}
	//�ͷ����������ڴ�
	if (UpgradeBuf!=NULL)
	{
		free(UpgradeBuf);
	}

	return UsbUpdataErr;

}


/*************************************************
Function: ReadUSBDiskFileName(char *fileName,int close)
Description: ��ȡU�������е��ļ���
Input: char *UpgradePath �����ļ�����·�� /mnt/usb_disk/*.hibom.auto
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: 
author: ���Ƹ� 2012-03-07
*************************************************/
BOOL ReadUSBDiskFileName(char *fileName,int close , char *Usbdisk_dir)
{
	static DIR *dir = NULL;
	static struct dirent *entry = NULL;
	
	if (close == 1)
	{
		if (dir != NULL)
		{
			closedir(dir);
			dir = NULL;
		}
		return True;
	}
	
	if (dir == NULL)
	{
		dir = opendir(Usbdisk_dir);
	}
	if (dir == NULL)
		return False;
	
	entry = readdir(dir);
	
	if (entry != NULL)
	{
		strcpy(fileName,entry->d_name);
		return True;
	}
	else
	{
		closedir(dir);
		dir = NULL;
		entry = NULL;
		return False;
	}
}

/*************************************************
Function: int USBDisk_Auto_UpdateFlash()
Description: carDVR���ؽ�����򣬷����Զ�������Ϣ���Զ�Ѱ�������ļ���������
Input: char *UpgradePath �����ļ�����·�� /mnt/usb_disk/*.hibom.auto
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: 
author: ���Ƹ� 2012-03-07
*************************************************/
int USBDisk_Auto_UpdateFlash()
{
	char  fromRamName[256];
	char  Usbdisk_dir[256];
	char  Usbdisk_Upgrade_Path[256];
	unsigned int  bFind=0 , bFindTimes=0 ;
	int nUSBDiskUpgradeMsg ;
	memset(Usbdisk_dir , 0 , sizeof(Usbdisk_dir));
	sprintf(Usbdisk_dir , "%s" , "/mnt/usb_disk");
//�����һ�η����Ҳ��������ļ�����һ��
RestartFindTimes:

	while(1)
	{		
		sync();
		if(ReadUSBDiskFileName(fromRamName , 0 , Usbdisk_dir))
		{
			
			if (strstr(fromRamName,".hibom") != NULL)
			{
				//����U���е������ļ�
				bFind = 1;
				break;
			}
			else
			{
				continue;
			}
		}
		else
		{
			nUSBDiskUpgradeMsg = FILE_UPGRADE_USBDISK_DONTFIND;
			break;
		}
		usleep(10);
	}

	//���������ļ�
	if (bFind==1)
	{
		memset(Usbdisk_Upgrade_Path , 0 ,sizeof(Usbdisk_Upgrade_Path));
		sprintf(Usbdisk_Upgrade_Path , "%s%s" , "/mnt/usb_disk/" , fromRamName);
		nUSBDiskUpgradeMsg = USBDisk_Manual_UpdateFlash(Usbdisk_Upgrade_Path);
	}

	//���δ���������ļ���׺������һ��
	if (bFind != 1&&bFindTimes<1)
	{
		bFindTimes++;
		goto RestartFindTimes;
	}
	return nUSBDiskUpgradeMsg;
		
}



/*************************************************
Function: GetDataCRC(LPVOID lpData,DWORD dwSize) 
Description: ��ȡCRCЧ��ֵ
Input: ����ԭʼ�ļ�
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
author: ���Ƹ� 2012-03-07
*************************************************/

DWORD Check_GetDataCRC(LPVOID lpData,DWORD dwSize)
{
	//��ʼ��CRC��
	ULONG crc32_table[256];
	ULONG ulPolynomial = 0x04C11DB7;
	int i;
	for(i = 0; i <= 0xFF; i++)
	{
		ULONG value = 0;
		int ref = i;
		int n;
		for(n = 1; n < (8 + 1); n++)
		{
			if(ref & 1)
				value |= 1 << (8 - n);
			ref >>= 1;
		}
		crc32_table[i]= value << 24;
		int j;
		for (j = 0; j < 8; j++)
			crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
		value = 0;
		ref = crc32_table[i];
		for(n = 1; n < (32 + 1); n++)
		{
			if(ref & 1)
				value |= 1 << (32 - n);
			ref >>= 1;
		}
		crc32_table[i] = value;
	}
	//����CRCУ��ֵ
	ULONG  m_crc = -1;
	BYTE* buffer = (BYTE *)lpData;
	int len = dwSize;
	while(len--)
		m_crc = (m_crc >> 8) ^ crc32_table[(m_crc & 0xFF) ^ *buffer++];
	return m_crc^ 0xffffffff;
}



/*************************************************
Function: Check_Upgrade_Data_Whether_Correct(TMUPDATAFILE_HEADER *updatakitheader , \
unsigned char *UpgradeBuf ,int  Upgradelen )   
Description: ��ѡ�������ļ�������Ч��
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int Check_Upgrade_Data_Whether_Correct(TMUPDATAFILE_HEADER *updatakitheader , \
									   unsigned char *UpgradeBuf ,	\
									   int  Upgradelen) 
{
	FILE_UPGRADE_DATACHECK_E UpgradeErr;

	//Ĭ����������
	UpgradeErr = FILE_UPGRADE_DATACHECK_OK;
	//���ͷbuffer
	if (updatakitheader==NULL)
	{
		UpgradeErr = FILE_UPGRADE_DATACHECK_BUFFER_ERR;
		goto ErrDispose;
	}
	//�������buffer
	if (UpgradeBuf==NULL)
	{
		UpgradeErr = FILE_UPGRADE_DATACHECK_BUFFER_ERR;
		goto ErrDispose;
	}
	

	/********����ͷFLAG********/
	if (updatakitheader->dwFlag!=UPDATA_D_FLAG)
	{
		printf("FLAG Error:0x%x \n" , updatakitheader->dwFlag);
		UpgradeErr = FILE_UPGRADE_DATACHECK_FLAG_ERR;
		goto ErrDispose;
	}
	

#if 1
	/********�������Image����********/
	if (updatakitheader->dwImageType==UPDATA_D_ROOTFS)
	{
		if (UpgradeBuf[0]==0x45&&UpgradeBuf[1]==0x3D)
		{
			printf("Image Type Rootfs \n");
		}
		else
		{
			printf("Image Type Error \n");
			UpgradeErr = FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR;
			goto ErrDispose;
		}
		
	}
	else if(updatakitheader->dwImageType==UPDATA_D_KERNEL)
	{
		if (UpgradeBuf[0]==0x27&&UpgradeBuf[1]==0x05)
		{
			printf("Image Type Kernel \n");
		}
		else
		{
			printf("Image Type Error \n");
			UpgradeErr = FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR;
			goto ErrDispose;
		}
		
	}
	//ͳһ����������
	else if (updatakitheader->dwImageType==UPDATA_D_MTD_ALL_THREE)
	{
		//����ͳһ��������ļ����Ƿ��Ѿ�����yffs2����
		if (UpgradeBuf[0]==0x74&&UpgradeBuf[1]==0x6D&&UpgradeBuf[2]==0x70&&UpgradeBuf[3]==0x2F&&UpgradeBuf[4]==0x00&&UpgradeBuf[5]==0x00&&UpgradeBuf[6]==0x00)
		{
			printf("Image Type All Three Block Store \n");
		}
		else
		{
			printf("Image Type Error \n");
			UpgradeErr = FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR;
			goto ErrDispose;
		}
	}
	else if (updatakitheader->dwImageType==UPDATA_D_APPAREA)
	{
		//����APP�����Ƿ��Ѿ�����yffs2����
		if (UpgradeBuf[0]==0x73&&UpgradeBuf[1]==0x79&&UpgradeBuf[2]==0x73&&UpgradeBuf[3]==0x5F&&UpgradeBuf[4]==0x61&&UpgradeBuf[5]==0x70&&UpgradeBuf[6]==0x70)
		{
			printf("Image Type App Store \n");
		}
		else
		{
			printf("Image Type Error \n");
			UpgradeErr = FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR;
			goto ErrDispose;
		}
	}
	else if (updatakitheader->dwImageType==UPDATA_D_MTD_DEV)
	{
		//�豸DEV������
		if(UpgradeBuf[0]==0x6F&&UpgradeBuf[1]==0x70&&UpgradeBuf[2]==0x74&&UpgradeBuf[3]==0x2F&&UpgradeBuf[4]==0x00&&UpgradeBuf[5]==0x00&&UpgradeBuf[6]==0x00)
		{
			printf("Image Type dev Store \n");
		}
		else
		{
			printf("Image Type Error \n");
			UpgradeErr = FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR;
			goto ErrDispose;
		}
	}
	else if (updatakitheader->dwImageType==UPDATA_D_MTD_LIB)
	{
		//����LIB����
		if  (UpgradeBuf[0]==0x73&&UpgradeBuf[1]==0x79&&UpgradeBuf[2]==0x73&&UpgradeBuf[3]==0x5F&&UpgradeBuf[4]==0x6C&&UpgradeBuf[5]==0x69&&UpgradeBuf[6]==0x62)
		{
			printf("Image Type Lib Store \n");
		}
		else
		{
			printf("Image Type Error \n");
			UpgradeErr = FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR;
			goto ErrDispose;
		}
	}
	else if (updatakitheader->dwImageType==UPDATA_D_MTD_FACTORY)
	{
		//��������������
		//����LIB����
		if  (UpgradeBuf[0]==0x73&&UpgradeBuf[1]==0x79&&UpgradeBuf[2]==0x73&&UpgradeBuf[3]==0x5F&&UpgradeBuf[4]==0x66&&UpgradeBuf[5]==0x61&&UpgradeBuf[6]==0x63)
		{
			printf("Image Type factory Store \n");
		}
		else
		{
			printf("Image Type Error \n");
			UpgradeErr = FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR;
			goto ErrDispose;
		}
	}
	else
	{
		printf("Image Type Error \n");
		UpgradeErr = FILE_UPGRADE_DATACHECK_IMAGE_TYPE_ERR;
		goto ErrDispose;
	}
#endif

	/********���CPU����********/
	if (updatakitheader->dwCPUType==UPDATA_D_DM365)
	{
		printf("DM365 \n");
	}
	else if (updatakitheader->dwCPUType==UPDATA_D_DM368)
	{
		printf("DM368 \n");
	}
	else if (updatakitheader->dwCPUType==UPDATA_D_DM385)
	{
		printf("DM385 \n");
	}
	else if (updatakitheader->dwCPUType==UPDATA_D_HI3531)
	{
		printf("Hi3531 \n");
	}
	else
	{
		printf("CPU Type Error \n");
		UpgradeErr = FILE_UPGRADE_DATACHECK_CPU_TYPE_ERR;
		goto ErrDispose;
	}



	/********�����Ƶͨ������********/

	if (updatakitheader->dwChCount>=4&&updatakitheader->dwChCount<=8)
	{
		printf("Video %ld Channel \n" , updatakitheader->dwChCount);
	}
	else
	{
		printf("Video Channel Error \n");
		UpgradeErr = FILE_UPGRADE_DATACHECK_VIDEOCOUNT_ERR;
		goto ErrDispose;
	}
	
	/********����������汾��********/
	int m_VerErr=0;
	//���汾���
	if (updatakitheader->dwMainVer>=1&&updatakitheader->dwMainVer<=8)
	{
		++m_VerErr;
	}
	//�Ӱ汾���
	if (updatakitheader->dwChildVer>=1&&updatakitheader->dwChildVer<=10)
	{	
		++m_VerErr;
	}
	//�޸ĺż��
	if (updatakitheader->dwModityVer>=1&&updatakitheader->dwModityVer<=99)
	{	
		++m_VerErr;
	}
	//�ܽ�汾������
	if (m_VerErr==3)
	{
		printf("Ver %ld.%ld.%ld \n" , updatakitheader->dwMainVer,		\
			updatakitheader->dwChildVer,updatakitheader->dwModityVer);
	}
	else
	{
		printf("Ver %ld.%ld.%ld_Version Error \n" , updatakitheader->dwMainVer,	\
			updatakitheader->dwChildVer,updatakitheader->dwModityVer);
		UpgradeErr = FILE_UPGRADE_DATACHECK_VERSION_ERR;
		goto ErrDispose;
	}

	

	/********��������ļ�����,���������ļ�����********/
	if ((int)updatakitheader->dwRawWriteFileLen!=Upgradelen)
	{
		printf("Error dwRawWriteFileLen %ld Byte Upgradelen  %d Byte \n" , updatakitheader->dwRawWriteFileLen , Upgradelen);
		UpgradeErr = FILE_UPGRADE_DATACHECK_DATALEN_ERR;
		goto ErrDispose;
	}
	

	/********���CRCЧ����,����CRCЧ����********/

	DWORD CalcdwDataCRC = Check_GetDataCRC(UpgradeBuf,Upgradelen);
	if (CalcdwDataCRC!=updatakitheader->dwDataCRC)
	{
		printf("CRC Error CalcdwDataCRC [0x%x] dwDataCRC [0x%x] \n" , CalcdwDataCRC , updatakitheader->dwDataCRC);
		UpgradeErr = FILE_UPGRADE_DATACHECK_CRC_ERR;
		goto ErrDispose;
	}

ErrDispose:

	return UpgradeErr;	
}





/*************************************************
Function: TcpSockListen
Description: TCP��������˿ڼ���
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int TcpSockListen(int ip, int port)
{
	int hSock = -1;
	int opt = -1;
	int ret = -1;
	socklen_t	len = 0;
	struct sockaddr_in 	addr;

	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	hSock = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == hSock)
	{
		upgrade_err();
		return -1;
	}
	do
	{
		opt = 1;
		ret = setsockopt(hSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		if (0 != ret)
		{
			upgrade_err();
			break;
		}
		opt = 1;
		ret = setsockopt(hSock,IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
		if (0 != ret)
		{
			upgrade_err();
			break;
		}
		opt = SOCK_SNDRCV_LEN;
		ret = setsockopt(hSock, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(opt));
		if (0 != ret)
		{
			upgrade_err();
			break;
		}
		opt = SOCK_SNDRCV_LEN;
		ret = setsockopt(hSock, SOL_SOCKET, SO_RCVBUF, &opt, sizeof(opt));
		if (0 != ret)
		{
			upgrade_err();
			break;
		}
		opt = sizeof(len);
		ret = getsockopt(hSock, SOL_SOCKET, SO_SNDBUF, &len, (socklen_t *)&opt);
		if (0 != ret)
		{
			upgrade_err();
			break;
		}
		opt = sizeof(len);
		ret = getsockopt(hSock, SOL_SOCKET, SO_RCVBUF, &len, (socklen_t *)&opt);
		if (0 != ret)
		{
			upgrade_err();
			break;
		}

		ret = bind(hSock, (struct sockaddr *)&addr, sizeof(addr));
		if (0 != ret)
		{
			upgrade_err();
			break;
		}

		ret = listen(hSock, 2);
		if (0!= ret)
		{
			upgrade_err();
			break;
		}
	}while(FALSE);

	if (0!= ret)
	{
		shutdown(hSock,2);
		close(hSock);
		return -1;
	}

	return hSock;
}

/*************************************************
Function: UdpPortListen
Description: UDP��������˿ڼ���
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int UdpPortListen(int ip, int port)
{
	int				hSock = -1;
	int				opt = -1;
	int				ret = -1;
	//struct timeval 	to;
    struct sockaddr_in 	addr;

	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);

	hSock = socket(AF_INET,SOCK_DGRAM,0);
	if (-1 == hSock)
	{
		upgrade_err();
		return -1;
	}
    ret = bind(hSock,(struct sockaddr *)&addr,sizeof(addr));
	if (0!= ret)
	{
		upgrade_err();
		return -1;
	}
	do
	{
		opt = 1;
		ret = setsockopt(hSock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
		if (0!= ret)
		{
			upgrade_err();
			break;
		}
		opt = SOCK_SNDRCV_LEN;
		ret = setsockopt(hSock,SOL_SOCKET,SO_SNDBUF,&opt,sizeof(opt));
		if (0!= ret)
		{
			upgrade_err();
			break;
		}
		opt = SOCK_SNDRCV_LEN;
		ret = setsockopt(hSock,SOL_SOCKET,SO_RCVBUF,&opt,sizeof(opt));
		if (0 != ret)
		{
			upgrade_err();
			break;
		}	
		//ret = setsockopt(hSock,SOL_SOCKET,SO_SNDTIMEO, &to, sizeof(to));

	}while(FALSE);
	if (0 != ret)
	{
		shutdown(hSock,2);
		close(hSock);
		hSock = -1;
	}
	return hSock;
}


/*************************************************
Function: TcpReceive(int hSock, char *pBuffer, DWORD nSize)
Description: UDP��������˿ڼ���
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int TcpReceive(int hSock, char *pBuffer, DWORD nSize)
{
	int ret = 0;
	fd_set fset;
	struct timeval to;
	DWORD dwRecved = 0;
	
	bzero(&to, sizeof(to));
	
	if (nSize <= 0)
	{
		return -2;
	}
	
	while (dwRecved < nSize)
	{
		FD_ZERO(&fset);
		FD_SET(hSock, &fset);
		to.tv_sec = 10;
		to.tv_usec = 0;
		
		if (hSock <= 0 || hSock > 65535)
		{
			upgrade_err();
			return -2;
		}
		
		ret = select(hSock+1, &fset, NULL, NULL, &to);  //&to
		if ( ret == 0 || (ret == -1 && errno == EINTR))
		{
			upgrade_err();
			return 0;
		}
		
		if (ret < 0) //&& errno == ECONNREFUSED)
		{
			upgrade_err();
			return -2;
		}
		
		if(!FD_ISSET(hSock, &fset))
		{
			upgrade_err();
			return -1;
		}
		
		ret = recv(hSock, pBuffer + dwRecved, nSize - dwRecved, 0);
		if ( (ret < 0) && (errno == EAGAIN || errno == EINTR))
		{
			upgrade_err();
			return -1;
		}

		if (ret <0)
		{
			upgrade_err();
			return -2;
		}
		
		dwRecved += ret;
	}
	return dwRecved;
}


/*************************************************
Function: TcpSend(int hSer,char *pBuf,int nLen)
Description: TCP���ݷ��ͺ���
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int TcpSend(int hSer,char *pBuf,int nLen)
{
	int ret = 0;
	int sendsize = 0;
	
	ret = send(hSer,pBuf,nLen ,0);
	if(ret != nLen)
	{
		upgrade_err();
	}
	return ret;
}


/*************************************************
Function: TcpMsgBuffer(int *nSock , NET_MESSAGE *msg , int nSignel)
Description: TCP��Ϣ���壬nSignel Ϊ0ʱ����ȡ��Ϣ��Ϊ1ʱд����Ϣ
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int TcpMsgBuffer(int *nSock , NET_MESSAGE *msg , int nSignel)
{
	int i , ret;

	//����
	pthread_mutex_lock(&hMsgBufferMutex);
	ret=0;
	//��ȡ������Ϣ
	if(nSignel==0)
	{
		for (i=0 ; i<UPDATE_TCP_BUFFER_MESSAGE_NUMBER ; i++)
		{
			if (g_Tcp_Send_Msg[i].bUse)
			{
				g_Tcp_Send_Msg[i].bUse=FALSE;
				*nSock = g_Tcp_Send_Msg[i].hConSock;
				memcpy((char *)msg , (char *)&g_Tcp_Send_Msg[i].szMsg , sizeof(NET_MESSAGE));
				ret=1;
				goto finish;
			}
		}
	}
	else
	{
		//д����Ϣ

		//�����Ϣ�����Ƿ��Ѿ�д��
		if(g_Tcp_Send_Msg[UPDATE_TCP_BUFFER_MESSAGE_NUMBER-1].bUse)
		{
			printf("Tcp Msg Buffer Already Full!\n");
			ret=0;
			goto finish;
		}

		//�������ǰ���Ѿ�����Ϣ��δ���ͣ�д���������λ��
		for (i=(UPDATE_TCP_BUFFER_MESSAGE_NUMBER-2) ; i>=0 ; i--)
		{
			if (g_Tcp_Send_Msg[i].bUse)
			{
				if(!g_Tcp_Send_Msg[i+1].bUse)
				{
					g_Tcp_Send_Msg[i+1].hConSock = *nSock ;
					memcpy((char *)&g_Tcp_Send_Msg[i+1].szMsg , (char *)msg , sizeof(NET_MESSAGE));
					g_Tcp_Send_Msg[i+1].bUse=TRUE;
					ret=1;
					goto finish;
				}
				
			}
		}
		//�������������Ϣ����Ϊ�գ�д��Ϣ����һ��λ��
		g_Tcp_Send_Msg[0].hConSock = *nSock ;
		memcpy((char *)&g_Tcp_Send_Msg[0].szMsg , (char *)msg , sizeof(NET_MESSAGE));
		g_Tcp_Send_Msg[0].bUse=TRUE;
		ret=1;

	}

finish :

	//����
	pthread_mutex_unlock(&hMsgBufferMutex);
	return ret;
	
}
	



/*************************************************
Function: InitialWriteFlashMsg(int *nSock , NET_MESSAGE *msg , int nSignel)
Description: ��ʼ��дFLASH��Ϣ֪ͨ����
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void InitialWriteFlashMsg(int nSock)
{
	memset(&g_FlashWrit_Progress_Msg , 0 , sizeof(TCP_SEND_NET_MESSAGE));
	g_FlashWrit_Progress_Msg.bUse=TRUE;
	g_FlashWrit_Progress_Msg.hConSock=nSock;
	g_FlashWrit_Progress_Msg.szMsg.dwNetFlag = NET_FLAG;
	g_FlashWrit_Progress_Msg.szMsg.dwCMD = NETCMD_NETWORK_UPDATA_PROGRESS;
	g_FlashWrit_Progress_Msg.szMsg.dwReserved = 0;
}

/*************************************************
Function: SendWriteFlashMsg(DWORD nWirteLen , DWORD nCorrentLen)
Description: ����дFLASH��Ϣ����,дFLASH���� DWORD nCorrentLen ��ǰд��������
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void SendWriteFlashMsg(DWORD nWirteLen , DWORD nCorrentLen)
{
	DWORD i;

	
	if (g_FlashWrit_Progress_Msg.bUse&&g_FlashWrit_Progress_Msg.hConSock>0)
	{
		if (nWirteLen == nCorrentLen)
		{
			g_FlashWrit_Progress_Msg.szMsg.dwReserved = 100;
			TcpMsgBuffer(&g_FlashWrit_Progress_Msg.hConSock , &g_FlashWrit_Progress_Msg.szMsg , 1);
		
		}
		else
		{
			i = ((nCorrentLen*100)/nWirteLen);
			if (i>g_FlashWrit_Progress_Msg.szMsg.dwReserved)
			{	
				g_FlashWrit_Progress_Msg.szMsg.dwReserved = i;
				TcpMsgBuffer(&g_FlashWrit_Progress_Msg.hConSock , &g_FlashWrit_Progress_Msg.szMsg , 1);
			}
	
		}

	
	}
	printf("dwReserved [%ld] \n",g_FlashWrit_Progress_Msg.szMsg.dwReserved);
}
//�����ʼ������
void ReleaseWriteFlashMsg()
{
	memset(&g_FlashWrit_Progress_Msg , 0 , sizeof(TCP_SEND_NET_MESSAGE));
}


/*************************************************
Function: TcpSendServer()
Description: TCP��Ϣ�����̣߳�ר�ż����Ϣ�����У��Ƿ�����Ϣ��Ҫ����
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void TcpSendServer()
{
	int ret = 0 , n;
	int sendsize = 0;
	int nSock ;
	NET_MESSAGE msg;
	char buf[200];
	while(1)
	{
		ret=TcpMsgBuffer(&nSock , &msg , 0); //��ȡ��Ϣ
		if(ret==1)
		{
			memset(buf , 0 , sizeof(buf));
			memcpy(buf , &msg , sizeof(NET_MESSAGE));
			n=TcpSend(nSock , buf , sizeof(NET_MESSAGE));
			if (n!=sizeof(NET_MESSAGE))
			{
				upgrade_err();
			}
		}
		//100����ȡһ����Ϣ����
		usleep(100000);
	}	

}



/*************************************************
Function: UdpReceive(int hSock, char *pBuffer, DWORD nSize)
Description: UDP��������˿ڼ���
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int UdpReceive(int hSock, char *pBuffer, DWORD nSize , struct sockaddr_in *addr)
{
	int ret = 0;
	int addrLen = -1 ;
	fd_set fset;
	struct timeval to;
	DWORD dwRecved = 0;
	

	bzero(&to, sizeof(to));
	
	if (nSize <= 0)
	{
		return -2;
	}
	
	while (dwRecved < nSize)
	{
#if 1
		FD_ZERO(&fset);
		FD_SET(hSock, &fset);
		to.tv_sec = 10;
		to.tv_usec = 0;
		
		if (hSock <= 0 || hSock > 65535)
		{
			upgrade_err();
			return -2;
		}
		
		ret = select(hSock+1, &fset, NULL, NULL, &to);  //&to
		if ( ret == 0 || (ret == -1 && errno == EINTR))
		{
			//upgrade_err();
			return -1;
		}
		
		if (ret == -1) //&& errno == ECONNREFUSED)
		{
			upgrade_err();
			return -2;
		}
		
		if(!FD_ISSET(hSock, &fset))
		{
			upgrade_err();
			return -1;
		}
		
		addrLen = sizeof(struct sockaddr);
		ret = recvfrom(hSock, pBuffer + dwRecved, nSize - dwRecved , 
			0, (struct sockaddr*)addr, (socklen_t *)&addrLen);
		if (ret <= 0)
		{
			upgrade_err();
			return -2;
		}
#else
		addrLen = sizeof(struct sockaddr);
		//MSG_DONTWAIT �������ᱻ������
		//MSG_WAITALL��Ҫ������������ֱ������õ����������㡣Ȼ���������׽���źţ�����������ӶϿ������������´α����յ��������Ͳ�ͬ���Ի᷵�����������������ݡ�

		ret = recvfrom(hSock, pBuffer + dwRecved, nSize - dwRecved , 
			0, (struct sockaddr*)addr, (socklen_t *)&addrLen);
		if (ret <= 0)
		{
			upgrade_err();
			return -2;
		}
		printf("recv data [%d]\n" , ret);
		
#endif
		dwRecved += ret;

		}
		return dwRecved;
}


/*************************************************
Function: UdpReceiveBufferClear
Description: ����UDP����BUFFER
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int UdpReceiveBufferClear(int hSock)
{
	int ret = 0;

	char pBuffer[1400];
	struct sockaddr_in Clearaddr;
	int addrLen;
	int dwRecved = 0;
	
	while (1)
	{	
		addrLen = sizeof(struct sockaddr);
		//MSG_DONTWAIT �������ᱻ������
		//MSG_WAITALL��Ҫ������������ֱ������õ����������㡣Ȼ���������׽���źţ�����������ӶϿ������������´α����յ��������Ͳ�ͬ���Ի᷵�����������������ݡ�
		ret = recvfrom(hSock, pBuffer , sizeof(pBuffer) , 
			MSG_DONTWAIT , (struct sockaddr*)&Clearaddr, (socklen_t *)&addrLen);
		if (ret>0)
		{
			dwRecved+=ret;	
		}
		if (ret!= sizeof(pBuffer))
		{
			break;
		}	
	}
	return dwRecved;
}



/*************************************************
Function: UdpSend(int hSer,char *pBuf,int nLen)
Description: Udp���ݷ��ͺ���
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
int UdpSend(int hSer,char *pBuf,int nLen , struct sockaddr_in *addr)
{
	int ret = 0;
	int sendsize = 0;
	
	while(sendsize < nLen)
	{

		ret = sendto(hSer, pBuf+sendsize, nLen - sendsize, 0, (struct sockaddr*)addr, sizeof(struct sockaddr));
		if(ret < 1)
		{
			upgrade_err();
			return ret;
		}
		sendsize = sendsize + ret;
	}
	
	return sendsize;
}


/*************************************************
Function: Restart_Upgrader_System()
Description: Restart_Upgrader_System()
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void Restart_Upgrader_System()
{
	//���ȹر�ס����carDVR
	//system("killall -9 carDVR");
	//ͬ��ϵͳ
	sync();
	//�ȴ�3��
	sleep(3);
	//����ϵͳ
	while(1)
	{
		printf("reboot system ..........\n");
		sync();
		system("/bin/reboot_ipnc");
		sleep(1);
		//system("reboot -f");	
		
	}
}

/*************************************************
Function: UdpPortListenThread()
Description: UDP��������˿ڼ���
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void	UdpPortListenThread()
{	
	struct sockaddr_in addr;
	int ret;
	int nUdpSock;
	int nWaits=0;
	unsigned short src_port;
	int nMsg;
	nUdpSock = g_Upgrade_ServerInfo.hUdpPortSock;
	NET_MESSAGE UI_CmdMsg , ReUI_CmdMsg;
	while (1)
	{
		ret=UdpReceive(nUdpSock , &UI_CmdMsg , sizeof(NET_MESSAGE) , &addr);
		if(ret!=sizeof(NET_MESSAGE))
		{
			if (nWaits>20)
			{
				nWaits=0;
				printf("Upgrade UDP Recv Data Failur [%d] \n" , ret);
			}
			nWaits++;
			continue;
		}
		//�������UDP��Ϣ
		ret= 0;
		ret=UdpReceiveBufferClear(nUdpSock);
		printf("clear udp buffer [%d] byte\n" , ret);


		if(UI_CmdMsg.dwNetFlag!=NET_FLAG)
		{
			printf("Upgrade UDP Recv NET_FLAG Err [0x%x] \n" , UI_CmdMsg.dwNetFlag);
			continue;
		}
		else
		{
			printf("Upgrade UDP Recv NET_FLAG Success [0x%x]\n" , UI_CmdMsg.dwNetFlag);
		}
		//��¼�Է�ԭʼ�˿�
		src_port = addr.sin_port;
		if(UI_CmdMsg.dwCMD==NETCMD_USBUPDATA_START)
		{
			printf("Upgrade UDP Recv NETCMD_USBUPDATA_START\n");
			//�ظ�UI��Ϣ,ȷ���յ�����
			memset(&ReUI_CmdMsg , 0 , sizeof(NET_MESSAGE));
			ReUI_CmdMsg.dwNetFlag=NET_FLAG;
			ReUI_CmdMsg.dwCMD=NETCMD_USBUPDATA_RETURN;	
			//ԭʼ�˿ڷ���
			ret=UdpSend(nUdpSock,&ReUI_CmdMsg, sizeof(NET_MESSAGE) , &addr);
			if (ret!=sizeof(NET_MESSAGE))
			{
				printf("Return UDP Msg NETCMD_USBUPDATA_RETURN Failure\n");
				continue;
			}

			printf("Return NETCMD_USBUPDATA_RETURN Msg Start Upgrade.......\n");
			//��ʼUSB����
			nMsg=USBDisk_Auto_UpdateFlash();

			printf("Upgrade Complete.......\n");

			memset(&ReUI_CmdMsg , 0 , sizeof(NET_MESSAGE));
			ReUI_CmdMsg.dwNetFlag=NET_FLAG;
			if (nMsg == FLASH_UPGRADE_OK)
			{
				ReUI_CmdMsg.dwCMD=NETCMD_USBUPDATA_SUCCESS;

			}
			else
			{
				ReUI_CmdMsg.dwCMD=NETCMD_USBUPDATA_FAILURE;
				ReUI_CmdMsg.dwReserved = nMsg;
				printf("Upgrade Msg NETCMD_USBUPDATA_FAILURE Failure [%d]\n" , nMsg);
			}
			//ԭʼ�˿ڷ���
			ret=UdpSend(nUdpSock,&ReUI_CmdMsg, sizeof(NET_MESSAGE) , &addr);
			if (ret!=sizeof(NET_MESSAGE))
			{
				printf("Return UDP Msg [%d] Failure\n" , nMsg);
			}
			printf("Return UDP Msg NETCMD_USBUPDATA_SUCCESS.......\n");
			//�����ɹ���������
			if (nMsg == FLASH_UPGRADE_OK)
			{
				Restart_Upgrader_System();
			}
		}
		else
		{
			printf("Upgrade UDP Recv Command Error [0x%x]\n" , UI_CmdMsg.dwCMD);
		}
	}
}


/*************************************************
Function: SetConnSockAttr(int hSock, int nTimeOver)
Description: ����linux Socketѡ��
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void	SetConnSockAttr(int hSock, int nTimeOver)
{
	int opt;
	int ret;
	int len;
	struct timeval 	to;
	
	opt = 1;
	ret = setsockopt(hSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	opt = 1;
	ret = setsockopt(hSock, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
	opt = SOCK_SNDRCV_LEN;
	ret = setsockopt(hSock, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(opt));
	opt = SOCK_SNDRCV_LEN;
	opt = sizeof(len);
	ret = getsockopt(hSock, SOL_SOCKET, SO_SNDBUF, &len, (socklen_t *)&opt);
	opt = sizeof(len);
	ret = getsockopt(hSock, SOL_SOCKET, SO_RCVBUF, &len, (socklen_t *)&opt);
	
	
	to.tv_sec = 10;
	to.tv_usec = 0;
	//ret = setsockopt(hSock,SOL_SOCKET,SO_RCVTIMEO,&to,sizeof(to));
	ret = setsockopt(hSock,SOL_SOCKET,SO_SNDTIMEO,&to,sizeof(to));
	
}





/*************************************************
Function: TCPListenThread
Description: TCP������������߳�
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void *access_listen_ret_thread(void *param)
{
	fd_set  				fset;
	int     				ret , n;
	struct sockaddr_in              	addr;
	int					hConnSock;
	listen_ret_t				listen_ret;
	int					i;
	NET_MESSAGE Netwrork_CmdMsg , ReNetwrork_CmdMsg;
	int nBlockNum , nCheckBlockNum;
	int nUpgraeCountSize;
	unsigned char *recvbuffer=NULL;
	memcpy(&listen_ret, param, sizeof(listen_ret_t));
	free(param);
	hConnSock = listen_ret.accept_sock;
	memcpy(&addr, &listen_ret.client_addr, sizeof(struct sockaddr_in));
	
	while(1)
	{
		ret=TcpReceive(hConnSock , (char *)&Netwrork_CmdMsg, sizeof(NET_MESSAGE));
		if(ret!=sizeof(NET_MESSAGE))
		{
			upgrade_err();
			break;
		}

		if (Netwrork_CmdMsg.dwNetFlag != NET_FLAG)
		{
			upgrade_err();
			break;
		}

		if (Netwrork_CmdMsg.dwNetFlag == NET_FLAG)
		{
			//�յ�������������
			if (Netwrork_CmdMsg.dwCMD == NETCMD_NETWORK_UPDATA_START)
			{
				g_Check_Updata_Type = 0;
				memset((char *)&ReNetwrork_CmdMsg , 0 , sizeof(NET_MESSAGE));
				ReNetwrork_CmdMsg.dwNetFlag=NET_FLAG;
				ReNetwrork_CmdMsg.dwCMD=NETCMD_NETWORK_UPDATA_RETURN;
				n=TcpMsgBuffer(&hConnSock , &ReNetwrork_CmdMsg , 1);
				if (n!=1)
				{
					upgrade_err();
				}
				//Ч��ְ����Ƿ���ȷ
				nBlockNum = Netwrork_CmdMsg.dwReserved;
				nUpgraeCountSize = Netwrork_CmdMsg.dwLength;
				printf("Msg Upgrade Data Count Len [%d] nBlockNum [%d]\n", nUpgraeCountSize , nBlockNum);
				if ((nUpgraeCountSize%BLOCK_NETWORK_UPDATA_BLOCKSIZE)>0)
				{
					nCheckBlockNum=(nUpgraeCountSize/BLOCK_NETWORK_UPDATA_BLOCKSIZE)+1;
				}
				else
				{
					nCheckBlockNum=(nUpgraeCountSize/BLOCK_NETWORK_UPDATA_BLOCKSIZE);
				}
				//�ȽϷְ�ֵ
				if (nBlockNum!=nCheckBlockNum)
				{
					ReNetwrork_CmdMsg.dwCMD = NETCMD_NETWORK_UPDATA_MESSAGE;
					ReNetwrork_CmdMsg.dwReserved = NETWORK_STATUS_CHECK_BLOCKE_ERROR;
					n=TcpMsgBuffer(&hConnSock , &ReNetwrork_CmdMsg , 1);
					if (n!=1)
					{
						upgrade_err();
					}
					upgrade_err();
				}

				system("killall -9 ipcam ");
				system("killall -9 av_server.out");
				sync();

				//����ϵͳ�ڴ�,������������
				recvbuffer = NULL;
				recvbuffer = (unsigned char *)malloc(nUpgraeCountSize+1);
				if (recvbuffer == NULL)
				{
					ReNetwrork_CmdMsg.dwCMD = NETCMD_NETWORK_UPDATA_MESSAGE;
					ReNetwrork_CmdMsg.dwReserved = NETWORK_STATUS_CHECK_MALLOC_MEMERY_FAILURE;
					n=TcpMsgBuffer(&hConnSock , &ReNetwrork_CmdMsg , 1);
					if (n!=1)
					{
						upgrade_err();
					}
					upgrade_err();

					break;
				}
				//������������
				ret=TcpReceive(hConnSock , recvbuffer , nUpgraeCountSize);
				if(ret!=nUpgraeCountSize)
				{
					ReNetwrork_CmdMsg.dwCMD = NETCMD_NETWORK_UPDATA_MESSAGE;
					ReNetwrork_CmdMsg.dwReserved = NETWORK_STATUS_RECV_ERROR;
					n=TcpMsgBuffer(&hConnSock , &ReNetwrork_CmdMsg , 1);
					if (n!=1)
					{
						upgrade_err();
					}
					upgrade_err();
					break;
				}
				printf("get upgrade ok ret [%d]\n" , ret);
				//Ч������������
				ret=Network_UpdateFile_Crc(recvbuffer , nUpgraeCountSize);
				if (ret != FILE_UPGRADE_DATACHECK_OK)
				{
					ReNetwrork_CmdMsg.dwCMD = NETCMD_NETWORK_UPDATA_MESSAGE;
					ReNetwrork_CmdMsg.dwReserved = ret;
					n=TcpMsgBuffer(&hConnSock , &ReNetwrork_CmdMsg , 1);
					if (n!=1)
					{
						upgrade_err();
					}
					upgrade_err();
					//CRC����ǰ������
				}
				//��ʼ��֪ͨдFLASH����
				InitialWriteFlashMsg(hConnSock);
				//��ʼд����FLASH����
				ret = Network_UpdateFlash(recvbuffer , nUpgraeCountSize);
				if (ret != FLASH_UPGRADE_OK)
				{
					ReNetwrork_CmdMsg.dwCMD = NETCMD_NETWORK_UPDATA_MESSAGE;
					ReNetwrork_CmdMsg.dwReserved = ret;
					n=TcpMsgBuffer(&hConnSock , &ReNetwrork_CmdMsg , 1);
					if (n!=1)
					{
						upgrade_err();
					}
					upgrade_err();
					//����ڴ����ɹ����ͷ��ڴ�
					if (recvbuffer != NULL)
					{
						free(recvbuffer);
						recvbuffer = 0;
					}
				}
				//�����ɹ�
				else
				{
					ReNetwrork_CmdMsg.dwCMD = NETCMD_NETWORK_UPDATA_MESSAGE;
					ReNetwrork_CmdMsg.dwReserved = FLASH_UPGRADE_OK;
					n=TcpMsgBuffer(&hConnSock , &ReNetwrork_CmdMsg , 1);
					if (n!=1)
					{
						upgrade_err();
					}
					//����ڴ����ɹ����ͷ��ڴ�
					if (recvbuffer != NULL)
					{
						free(recvbuffer);
						recvbuffer = 0;
					}

					//ִ��Ԥ������
					Check_Block_Flash_Eraseall();  

				}
				//�����ɹ���񶼽�������
				Restart_Upgrader_System();
			}
			//��ʽ������IPC��/dev/mtd5 Ӧ�ó�����
			else if (Netwrork_CmdMsg.dwCMD == NETCMD_NETWORK_FORMAT_APP)
			{
				system("killall -9 ipcam");
				printf("flash_eraseall /dev/mtd5 \n");
				system("/usr/sbin/flash_eraseall /dev/mtd5 ");
				//��������
				Restart_Upgrader_System();
			}
			//��ʽ������IPC��/dev/mtd6 ����������
			else if (Netwrork_CmdMsg.dwCMD == NETCMD_NETWORK_FORMAT_DEV)
			{
				system("killall -9 ipcam");
				printf("flash_eraseall /dev/mtd6 \n");
				system("/usr/sbin/flash_eraseall /dev/mtd6 ");
				//��������
				Restart_Upgrader_System();
			}
			//��ʽ������IPC��/dev/mtd7 ���п���
			else if (Netwrork_CmdMsg.dwCMD == NETCMD_NETWORK_FORMAT_LIB)
			{
				system("killall -9 ipcam");
				printf("flash_eraseall /dev/mtd7 \n");
				system("/usr/sbin/flash_eraseall /dev/mtd7 ");
				//��������
				Restart_Upgrader_System();
			}
			//��ʽ������IPC��/dev/mtd8 ��ʱ�ļ���
			else if (Netwrork_CmdMsg.dwCMD == NETCMD_NETWORK_FORMAT_TEMP)
			{
				system("killall -9 ipcam");
				printf("flash_eraseall /dev/mtd8 \n");
				system("/usr/sbin/flash_eraseall /dev/mtd8 ");
				//��������
				Restart_Upgrader_System();
			}
			else
			{
				upgrade_err();
			}
		}
		
	}

	sleep(1);//�ȴ�1���Ӻ��˳�
	//�ر�Socket
	if (hConnSock>0)
	{
		close(hConnSock);//�ر�SOCKET
	}
	//����ڴ����ɹ����ͷ��ڴ�
	if (recvbuffer != NULL)
	{
		free(recvbuffer);
		recvbuffer = 0;
	}
		
	return (void *)NULL;
}


/*************************************************
Function: TCPListenThread
Description: TCP������������߳�
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
void TCPListenThread()
{
	int						hConnSock;
	int						hListenSock;
	int						ret;
	int						len;
	fd_set			        fset;
	struct timeval      	to;
	struct sockaddr_in      addr;
	pthread_t               thrdID;
	listen_ret_t			*plisten_ret;
	FD_ZERO(&fset);
	bzero(&to, sizeof(to));
	bzero(&addr, sizeof(addr));
	len = sizeof(addr);
	hListenSock = g_Upgrade_ServerInfo.hTcpListenSock;

	while(1)
	{
		to.tv_sec = 10; //10��
		to.tv_usec = 0;
		FD_SET(hListenSock, &fset);
		ret = select(hListenSock+1, &fset, NULL, NULL, &to);
		if (0 == ret) // time out
		{
			continue;
		}
		if (-1 == ret) // error
		{		
			upgrade_err();		
			continue;
		
		}
		if (!FD_ISSET(hListenSock, &fset))
		{
			continue;
		}
		//��������
		hConnSock = accept(hListenSock, (struct sockaddr*)&addr, (socklen_t *)&len);
		if (hConnSock < 0) 
		{
			upgrade_err();
			if (errno == 24) 
			{
				break;
			}
			else
			{
				continue;
			}
		}

		//����Socket����
		SetConnSockAttr(hConnSock, SOCK_TIME_OVER);
		plisten_ret = (listen_ret_t *)malloc(sizeof(listen_ret_t));
		if (plisten_ret == NULL)
		{
			upgrade_err();
			return;
		}
		plisten_ret->accept_sock = hConnSock;
		memcpy(&plisten_ret->client_addr, &addr, sizeof(struct sockaddr_in));
		/* create a thread for access the client request */
		printf("Start Logon accept()...\n");
		ret = pthread_create(&thrdID, NULL, access_listen_ret_thread, (void *)plisten_ret);
		if (ret != 0)
		{
			upgrade_err();
		}
	}
}



/*************************************************
Function:  Upgrade_Net_ServerStart()
Description: ��ʼ��������˿ڼ���
Input: ��Ϣ��Ϣ
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: // ����˵��
*************************************************/
BOOL Upgrade_Net_ServerStart()
{
	int	i = 0;
	int	ret = 0;
	int	hSock = -1;

	signal(SIGPIPE, SIG_IGN);
	pthread_t thrdUdpID , thrdTcpID , thrdMsgID;
	//signal(SIGPIPE, SIG_IGN);
	//����TCL����
	hSock = TcpSockListen(INADDR_ANY, UPDATE_TCP_UDP_LISTEN_PORT);
	if(hSock<0)
	{
		upgrade_err();
		return FALSE;
	}
	g_Upgrade_ServerInfo.hTcpListenSock = hSock;
	
	//����UDP����
	hSock = -1;
	hSock = UdpPortListen(INADDR_ANY, UPDATE_TCP_UDP_LISTEN_PORT);
	if(hSock<0)
	{
		upgrade_err();
		return FALSE;
	}
	g_Upgrade_ServerInfo.hUdpPortSock = hSock;
	
	//����UDP�����߳�
	ret = pthread_create(&thrdUdpID, NULL, UdpPortListenThread, NULL);
	if (ret!= 0)
	{
		upgrade_err();
	}
	//����TCP�����߳�
	ret = pthread_create(&thrdTcpID, NULL, TCPListenThread, NULL);
	if (ret!= 0)
	{
		upgrade_err();
	}
	//������Ϣ�����߳�
	ret = pthread_create(&thrdMsgID, NULL, TcpSendServer , NULL);
	if (ret!= 0)
	{
		upgrade_err();
	}
	return TRUE;
}





