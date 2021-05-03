#pragma once


namespace Wyrd
{
	/**
	 * @brief Timestep interval class
	*/
	class Timestep
	{
	public:
		/**
		 * @brief constructure
		 * @param time 
		*/
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		/**
		 * @brief float type override
		*/
		operator float() const { return m_Time; }

		/**
		 * @brief Retrieve the current timestep in seconds
		 * @return float - timestep in seconds
		*/
		inline float GetSeconds() const { return m_Time; }

		/**
		 * @brief Retreive the current timestep in milliseconds
		 * @return float - timestep in milliseconds
		*/
		inline float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}