#include <Windows.h>
#pragma once


/* 
 * This class is in charge of providing a time variable that will be 
 * execute in the gameloop.
 */
class CTime {
public:
	CTime();
	~CTime();

	void 
	init();

	void 
	update();

	void 
	render();

	void 
	destroy();

public:
	float m_deltaTime;
private:
	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_lastTime;
	LARGE_INTEGER m_timerFrequency ;
};

