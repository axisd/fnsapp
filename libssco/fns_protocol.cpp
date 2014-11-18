#include "fns_protocol.h"

const QByteArray CFNSProtocolRequest::XML_TAG = "FNS_REQUEST";
const QByteArray CFNSProtocolRequest::HEADER_TAG = "FNS_REQUEST_HEADER";
const QByteArray CFNSProtocolRequest::BODY_TAG = "FNS_REQUEST_BODY";

CFNSProtocolRequest::CFNSProtocolRequest(unsigned int __efts): m_task_efts(__efts), SaxSerializerBase("FNS_REQUEST")
{
	m_protocol_version = 2;
}

CFNSProtocolRequest CFNSProtocolRequest::genNewRequest (int __type)
{
    return CFNSProtocolRequest(__type);
}

void CFNSProtocolRequest::setData(const QString& __data)
{
	m_data += __data;
}

void CFNSProtocolRequest::serialize (QXmlStreamWriter& __writer)
{
	__writer.writeStartDocument("1.0");
	__writer.writeStartElement(XML_TAG);
	__writer.writeStartElement(HEADER_TAG);
	__writer.writeTextElement("task_efts", QString::number(m_task_efts));
	__writer.writeTextElement("protocol_version", QString::number(m_protocol_version));
	__writer.writeEndElement();
	__writer.writeStartElement(BODY_TAG);
	__writer.writeCDATA(m_data);
	__writer.writeEndElement();
	__writer.writeEndElement();
	__writer.writeEndDocument();
}

void CFNSProtocolRequest::deserialize (QXmlStreamReader& __reader)
{
	__reader.readNext();
	skipEmpty(__reader);
	__reader.readNext();	
	readOpenTag(__reader, XML_TAG);
	readOpenTag(__reader, HEADER_TAG);
	readTag(__reader, "task_efts", m_task_efts);
	readTag(__reader, "protocol_version", m_protocol_version);
	readCloseTag(__reader, HEADER_TAG);
	readTag(__reader, BODY_TAG, m_data);
	readCloseTag(__reader, XML_TAG);
}

QByteArray CFNSProtocolRequest::generate()
{
	QByteArray data;
	QXmlStreamWriter data_writer (&data);
	serialize(data_writer);
	return data;
}

void CFNSProtocolResponse::parseRequestStr(const QString& __resp_str)
{
	QString resp (__resp_str);
	QXmlStreamReader reader (resp);
	reader.readNext();
	reader.readNext();
	read(reader);
}

void CFNSProtocolResponse::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "task_status", m_task_status);
	writeTag(__writer, "response_data", m_data);
}

void CFNSProtocolResponse::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "task_status", m_task_status);
	readTag(__reader, "response_data", m_data);
}
