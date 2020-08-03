#pragma once
#include <util/util.hpp>
#include <QPointer>
#include <memory>
#include <string>
#include <vector>
#include <QtCore/QString>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSystemTrayIcon>
#include <mapper.hpp>
#include <obs.hpp>
#include "ui_OBSHotkeysWidget.h"
#include <QtWidgets/qwidget.h>
#include <QLineEdit>
#include <QKeyEvent>
#include <QPushButton>
#include <QWidget>
#include <QPointer>
#include <QLabel>

void ___source_dummy_addref(obs_source_t *);
void ___sceneitem_dummy_addref(obs_sceneitem_t *);
void ___data_dummy_addref(obs_data_t *);
void ___data_array_dummy_addref(obs_data_array_t *);
void ___output_dummy_addref(obs_output_t *);

using OBSSourceAutoRelease =
	OBSRef<obs_source_t *, ___source_dummy_addref, obs_source_release>;
using OBSSceneItemAutoRelease =
	OBSRef<obs_sceneitem_t *, ___sceneitem_dummy_addref,
	       obs_sceneitem_release>;
using OBSDataAutoRelease =
	OBSRef<obs_data_t *, ___data_dummy_addref, obs_data_release>;
using OBSDataArrayAutoRelease =
	OBSRef<obs_data_array_t *, ___data_array_dummy_addref,
	       obs_data_array_release>;
using OBSOutputAutoRelease =
	OBSRef<obs_output_t *, ___output_dummy_addref, obs_output_release>;

void ___data_item_dummy_addref(obs_data_item_t *);
void ___data_item_release(obs_data_item_t *);
using OBSDataItemAutoRelease =
	OBSRef<obs_data_item_t *, ___data_item_dummy_addref,
	       ___data_item_release>;
class OBSHotkeysWidget : public QWidget {
	Q_OBJECT
public:
	OBSHotkeysWidget();
	~OBSHotkeysWidget();
	QString hotkeycombo;

protected:
	obs_key_combination_t original;
	obs_key_combination_t key;
	bool changed = false;
	OBSSignal layoutChanged;

public slots:

	void EditTrigger(QString type, obs_data_t *trigger);
	void dothing(QKeySequence keys);
	void ResetToDefaults();

signals:
	void KeyChanged(obs_key_combination_t);
	void updated(QString type, obs_data_t *Action);

private:
	Ui::OBSHotkeysWidget *ui;
};
