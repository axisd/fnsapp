#ifndef _MODEL_ITEM_H
#define _MODEL_ITEM_H

// Misc
#include "cfixedpointround.h"

// Qt
#include <QString>

// Boost
#include <boost/optional.hpp>

/*
//	Модель товара
*/
namespace SSCO
{
	struct ModelItem
	{
		ModelItem() : m_version(0) {};

		QString m_id;
		QString m_classif;
		QString m_name;
		currency m_price;
		boost::optional<currency> m_minprice;
		QString m_measure;
		int m_measureprec;
		bool m_weighting;
		bool m_age_control;
		unsigned int m_version;
	};
}
#endif



