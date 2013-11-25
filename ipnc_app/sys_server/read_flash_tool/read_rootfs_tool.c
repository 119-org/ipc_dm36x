/*************************************************
Copyright (C), 2001-2012,  Co., Ltd.
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




int main()
{
	int err_msg;
	
	printf("******************************************\n");
	printf("*********LiuWenGao 2013-01-23-Write**********\n");
	printf("Read Flash DM368 /dev/mtdblock Create\n");
	printf("******************************************\n");

#if 0
	/*********��ȡBootleader����**********/
	system("rm -r Bootleader_mtdblock0.fs");
	err_msg=Read_Flash_Data(0x2FFFFF , NULL , "/dev/mtd0", "Bootleader_mtdblock0.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock0 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock0 success! \n");
	}


	/*********��ȡParams����**********/
	system("rm -r Params_mtdblock1.fs");
	err_msg=Read_Flash_Data(0x1FFFFF , NULL , "/dev/mtd1", "Params_mtdblock1.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock1 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock1 success! \n");
	}



	/*********��ȡKernel����**********/
	system("rm -r Kernel_mtdblock2.fs");
	err_msg=Read_Flash_Data(0x1FFFFF , NULL , "/dev/mtd2", "Kernel_mtdblock2.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock2 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock2 success! \n");
	}


	/*********��ȡfilesystem����**********/
	system("rm -r filesystem_mtdblock3.fs");
	err_msg=Read_Flash_Data(0x17FFFFF , NULL , "/dev/mtd3", "filesystem_mtdblock3.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtd3 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtd3 success! \n");
	}


	/*********��ȡfactory_info����**********/
	system("rm -r factory_info_mtdblock4.fs");
	err_msg=Read_Flash_Data(0x1FFFFF , NULL , "/dev/mtd4", "factory_info_mtdblock4.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock4 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock4 success! \n");
	}
	/*********��ȡsys_app����**********/
	system("rm -r sys_app_mtdblock5.fs");
	err_msg=Read_Flash_Data(0xFFFFFF , NULL , "/dev/mtd5", "sys_app_mtdblock5.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock5 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock5 success! \n");
	}

	/*********��ȡsys_dev����**********/
	system("rm -r sys_dev_mtdblock6.fs");
	err_msg=Read_Flash_Data(0xFFFFFF , NULL , "/dev/mtd6", "sys_dev_mtdblock6.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock6 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock6 success! \n");
	}


	/*********��ȡsys_lib����**********/
	system("rm -r sys_lib_mtdblock7.fs");
	err_msg=Read_Flash_Data(0xFFFFFF , NULL , "/dev/mtd7", "sys_lib_mtdblock7.fs");
	if(err_msg!=FLASH_UPGRADE_OK)
	{
		printf("read flash /dev/mtdblock7 error err_msg [%d]\n" , err_msg);
	}
	else
	{
		printf("read flash /dev/mtdblock7 success! \n");
	}

#endif

	//���ȹر��������е�������
	system("killall -9 ipcam");
	system("killall -9 av_server.out");
	sync();

	//DM368����IPC�������ļ���ȡ
	//2013��1��23�գ����Ƹ߱�д
	unsigned long nUpdataLen;
	float fLen;


	/*********��ȡ��������ͳһ������**********/
	system("rm -r sys_all_three_mtdblock.fs");
	//������������ͳһ�����������ļ�������Ҫ����й��ţ�ֻ�������κ�趨Ŀ¼
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_all_three_mtdblock.fs /sys_app /opt /sys_lib /tmp ");
	sync();
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_all_three_mtdblock.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_all_three_mtdblock.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");


	/*********��ȡsys_fac����,������������**********/
	system("rm -r sys_fac_mtdblock4.fs");
	//����APP���������ļ�������Ҫ����й��ţ�ֻ�������κ�趨Ŀ¼
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_fac_mtdblock4.fs /sys_fac ");
	sync();
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_fac_mtdblock4.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_fac_mtdblock4.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");


#if 0
	/*********��ȡsys_app����**********/
	system("rm -r sys_app_mtdblock5.fs");
	//����APP���������ļ�������Ҫ����й��ţ�ֻ�������κ�趨Ŀ¼
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_app_mtdblock5.fs /sys_app ");
	sync();
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_app_mtdblock5.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_app_mtdblock5.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");





	/*********��ȡsys_dev����**********/
	system("rm -r sys_dev_mtdblock6.fs");
	//����DEV���������ļ�������Ҫ����й��ţ�ֻ�������κ�趨Ŀ¼
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_dev_mtdblock6.fs /opt ");
	sync();
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_dev_mtdblock6.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_dev_mtdblock6.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");


	/*********��ȡsys_lib����**********/
	system("rm -r sys_lib_mtdblock7.fs");
	//����LIB���������ļ�������Ҫ����й��ţ�ֻ�������κ�趨Ŀ¼
	printf("----------------------------------------\n");
	system("/bin/tar -cf sys_lib_mtdblock7.fs /sys_lib ");
	nUpdataLen =0;
	fLen =0;
	nUpdataLen = get_file_size("sys_lib_mtdblock7.fs");
	fLen = (float)((float)(nUpdataLen/1024)/(float)1024);
	printf("sys_lib_mtdblock7.fs Length [%f M]\n" , fLen);
	printf("----------------------------------------\n");

#endif

	sync();

	printf("********************complete**********************\n");

	return 0;
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
	int i , nCheckValue ;
	unsigned long nBlockValue;
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

		nBlockValue = 0;
		for (i = 0 ; i<ReadBlock ; i++)
		{
			nCheckValue = 0;
			nCheckValue = 0xFF - readbuffer[i];
			nBlockValue += nCheckValue;
		}

		printf("read nBlockValue [%ld]\n" , nBlockValue);
		if (nBlockValue == 0)
		{
			
			printf("read ok 0xFF black\n");
			//goto ErrDispose;
		}

		printf("read ok Count [%d] ReadBlock [%d] ReadFlashOffset [%d]\n" , nReadsize ,ReadBlock , ReadFlashOffset);
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






