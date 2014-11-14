#ifndef _SAX_SERIALIZER_BASE
#define _SAX_SERIALIZER_BASE


#include "cfixedpointround.h"

#include <QDateTime>
#include <QTime>
#include <QString>
#include <QVector>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

class SaxSerializerBase
{
public:

	/*
	// Название открывающего/закрывающего тэга
	*/
	SaxSerializerBase(const QString &__tag_name);

	void write (QXmlStreamWriter& __writer);
	void read (QXmlStreamReader& __reader);

	QString tagName();

	/*
	//	Сериализация объекта.
	*/
	virtual void serialize (QXmlStreamWriter& __writer) = 0;
	/*
	//	Десериализация объекта.
	*/
	virtual void deserialize (QXmlStreamReader& __reader) = 0;

protected:
	/*
	//	Пропускает пустые символы в потоке. 
	*/
	static void skipEmpty (QXmlStreamReader& __reader);
	/*
	//	Вычитывает первый открывающий тэг из потока. Пропускает пустые элементы (табы, например) до первого открывающего тега. Если не совпадает имя считанного тэга и __prop_tag, выбрасывает исключение. 
	*/
	static void readOpenTag ( QXmlStreamReader& __reader, const QString& __tag_name);
	/*
	//	Вычитывает первый закрывающий тэг из потока аналогично readOpenTag
	*/
	static void readCloseTag ( QXmlStreamReader& __reader, const QString& __tag_name);
	/*
	// Считывает текст ближайшего элемента XML. 
	*/
	static QString readSimpleProperty (QXmlStreamReader& __reader, const QString& __prop_tag );

	static void readTag (QXmlStreamReader& __reader, const QString& __tag_name, bool& __val);
	static void readTag (QXmlStreamReader& __reader, const QString& __tag_name, int &__val );
	static void readTag (QXmlStreamReader& __reader, const QString& __tag_name, unsigned int &__val );
	static void readTag (QXmlStreamReader& __reader, const QString& __tag_name, short int &__val );
	static void readTag (QXmlStreamReader& __reader, const QString& __tag_name, QString &__val );

	static void readTag (QXmlStreamReader& __reader, const QString& __tag_name, currency &__val );
	static void readTag (QXmlStreamReader& __reader, const QString& __tag_name, quantity_t &__val );
	static void readTag (QXmlStreamReader& __reader, const QString& __tag_name, QDateTime &__val );


	static void writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const bool __val);	
	static void writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const int __val);		
	static void writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const unsigned int __val);		
//	static void writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const short int __val);		
	static void writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const QString& __val);

	static void writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const currency& __val);
	static void writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const quantity_t& __val);
	static void writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const QDateTime& __val);


	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<int> &__val);
	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<unsigned int> &__val);
	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<unsigned long long> &__val);

	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<bool> &__val);	
	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<short int> &__val);
	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<QString> &__val);

	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<QTime> &__val);
	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<QDateTime> &__val);
	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<QDate> &__val);

	static void writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<currency> &__val);


	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<int> &__val);
	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<unsigned int> &__val);
	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<unsigned long long> &__val);

	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<bool> &__val);	
	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<short> &__val);	
	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<QString> &__val);	

	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<QTime> &__val);
	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<QDateTime> &__val);
	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<QDate> &__val);

	static void readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<currency> &__val);


	template<typename T> 
	void readObject (QXmlStreamReader& __reader, const QString& __v_props_name, boost::shared_ptr<T>& __result)
	{
		skipEmpty(__reader);

		QString tag = __reader.name().toString();

		if (__reader.isStartElement() && tag == __v_props_name)
		{
			__result.reset(new T());
			__result->read(__reader);
		}
	}

	/*
	//	Запись QVector-а произвольных объектов
	*/
	template<typename T> 
	void writeVector (QXmlStreamWriter& __writer, const QString& __list_props_name, QVector<boost::shared_ptr<T> >& __v_props)
    {
		__writer.writeStartElement(__list_props_name);
//        for ( QVector<boost::shared_ptr<T> >::iterator prop = __v_props.begin(); prop!=__v_props.end(); ++prop )
//		{
//			(*prop)->write(__writer);
//		}

        for ( int a = 0; a < __v_props.size(); ++a )
        {
            __v_props.at(a)->write(__writer);
        }

		__writer.writeEndElement();
	}
	/*
	//	Чтение QVector-а произвольных объектов
	*/
	template<typename T> 
	void readVector (QXmlStreamReader& __reader, const QString& __v_props_name, QVector<boost::shared_ptr<T> > &__result)
	{
		__result.clear();
		readOpenTag(__reader, __v_props_name);
		for ( ; !__reader.atEnd() && !(__reader.isEndElement() && (__reader.name()==m_tagName || __reader.name()==__v_props_name)) ; )
		{
			boost::shared_ptr<T> sax_prop( new T());
			sax_prop->read(__reader);
			__result.push_back(sax_prop);

			skipEmpty(__reader);
			if (__reader.isEndElement() && __reader.name()==__v_props_name)
			{
				break;
			}
		}
		readCloseTag(__reader, __v_props_name);
	}

	QString m_tagName;
};

#endif
