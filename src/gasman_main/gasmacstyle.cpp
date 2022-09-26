#include "gasmacstyle.h"

int GasMacStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const
{
	if(hint == QStyle::SH_TabBar_PreferNoArrows)
		return false;
	else
        return QProxyStyle::styleHint(hint, option, widget, returnData);
}
