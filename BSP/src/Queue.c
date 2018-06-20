

/************************************************************
  * ��Ȩ����(C)2014
  * ��Ŀ����:
  * �ļ�����:       Queue.c
  * ����ƽ̨:       STM32F103VC
  * ���뻷��:       Keil 4.72 for Arm
  * ��������:       c
  * ����ժҪ:
  *----------------------------------------------------------
  * ��    ��:       .���
  * ��������:
  * �� �� ��:       v1.0
  * ��    ��:
  *----------------------------------------------------------
  * �� �� ��:
  * �޸�����:
  * ��    ��:
************************************************************/
#include <includes.h>

/**********************************************************************
  * ��������:   char * strRev (char * str)
  * ��������:   �ַ�����ת
  * �������:   Ҫ��ת���ַ���
  * �������:   str
  * �� �� ֵ:   char * str
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:
  * ����˵��:
************************************************************************/
char * strRev( char * str )
{
    char * p1 = NULL;
    char * p2 = NULL;

    if ( !str || ! *str )
    {
        return str;
    }

    for ( p1 = str, p2 = str + strlen( str ) - 1; p2 > p1; ++p1, --p2 )
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }

    return str;
}

/**********************************************************************
  * ��������:   char * ftoa (int n, char *s, int b)
  * ��������:   ����������ת����ASCII
  * �������:   int n:�Ѿ�ת��Ϊ���͵ĸ�����(100��), char *s:Ҫ�������ݵ�����, int b:ת����Ŀ�����
  * �������:   char *s
  * �� �� ֵ:   char *s
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:
  * ����˵��:
************************************************************************/
char * ftoa( int n, char * s, int b )
{
    static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i = 0, sign = 0;
    int temp = 0;

    if ( ( sign = n ) < 0 )
    {
        n   = -n;
    }

    temp = n;

    do
    {
        // ������������Ǳ��Ŵ���100����,���Լ�������2λС��
        if ( 2 == i )
        {
            s[i++] = '.';
        }

        s[i++] = digits[n % b];
    }
    while( ( n /= b ) > 0 );

    // �����ʱС��10,����Ϊ����0,��С����,һ��ǰ��0
    if ( temp < b )
    {
        s[i++] = '0';
        s[i++] = '.';
        s[i++] = '0';
    }

    // �����ʱ����С��100,����10,����Ϊ����С����,��һ��ǰ��0
    if ( temp >= b && temp < ( b * b ) )
    {
        s[i++] = '.';
        s[i++] = '0';
    }

    // �������С��0,��Ҳ��Ϊ��ǰ�����'-'
    if ( sign < 0 )
    {
        s[i++] = '-';
    }

    s[i] = '\0';
    return strRev( s );
}

/**********************************************************************
  * ��������:   u8 canInitQueue (CanQueue * p_tQueue)
  * ��������:   ��ʼ������
  * �������:   CanQueue * tRxQueue: ����ָ��
  * �������:
  * �� �� ֵ:   �ɹ�Ϊ0
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   20140512
  * ����˵��:   ��
************************************************************************/
u8 canInitQueue( CanQueue * p_tQueue )
{
    memset( p_tQueue, 0, sizeof( CanQueue ) );
    p_tQueue->empty = 1;                // ����Ϊ��
    p_tQueue->full = 0;                 // ����Ϊ��
    p_tQueue->top = 0;                  // ���е��±�ָ��
    p_tQueue->bottom = 0;               // ���е�Ŀ��ָ��
    return 0;
}

/**********************************************************************
  * ��������:   u8 canInQueue (CanQueue * const p_tQueue, CanRxMsg * const p_tNewNode)
  * ��������:   ���������,�������õ�Ϊѭ������,���д�СΪRX_DATA_BUFF_SIZE,�����ݴ�����,����ͷ������ɾ��
  * �������:   CanQueue * tRxQueue: ����ָ��, CanRxMsg * const p_tNewNode:��������е�����ָ��
  * �������:
  * �� �� ֵ:   �ɹ�Ϊ0, ���ɹ�Ϊ-1����-2
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   20140512
  * ����˵��:   ��
************************************************************************/
u8 canInQueue( CanQueue * const p_tQueue, CanRxMsg * const p_tNewNode )
{
    int i = 0;

    // ���е�ַ��Ч
    if ( !p_tQueue )
        return 1;

    // ��������ݵ�ַ��Ч
    if ( !p_tNewNode )
        return 1;

    // ����������һ��Ԫ�ؽ��бȽ�,��������������ȫһ��,�����ݲ������,ֱ�ӷ���,���������ظ�����

    /*
    if (1 != p_tQueue->empty)
    {
        for (i = 0; i < 8; i++)
        {
            // ����������,������Ƚ�,ֱ�����һ������
            if (p_tQueue->news[p_tQueue->top - 1].Data[i] == p_tNewNode->Data[i])
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
    // ���Ƚϵ����һ�����ݻ������,��ֱ�ӷ���
    if (8 == i)
    {
        return 2;
    }
    */
    if ( ( ( p_tQueue->top + 1 ) % RX_DATA_BUFF_SIZE ) == p_tQueue->bottom )
    {
        // ѭ������
        p_tQueue->bottom = ( p_tQueue->bottom + 1 ) % RX_DATA_BUFF_SIZE;

        // ������
        p_tQueue->full = 1;
    }

    // ���ݽ������
    p_tQueue->news[p_tQueue->top] = *p_tNewNode;
    p_tQueue->top = ( p_tQueue->top + 1 ) % RX_DATA_BUFF_SIZE;
    p_tQueue->empty = 0;
    return 0;
}

