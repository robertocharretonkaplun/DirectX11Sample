#include "CTime.h"

CTime::CTime() {
}

CTime::~CTime() {
}

void 
CTime::init() {
	QueryPerformanceCounter(&m_lastTime);
	QueryPerformanceFrequency(&m_timerFrequency);
}

void 
CTime::update() {
  QueryPerformanceCounter(&m_currentTime);
  m_deltaTime = (m_currentTime.QuadPart - m_lastTime.QuadPart) / (float)m_timerFrequency.QuadPart;
  m_lastTime = m_currentTime;
  m_deltaTime = min(m_deltaTime, 0.1f);
}

void 
CTime::render() {

}

void 
CTime::destroy() {

}
