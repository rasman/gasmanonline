
#ifndef GASGAUGEPLUGIN_H
#define GASGAUGEPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class GasGaugePlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
    Q_PLUGIN_METADATA(IID "gasman.gasgauge_plugin" FILE "gasgauge_plugin.json")

public:
    GasGaugePlugin(QObject *parent = 0);

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
