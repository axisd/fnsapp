#include "fnsterminalprotocol.h"

// Boost
#include <boost/bind.hpp>

const QByteArray CFNSTerminalRequest::XML_TAG = QByteArray("FNS_UKM_REQUEST");
const QByteArray CFNSTerminalRequest::HEADER_TAG = QByteArray("FNS_UKM_REQUEST_HEADER");
const QByteArray CFNSTerminalRequest::BODY_TAG = QByteArray("FNS_UKM_REQUEST_BODY");

void CFNSTerminalRequest::serialize (QXmlStreamWriter& __writer)
{
}

void CFNSTerminalRequest::deserialize (QXmlStreamReader& __reader)
{
	__reader.readNext();
	skipEmpty(__reader);
	__reader.readNext();	
	readOpenTag(__reader, XML_TAG);
	readOpenTag(__reader, HEADER_TAG);
	readTag(__reader, "sco_id", m_sco_id);
	readTag(__reader, "task_guid", m_task_guid);
	readTag(__reader, "task_efts", m_task_efts);
	readTag(__reader, "protocol_version", m_protocol_version);
	readCloseTag(__reader, HEADER_TAG);
	readTag(__reader, BODY_TAG, m_data);
	readCloseTag(__reader, XML_TAG);
}

void CFNSTerminalResponse::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "task_guid", m_task_guid);
	writeTag(__writer, "task_status", m_task_status);
	writeTag(__writer, "response_data", m_data);
}

