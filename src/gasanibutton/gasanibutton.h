
#ifndef GASANIBUTTON_H
#define GASANIBUTTON_H

#include <QAbstractButton>

class GasAniButton : public QAbstractButton
{
    Q_OBJECT
    //Declares the properties
    Q_PROPERTY( QPixmap DownPixmap READ GetSryngeDown WRITE SetSryngeDown )
    Q_PROPERTY( QPixmap UpPixmap READ GetSryngeUp WRITE SetSryngeUp )
public:
    GasAniButton( QWidget *parent = 0 );

    //set/get syringe down icon
    inline const QPixmap &GetSryngeDown() const { return  srynge_down; }
    inline void SetSryngeDown( const QPixmap &pixmap ) { srynge_down = pixmap; }

    //set/get syringe up icon
    inline const QPixmap &GetSryngeUp() const { return srynge_up; }
    inline void SetSryngeUp( const QPixmap &pixmap ) { srynge_up = pixmap; }

protected:
    virtual void paintEvent( QPaintEvent *evnt );

private:
    QPixmap srynge_down;		//Syringe down icon
    QPixmap srynge_up;			//Syringe up icon
};

#endif
