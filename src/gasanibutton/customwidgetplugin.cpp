
#include "gasanibutton.h"
#include "customwidgetplugin.h"
#include "gasanibutton.xpm"

#include <QtPlugin>


GasAniButtonPlugin::GasAniButtonPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void GasAniButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool GasAniButtonPlugin::isInitialized() const
{
    return initialized;
}

QWidget *GasAniButtonPlugin::createWidget(QWidget *parent)
{
    return new GasAniButton(parent);
}

QString GasAniButtonPlugin::name() const
{
    return "GasAniButton";
}

QString GasAniButtonPlugin::group() const
{
    return "GasMan";
}

QIcon GasAniButtonPlugin::icon() const
{
    return QIcon(QPixmap(gasanibutton));
}

QString GasAniButtonPlugin::toolTip() const
{
    return "";
}

QString GasAniButtonPlugin::whatsThis() const
{
    return "GasAniButton";
}

bool GasAniButtonPlugin::isContainer() const
{
    return false;
}

QString GasAniButtonPlugin::domXml() const
{
    return "<widget class=\"GasAniButton\" name=\"gasAniButton\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>32</width>\n"
           "   <height>32</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString GasAniButtonPlugin::includeFile() const
{
    return "gasanibutton.h";
}

Q_PLUGIN_METADATA(GasAniButtonPlugin)
