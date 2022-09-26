
#ifndef GASPTRLIST_H
#define GASPTRLIST_H

#include <QList>
#include <QDataStream>

//The GasPtrList class is a template class that provides a list with an auto-delete feature

//If auto-deleting is turned on, all the items in a collection are deleted 
//when the collection itself is deleted. The default setting is true

template <typename T>
class GasPtrList : public QList<T *>
{
public:
	GasPtrList() : QList<T *>(), autoDelete( true ) {}
	GasPtrList( const GasPtrList<T *> & other ) : QList<T *>( other ), autoDelete( true ) {}
	~GasPtrList() { clear(); }

	inline void setAutoDelete( bool del ) { autoDelete = del; }
    //inline void isAutoDelete() const { return autoDelete; }

	void Serialize( QDataStream &stream, bool storing, quint32 serializationFlags = 0 )
	{
		if ( storing ) {
            int m_nSize = GasPtrList::size();
			stream << m_nSize;
			for ( int i = 0; i < m_nSize; ++i )
				GasPtrList::at(i)->Serialize( stream, storing, serializationFlags );
		}
		else {
			int m_nSize;
			stream >> m_nSize;
			for (int i = 0; i < m_nSize; ++i ) {
				GasPtrList::append( new T );
				GasPtrList::at(i)->Serialize( stream, storing, serializationFlags );
			}
		}
	}

	void clear() {
		if ( !autoDelete ) {
			QList<T *>::clear();
			return;
		}
		qDeleteAll( *this );
		QList<T *>::clear();
	}

private:
	bool autoDelete;
};

#endif
