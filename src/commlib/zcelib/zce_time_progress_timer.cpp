#include "zce_predefine.h"
#include "zce_trace_log_debug.h"
#include "zce_os_adapt_sysinfo.h"
#include "zce_os_adapt_error.h"
#include "zce_time_progress_timer.h"

/************************************************************************************************************
Class           : ZCE_Progress_Timer ���ڼ�¼һ���¼���ʱ�ļ�ʱ��
************************************************************************************************************/

ZCE_Progress_Timer::ZCE_Progress_Timer():
    start_time_(0),
    end_time_(0),
    addup_time_(0)
{
}

ZCE_Progress_Timer::~ZCE_Progress_Timer()
{
}

//���¿�ʼ��ʱ
void ZCE_Progress_Timer::restart()
{
    end_time_ = 0;
    addup_time_ = 0;
    start_time_ = std::clock();
}

//������ʱ
void ZCE_Progress_Timer::end()
{
    end_time_ = std::clock();
}

//�ۼƼ�ʱ��ʼ
void ZCE_Progress_Timer::addup_start()
{
    if (end_time_ > start_time_)
    {
        addup_time_ += end_time_ - start_time_;
    }

    end_time_ = 0;
    start_time_ = std::clock();
}

//�������ĵ�ʱ��
double ZCE_Progress_Timer::elapsed_sec() const
{
    //��ʱȥ��������ԣ���WINDOWSƽ̨����ʹ��ʹ����ȷ��Ҳ���ܳ����������,�����˵��д����ˣ��Լ��Ķ�
    //ZCE_ASSERT(end_time_ > start_time_);

    ZCE_ASSERT(end_time_ > 0);
    if (end_time_ > start_time_)
    {
        return (double(end_time_) - start_time_ + addup_time_) / CLOCKS_PER_SEC;
    }
    else
    {
        return  ( addup_time_) / CLOCKS_PER_SEC;
    }

}

//������С�ļ�ʱ���ȵ�λ��s��������ƽ̨��̫һ�£�
double ZCE_Progress_Timer::precision()
{
    return double(1) / double(CLOCKS_PER_SEC);
}

/************************************************************************************************************
Class           : ZCE_Auto_Progress_Timer ���������Զ�ֹͣ�ĵļ�ʱ��
************************************************************************************************************/

ZCE_Auto_Progress_Timer::ZCE_Auto_Progress_Timer()
{
}
ZCE_Auto_Progress_Timer::~ZCE_Auto_Progress_Timer()
{
    end();
    std::cout << "This operation use time " << std::setprecision(6) << elapsed_sec() << " second." << std::endl;
}

/************************************************************************************************************
Class           : ZCE_HR_Progress_Timer �����ܼ�ʱ��
************************************************************************************************************/
ZCE_HR_Progress_Timer::ZCE_HR_Progress_Timer()
{

#if defined ZCE_OS_WINDOWS

    start_time_.QuadPart = 0;
    end_time_.QuadPart = 0;
    addup_time_.QuadPart = 0;
    frequency_.QuadPart = 0;
    old_affinity_mask_ = 0;
#elif defined ZCE_OS_LINUX

    start_time_.tv_sec = 0;
    start_time_.tv_nsec = 0;
    end_time_.tv_sec = 0;
    end_time_.tv_nsec = 0;
    addup_time_ = 0;

    int ret = ::clock_getres(CLOCK_MONOTONIC, &precision_);
    if (ret != 0)
    {
        ZLOG_ERROR("::clock_getres return fail. error is %d.", ZCE_OS::last_error());
    }

#endif
}

ZCE_HR_Progress_Timer::~ZCE_HR_Progress_Timer()
{
}

//���¿�ʼ��ʱ
void ZCE_HR_Progress_Timer::restart()
{
#if defined ZCE_OS_WINDOWS
    //������ò��ɹ����᷵��0
    old_affinity_mask_ = ::SetThreadAffinityMask(GetCurrentThread(), ONLY_YOU_PROCESSOR);
    //
    ::QueryPerformanceFrequency(&frequency_);
    ::QueryPerformanceCounter(&start_time_);
    end_time_.QuadPart = 0;
    addup_time_.QuadPart = 0;
#elif defined ZCE_OS_LINUX

    int ret = ::clock_gettime(CLOCK_MONOTONIC, &start_time_);
    if (ret != 0)
    {
        ZLOG_ERROR("::clock_gettime return fail. error is %d.", ZCE_OS::last_error());
    }

    end_time_.tv_sec = 0;
    end_time_.tv_nsec = 0;
    addup_time_ = 0;

#endif
}

//�ۼƼ�ʱ��ʼ,���ڶ�μ�ʱ�Ĺ��̣�
void ZCE_HR_Progress_Timer::addup_start()
{
#if defined ZCE_OS_WINDOWS

    addup_time_.QuadPart += end_time_.QuadPart - start_time_.QuadPart;
    //������ò��ɹ����᷵��0
    old_affinity_mask_ = ::SetThreadAffinityMask(GetCurrentThread(), ONLY_YOU_PROCESSOR);
    //
    ::QueryPerformanceFrequency(&frequency_);
    ::QueryPerformanceCounter(&start_time_);
    end_time_.QuadPart = 0;

#elif defined ZCE_OS_LINUX

    const uint64_t NSEC_PER_SEC = 1000000000ULL;

    addup_time_ += ((end_time_.tv_sec * NSEC_PER_SEC  + end_time_.tv_nsec ) -
                    (start_time_.tv_sec * NSEC_PER_SEC + start_time_.tv_nsec ));

    int ret = ::clock_gettime(CLOCK_MONOTONIC, &start_time_);
    if (ret != 0)
    {
        ZLOG_ERROR("::clock_gettime return fail. error is %d.", ZCE_OS::last_error());
    }

    end_time_.tv_sec = 0;
    end_time_.tv_nsec = 0;

#endif
}

