#include <QProxyStyle>

class GasMacStyle : public QProxyStyle
{
	Q_OBJECT

public:
	GasMacStyle(){}
	~GasMacStyle(){}
	int styleHint( StyleHint hint, const QStyleOption * option = 0, const QWidget * widget = 0, QStyleHintReturn * returnData = 0 ) const;
};
