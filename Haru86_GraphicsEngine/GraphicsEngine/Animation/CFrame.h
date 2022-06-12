#pragma once

namespace animation {
	template<unsigned int N>
	class CFrame
	{
	public:
		float m_Value[N];
		float m_In[N];
		float m_Out[N];
		float m_Time;
	};

	typedef CFrame<1> ScalarFrame;
	typedef CFrame<3> VectorFrame;
	typedef CFrame<4> QuaternionFrame;

}