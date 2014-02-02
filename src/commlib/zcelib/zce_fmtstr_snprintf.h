/*!
* @copyright  2004-2013  Apache License, Version 2.0 FULLSAIL
* @filename   zce_fmtstr_snprintf.h
* @author     Sailzeng <sailerzeng@gmail.com>
* @version
* @date       2013��2��6��
* @brief      ʵ��һ��C++��snprintf�������޹صģ��ٶȿ�������ͨsnprintf��30-50%��
*             ���������Ͱ�ȫ�ģ�
* 
* @details    �ڴ��ٴθ�л������С���ӣ�fastformat�����������Ҵ򿪵ģ���һֱ��ΪC++
*             �ĸ�ʽ������������������C�ġ�
*             ��������֪����fastformt����⣬�Լ����Ĳ��ִ��롣���������ź��ҷ���
*             ��ԭ������֪��ʵ����
*             C++������ʵ����ʵ�֡�
* 
* @note
* 
*/

#ifndef ZCE_FMTSTR_SNPRINTF_H_
#define ZCE_FMTSTR_SNPRINTF_H_

#include "zce_fmtstr_helper.h"

namespace ZCE_LIB
{

//��ʽ���ַ����ı�ʶ������һ��%?����ʶ����һ�β������
static const char SNRPINTF_FMT_IDENTIFY[] = "%?";
//��ʽ���ַ����ĳ���
static const size_t LEN_OF_FMT_IDENTIFY = 2;

//ת���ַ���
static const char SNRPINTF_ESCAPE_IDENTIFY[] = "%??";
//ת���ַ�������
static const size_t LEN_OF_ESCAPE_IDENTIFY = 3;
//ת���ַ������ǰ�����%?�ٳ���%���ͱ�ʶת��
static const char SNRPINTF_FMT_ESCAPE_CHAR  = '?';

//��Ϊģ�溯��д��.h�ļ��У����Ժ�Ҳж����.h�ļ����棬����ؼҿ����ܷ���export,

#define __ZCE_SNPRINTF_BEGIN   foo_use_len = 0; \
    if ( 0 == foo_max_len ) \
    { \
        return foo_buffer; \
    } \
    size_t max_len = foo_max_len-1; \
    size_t use_len = 0; \
    char *buffer = foo_buffer; \
    buffer[max_len] = '\0';   \
    const char *fmt_spec = foo_fmt_spec; \
    const char *id_pos = NULL

#define __ZCE_SNPRINTF_REPEAT(x)     while( max_len > 0) \
    {  \
        id_pos = strstr(fmt_spec,ZCE_LIB::SNRPINTF_FMT_IDENTIFY);  \
        ZCE_LIB::fmt_str(buffer,max_len,use_len,fmt_spec,(id_pos == NULL)?strlen(fmt_spec):(id_pos-fmt_spec)); \
        buffer += use_len; \
        max_len -= use_len; \
        foo_use_len += use_len; \
        fmt_spec += use_len;  \
        if (0 == max_len || id_pos == NULL ) \
        { \
            buffer[0] = '\0';  \
            return foo_buffer; \
        }  \
        if (* (id_pos + LEN_OF_FMT_IDENTIFY) != ZCE_LIB::SNRPINTF_FMT_ESCAPE_CHAR) \
        { \
            ZCE_LIB::output_helper(buffer,max_len,use_len,out_data##x);  \
            buffer += use_len; \
            max_len -= use_len; \
            foo_use_len += use_len; \
            fmt_spec += LEN_OF_FMT_IDENTIFY; \
            break; \
        } \
        else  \
        { \
            ZCE_LIB::fmt_str(buffer,max_len,use_len,SNRPINTF_FMT_IDENTIFY,LEN_OF_FMT_IDENTIFY); \
            buffer += use_len; \
            max_len -= use_len; \
            foo_use_len += use_len; \
            fmt_spec += LEN_OF_ESCAPE_IDENTIFY;  \
            continue;  \
        } \
    }

#define __ZCE_SNPRINTF_END ZCE_LIB::fmt_str(buffer, \
                                            max_len, \
                                            use_len, \
                                            fmt_spec, \
                                            strlen(fmt_spec)); \
foo_use_len += use_len; \
buffer[use_len] = '\0'; \
return foo_buffer

/*!
@brief      �������������snprintf������
            �κ������λ�ã�����%?���������������ú궨�壬��Ҫ����Ϊ�˰������ǵ��ԣ�
            ���Ժ�Ĵ��룬�㲻Ҫ���������š�
@tparam     T1  out_data1 ������
@tparam     T2  out_data2 ������
@return     char*        ���ص�BUFFERָ�룬����������һЩ����
@param[out] foo_buffer   ����ַ�����buffer
@param[in]  foo_max_len  buffer����󳤶�
@param[out] foo_use_len  ���ظ�ʽ����ʹ�õ��ַ�������
@param[in]  foo_fmt_spec ��ʽ���ַ�����format��䣬������%?��ʶһ�����λ��
@param      out_data1    ���������1
@param      out_data2    ���������2
*/
template <class T1, class T2>
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2)
{
    foo_use_len = 0;

    if ( 0 == foo_max_len )
    {
        return foo_buffer;
    }

    size_t max_len = foo_max_len - 1;
    size_t use_len = 0;
    char *buffer = foo_buffer;
    buffer[max_len] = '\0';
    const char *fmt_spec = foo_fmt_spec;

    const char *id_pos = NULL;

    //������һ�����ݵ������
    while ( max_len > 0)
    {
        //�ҵ�%?��
        id_pos = strstr(fmt_spec, ZCE_LIB::SNRPINTF_FMT_IDENTIFY);
        //��%?ǰ����ַ������
        ZCE_LIB::fmt_str(buffer, max_len, use_len, fmt_spec, (id_pos == NULL) ? strlen(fmt_spec) : (id_pos - fmt_spec));

        buffer += use_len;
        max_len -= use_len;
        foo_use_len += use_len;
        fmt_spec += use_len;

        if (0 == max_len || id_pos == NULL )
        {
            buffer[0] = '\0';
            return foo_buffer;
        }

        //�������ַ��Ƿ���?�������ж��Ƿ���ת��
        if (* (id_pos + LEN_OF_FMT_IDENTIFY) != ZCE_LIB::SNRPINTF_FMT_ESCAPE_CHAR)
        {
            ZCE_LIB::output_helper(buffer, max_len, use_len, out_data1);
            buffer += use_len;
            max_len -= use_len;
            foo_use_len += use_len;
            fmt_spec += LEN_OF_FMT_IDENTIFY;
            break;
        }
        //%??��ת��Ϊ%?���
        else
        {
            ZCE_LIB::fmt_str(buffer, max_len, use_len, SNRPINTF_FMT_IDENTIFY, LEN_OF_FMT_IDENTIFY);
            buffer += use_len;
            max_len -= use_len;
            foo_use_len += use_len;
            fmt_spec += LEN_OF_ESCAPE_IDENTIFY;
            continue;
        }
    }

    //�����ڶ������ݵ������
    while ( max_len > 0)
    {
        //�ҵ�%?��
        id_pos = strstr(fmt_spec, ZCE_LIB::SNRPINTF_FMT_IDENTIFY);
        //��%?ǰ����ַ������
        ZCE_LIB::fmt_str(buffer, max_len, use_len, fmt_spec, (id_pos == NULL) ? strlen(fmt_spec) : (id_pos - fmt_spec));

        buffer += use_len;
        max_len -= use_len;
        foo_use_len += use_len;
        fmt_spec += use_len;

        if (0 == max_len || id_pos == NULL )
        {
            buffer[0] = '\0';
            return foo_buffer;
        }

        //�������ַ��Ƿ���?�������ж��Ƿ���ת��
        if (* (id_pos + LEN_OF_FMT_IDENTIFY) != ZCE_LIB::SNRPINTF_FMT_ESCAPE_CHAR)
        {
            ZCE_LIB::output_helper(buffer, max_len, use_len, out_data2);
            buffer += use_len;
            max_len -= use_len;
            foo_use_len += use_len;
            fmt_spec += LEN_OF_FMT_IDENTIFY;
            break;
        }
        //%??��ת��Ϊ%?���
        else
        {
            ZCE_LIB::fmt_str(buffer, max_len, use_len, SNRPINTF_FMT_IDENTIFY, LEN_OF_FMT_IDENTIFY);
            buffer += use_len;
            max_len -= use_len;
            foo_use_len += use_len;
            fmt_spec += LEN_OF_ESCAPE_IDENTIFY;
            continue;
        }
    }

    ZCE_LIB::fmt_str(buffer,
                     max_len,
                     use_len,
                     fmt_spec,
                     strlen(fmt_spec));
    foo_use_len += use_len;
    buffer[use_len] = '\0';
    //����
    return foo_buffer;
}

