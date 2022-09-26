#ifndef GASEVENT_H
#define GASEVENT_H

#include <QEvent>
#include <QStack>
#include <QWidget>

class GasDoc;
class QLineEdit;

enum GasQEvent
{
	GasUpdateQEvent = QEvent::User + 1,
	GasCallbackQEvent
};

class GasUpdateEvent : public QEvent
{
public:
	enum AnEvent			
	{
		UPD_NONE = 0,					// these are passed to
		UPD_AGENT = 1,					// customEvent as hints
		UPD_CIRCUIT = UPD_AGENT << 1,
		UPD_SPEED = UPD_CIRCUIT << 1,
		UPD_DEL = UPD_SPEED << 1,
		UPD_FGF = UPD_DEL << 1,
		UPD_VA = UPD_FGF << 1,
		UPD_CO = UPD_VA << 1,
		UPD_TICK = UPD_CO << 1,
		UPD_STATE = UPD_TICK << 1,
		UPD_WEIGHT = UPD_STATE << 1,
		UPD_INJECT = UPD_WEIGHT << 1,
		UPD_NGAS = UPD_INJECT << 1
	};

	GasUpdateEvent( AnEvent t, int n, GasDoc *pDoc ) : QEvent( (QEvent::Type)GasUpdateQEvent ), m_pDocument( pDoc ), nGas( n ) { m_type = t; }

	inline GasDoc *document() const { return m_pDocument; }
	inline AnEvent type() const { return m_type; }
	inline int gas() const { return nGas; }

private:
	AnEvent m_type;
	GasDoc *m_pDocument;
	int nGas;
};

/*
	Callback helper template class

	This helper class facilitates an ability for a QObject (of subclass T) to call
	itself back in spare time.

	To use:
		1) Create a GasCallbackEvent<T> (a type of QEvent)
		2) Push any arguments required by the callback onto the GasCallbackEvent<T> stack
		3) Post the event to the creating QObject via the message queue for the correct
		   thread (usually the application), by calling 'postEvent()'

		   (...and go about further business...)

		4) Call the static GasCallbackEvent<T>::TryCallback() member from an override of
		   QOject::customEvent() in the creating QObject
		5) Pop argments in the callback function
		6) Don't forget to 'delete' anything allocated and pushed in (2)
*/
template<class T>
class GasCallbackEvent : public QEvent
{
public:
	// Type of a pointer to the member function that gets called back. The first argument
	// in the signature is for convenience, in the common case that the callback should be
	// passed some widget. The second argument points back to this event and is used to
	// get the remaining parameters.
	typedef void (T::*pfnCallback)( QWidget *pWidget, GasCallbackEvent<T> *pCallbackEvent );

	// Constructor: Passed a callback member function and the widget called back.
	GasCallbackEvent( pfnCallback pCallback, QWidget *pWidget )
		: QEvent( (QEvent::Type)GasCallbackQEvent )
		, m_pCallback( pCallback )
		, m_pWidget( pWidget )
	{}

	// Call from an override of QObject::customEvent().  Calls the callback and
	// returns 'true' if passed a GasCallbackEvent<T>, else returns 'false'
	//
	// Pass a pointer to an object of type T and a candidate QEvent
	static bool TryCallback(T* pObject, QEvent *pEvent)
	{
		if ( pEvent->type() != (QEvent::Type)GasCallbackQEvent )
			return false;
		GasCallbackEvent<T> *p = dynamic_cast<GasCallbackEvent<T> *>( pEvent );
		if ( p == NULL )
			return false;
		p -> CallCallback(pObject);
		return true;
	}

	// Push an argument for the callback onto a stack
	inline void PushArg(void *arg) { m_args.push(arg); }

	// Pop an argument for the callback from the stack
	inline void *PopArg() { return m_args.isEmpty() ? NULL : m_args.pop(); }

private:
	QStack<void *> m_args;
	inline void CallCallback(T* pObject){ (pObject->*m_pCallback)( m_pWidget, this ); }
	pfnCallback m_pCallback;
	QWidget *m_pWidget;
};

#endif
