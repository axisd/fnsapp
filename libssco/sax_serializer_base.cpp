#include "sax_serializer_base.h"

// MISC
#include "../liblogger/cdebug.h"

SaxSerializerBase::SaxSerializerBase(const QString &__tag_name) :
	m_tagName(__tag_name)
{

}

void SaxSerializerBase::write (QXmlStreamWriter& __writer)
{
	__writer.writeStartElement( this->m_tagName );
	this->serialize(__writer);
	__writer.writeEndElement();
}

void SaxSerializerBase::read (QXmlStreamReader& __reader)
{
	readOpenTag(__reader, m_tagName);
	this->deserialize(__reader);
	readCloseTag(__reader, m_tagName);
}

QString SaxSerializerBase::tagName()
{
	return m_tagName;
}

void SaxSerializerBase::writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const bool __val)
{
	__writer.writeTextElement(__tag_name, __val ? "1" : "0");
}

void SaxSerializerBase::writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const int __val)
{
	__writer.writeTextElement(__tag_name, QString::number(__val));
}

void SaxSerializerBase::writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const unsigned int __val)
{
	__writer.writeTextElement(__tag_name, QString::number(__val));
}

void SaxSerializerBase::writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const QString& __val)
{
	__writer.writeTextElement(__tag_name, __val);
}

void SaxSerializerBase::writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const currency& __val)
{
	__writer.writeTextElement(__tag_name, __val.to_qstring());
}

void SaxSerializerBase::writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const quantity_t& __val)
{
	__writer.writeTextElement(__tag_name, __val.to_qstring());
}

void SaxSerializerBase::writeTag (QXmlStreamWriter& __writer, const QString& __tag_name, const QDateTime& __val)
{
	__writer.writeTextElement(__tag_name, __val.toString("yyyy-MM-dd HH:mm:ss"));
}

void SaxSerializerBase::readTag (QXmlStreamReader& __reader, const QString& __tag_name, bool& __val)
{
	QString textval = readSimpleProperty(__reader, __tag_name);
	__val = ( textval == "1" );
}

void SaxSerializerBase::readTag (QXmlStreamReader& __reader, const QString& __tag_name, unsigned int& __val)
{
	bool int_ok;	
	__val = readSimpleProperty(__reader, __tag_name).toUInt(&int_ok);
}

void SaxSerializerBase::readTag (QXmlStreamReader& __reader, const QString& __tag_name, short int& __val)
{
	bool int_ok;	
	__val = readSimpleProperty(__reader, __tag_name).toShort(&int_ok);
}
void SaxSerializerBase::readTag (QXmlStreamReader& __reader, const QString& __tag_name, int& __val)
{
	bool int_ok (true);	
	__val = readSimpleProperty(__reader, __tag_name).toInt(&int_ok);
	if (!int_ok)
	{
		throw std::runtime_error("Bad int value");
	}
}

void SaxSerializerBase::readTag (QXmlStreamReader& __reader, const QString& __tag_name, QString &__val )
{
	__val = readSimpleProperty(__reader, __tag_name);
}

void SaxSerializerBase::readTag (QXmlStreamReader& __reader, const QString& __tag_name, currency &__val )
{
	__val.from_qstring(readSimpleProperty(__reader, __tag_name));
}

void SaxSerializerBase::readTag(QXmlStreamReader& __reader, const QString& __tag_name, quantity_t& __val )
{
	__val.from_qstring(readSimpleProperty(__reader, __tag_name));
}

void SaxSerializerBase::readTag(QXmlStreamReader& __reader, const QString& __tag_name, QDateTime& __val )
{
	__val.fromString(readSimpleProperty(__reader, __tag_name), "yyyy-MM-dd HH:mm:ss");
}

void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<bool> &__val)
{
	if (__val.is_initialized())
	{
		__writer.writeTextElement(__tag_name, __val ? "1" : "0");
	}
	else
	{
		__writer.writeTextElement(__tag_name, "");
	}	
}

void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<QTime> &__val)
{
	__writer.writeTextElement(__tag_name, __val ? __val->toString("hh:mm:ss") : "");
}


void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<QDateTime> &__val)
{
	__writer.writeTextElement(__tag_name, __val ? __val->toString("yyyy-MM-dd HH:mm:ss") : "");
}

void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<QDate> &__val)
{
	__writer.writeTextElement(__tag_name, __val ? __val->toString("yyyy-MM-dd") : "");
}

void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<short int> &__val)
{
	__writer.writeTextElement(__tag_name, __val ? QString::number(*__val) : "");
}

void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<unsigned int> &__val)
{
	__writer.writeTextElement(__tag_name, __val ? QString::number(*__val) : "");
}

void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<int> &__val)
{
	__writer.writeTextElement(__tag_name, __val ? QString::number(*__val) : "");
}

void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<unsigned long long> &__val)
{
	__writer.writeTextElement(__tag_name, __val ? QString::number(*__val) : "");
}

void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<currency> &__val)
{
	__writer.writeTextElement(__tag_name, __val ? __val->to_qstring() : "");
}