/**********************************************************************
  * ��������:   u8 canOutQueue (CanQueue * const p_tQueue, CanRxMsg * p_tReturnNode)
  * ��������:   ������
  * �������:   CanQueue * const p_tQueue:�����׵�ַ, CanRxMsg * p_tReturnNode:������Ԫ�ر����ַ
  * �������:   ������Ԫ��
  * �� �� ֵ:   �ɹ�Ϊ0
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:
  * ����˵��:
************************************************************************/
u8 canOutQueue( CanQueue * const p_tQueue, CanRxMsg * p_tReturnNode )
{
    if ( !p_tQueue )
        return 1;

    if ( !p_tReturnNode )
        return 1;

    if ( 1 == p_tQueue->empty )
        return 1;

    // �ն���
    if ( ( p_tQueue->bottom % RX_DATA_BUFF_SIZE ) == p_tQueue->top )
    {
        p_tQueue->empty = 1;
        return 1;
    }

    CAN_ITConfig(CAN1,CAN_IT_FMP0, DISABLE);
    // ���ݳ�����
    *p_tReturnNode = p_tQueue->news[p_tQueue->bottom];
    p_tQueue->bottom = ( p_tQueue->bottom + 1 ) % RX_DATA_BUFF_SIZE;
    p_tQueue->full = 0;
    CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);

    return 0;
}

/**********************************************************************
  * ��������:   u8 uartInitQueue (UartQueue * tQueue)
  * ��������:   ��ʼ������
  * �������:   CanQueue * tRxQueue: ����ָ��
  * �������:
  * �� �� ֵ:   �ɹ�Ϊ0
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   20140512
  * ����˵��:   ��
************************************************************************/
u8 uartInitQueue( UartQueue * p_tQueue )
{
    memset( p_tQueue, 0, sizeof( UartQueue ) );
    p_tQueue->empty = 1;                // ����Ϊ��
    p_tQueue->full = 0;                 // ����Ϊ��
    p_tQueue->top = 0;                  // ���е��±�ָ��
    p_tQueue->bottom = 0;               // ���е�Ŀ��ָ��
    return 0;
}

/**********************************************************************
  * ��������:   u8 canInQueue (CanQueue * const p_tQueue, CanRxMsg * const p_tNewNode)
  * ��������:   ���������,�������õ�Ϊѭ������,���д�СΪRX_DATA_BUFF_SIZE,�����ݴ�����,����ͷ������ɾ��
  * �������:   CanQueue * tRxQueue: ����ָ��, CanRxMsg * const p_tNewNode:��������е�����ָ��
  * �������:
  * �� �� ֵ:   �ɹ�Ϊ0, ���ɹ�Ϊ-1����-2
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:   20140512
  * ����˵��:   ��
************************************************************************/
u8 uartInQueue( UartQueue * const p_tQueue, u8 * const p_ucaNewNode )
{
    int i = 0;

    // ���е�ַ��Ч
    if ( !p_tQueue )
        return 1;

    // ��������ݵ�ַ��Ч
    if ( !p_ucaNewNode )
        return 1;

    if ( ( ( p_tQueue->top + 1 ) % RX_DATA_BUFF_SIZE ) == p_tQueue->bottom )
    {
        // ѭ������
        p_tQueue->bottom = ( p_tQueue->bottom + 1 ) % RX_DATA_BUFF_SIZE;

        // ������
        p_tQueue->full = 1;
    }

    // ���ݽ������
    //p_tQueue->news[p_tQueue->top] = *p_ucaNewNode;
    strcpy( p_tQueue->news[p_tQueue->top], p_ucaNewNode );
    p_tQueue->top = ( p_tQueue->top + 1 ) % RX_DATA_BUFF_SIZE;
    p_tQueue->empty = 0;
    return 0;
}

/**********************************************************************
  * ��������:   u8 canOutQueue (CanQueue * const p_tQueue, CanRxMsg * p_tReturnNode)
  * ��������:   ������
  * �������:   CanQueue * const p_tQueue:�����׵�ַ, CanRxMsg * p_tReturnNode:������Ԫ�ر����ַ
  * �������:   ������Ԫ��
  * �� �� ֵ:   �ɹ�Ϊ0
  * �� �� ��:   ���
  * ����ʱ��:   20140512
  * �޸�ʱ��:
  * ����˵��:
************************************************************************/
u8 uartOutQueue( UartQueue * const p_tQueue, u8 * p_ucaReturnNode )
{
    if ( !p_tQueue )
        return 1;

    if ( !p_ucaReturnNode )
        return 1;

    if ( 1 == p_tQueue->empty )
        return 1;

    // �ն���
    if ( ( p_tQueue->bottom % RX_DATA_BUFF_SIZE ) == p_tQueue->top )
    {
        p_tQueue->empty = 1;
        return 1;
    }
    // ���ݳ�����
    strcpy( p_ucaReturnNode, p_tQueue->news[p_tQueue->bottom] );
    p_tQueue->bottom = ( p_tQueue->bottom + 1 ) % RX_DATA_BUFF_SIZE;
    p_tQueue->full = 0;

    return 0;
}
