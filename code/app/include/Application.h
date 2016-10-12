#ifndef __APPLICATION__
#define __APPLICATION__

#include "../include/Comm.h"

#include "../../sys/include/Applet.h"
// #include "../../sys/include/Timer.h"

#include <memory>

class Application : public Applet
{
public:
	static Application* _ (void);
	void run (void);

private:
	Application (void);

private:
	static void appCommCB         (void* user_data, const void* vCB);
	void appCommCB                (const CommCB* oCB);

	static void progressCB        (void* user_data, uint16_t progress, uint16_t full);
	void progressCB               (uint16_t progress, uint16_t full);

	void nopCB                    (const CommCB* oCB);
	void identityCB               (const CommCB* oCB);

	void keepAliveCB              (const CommCB* oCB);

protected:
	virtual void check (void);

private:
	bool isSessionActive (void) const {
		return _active_session;
	}
	void monitorSession (void);
	void endSession (void);

private:
	Comm*                    _comm;
// 	Timer*                   _timer;
	
	struct Session
	{
		Session (uint16_t id_) :
			id (id_)
		{}
		
		uint16_t id;
	};

private:
	bool _active_session;

// private:
// 	Timer::Tick _end_session_after;
};

#define application    Application::_()

#endif
