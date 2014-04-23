#ifndef _UNINET_SIP_OVERFLOW
#define _UNINET_SIP_OVERFLOW

#include <time.h>

#define FLOW_LIMITION_PERCENT 1.2
#define FLOW_SLOWSTART1_PERCENT 0.3
#define FLOW_SLOWSTART2_PERCENT 0.6

struct TOverFlowInfo
{
	int 	max_caps;
	time_t  max_caps_time;
};


class COverflow
{
private:
	time_t 	m_pre_time;	//�ϴ�һ���ʱ��
	int 	m_pre_policy; //��һ�δ������

	int 	m_cur_caps;	//��ǰ����
	time_t 	m_cur_time;	//��ǰ���ʱ��

	time_t 	m_max_caps_time;	//�������������ʱ��
	int 	m_max_caps;	//�������

	int 	m_caps_limition;	//�����������

	//���ؽ׶�
	//	0=������δ�����أ��糬���������ƣ���ܾ�����������ڵĺ������
	//	1=���ؽ׶�1����һ������ڳ�����������20%������������1��������ֵΪ1������ܾ�
	//	2=���ؽ׶�2����һ�������Ϊ�׶�1��������ֵΪ2������ܾ�
	//	���ز���0 - 1 - 2 - 1 - 0
	int m_pd;			
	int m_pd_limition;
	int m_start1_limition;
	int m_start2_limition;
	int m_cur_limition;

	bool m_flow_control;
	
public:
	COverflow(int max_caps=0) 
	{
		m_caps_limition=max_caps; 
		m_cur_time=time(0); 
		m_pre_time=m_cur_time;
		m_max_caps_time=m_cur_time;
		m_cur_caps=0; 
		m_max_caps=0;
		m_pd=0;
		m_pd_limition=int(m_caps_limition*FLOW_LIMITION_PERCENT);
		m_start1_limition=int(m_caps_limition*FLOW_SLOWSTART1_PERCENT);
		m_start2_limition=int(m_caps_limition*FLOW_SLOWSTART2_PERCENT);
		m_cur_limition=m_caps_limition;
		m_pre_policy=true;
		m_flow_control=false;
	};

	void resetThreshold(int max_caps=0) 
	{
		m_caps_limition=max_caps;
		m_pd_limition=int(m_caps_limition*FLOW_LIMITION_PERCENT);
		m_start1_limition=int(m_caps_limition*FLOW_SLOWSTART1_PERCENT);
		m_start2_limition=int(m_caps_limition*FLOW_SLOWSTART2_PERCENT);
		m_cur_limition=m_caps_limition;
	};
	
	//�����������Ϣ
	void getFlowInfo(TOverFlowInfo &info)
	{
		info.max_caps=m_max_caps;
		info.max_caps_time=m_max_caps_time;
		m_max_caps=0;
		m_max_caps_time=0;
	};
	

	bool checkOverflow();
	
};

#endif




