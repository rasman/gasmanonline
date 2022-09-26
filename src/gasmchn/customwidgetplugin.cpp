#include "gasmchn.h"
#include "customwidgetplugin.h"
#include "gasmchn.xpm"

#include <QtPlugin>

GasMchnPlugin::GasMchnPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void GasMchnPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool GasMchnPlugin::isInitialized() const
{
    return initialized;
}

QWidget *GasMchnPlugin::createWidget(QWidget *parent)
{
    return new GasMchn(parent);
}

QString GasMchnPlugin::name() const
{
    return "GasMchn";
}

QString GasMchnPlugin::group() const
{
    return "GasMan";
}

QIcon GasMchnPlugin::icon() const
{
    return QIcon(QPixmap(gasmchn));
}

QString GasMchnPlugin::toolTip() const
{
    return "";
}

QString GasMchnPlugin::whatsThis() const
{
    return "GasMchn";
}

bool GasMchnPlugin::isContainer() const
{
    return true;
}

QString GasMchnPlugin::domXml() const
{
    return "<widget class=\"GasMchn\" name=\"gasMchn\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>300</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString GasMchnPlugin::includeFile() const
{
    return "gasmchn.h";
}

Q_PLUGIN_METADATA(GasMchnPlugin)
