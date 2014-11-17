#include "shift_close.h"

using namespace SSCO;

ShiftClose::ShiftClose() : SaxSerializerBase("ShiftClose")
{

}

void ShiftClose::serialize (QXmlStreamWriter& __writer)
{
	writeTag(__writer, "id", m_id);
	writeTag(__writer, "date", m_date);

	writeOptional(__writer, "kkm_shift_number", m_kkm_shift_number);

	writeTag(__writer, "kkm_serial_number", m_kkm_serial_number);
	writeTag(__writer, "kkm_registration_number", m_kkm_registration_number);
	writeTag(__writer, "kkm_owner_number", m_kkm_owner_number);
	writeTag(__writer, "eklz_number", m_eklz_number);

	writeOptional(__writer, "eklz_date_activate", m_eklz_date_activate);
	writeOptional(__writer, "eklz_fast_full", m_eklz_fast_full);

	writeTag(__writer, "kkm_model_name", m_kkm_model_name);
	writeTag(__writer, "uploaded", m_uploaded);
}

void ShiftClose::deserialize (QXmlStreamReader& __reader)
{
	readTag(__reader, "id", m_id);
	readTag(__reader, "date", m_date);

	readOptional(__reader, "kkm_shift_number", m_kkm_shift_number);

	readTag(__reader, "kkm_serial_number", m_kkm_serial_number);
	readTag(__reader, "kkm_registration_number", m_kkm_registration_number);
	readTag(__reader, "kkm_owner_number", m_kkm_owner_number);
	readTag(__reader, "eklz_number", m_eklz_number);

	readOptional(__reader, "eklz_date_activate", m_eklz_date_activate);
	readOptional(__reader, "eklz_fast_full", m_eklz_fast_full);

	readTag(__reader, "kkm_model_name", m_kkm_model_name);
	readTag(__reader, "uploaded", m_uploaded);
}
