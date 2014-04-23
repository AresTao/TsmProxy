
#include "sipoverflow.h"

bool COverflow::checkOverflow() 
{
	m_cur_time=time(0); 
	m_cur_caps++;
		
	int i=0;
	i=m_cur_time-m_pre_time;
	if(i==0)
	{
		if(m_caps_limition<=0) return true;
		//���������
		if(m_flow_control) return false;
		
		if(m_cur_caps>=m_cur_limition && m_cur_caps< m_pd_limition)
		{
			m_pre_policy=!m_pre_policy;
			return m_pre_policy;
		}
		if(m_cur_caps>=m_pd_limition)
		{
			m_flow_control=true;
			return false;
		}
		return true;
	}

	//�µļ������
	if(m_cur_caps>m_max_caps)	//�������������¼
	{
		m_max_caps_time=m_pre_time;
		m_max_caps=m_cur_caps-1;
	}

	m_cur_caps=1;
	m_pre_time=m_cur_time;
	
	if(i==1)
	{
	//���һ���������
		if(m_flow_control)
		{ 
			m_pd=1;
			m_cur_limition=m_start1_limition;
		}
		else if(m_pd==1) 
		{
			m_pd=2;
			m_cur_limition=m_start2_limition;
		}
		else if(m_pd==2) 
		{
			m_pd=0;
			m_cur_limition=m_caps_limition;
		}
	}
	else if(i==2)
	{
		//�µļ�����ڣ�������һ�����
		if(m_flow_control)
		{ 
			m_pd=2;
			m_cur_limition=m_start2_limition;
		}
		else if(m_pd) 
		{
			m_pd=0;
			m_cur_limition=m_caps_limition;
		}
	}
	else if(i>2)
	{
		//�µļ�����ڣ�������һ������ϳ�
		m_pd=0;
		m_cur_limition=m_caps_limition;
	}
	
	m_flow_control=false;
	return true;

}


