
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
		//检测周期内
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

	//新的检测周期
	if(m_cur_caps>m_max_caps)	//更新最大流量记录
	{
		m_max_caps_time=m_pre_time;
		m_max_caps=m_cur_caps-1;
	}

	m_cur_caps=1;
	m_pre_time=m_cur_time;
	
	if(i==1)
	{
	//间隔一个检测周期
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
		//新的检测周期，且与上一个间隔
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
		//新的检测周期，且与上一个间隔较长
		m_pd=0;
		m_cur_limition=m_caps_limition;
	}
	
	m_flow_control=false;
	return true;

}



