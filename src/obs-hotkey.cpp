
#include <iostream>
#include <obs-module.h>
#if __has_include(<obs-frontend-api.h>)
#include <obs-frontend-api.h>
#else
#include <obs-frontend-api/obs-frontend-api.h>
#endif
#include <obs-data.h>
#include <string>
#include <map>
#include <iostream>
#include <utility>
#include "obs-hotkey.h"
#include "forms/obs-hotkeys-widget.hpp"
#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QIcon>
#include <util/dstr.h>
#include "qt-wrappers.hpp"
#include "mapper.hpp"
using namespace std;


OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-hotkeys", "en-US")


void register_gui()
{
	QWidget *input = new OBSHotkeysWidget();
	QString *na = new QString("Hotkeys");
	obs_frontend_add_input_control(na, input);
}

bool obs_module_load(void)
{
	HKC *HotkeyController = new HKC();
	register_gui();
	return true;
}
void obs_module_unload()
{

	blog(LOG_INFO, "goodbye!");
}

const char *tocchar(QKeySequence string)
{
	return string.toString().simplified().toStdString().c_str();
}

	/**********************Hotkey Controller************ MAKE INTO PLUGIN ********************************/

HKC::HKC()
{
	ControlMapper *mapper = (ControlMapper *)obs_frontend_get_mapper();

	connect(mapper, SIGNAL(mapLoadAction(obs_data_t *)), this,
		SLOT(loadmap(obs_data_t *)));
	connect(this, SIGNAL(LoadMappings()), mapper, SLOT(LoadMapping()));
	emit(LoadMappings());
	
	connect(this, SIGNAL(Trigger(QString, obs_data_t *)), mapper,
		SLOT(TriggerEvent(QString, obs_data_t *)));
	obs_hotkey_enable_background_press(true);
	blog(LOG_INFO, "Hotkey Controller Loaded");
}

void HKC::AddHK(QKeySequence ks)
{
	if (!pressmap.contains(ks)) {
		auto x = new KPair();
		x->Sequence = ks;
		x->pressed = false;
		x->hotkey = new QHotkey(QKeySequence(ks), true, this);
		pressmap.insert(ks, x);
		qDebug() << "Is Registered: " << x->hotkey->isRegistered();
		QObject::connect(x->hotkey, &QHotkey::activated, this,
				 &HKC::DoQHK);
	}
}

void HKC::DoQHK()
{

	obs_data_t *data = obs_data_create();
	
	QHotkey *x = qobject_cast<QHotkey *>(sender());
	auto y = x->shortcut();
	blog(1, "Qhotkey Pressed  -- %s",
	     y.toString().simplified().toStdString().c_str());
	obs_data_set_string(
		data, "Hotkey",y.toString().simplified().toStdString().c_str());
	obs_data_set_string(data, "Type", "Hotkeys");
	emit(Trigger("Hotkeys", data));
}


void HKC::loadmap(obs_data_t *map)
{
	obs_data_t *ts =obs_data_create_from_json(obs_data_get_string(map, "trigger"));
	if (QString(obs_data_get_string(ts, "Type")) ==
	    QString("Hotkeys")) {
	

		AddHK(QKeySequence(obs_data_get_string(ts, "Hotkey")));
		blog(LOG_INFO, "add hotkey, %s",
		     obs_data_get_string(ts, "Hotkey"));
	}
}

HKC::~HKC()
{

	QMapIterator<QKeySequence, KPair *> i(pressmap);
	while (i.hasNext()) {
		i.next();
		i.value()->hotkey->~QHotkey();
	}
	delete this;
}
