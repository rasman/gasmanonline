
#include "gasgauge.h"
#include "customwidgetplugin.h"
#include "gasgauge.xpm"

#include <QtPlugin>

GasGaugePlugin::GasGaugePlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void GasGaugePlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool GasGaugePlugin::isInitialized() const
{
    return initialized;
}

QWidget *GasGaugePlugin::createWidget(QWidget *parent)
{
    return new GasGauge(parent);
}

QString GasGaugePlugin::name() const
{
    return "GasGauge";
}

QString GasGaugePlugin::group() const
{
    return "GasMan";
}

QIcon GasGaugePlugin::icon() const
{
    return QIcon(QPixmap(gasgauge));
}

QString GasGaugePlugin::toolTip() const
{
    return "";
}

QString GasGaugePlugin::whatsThis() const
{
    return "GasGauge";
}

bool GasGaugePlugin::isContainer() const
{
    return false;
}

QString GasGaugePlugin::domXml() const
{
    return "<widget class=\"GasGauge\" name=\"gasGauge\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>100</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString GasGaugePlugin::includeFile() const
{
    return "gasgauge.h";
}

Q_PLUGIN_METADATA(GasGaugePlugin)
