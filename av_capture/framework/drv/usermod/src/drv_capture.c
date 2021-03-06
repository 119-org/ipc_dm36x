#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include <drv_capture.h>

int DRV_captureOpen(DRV_CaptureConfig *config, DRV_CaptureInfo *info)
{
    int status = drvImgsFunc->imgsOpen(&config->imgsConfig);
    if (status != OSA_SOK)
    {
        goto error_exit;
    }

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Opening ISIF.\n");
#endif

    status = DRV_isifOpen(&config->isifConfig);
    if (status != OSA_SOK)
    {
        goto isif_exit;
    }

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Opening H3A.\n");
#endif

    status = DRV_h3aOpen(&config->h3aConfig);
    if (status != OSA_SOK)
    {
        goto h3a_exit;
    }

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Opening IPIPE.\n");
#endif

    status = DRV_ipipeOpen(&config->ipipeConfig);
    if (status != OSA_SOK)
    {
        goto ipipe_exit;
    }

    if (info != NULL)
    {
        DRV_ipipeGetInfo(&info->ipipeInfo);
        DRV_isifGetInfo(&info->isifInfo);
        DRV_h3aGetInfo(&info->h3aInfo);
    }

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Open DONE.\n");
#endif

    return OSA_SOK;

ipipe_exit:
    DRV_h3aClose();

h3a_exit:
    DRV_isifClose();

isif_exit:
    drvImgsFunc->imgsClose();

error_exit:
    OSA_ERROR("Open failed\n");
    return OSA_EFAIL;
}

int DRV_captureClose()
{
#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Closing imager.\n");
#endif

    drvImgsFunc->imgsClose();

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Closing ISIF.\n");
#endif

    DRV_isifClose();

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Closing H3A.\n");
#endif

    DRV_h3aClose();

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Closing IPIPE.\n");
#endif

    DRV_ipipeClose();

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Close DONE.\n");
#endif

    return OSA_SOK;
}

Uint32 GetCurTime_mSec()
{
    struct timespec tv;
    clock_gettime(CLOCK_MONOTONIC, &tv);
    return (tv.tv_sec * 1000) + (tv.tv_nsec) / (1000 * 1000);
}

int DRV_captureStart(Bool rawOutEnable, Bool yuvOutEnable)
{
#define ISIF_WAIT_TIMEOUT_MS 3000 //ms
    int timeStart, timeEnd;
    
#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Starting IPIPE.\n");
#endif

    DRV_ipipeEnable(yuvOutEnable, TRUE, TRUE, TRUE);

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Starting ISIF.\n");
#endif

    if (rawOutEnable)
    {
        DRV_isifOutEnable(TRUE);
    }
    DRV_isifEnable(TRUE);

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Starting imager.\n");
#endif

    drvImgsFunc->imgsEnable(TRUE);

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Starting isifWaitInt...\n");
#endif
    timeStart = GetCurTime_mSec();
    DRV_isifWaitInt(DRV_ISIF_INT_VD0, 2, (ISIF_WAIT_TIMEOUT_MS + 1000));
    timeEnd = GetCurTime_mSec();
    if (timeEnd - timeStart >= (ISIF_WAIT_TIMEOUT_MS))
    {
        OSA_printf("\n################################################################################\n");
	    OSA_printf("          ERROR : No Video Signal Detected!!! Timeout:%d ms\n", (timeEnd - timeStart));
	    OSA_printf("################################################################################\n\n");
        SetSensorStatus(SENSOR_ERROR);
    }
    else
    {
        OSA_printf("OK! Video Signal Detected.\n");
        SetSensorStatus(SENSOR_OK);
    }
    
#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Starting H3A.\n");
#endif

    DRV_h3aEnable(DRV_H3A_MOD_AF, TRUE);          /*AF Change*/
    DRV_h3aEnable(DRV_H3A_MOD_AEWB, TRUE);

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Start DONE.\n");
#endif

    return OSA_SOK;
}

int DRV_captureStop()
{
    DRV_isifOutEnable(FALSE);

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Stopping IPIPE.\n");
#endif
    DRV_ipipeEnable(FALSE, FALSE, FALSE, FALSE);

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Stopping H3A.\n");
#endif

    DRV_h3aEnable(DRV_H3A_MOD_AEWB, FALSE);

    OSA_waitMsecs(100);

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Stopping ISIF.\n");
#endif

    DRV_isifEnable(FALSE);

    OSA_waitMsecs(100);

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Stopping imager.\n");
#endif

    drvImgsFunc->imgsEnable(FALSE);

#ifdef DRV_SHOW_INFO
    OSA_printf(" CAPTURE: Stop DONE.\n");
#endif

    return OSA_SOK;
}


