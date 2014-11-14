#if !defined(CFIXEDPOINT_H)
#define CFIXEDPOINT_H

#include "large.h"

// BOOST
#include <boost/static_assert.hpp>

#include <QStringList>

// STD
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

template<int N>
struct power
{
	static const long long value = 10LL * power<N - 1>::value;
};

template<>
struct power<0>
{
	static const long long value = 1LL;
};

static long long _llabs(const long long &__ll)
{
	return
#if defined(__linux__)
		::llabs(__ll)
#elif defined(_WIN32)
#ifndef NDEBUG
		(__ll < 0 ? -__ll : __ll)
#else
		_abs64(__ll)
#endif
#endif
		;
}

template <unsigned char Precision, unsigned char Round>
class fixedpoint
{
public:
	fixedpoint() : m_value(0)
	{
		BOOST_STATIC_ASSERT(Precision > Round);
	};

	template<unsigned char _Round>
	fixedpoint(const fixedpoint<Precision, _Round> &_Val)
	{
		BOOST_STATIC_ASSERT(Precision > Round);
		set(_Val.get());
	};

	template<unsigned char _Round>
	fixedpoint(fixedpoint<Precision, _Round>* const _Val)
	{
		BOOST_STATIC_ASSERT(Precision > Round);
		set(_Val->get());
	};

	// Преобразование выполняется с учётом округления, то есть:
	// <CODE>
	// fixedpoint<4, 2> fp1(1.2);   // fp1 содержит 1.20
	// fixedpoint<4, 2> fp2(1.235); // fp2 содержит 1.24
	// fixedpoint<4, 2> fp3(1);     // fp3 содержит 1.00
	// fixedpoint<4, 2> fp4(100L);  // fp4 содержит 100.00
	// </CODE>
	template<typename T>
	explicit fixedpoint(const T &_Val)
	{
		BOOST_STATIC_ASSERT(Precision > Round);
		set(static_cast<long long>(_Val * mc_dwExponent));
	};

	explicit fixedpoint(const QString& _Val)
	{
		BOOST_STATIC_ASSERT(Precision > Round);
		from_qstring(_Val);
	}

	template<unsigned char _Round>
	fixedpoint& operator =(const fixedpoint<Precision, _Round> &_Val)
	{
		set(_Val.get());
		return *this;
	};

	fixedpoint operator +() const
	{
		return this;
	};

	fixedpoint operator -() const
	{
		fixedpoint f;
		f.m_value = -m_value;
		return f;
	};

	template<unsigned char _Round>
	fixedpoint& operator +=(const fixedpoint<Precision, _Round> &_Val)
	{
		set(m_value + _Val.get());
		return *this;
	};

	template<unsigned char _Round>
	fixedpoint& operator -=(const fixedpoint<Precision, _Round> &_Val)
	{
		set(m_value - _Val.get());
		return *this;
	};

	template<unsigned char _Round>
	fixedpoint& operator *=(const fixedpoint<Precision, _Round> &_Val)
	{
		large lm_value(m_value);
		large lV_get(_Val.get());
		large l_tmp = lm_value * lV_get;
		large l_exp(power<Precision + Precision - Round>::value);
		large l_exp_half = l_exp/large(2);
		large l_m_StripExponent(m_StripExponent);
		large l_rez;
		l_rez = ((l_tmp + (l_tmp < 0 ? -l_exp_half : l_exp_half)) / l_exp) * l_m_StripExponent;
		m_value = l_rez.tolonglong();
		return *this;
	};

	// Деление выполняется с точностью на один знак выше для
	// выполнения матиматического округления.
	// <CODE>
	// CFixedPoint\<3\> fp1(2L); // Внутренне представление 2000
	// CFixedPoint\<3\> fp2(7L); // Внутренне представление 7000
	// fp1 = fp1 / fp2;        // Внутренне представление 286
	// </CODE>
	// Если поделить простым делением мы получим 0.285, то есть 2000
	// \* 1000 / 7000, хотя должно было быть 0.286 Попытаемся
	// сохранить последний знак для округления Внутренне
	// представление делимого увеличиваем на Precision + 1 порядок,
	// то есть в 10000 раз. Выполняем деление 2000 * 10000 / 7000,
	// получаем 2857. Выполняем корректировку для округления 2857 +
	// sign(2857) * 5, получаем 2857+5 = 2862 Полученное значение
	// имеет точность Precision + 1, приводим к оригинальному виду
	// 2862 / 10 = 286, то есть 0.286
	template<unsigned char _Round>
	fixedpoint& operator /=(const fixedpoint<Precision, _Round> &_Val)
	{
		large l_tmp;
		large lm_value = large(m_value);
		large lmc_dwExponentSucc = large(mc_dwExponentSucc);
		large vlm_value = large(_Val.get());

		l_tmp = (lm_value * lmc_dwExponentSucc)/vlm_value;

		long long tmp = l_tmp.tolonglong();
		set((tmp + (tmp > 0 ? 5 : -5)) / 10);
		return *this;
	};

