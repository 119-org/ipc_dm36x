/*************************************************
Copyright (C), 2001-2012, TMVIDEO. Co., Ltd.
File name: // �ļ���f
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
#include <netdb.h>
#include <arpa/inet.h>
#include <linux/route.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>


#include "dev_Upgrade.h"
#include <sys/stat.h> 


//��ȡ�ļ�����
/*************************************************
Function: unsigned long get_file_size(const char *path)
Description: ��ȡ�ļ����ȡ�
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����18M Rootfs�ռ����
author: ���Ƹ� 2013-01-07
*************************************************/
unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
}  






//���������ļ���������
/*************************************************
Function: int Auto_Temp_Updata_File()  
Description: ���ȷ��������ļ��������Զ�������
Input: 
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: 
author: ���Ƹ� 2013-01-27
*************************************************/
int Auto_Temp_Updata_File()  
{  
	unsigned long nUpdatalength;


	

	//����ͳһ�����������ļ�,���Զ�����
	printf("\n-------------------------------------------\n");
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_all_three_mtdblock.tar");
	printf("0 . ALL THREE sys_all_three_mtdblock.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x3000000)
	{
		//���ȹر��������е�������
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//�������ļ����¶��ڵ��ļ�·������
		system("/bin/tar -xf /sys_ext/sys_all_three_mtdblock.tar -C / ");
		//ͬ��
		sync();
		//ɾ����ʱ�洢�ļ�
		system("rm -f /sys_ext/sys_all_three_mtdblock.tar");
		sync();	
		return 1;
	}


   //����APP�����ļ�,���Զ�����
	printf("\n-------------------------------------------\n");
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_app_mtdblock5.tar");
	printf("1 . APP sys_lib_mtdblock5.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x2000000)
	{
		//���ȹر��������е�������
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//�������ļ����¶��ڵ��ļ�·������
		system("/bin/tar -xf /sys_ext/sys_app_mtdblock5.tar -C / ");
		//ͬ��
		sync();
		//ɾ����ʱ�洢�ļ�
		system("rm -f /sys_ext/sys_app_mtdblock5.tar");
		sync();	
		return 1;
	}

	//����DEV�����ļ�,���Զ�����
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_dev_mtdblock6.tar");
	
	printf("2 DEV sys_lib_mtdblock6.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x2000000)
	{
		//���ȹر��������е�������
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//�������ļ����¶��ڵ��ļ�·������
		system("/bin/tar -xf /sys_ext/sys_dev_mtdblock6.tar -C / ");
		//ͬ��
		sync();
		//ɾ����ʱ�洢�ļ�
		system("rm -f /sys_ext/sys_dev_mtdblock6.tar");
		sync();
		return 1;	
	}

	//����LIB�����ļ�,���Զ�����
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_lib_mtdblock7.tar");
	
	printf("3 . LIB sys_lib_mtdblock7.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x2000000)
	{
		//���ȹر��������е�������
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//�������ļ����¶��ڵ��ļ�·������
		system("/bin/tar -xf /sys_ext/sys_lib_mtdblock7.tar -C / ");
		//ͬ��
		sync();
		//ɾ����ʱ�洢�ļ�
		system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
		sync();
		return 1;	
	} 

	//���ҹ����������������ļ�,���Զ�����
	nUpdatalength = 0;
	nUpdatalength = get_file_size("/sys_ext/sys_fac_mtdblock4.tar");
	
	printf("4 . FACTORY sys_fac_mtdblock4.tar file size [%ld]\n" , nUpdatalength);
	if(nUpdatalength>1024&&nUpdatalength<0x2000000)
	{
		//���ȹر��������е�������
		system("killall -9 ipcam");
		system("killall -9 av_server.out");
		sync();
		//�������ļ����¶��ڵ��ļ�·������
		system("/bin/tar -xf /sys_ext/sys_fac_mtdblock4.tar -C / ");
		//ͬ��
		sync();
		//ɾ����ʱ�洢�ļ�
		system("rm -f /sys_ext/sys_fac_mtdblock4.tar");
		sync();
		return 1;	
	} 
	return 0;
}  




/*************************************************
Function: Rootfs_UpdateFlash(int size, char *buffer)
Description: ��FLASH�ļ�д�����ݡ�
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����18M Rootfs�ռ����
author: ���Ƹ� 2012-03-07
*************************************************/
int Rootfs_UpdateFlash(int size, char *buffer)
{

	int rootfs_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=0x17FFFFF;	//�ļ�ϵͳ����Ϊ18M

	nFileLength=size;
	//Ĭ��д��FLASH��ȷ
	flash_err=FLASH_UPGRADE_OK;
	//����ļ���С����ֹ����ߴ��ļ���д��
	if(nFileLength<=23*1024*1024||nFileLength>=24*1024*1024)
	{
		printf("file size err [%d] \n", nFileLength);
		//����д�����ݴ�С����
		flash_err=FLASH_UPGRADE_DATA_SIZE_ERR;
		goto ErrDispose;
	}


# if D_WRITE_FLASH_TASK

	
	if ((rootfs_flashfs=open(MTD_ROOTFS_UPDATE_DEV, O_WRONLY))<0)
	{
		printf("Rootfs O_WRONLY open FLASH Block [%s] fail\n", MTD_ROOTFS_UPDATE_DEV);
		//��FLASH�豸ʧ��
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Rootfs O_WRONLY Open Success [%s] rootfs_flashfs [%d]\n" , MTD_ROOTFS_UPDATE_DEV , rootfs_flashfs);
	}
	//д������
	FlashOffset=0 ;
	WriteBlock=64*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			//WriteBlock=nFileLength;

			//WriteBlock=32*1024;

			WriteBlock=nFileLength-(nFileLength%64);


			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
	
		//д�������ļ�
		if(write(rootfs_flashfs , buffer+FlashOffset , WriteBlock)!=WriteBlock)
		{
			printf("Rootfs write error WriteBlock [%d]\n" , WriteBlock);
			//д�����ݵ�FLASH�豸ʧ��
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(200000);
		FlashOffset+=WriteBlock;
		//֪ͨ�������
		SendWriteFlashMsg(0x17FFFFF , FlashOffset);
		printf("Rootfs Write ok WriteBlock [%d] FlashOffset [%d]\n" , WriteBlock , FlashOffset);
		
	}

/*
	//����ȥ��������Ϊ0XFF		
	unsigned char flushbufer[64*1024];
	FlushBlock=64*1024;
	nRootfs_CountLen=0x17FFFFF;
	if(nRootfs_CountLen>FlashOffset)
	{
		nRootfs_CountLen-=FlashOffset;
	}
	else
	{
		nRootfs_CountLen=0;
	}
	printf("Rootfs Flush nRootfs_CountLen [%d]\n" , nRootfs_CountLen);
	while(nRootfs_CountLen>0)
	{
		if(nRootfs_CountLen<FlushBlock)
		{
			FlushBlock=nFileLength;
			nRootfs_CountLen=0;
		}
		else
		{
			nRootfs_CountLen-=FlushBlock;
		}
	
		//��дFLASH
		memset(flushbufer , 0xFF , sizeof(flushbufer));
		if(write(rootfs_flashfs ,flushbufer , FlushBlock)!=FlushBlock)
		{
			printf("write error FlushBlock [%d]\n" , FlushBlock);
			//д�����ݵ�FLASH�豸ʧ��
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(10);
		FlashOffset+=FlushBlock;
		//֪ͨ�������
		SendWriteFlashMsg(0x17FFFFF , FlashOffset);
		printf("Rootfs Flush Write ok FlushBlock [%d] FlashOffset [%d]\n" , FlushBlock , FlashOffset);
	}
*/

# endif

ErrDispose:
	
	sync();
	if (rootfs_flashfs>0)
	{
		close(rootfs_flashfs);
		rootfs_flashfs=-1;
	}

	/*���ǲ���д��FLASH�Ƿ�ɹ����������*/
# if D_READ_FLASH_TEST
	Read_Flash_Data(0x300000 , NULL , MTD_ROOTFS_UPDATE_DEV , "rRootfs.jfs2");
#endif
	return flash_err;
}



/*************************************************
Function: Kernel_UpdateFlash(int size, char *buffer)
Description: ��Kernel ���� FLASH�ļ�д�����ݡ�
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����2M Kernel�ռ����
author: ���Ƹ� 2012-03-07
*************************************************/
int Kernel_UpdateFlash(int size, char *buffer)
{

	int kernel_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=0x1FFFFF;	//Kernel Ϊ2M

	nFileLength=size;
	//Ĭ��д��FLASH��ȷ
	flash_err=FLASH_UPGRADE_OK;
	//����ļ���С����ֹ����ߴ��ļ���д��
	if(nFileLength<=1*1024*1024||nFileLength>=2*1024*1024)
	{
		printf("file size err [%d] \n", nFileLength);
		//����д�����ݴ�С����
		flash_err=FLASH_UPGRADE_DATA_SIZE_ERR;
		goto ErrDispose;
	}

# if D_WRITE_FLASH_TASK

	
	if ((kernel_flashfs=open(MTD_KERNEL_UPDATE_DEV, O_WRONLY))<0)
	{
		printf("Kernel O_WRONLY open FLASH Block [%s] fail\n", MTD_KERNEL_UPDATE_DEV);
		//��FLASH�豸ʧ��
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success [%s] kernel_flashfs [%d]\n" , MTD_KERNEL_UPDATE_DEV , kernel_flashfs);
	}
	//д������
	FlashOffset=0 ;
	WriteBlock=64*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			//WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
	
		//д�������ļ�
		if(write(kernel_flashfs , buffer+FlashOffset , WriteBlock)!=WriteBlock)
		{
			printf("write error WriteBlock [%d]\n" , WriteBlock);
			//д�����ݵ�FLASH�豸ʧ��
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(200000);
		FlashOffset+=WriteBlock;
		//֪ͨ�������
		SendWriteFlashMsg(0x1FFFFF , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d]\n" , WriteBlock , FlashOffset);
		
	}

/*
	//����ȥ��������Ϊ0XFF		
	unsigned char flushbufer[64*1024];
	FlushBlock=64*1024;
	nRootfs_CountLen=0x1FFFFF;
	if(nRootfs_CountLen>FlashOffset)
	{
		nRootfs_CountLen-=FlashOffset;
	}
	else
	{
		nRootfs_CountLen=0;
	}
	printf("Kernel Flush nRootfs_CountLen [%d]\n" , nRootfs_CountLen);
	while(nRootfs_CountLen>0)
	{
		if(nRootfs_CountLen<FlushBlock)
		{
			FlushBlock=nFileLength;
			nRootfs_CountLen=0;
		}
		else
		{
			nRootfs_CountLen-=FlushBlock;
		}
	
		//��дFLASH
		memset(flushbufer , 0xFF , sizeof(flushbufer));
		if(write(kernel_flashfs ,flushbufer , FlushBlock)!=FlushBlock)
		{
			printf("write error FlushBlock [%d]\n" , FlushBlock);
			//д�����ݵ�FLASH�豸ʧ��
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(10);
		FlashOffset+=FlushBlock;
		//֪ͨ�������
		SendWriteFlashMsg(0x1FFFFF , FlashOffset);
		printf("Kernel Flush Write ok FlushBlock [%d] FlashOffset [%d]\n" , FlushBlock , FlashOffset);
	}
*/

# endif


ErrDispose:
	
	sync();
	if (kernel_flashfs>0)
	{
		close(kernel_flashfs);
		kernel_flashfs=-1;
	}

/*���ǲ���д��FLASH�Ƿ�ɹ����������*/
# if D_READ_FLASH_TEST
	Read_Flash_Data(0x200000 , NULL , MTD_KERNEL_UPDATE_DEV , "rkernel.image");
#endif

	return flash_err;
			
}




/*************************************************
Function: App_UpdateFlash(int size, char *buffer)
Description: ��Kernel ���� FLASH�ļ�д�����ݡ�
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����28M Kernel�ռ����
author: ���Ƹ� 2012-03-07
����λ�ã�sys_app_mtdblock5
*************************************************/
int App_UpdateFlash(int size, char *buffer)
{

	//DM368,�����ļ�
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//Ӧ�ó�����
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//Ĭ��д��FLASH��ȷ
	flash_err=FLASH_UPGRADE_OK;
	
	system("rm -f /sys_ext/sys_app_mtdblock5.tar");
	sync();
	
	if ((app_flashfs=open("/sys_ext/sys_app_mtdblock5.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_app_mtdblock5.tar [%d] fail\n", app_flashfs);
		//��FLASH�豸ʧ��
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_app_mtdblock5.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//д������
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//д�������ļ�
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//д�����ݵ�FLASH�豸ʧ��
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//֪ͨ�������
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}
	
	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//���û�з����κδ��󣬽�������
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_app_mtdblock5.tar");
		
		printf("Network size [%d] sys_lib_mtdblock5.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			//ͬ��
			sync();
			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			system("/usr/sbin/flash_eraseall /dev/mtd5 ");
			/*
			//���ȹر��������е�������
			system("killall -9 ipcam");
			//�������ļ����¶��ڵ��ļ�·������
			system("/bin/tar -xf /sys_ext/sys_app_mtdblock5.tar -C / ");
			//ͬ��
			sync();
			//ɾ����ʱ�洢�ļ�
			system("rm -f /sys_ext/sys_app_mtdblock5.tar");
			sync();
			*/
			
		}
		else
		{
			//ɾ����ʱ�洢�ļ�
			system("rm -f /sys_ext/sys_app_mtdblock5.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
			
}




/*************************************************
Function: MTD_SYS_DEV
Description: ��Kernel ���� FLASH�ļ�д�����ݡ�
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����16M Kernel�ռ����
author: ���Ƹ� 2012-03-07
����λ�ã�sys_app_mtdblock5
*************************************************/
int SYS_Dev_Flash(int size, char *buffer)
{

	//DM368,�����ļ�
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//Ӧ�ó�����
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//Ĭ��д��FLASH��ȷ
	flash_err=FLASH_UPGRADE_OK;
	
	system("rm -f /sys_ext/sys_dev_mtdblock6.tar");
	sync();
	
	if ((app_flashfs=open("/sys_ext/sys_dev_mtdblock6.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_dev_mtdblock6.tar [%d] fail\n", app_flashfs);
		//��FLASH�豸ʧ��
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_dev_mtdblock6.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//д������
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//д�������ļ�
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//д�����ݵ�FLASH�豸ʧ��
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//֪ͨ�������
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}
	
	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//���û�з����κδ��󣬽�������
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_dev_mtdblock6.tar");
		
		printf("Network size [%d] sys_lib_mtdblock6.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			//ͬ��
			sync();
			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			system("/usr/sbin/flash_eraseall /dev/mtd6 ");
			/*
			//���ȹر��������е�������
			system("killall -9 ipcam");
			//�������ļ����¶��ڵ��ļ�·������
			system("/bin/tar -xf /sys_ext/sys_dev_mtdblock6.tar -C / ");
			//ͬ��
			sync();
			//ɾ����ʱ�洢�ļ�
			system("rm -f /sys_ext/sys_dev_mtdblock6.tar");
			sync();
			*/	
		}
		else
		{
			//ɾ����ʱ�洢�ļ�
			system("rm -f /sys_ext/sys_dev_mtdblock6.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
	
}




/*************************************************
Function: MTD_SYS_LIB
Description: ��Kernel ���� FLASH�ļ�д�����ݡ�
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����16M Kernel�ռ����
author: ���Ƹ� 2012-03-07
����λ�ã�sys_app_mtdblock5
*************************************************/
int SYS_Dev_LIB(int size, char *buffer)
{


	//DM368,�����ļ�
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//Ӧ�ó�����
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//Ĭ��д��FLASH��ȷ
	flash_err=FLASH_UPGRADE_OK;

	system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
	sync();

	if ((app_flashfs=open("/sys_ext/sys_lib_mtdblock7.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_lib_mtdblock7.tar [%d] fail\n", app_flashfs);
		//��FLASH�豸ʧ��
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_lib_mtdblock7.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//д������
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//д�������ļ�
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//д�����ݵ�FLASH�豸ʧ��
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//֪ͨ�������
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}

	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//���û�з����κδ��󣬽�������
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_lib_mtdblock7.tar");

		printf("Network size [%d] sys_lib_mtdblock7.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			sync();
			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			system("/usr/sbin/flash_eraseall /dev/mtd7 ");
			/*
			//���ȹر��������е�������
			system("killall -9 ipcam");
			//�������ļ����¶��ڵ��ļ�·������
			system("/bin/tar -xf /sys_ext/sys_lib_mtdblock7.tar -C / ");
			//ͬ��
			sync();
			//ɾ����ʱ�洢�ļ�
			system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
			sync();
			*/

		}
		else
		{
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			//ɾ����ʱ�洢�ļ�
			system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
}





/*************************************************
Function: SYS_All_Three_block
Description: ������������
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����16M Kernel�ռ����
author: ���Ƹ� 2012-03-07
����λ�ã�sys_app_mtdblock5
*************************************************/
int SYS_All_Three_block(int size, char *buffer)
{


	//DM368,�����ļ�
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//Ӧ�ó�����
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//Ĭ��д��FLASH��ȷ
	flash_err=FLASH_UPGRADE_OK;

	system("rm -f /sys_ext/sys_all_three_mtdblock.tar");
	sync();

	if ((app_flashfs=open("/sys_ext/sys_all_three_mtdblock.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_all_three_mtdblock.tar [%d] fail\n", app_flashfs);
		//��FLASH�豸ʧ��
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_all_three_mtdblock.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//д������
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//д�������ļ�
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//д�����ݵ�FLASH�豸ʧ��
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//֪ͨ�������
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}

	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//���û�з����κδ��󣬽�������
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_all_three_mtdblock.tar");

		printf("Network size [%d] sys_all_three_mtdblock.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			sync();
			usleep(100);

			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			usleep(100);
			printf("kill ipcam and av_server.out \n");
			
			system("/usr/sbin/flash_eraseall /dev/mtd5 ");
			sync();
			usleep(100);
			printf("/usr/sbin/flash_eraseall /dev/mtd5  \n");

			system("/usr/sbin/flash_eraseall /dev/mtd6 ");
			sync();
			usleep(100);
			printf("/usr/sbin/flash_eraseall /dev/mtd6  \n");

			system("/usr/sbin/flash_eraseall /dev/mtd7 ");
			sync();
			usleep(100);
			printf("/usr/sbin/flash_eraseall /dev/mtd7 \n");

			/*
			//���ȹر��������е�������
			system("killall -9 ipcam");
			//�������ļ����¶��ڵ��ļ�·������
			system("/bin/tar -xf /sys_ext/sys_lib_mtdblock7.tar -C / ");
			//ͬ��
			sync();
			//ɾ����ʱ�洢�ļ�
			system("rm -f /sys_ext/sys_lib_mtdblock7.tar");
			sync();
			*/

		}
		else
		{
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			//ɾ����ʱ�洢�ļ�
			system("rm -rf /sys_ext/sys_all_three_mtdblock.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
}




/*************************************************
Function:  MTD_PARAMETER_UPDATE_DEV
Description: ������ Flash�����ռ��ļ���
Input: int size Ϊ�����ļ���С, char *buffer �����ļ�����
Output: ��������Ϣ�ļ�
Return: // ��������ֵ��˵��
Others: ����16M Kernel�ռ����
author: ���Ƹ� 2012-03-07
����λ�ã�sys_app_mtdblock5
*************************************************/
int SYS_Dev_PARAMETER(int size, char *buffer)
{
	
	//DM368,�����ļ�
	
	int app_flashfs=-1;
	int FlashOffset;
	int WriteBlock , FlushBlock ,readywrite;
	int nFileLength;
	int flash_err ;
	int nRootfs_CountLen=size;	//Ӧ�ó�����
	char cmd[80];
	unsigned long nUpdatalength;
	
	nFileLength=size;
	//Ĭ��д��FLASH��ȷ
	flash_err=FLASH_UPGRADE_OK;

	system("rm -f /sys_ext/sys_fac_mtdblock4.tar");
	sync();

	if ((app_flashfs=open("/sys_ext/sys_fac_mtdblock4.tar", O_WRONLY|O_CREAT))<0)
	{
		printf("Kernel O_WRONLY open Create /sys_ext/sys_fac_mtdblock4.tar [%d] fail\n", app_flashfs);
		//��FLASH�豸ʧ��
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("Kernel O_WRONLY Open Success Create /sys_ext/sys_fac_mtdblock4.tar app_flashfs [%d]\n" , app_flashfs);
	}
	//д������
	FlashOffset=0 ;
	WriteBlock=8*1024;
	while(nFileLength>0)
	{	
		if(nFileLength<WriteBlock)
		{
			WriteBlock=nFileLength;
			nFileLength=0;
		}
		else
		{
			nFileLength-=WriteBlock;
		}
		
		//д�������ļ�
		readywrite=write(app_flashfs , buffer+FlashOffset , WriteBlock);
		if(readywrite!=WriteBlock)
		{
			printf("write error WriteBlock [%d] readywrite [%d]\n" , WriteBlock ,readywrite);
			//д�����ݵ�FLASH�豸ʧ��
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			goto ErrDispose;
		}
		sync();
		usleep(100);
		FlashOffset+=WriteBlock;
		//֪ͨ�������
		SendWriteFlashMsg(nRootfs_CountLen , FlashOffset);
		printf("Kernel Write ok WriteBlock [%d] FlashOffset [%d] nFileLength [%d]\n" , WriteBlock , FlashOffset , nFileLength);
		
	}

	
ErrDispose:
	
	sync();
	if (app_flashfs>0)
	{
		close(app_flashfs);
		app_flashfs=-1;
	}
	sync();
	//���û�з����κδ��󣬽�������
	if(flash_err==FLASH_UPGRADE_OK)
	{
		nUpdatalength = 0;
		nUpdatalength = get_file_size("/sys_ext/sys_fac_mtdblock4.tar");

		printf("Network size [%d] sys_fac_mtdblock4.tar file size [%ld]\n" , size , nUpdatalength);
		if(nUpdatalength>1024&&(size==nUpdatalength))
		{
			sync();
			system("killall -9 ipcam");
			system("killall -9 av_server.out");
			sync();
			system("/usr/sbin/flash_eraseall /dev/mtd4 ");
			/*
			//���ȹر��������е�������
			system("killall -9 ipcam");
			//�������ļ����¶��ڵ��ļ�·������
			system("/bin/tar -xf /sys_ext/sys_lib_mtdblock4.tar -C / ");
			//ͬ��
			sync();
			//ɾ����ʱ�洢�ļ�
			system("rm -f /sys_ext/sys_lib_mtdblock4.tar");
			sync();
			*/

		}
		else
		{
			flash_err=FLASH_UPGRADE_WRITE_DATA_FAIL;
			//ɾ����ʱ�洢�ļ�
			system("rm -f /sys_ext/sys_fac_mtdblock4.tar");
			sync();
		}
	}
	
	
	return flash_err;
	
}





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
int Read_Flash_Data(int nReadsize , char *buffer , char *dev , char *savename)
{

	
	int read_flashfs=-1;
	int  flash_err ;	
	FILE *fp=NULL;
	int fpwrlen;
	//Ĭ�϶�ȡFLASH��ȷ
	flash_err=FLASH_UPGRADE_OK;
	sync();
	if ((read_flashfs=open(dev,  O_RDONLY))<0)
	{
		printf("O_RDONLY open FLASH Block [%s] fail\n", dev);
		//��FLASH�豸ʧ��
		flash_err=FLASH_UPGRADE_OPEN_DEVICE_FAIL;
		goto ErrDispose;
	}
	else
	{
		printf("O_RDONLY Open Success [%s] read_flashfs [%d]\n" , dev , read_flashfs);
	}
	
	//���bufferΪ��,�򱣳ֶ�ȡ���ݵ��ļ���
	if (buffer==NULL&&savename!=NULL)
	{
		fp=fopen(savename,"wb");
		if (fp==NULL)
		{
			printf("create file fail  \n");
			goto ErrDispose;
		}
	}

	int ReadFlashOffset=0 ;
	int ReadBlock=64*1024;
	int ReadnFileLength = nReadsize;
	unsigned char readbuffer[64*1024];
	while(ReadnFileLength>0)
	{	
		if(ReadnFileLength<ReadBlock)
		{
			ReadBlock=ReadnFileLength;
			ReadnFileLength=0;
		}
		else
		{
			ReadnFileLength-=ReadBlock;
		}
		memset(readbuffer , 0 , sizeof(readbuffer));
		//��ȡFlash�ļ���
		if(read(read_flashfs , readbuffer , ReadBlock)!=ReadBlock)
		{
			printf("read error ReadBlock [%d]\n" , ReadBlock);
			flash_err=FLASH_UPGRADE_READ_DATA_FAIL;
			goto ErrDispose;
		}		
		sync();

		//���buffer��Ϊ�գ�����ȡFlash����д��buffer��
		if (buffer!=NULL)
		{
			memcpy(buffer+ReadFlashOffset , readbuffer , ReadBlock);
		}
		else
		{
			//����ȡFlash����д���ļ���
			if (fp!=NULL)
			{
				fpwrlen=fwrite(readbuffer,ReadBlock,1,fp);//���ַ�������д�뵽�ļ� 
				if (fpwrlen!=1)
				{
					printf("fwrite error fpwrlen [%d]\n" , fpwrlen);
				}
			}
		}		
		ReadFlashOffset+=ReadBlock;
		printf("read ok ReadBlock [%d] ReadFlashOffset [%d]\n" , ReadBlock , ReadFlashOffset);
	}
		
ErrDispose:
		
	sync();
	if (read_flashfs>0)
	{
		close(read_flashfs);
		read_flashfs=-1;
	}
	if (fp!=NULL)
	{
		fclose(fp);
	}
	return flash_err;
	
}






