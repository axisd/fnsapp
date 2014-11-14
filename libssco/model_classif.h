#ifndef _MODEL_CLASSIF_H
#define _MODEL_CLASSIF_H

#include <QString>
#include <boost/optional.hpp>

/*
//	модель классификатора
*/
namespace SSCO
{
	struct ModelClassif
	{
		QString m_id;
		boost::optional<QString> m_owner;
		QString m_name;
		bool m_age_control;
		unsigned int m_version;
	};
}
#endif