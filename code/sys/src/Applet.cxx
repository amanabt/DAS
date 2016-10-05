#include "../include/Applet.h"

using namespace std;

bool Applet::first_ = true;
vector<Applet*, APPLET_UPPER_BOUND> Applet::applets_;

/************************************************************************/

void Applet::poll (void)
{
	vector<Applet*, APPLET_UPPER_BOUND>::const_iterator it;
	for (it = applets_.begin(); it != applets_.end(); ++it)
		(*it)->check();
}

void Applet::run (void)
{
	while (1) poll();
}

/************************************************************************/

Applet::Applet (void) :
    user_data_ (0),
    cb_ (default_callback)
{
	applets_.push_back (this);
}

void Applet::callback (AppletCallback cb, void* user_data)
{
	cb_ = cb;
	user_data_ = user_data;
}

void Applet::do_callback (const void* oCB)
{
	(*cb_)(user_data_, oCB);
}

void Applet::no_callback (void)
{
	cb_ = default_callback;
	user_data_ = 0;
}

/************************************************************************/