void SaxSerializerBase::writeOptional (QXmlStreamWriter& __writer, const QString& __tag_name, const boost::optional<QString> &__val)
{
	__writer.writeTextElement(__tag_name, __val ? *__val : "");
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<bool> &__val)
{
	__val.reset();
	QString textval = readSimpleProperty(__reader, __tag_name);
	if (textval != "")
	{
		__val.reset(textval == "1");
	}
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<int> &__val)
{
	__val.reset();
	bool int_ok;	

	QString textval = readSimpleProperty(__reader, __tag_name);
	if (textval != "")
	{
		int intval = textval.toInt(&int_ok);
		if (int_ok)
		{
			__val.reset(intval);
		}
		else
		{

		}
	}
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<short> &__val)
{
	bool int_ok;	

	QString textval = readSimpleProperty(__reader, __tag_name);
	if (textval != "")
	{
		__val.reset(textval.toShort(&int_ok));
	}
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<unsigned int> &__val)
{
	bool int_ok;	

	QString textval = readSimpleProperty(__reader, __tag_name);
	if (textval != "")
	{
		__val.reset(textval.toUInt(&int_ok));
	}
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<unsigned long long> &__val)
{
	bool int_ok;	

	QString textval = readSimpleProperty(__reader, __tag_name);
	if (textval != "")
	{
		__val.reset(textval.toULongLong(&int_ok));
	}
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<QString> &__val)
{
	QString textval = readSimpleProperty(__reader, __tag_name);
	if (textval != "")
	{
		__val.reset(textval);
	}
	return;
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<QTime> &__val)
{
	__val.reset();
	QString textval = readSimpleProperty(__reader, __tag_name);
	if (textval != "")
	{
		__val.reset(QTime::fromString(textval, "HH:mm:ss"));
	}
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<QDateTime> &__val)
{
	__val.reset();
	QString textval = readSimpleProperty(__reader, __tag_name);
	if (textval != "")
	{
		__val.reset(QDateTime::fromString(textval, "yyyy-MM-dd HH:mm:ss"));
	}
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<QDate> &__val)
{
	__val.reset();
	QString textval = readSimpleProperty(__reader, __tag_name);
	if (textval != "")
	{
		__val.reset(QDate::fromString(textval, "yyyy-MM-dd"));
	}
}

void SaxSerializerBase::readOptional (QXmlStreamReader& __reader, const QString& __tag_name, boost::optional<currency> &__val)
{
	QString textval = readSimpleProperty(__reader, __tag_name);
	if (!textval.isEmpty())
	{
		__val.reset(new currency(0));
		__val->from_qstring(textval);
	}
	return;
}

void SaxSerializerBase::skipEmpty (QXmlStreamReader& __reader)
{
	for ( ; __reader.isWhitespace(); __reader.readNext())
	{}
}

void SaxSerializerBase::readOpenTag ( QXmlStreamReader& __reader, const QString& __tag_name)
{
	skipEmpty(__reader);
	if (!__reader.isStartElement())
	{
		throw std::runtime_error( QString("Неверный тэг. Ожидается открывающий (%1 : %2)")
			.arg(__reader.lineNumber())
			.arg(__reader.errorString()).toStdString() );
	}
	
	if ( __reader.name() != __tag_name )
	{
		throw std::runtime_error( QString("Неверный открывающий тэг. Ожидается <%1> (%2 : %3")
			.arg(__tag_name)
			.arg(__reader.lineNumber())
			.arg(__reader.errorString()).toStdString());
	}

	__reader.readNext();
}

void SaxSerializerBase::readCloseTag ( QXmlStreamReader& __reader, const QString& __tag_name)
{
	skipEmpty(__reader);
	QString tag = __reader.name().toString();
	if (!__reader.isEndElement())
	{
		throw std::runtime_error( QString("Неверный тэг. Ожидается закрывающий (%1 : %2)")
			.arg(__reader.lineNumber())
			.arg(__reader.errorString()).toStdString() );
	}

	if ( __reader.name() !=__tag_name )
	{
		throw std::runtime_error( QString("Неверный закрывающий тэг. Ожидается <%1> (%2 : %3")
			.arg(__tag_name)
			.arg(__reader.lineNumber())
			.arg(__reader.errorString()).toStdString());
	}

	__reader.readNext();
}

QString SaxSerializerBase::readSimpleProperty (QXmlStreamReader& __reader, const QString& __prop_tag )
{
	skipEmpty(__reader);
	if ( __reader.name() != __prop_tag )
	{
		LOG_MESSAGE(logger::t_fatal, "debug", 
			QString("Неверный текстовый элемент. Ожидается: %1, предлагается: %2")
			.arg(__prop_tag)
			.arg(__reader.name().toString())
			);

		throw std::runtime_error (QObject::tr("Неверный текстовый элемент. Ожидается %1")
			.arg(__prop_tag).toStdString());
	}
	QString text = __reader.readElementText();
	__reader.readNext();
	return text;
}
