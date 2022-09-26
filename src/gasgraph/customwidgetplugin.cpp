
#include "gasgraph.h"
#include "customwidgetplugin.h"
#include "gasgraph.xpm"

#include <QtPlugin>

GasGraphPlugin::GasGraphPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void GasGraphPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool GasGraphPlugin::isInitialized() const
{
    return initialized;
}

QWidget *GasGraphPlugin::createWidget(QWidget *parent)
{
    return new GasGraph(parent);
}

QString GasGraphPlugin::name() const
{
    return "GasGraph";
}

QString GasGraphPlugin::group() const
{
    return "GasMan";
}

QIcon GasGraphPlugin::icon() const
{
    return QIcon(QPixmap(gasgraph));
}

QString GasGraphPlugin::toolTip() const
{
    return "";
}

QString GasGraphPlugin::whatsThis() const
{
    return "GasGraph";
}

bool GasGraphPlugin::isContainer() const
{
    return false;
}

QString GasGraphPlugin::domXml() const
{
    return "<widget class=\"GasGraph\" name=\"gasGraph\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>350</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString GasGraphPlugin::includeFile() const
{
    return "gasgraph.h";
}

Q_PLUGIN_METADATA(GasGraphPlugin)