//������ʱ
void ZCE_HR_Progress_Timer::end()
{
#if defined ZCE_OS_WINDOWS

    ::QueryPerformanceCounter(&end_time_);
    //��ԭ�̵߳Ĵ������󶨹�ϵ
    if (old_affinity_mask_ != 0)
    {
        ::SetThreadAffinityMask(GetCurrentThread(), old_affinity_mask_);
    }

#elif defined ZCE_OS_LINUX

    int ret = ::clock_gettime(CLOCK_MONOTONIC, &end_time_);
    if (ret != 0)
    {
        ZLOG_ERROR("::clock_gettime return fail. error is %d.", ZCE_OS::last_error());
    }
#endif
}

//�������ĵ�ʱ��(us)
double ZCE_HR_Progress_Timer::elapsed_usec() const
{

#if defined ZCE_OS_WINDOWS

    ZCE_ASSERT(end_time_.QuadPart >= start_time_.QuadPart);
    const double USEC_PER_SEC = 1000000.0;
    return double(end_time_.QuadPart - start_time_.QuadPart + addup_time_.QuadPart ) * USEC_PER_SEC / frequency_.QuadPart;
#elif defined ZCE_OS_LINUX

    const uint64_t NSEC_PER_SEC = 1000000000ULL;

    ZCE_ASSERT((end_time_.tv_sec * NSEC_PER_SEC  + end_time_.tv_nsec ) >
               (start_time_.tv_sec * NSEC_PER_SEC + start_time_.tv_nsec ));
    const double NSEC_PER_USEC = 1000.0;

    return ((end_time_.tv_sec * NSEC_PER_SEC  + end_time_.tv_nsec ) -
            (start_time_.tv_sec * NSEC_PER_SEC + start_time_.tv_nsec ) + addup_time_) / NSEC_PER_USEC ;
#endif
}

//�õ���ʱ���ľ��ȣ�us΢��-6��
double ZCE_HR_Progress_Timer::precision_usec()
{
#if defined ZCE_OS_WINDOWS
    const double USEC_PER_SEC = 1000000.0;
    return USEC_PER_SEC / ((uint64_t)( frequency_.QuadPart));
#elif defined ZCE_OS_LINUX
    const double NSEC_PER_USEC = 1000.0;
    const uint64_t NSEC_PER_SEC = 1000000000LL;
    return (precision_.tv_sec * NSEC_PER_SEC  + precision_.tv_nsec ) * NSEC_PER_USEC ;
#endif
}

/************************************************************************************************************
Class           : ZCE_TSC_Progress_Timer TSC��ʱ����
************************************************************************************************************/
uint64_t ZCE_TSC_Progress_Timer::cpu_hz_ = 0;

//���캯��
ZCE_TSC_Progress_Timer::ZCE_TSC_Progress_Timer():
    start_time_(0),
    end_time_(0),
    addup_time_(0)
{
}

//��������
ZCE_TSC_Progress_Timer::~ZCE_TSC_Progress_Timer()
{
}

///���¿�ʼ��ʱ
void ZCE_TSC_Progress_Timer::restart()
{
    end_time_ = 0;
    addup_time_ = 0;
    start_time_ = ZCE_OS::rdtsc();
}

//������ʱ
void ZCE_TSC_Progress_Timer::end()
{
    end_time_ = ZCE_OS::rdtsc();
}

///�ۼƼ�ʱ��ʼ,���ڶ�μ�ʱ�Ĺ��̣�
void ZCE_TSC_Progress_Timer::addup_start()
{
    if (end_time_ > start_time_)
    {
        addup_time_ += end_time_ - start_time_;
    }
    //����һЩ������������� start_time_ > end_time_,���������ڲ�ͬ��CPU�ϼ�ʱ��,

    end_time_ = 0;
    start_time_ = ZCE_OS::rdtsc();
}

//�������ĵ�TICK��CPU���ڣ�������ע�����ֵ��ֻ�����Լ��Ļ��������ԱȲ������壬
uint64_t ZCE_TSC_Progress_Timer::elapsed_tick() const
{
    if (end_time_ > start_time_)
    {
        return end_time_ - start_time_ + addup_time_;
    }
    return addup_time_;
}

//�������ĵ�ʱ��(us),ע�������ֵ����̫׼ȷ
double ZCE_TSC_Progress_Timer::elapsed_usec() const
{
    const uint64_t DEFAULT_CPU_HZ = 1024 * 1024 * 1024;
    int ret = 0;
    //�����̬����û�г�ʼ��
    if ( 0 == cpu_hz_ )
    {
        ZCE_SYSTEM_INFO system_info;
        ret = ZCE_OS::get_system_info(&system_info);
        if (0 == ret )
        {
            cpu_hz_ = system_info.cpu_hz_;
        }
        else
        {
            ZLOG_ERROR("ZCE_OS::get_system_info return fail. cpu use default 1G.");
            //��1G��Ϊ��ΪĬ��ֵ
            cpu_hz_ = DEFAULT_CPU_HZ;
        }
    }
    const uint64_t USEC_PER_SEC = 1000 * 1000;
    return double (end_time_ - start_time_ + addup_time_) / double(cpu_hz_ * USEC_PER_SEC );
}
