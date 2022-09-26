#ifndef GASMCHNPLUGIN_H
#define GASMCHNPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class GasMchnPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
    Q_PLUGIN_METADATA(IID "gasman.gasmchn_plugin" FILE "gasmchn_plugin.json")

public:
    GasMchnPlugin(QObject *parent = 0);

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    bool initialized;
};

#endif