template < class T1 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23,
                   const T24 &out_data24 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_REPEAT(24);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23,
                   const T24 &out_data24,
                   const T25 &out_data25 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_REPEAT(24);
    __ZCE_SNPRINTF_REPEAT(25);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23,
                   const T24 &out_data24,
                   const T25 &out_data25,
                   const T26 &out_data26 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_REPEAT(24);
    __ZCE_SNPRINTF_REPEAT(25);
    __ZCE_SNPRINTF_REPEAT(26);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23,
                   const T24 &out_data24,
                   const T25 &out_data25,
                   const T26 &out_data26,
                   const T27 &out_data27 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_REPEAT(24);
    __ZCE_SNPRINTF_REPEAT(25);
    __ZCE_SNPRINTF_REPEAT(26);
    __ZCE_SNPRINTF_REPEAT(27);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27, class T28 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23,
                   const T24 &out_data24,
                   const T25 &out_data25,
                   const T26 &out_data26,
                   const T27 &out_data27,
                   const T28 &out_data28 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_REPEAT(24);
    __ZCE_SNPRINTF_REPEAT(25);
    __ZCE_SNPRINTF_REPEAT(26);
    __ZCE_SNPRINTF_REPEAT(27);
    __ZCE_SNPRINTF_REPEAT(28);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27, class T28, class T29 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23,
                   const T24 &out_data24,
                   const T25 &out_data25,
                   const T26 &out_data26,
                   const T27 &out_data27,
                   const T28 &out_data28,
                   const T29 &out_data29 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_REPEAT(24);
    __ZCE_SNPRINTF_REPEAT(25);
    __ZCE_SNPRINTF_REPEAT(26);
    __ZCE_SNPRINTF_REPEAT(27);
    __ZCE_SNPRINTF_REPEAT(28);
    __ZCE_SNPRINTF_REPEAT(29);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27, class T28, class T29, class T30 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23,
                   const T24 &out_data24,
                   const T25 &out_data25,
                   const T26 &out_data26,
                   const T27 &out_data27,
                   const T28 &out_data28,
                   const T29 &out_data29,
                   const T30 &out_data30 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_REPEAT(24);
    __ZCE_SNPRINTF_REPEAT(25);
    __ZCE_SNPRINTF_REPEAT(26);
    __ZCE_SNPRINTF_REPEAT(27);
    __ZCE_SNPRINTF_REPEAT(28);
    __ZCE_SNPRINTF_REPEAT(29);
    __ZCE_SNPRINTF_REPEAT(30);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27, class T28, class T29, class T30, class T31 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23,
                   const T24 &out_data24,
                   const T25 &out_data25,
                   const T26 &out_data26,
                   const T27 &out_data27,
                   const T28 &out_data28,
                   const T29 &out_data29,
                   const T30 &out_data30,
                   const T31 &out_data31 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_REPEAT(24);
    __ZCE_SNPRINTF_REPEAT(25);
    __ZCE_SNPRINTF_REPEAT(26);
    __ZCE_SNPRINTF_REPEAT(27);
    __ZCE_SNPRINTF_REPEAT(28);
    __ZCE_SNPRINTF_REPEAT(29);
    __ZCE_SNPRINTF_REPEAT(30);
    __ZCE_SNPRINTF_REPEAT(31);
    __ZCE_SNPRINTF_END;
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26, class T27, class T28, class T29, class T30, class T31, class T32 >
char *zce_snprintf(char *foo_buffer,
                   size_t foo_max_len,
                   size_t &foo_use_len,
                   const char *foo_fmt_spec,
                   const T1 &out_data1,
                   const T2 &out_data2,
                   const T3 &out_data3,
                   const T4 &out_data4,
                   const T5 &out_data5,
                   const T6 &out_data6,
                   const T7 &out_data7,
                   const T8 &out_data8,
                   const T9 &out_data9,
                   const T10 &out_data10,
                   const T11 &out_data11,
                   const T12 &out_data12,
                   const T13 &out_data13,
                   const T14 &out_data14,
                   const T15 &out_data15,
                   const T16 &out_data16,
                   const T17 &out_data17,
                   const T18 &out_data18,
                   const T19 &out_data19,
                   const T20 &out_data20,
                   const T21 &out_data21,
                   const T22 &out_data22,
                   const T23 &out_data23,
                   const T24 &out_data24,
                   const T25 &out_data25,
                   const T26 &out_data26,
                   const T27 &out_data27,
                   const T28 &out_data28,
                   const T29 &out_data29,
                   const T30 &out_data30,
                   const T31 &out_data31,
                   const T32 &out_data32 )
{
    __ZCE_SNPRINTF_BEGIN;
    __ZCE_SNPRINTF_REPEAT(1);
    __ZCE_SNPRINTF_REPEAT(2);
    __ZCE_SNPRINTF_REPEAT(3);
    __ZCE_SNPRINTF_REPEAT(4);
    __ZCE_SNPRINTF_REPEAT(5);
    __ZCE_SNPRINTF_REPEAT(6);
    __ZCE_SNPRINTF_REPEAT(7);
    __ZCE_SNPRINTF_REPEAT(8);
    __ZCE_SNPRINTF_REPEAT(9);
    __ZCE_SNPRINTF_REPEAT(10);
    __ZCE_SNPRINTF_REPEAT(11);
    __ZCE_SNPRINTF_REPEAT(12);
    __ZCE_SNPRINTF_REPEAT(13);
    __ZCE_SNPRINTF_REPEAT(14);
    __ZCE_SNPRINTF_REPEAT(15);
    __ZCE_SNPRINTF_REPEAT(16);
    __ZCE_SNPRINTF_REPEAT(17);
    __ZCE_SNPRINTF_REPEAT(18);
    __ZCE_SNPRINTF_REPEAT(19);
    __ZCE_SNPRINTF_REPEAT(20);
    __ZCE_SNPRINTF_REPEAT(21);
    __ZCE_SNPRINTF_REPEAT(22);
    __ZCE_SNPRINTF_REPEAT(23);
    __ZCE_SNPRINTF_REPEAT(24);
    __ZCE_SNPRINTF_REPEAT(25);
    __ZCE_SNPRINTF_REPEAT(26);
    __ZCE_SNPRINTF_REPEAT(27);
    __ZCE_SNPRINTF_REPEAT(28);
    __ZCE_SNPRINTF_REPEAT(29);
    __ZCE_SNPRINTF_REPEAT(30);
    __ZCE_SNPRINTF_REPEAT(31);
    __ZCE_SNPRINTF_REPEAT(32);
    __ZCE_SNPRINTF_END;
}

};

#endif