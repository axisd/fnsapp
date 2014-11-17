#ifndef FNS_TERMINAL_H_
#define FNS_TERMINAL_H_

#include "sax_serializer_base.h"

#include <QMap>

class CFNSTerminalRequest: public SaxSerializerBase
{
public:
	unsigned int m_task_efts;
	unsigned int m_protocol_version;
	QString m_data;	

    CFNSTerminalRequest(): SaxSerializerBase("SCO_UKM_PROTOCOL_REQUEST") {}
	void serialize (QXmlStreamWriter& __writer);
    void deserialize (QXmlStreamReader& __reader);

	static const QByteArray XML_TAG;
	static const QByteArray HEADER_TAG;
	static const QByteArray BODY_TAG;
};

class CFNSTerminalResponse: public SaxSerializerBase
{
public:
	unsigned int m_task_status;
	QString m_data;

    CFNSTerminalResponse(unsigned int __status, const QString& __data):
      m_task_status(__status), m_data(__data),
	  SaxSerializerBase("SCO_UKM_PROTOCOL_RESPONSE") 
	  {}
	void serialize (QXmlStreamWriter& __writer);
    void deserialize (QXmlStreamReader& __reader) {}
};

typedef boost::shared_ptr<CFNSTerminalResponse> CFNSTerminalResponsePtr;

#endif
