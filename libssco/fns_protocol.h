#ifndef FNS_PROTOCOL_H_
#define FNS_PROTOCOL_H_

#include "sax_serializer_base.h"
#include "fns_task_description.h"


class CFNSProtocolRequest: public SaxSerializerBase
{
	unsigned int m_task_efts;
	unsigned int m_protocol_version;
	QString m_data;
	
public:
    CFNSProtocolRequest(): SaxSerializerBase("FNS_REQUEST") {}
    CFNSProtocolRequest(unsigned int __efts);
	void serialize (QXmlStreamWriter& __writer);
	void deserialize (QXmlStreamReader& __reader);
    unsigned int getTaskEfts() { return m_task_efts; }
	unsigned int getProtocolVersion() { return m_protocol_version; }
	QString getData() { return m_data; }

	static const QByteArray XML_TAG;
	static const QByteArray HEADER_TAG;
	static const QByteArray BODY_TAG;	
	
    static CFNSProtocolRequest genNewRequest(int __type);
	
	void setData(const QString& __data);
	QByteArray generate();
};

class CFNSProtocolResponse: public SaxSerializerBase
{
	unsigned int m_task_status;
	QString m_data;

public:
    CFNSProtocolResponse(unsigned int __status, const QString& __data):
      m_task_status(__status), m_data(__data), SaxSerializerBase("FNS_RESPONSE")
	  {}
	  
    CFNSProtocolResponse(): SaxSerializerBase("FNS_RESPONSE") { m_task_status = FNS_TASK_STARTED; }
	void serialize (QXmlStreamWriter& __writer);
	void deserialize (QXmlStreamReader& __reader);
	void parseRequestStr(const QString& __resp_str);
	QString data() { return m_data; }
	unsigned int status() { return m_task_status; }
};

#endif