	void set_mde(long long _Val)
	{
		m_value = _Val * m_StripExponent;
	};

	// Возвращает в "копейках"
	long long get_mde() const
	{
		return m_value / m_StripExponent;
	};

	// Фактически возвращает только целую часть, дробная просто отсекается
	// <CODE>
	// CFixedPoint<2> fp(1.235);        // fp содержит 1.24
	// long long l = fp.to_long_long(); // l содержит 1
	// </CODE>
	long long to_long_long() const
	{
		return m_value / mc_dwExponent;
	};

	// <CODE>
	// CFixedPoint<2> fp(1.235);  // fp содержит 1.24
	// double d = fp.to_double(); // d содержит 1.24
	// </CODE>
	double to_double() const
	{
		return double(m_value) / mc_dwExponent;
	};

	QString to_qstring() const
	{
		QString s = QString("%1").arg(_llabs(get()), Precision+1, 10, QChar('0'));
		int d = s.size() - Precision;

		return QString(
			(get() < 0 ? "-" : "")
			+ s.left(d)
			+ (Round > 0 ? "." : "")
			+ s.mid(d, Round)
		);
	}

	QByteArray to_qbytearray() const
	{
		QByteArray s = QByteArray::number(_llabs(get()), 10)
			.rightJustified(Precision+1, '0');
		int d = s.size() - Precision;

		return QByteArray(
			(get() < 0 ? "-" : "")
			+ s.left(d)
			+ (Round > 0 ? "." : "")
			+ s.mid(d, Round)
			);
	}

	void from_qstring(const QString& __from, bool* bOk = 0)
	{
		if(__from.isEmpty())
		{
			throw std::runtime_error("Нельзя преобразовать пустую строку к fixedpoint!");
		}

		QString strFrom(__from);
		QChar sign('+');

		if(strFrom.at(0) == '-')
		{
			sign = '-';
			strFrom.remove(0, 1);
		}

		QStringList sl = strFrom.split(QChar('.'));

		bool bRes = true;
		QString str(sl.at(0));
		long long n = str.isEmpty() ? 0 : sl.at(0).toLongLong(&bRes);
		if ( bOk )
			*bOk = bRes;

		sl.pop_front();
		int p = Round;
		if(!sl.empty())
		{
			QString s = sl.join("");
			for(QString::const_iterator it = s.begin(); p-- && it != s.end(); ++it)
			{
				if(!it->isDigit())
					break;

				n *= 10;
				if(n >= 0)
				{
					n += (long long) (it->toAscii() - '0');
				}
				else
				{
					n -= (long long) (it->toAscii() - '0');
				}
			}
			++p;
		}

		p += Precision - Round;
		while (p--)
		{
			n *= 10;
		}

		if (sign == '-')
			n = -n;

		set(n);
	}

	void set(const long long &_Val)
	{
		m_value = _Val;
		Defiation();
	};

	const long long& get() const
	{
		return m_value;
	};

	int sign() const
	{
		return m_value < 0 ? -1 : m_value > 0 ? 1 : 0;
	};

	fixedpoint abs() const
	{
		fixedpoint r;
		r.set(m_value < 0 ? -m_value : m_value);
		return r;
	}

	fixedpoint multiplier(const fixedpoint &__multiplier, bool __floor_round = false) const
	{
		long long delta = m_value % __multiplier.get();		
		long long fix = -delta;

		if(!__floor_round)
		{
			// округление к ближайшему целому
			long long inaccuracy = __multiplier.get() / 2;

			// если >  то 5 будет округляться в меньшую сторону,
			// если >= то в большую
			if (m_value >= 0)
			{
				if (delta >= inaccuracy)
				{
					fix += __multiplier.get();
				}
			}
			else
			{
				if (-delta >= inaccuracy)
				{
					fix -= __multiplier.get();
				}
			}
		}

		fixedpoint<Precision, Round> result;
		result.set(m_value + fix);
		return result;
	}

private:
	long long m_value;
	// Значение экспоненты
	// <CODE>
	// CFixedPoint<4> fp; // fp.mc_dwExponent содержит 10 в степени 4, то есть 10000
	// </CODE>
	static const long long mc_dwExponent = power<Precision>::value;
	// Половина экспоненты. Используется при округлении в стиле int(d < 0 ? d - .5 : d + .5)
	// <CODE>
	// CFixedPoint<4> fp; // fp.mc_dwExponentHalf содержит 5000
	// </CODE>
	static const long long mc_dwExponentHalf = mc_dwExponent / 2;
	// Значение экспонента на один порядок выше требуемой. Используется в operator/ для округления
	static const long long mc_dwExponentSucc = mc_dwExponent * 10;
	// Значение экспоненты для отбрасывания незначимых знаков после запятой при понижении точности с сохранением
	// количества знаков после запятой во внутреннем представлении. Отбрасывание выполняется путём целочисленного
	// деления с последующим умножением.
	// 12345 / 1000 * 1000 = 12000
	static const long long m_StripExponent = power<Precision - Round>::value;
	// Используется для округления при понижении точности.
	// (12345 + 50) / 100 * 100 = 12300
	static const long long m_StripExponentHalf = m_StripExponent / 2;

