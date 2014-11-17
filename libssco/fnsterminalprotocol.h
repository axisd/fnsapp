#ifndef FNS_TERMINAL_H_
#define FNS_TERMINAL_H_

#include "sax_serializer_base.h"

#include <QMap>

class CFNSTerminalRequest: public SaxSerializerBase
{
	unsigned int m_sco_id;
	QString m_task_guid;
	unsigned int m_task_efts;
	unsigned int m_protocol_version;
	QString m_data;
	
public:
    CFNSTerminalRequest(): SaxSerializerBase("SCO_UKM_PROTOCOL_REQUEST") {}
	void serialize (QXmlStreamWriter& __writer);
	void deserialize (QXmlStreamReader& __reader);
	unsigned int getScoId() { return m_sco_id; }
	QString getTaskGuid () { return m_task_guid; }
	unsigned int getTaskEfts() { return m_task_efts; };
	unsigned int getProtocolVersion() { return m_protocol_version; }
	QString getData() { return m_data; }

	static const QByteArray XML_TAG;
	static const QByteArray HEADER_TAG;
	static const QByteArray BODY_TAG;
};

class CFNSTerminalResponse: public SaxSerializerBase
{
public:
	QString m_task_guid;
	unsigned int m_task_status;
	QString m_data;

    CFNSTerminalResponse(const QString& __guid, unsigned int __status, const QString& __data):
	  m_task_guid(__guid), m_task_status(__status), m_data(__data),
	  SaxSerializerBase("SCO_UKM_PROTOCOL_RESPONSE") 
	  {}
	void serialize (QXmlStreamWriter& __writer);
    void deserialize (QXmlStreamReader& __reader) {}
};

typedef boost::shared_ptr<CFNSTerminalResponse> CFNSTerminalResponsePtr;

#endif