	// Понижение точности с округлением
	// <CODE>
	// fixedpoint<4, 4> fpr1(1.2345); // Округление не нужно внутренне представление остаётся 12345
	// fixedpoint<4, 1> fpr2(1.2345); // Округление выполняется внутренне представление становится 12000
	// fixedpoint<4, 1> fpr3(4.5678); // Округление выполняется внутренне представление становится 46000
	// </CODE>
	void Defiation()
	{
		this->m_value = ((this->m_value +
			(this->m_value > 0 ? m_StripExponentHalf : -m_StripExponentHalf)) /
			m_StripExponent) * m_StripExponent;
	};
};

// Typedefs

typedef fixedpoint<4, 2> currency;
typedef fixedpoint<4, 3> quantity_t;

// Functions

// Comparision

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
bool operator ==(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	return _Val1.get() == _Val2.get();
};

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
bool operator !=(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	return _Val1.get() != _Val2.get();
};

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
bool operator <(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	return _Val1.get() < _Val2.get();
};

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
bool operator <=(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	return _Val1.get() <= _Val2.get();
};

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
bool operator >(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	return _Val1.get() > _Val2.get();
};

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
bool operator >=(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	return _Val1.get() >= _Val2.get();
};

// Arithmetic

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
const fixedpoint<Precision, Round1> operator +(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	fixedpoint<Precision, Round1> res(_Val1);
	res += _Val2;
	return res;
};

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
const fixedpoint<Precision, Round1> operator -(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	fixedpoint<Precision, Round1> res(_Val1);
	res -= _Val2;
	return res;
};

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
const fixedpoint<Precision, Round1> operator *(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	fixedpoint<Precision, Round1> res(_Val1);
	res *= _Val2;
	return res;
};

template<unsigned char Precision, unsigned char Round1, unsigned char Round2>
const fixedpoint<Precision, Round1> operator /(const fixedpoint<Precision, Round1> &_Val1, const fixedpoint<Precision, Round2> &_Val2)
{
	fixedpoint<Precision, Round1> res(_Val1);
	res /= _Val2;
	return res;
}

// Stream

template <unsigned char Precision, unsigned char Round>
std::wostream& operator<<(std::wostream& os, const fixedpoint<Precision, Round>& fp)
{
	QString s = fp.to_qstring();
	os << s.toStdWString().c_str();
	return os;
}

template <unsigned char Precision, unsigned char Round>
std::wistream& operator>>(std::wistream& ins, fixedpoint<Precision, Round>& fp)
{
	std::wstring ws;
	ins >> ws;
	QString qs;
	qs = QString::fromStdWString(ws);
	fp.from_qstring(qs);
	return ins;
}

template <unsigned char Precision, unsigned char Round>
std::ostream& operator<<(std::ostream& os, const fixedpoint<Precision, Round>& fp)
{
	std::ostringstream str;
	std::string s;
	{
		std::ostringstream str;
		str << std::setw(Precision + 1) << std::setfill('0') << _llabs(fp.get());
		s = str.str();
	}
	size_t sz = s.length();
	str <<
		(fp.get() < 0 ? "-" : "") <<
		s.substr(0, sz - Precision)
		<<
		(Round > 0 ? "." : "")
		<<
		s.substr(sz -
			Precision,
			Round);
	os << str.str();
	return os;
};

template <unsigned char Precision, unsigned char Round>
std::istream& operator>>(std::istream& ins, fixedpoint<Precision, Round>& fp)
{
	char sign;
	ins.get(sign);
	if (!ins)
		return ins;
	if (sign != '-'){
		ins.unget();
		sign = '+';
	}
	long long n;
	ins >> n;
	if (!ins)
	{
		n = 0;
		ins.clear();
	};
	char c;
	ins.get(c);
	int p = Round;
	if (c == '.')
	{
		while (ins && p--)
		{
			ins.get(c);
			if (ins && isdigit(c))
			{
				n *= 10;
				if (n >= 0)
				{
					n += (long long) (c - '0');
				}
				else
				{
					n -= (long long) (c - '0');
				};
			}
			else
			{
				ins.unget();
				break;
			}
		};
		++p;
	}
	else
	{
		ins.unget();
	};

	p += Precision - Round;
	while (p--)
	{
		n *= 10;
	};
	if (sign == '-')
		n = -n;
	fp.set(n);

	if (ins.eof())
	{
		ins.clear();
	}
	else
	{
		do
		{
			ins.get(c);
		}
		while (!ins.eof() && c == '0');

		if (ins.eof())
		{
			ins.clear();
		}
		else
		{
			ins.unget();
		};
	};
	return ins;
};


#endif
