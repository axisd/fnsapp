#ifndef LARGE_H
#define LARGE_H

#include <vector>
#include <QString>

class large
{
public:
	large();
	large( const large& v );
	large( int i );
	large( unsigned int i );
	large( long long i );
	large( unsigned long long i );

	bool    operator==( const large& v ) const        {    return    compare(v) == 0;    }
	bool    operator!=( const large& v ) const        {    return    compare(v) != 0;    }
	bool    operator>( const large& v ) const        {    return    compare(v) > 0;        }
	bool    operator<( const large& v ) const        {    return    compare(v) < 0;        }
	bool    operator>=( const large& v ) const        {    return    compare(v) >= 0;    }
	bool    operator<=( const large& v ) const        {    return    compare(v) <= 0;    }

	large    operator-() const;
	large    operator+( const large& v ) const;
	large    operator-( const large& v ) const;
	large    operator*( const large& v ) const;
	large    operator/( const large& v ) const;
	large    operator%( const large& v ) const;
	large    operator^( int v ) const;                // it's a power of v, not XOR
	large    operator<<( size_t v ) const;
	large    operator>>( size_t v ) const;

	large&    operator=( const large& v );
	large&    operator+=( const large& v )            {    return    *this = *this + v;    }
	large&    operator-=( const large& v )            {    return    *this = *this - v;    }
	large&    operator*=( const large& v )            {    return    *this = *this * v;    }
	large&    operator/=( const large& v )            {    return    *this = *this / v;    }
	large&    operator%=( const large& v )            {    return    *this = *this % v;    }
	large&    operator^=( int v )                        {    return    *this = *this ^ v;    }
	large&    operator<<=( int v )                    {    return    *this = *this << v;    }
	large&    operator>>=( int v )                    {    return    *this = *this >> v;    }
	large&    operator++()                            {    return    *this    += 1;        }
	large&    operator--()                            {    return    *this    -= 1;        }

	large    modExp( const large& exponent, const large& modulus ) const;

	size_t    bitCount() const;

	long long tolonglong();
	int            toInt() const;
	QString    toString() const;
	QString    toHex() const;
	size_t        toSignedBinary( unsigned char* bin, size_t size ) const;
	size_t        toUnsignedBinary( unsigned char* bin, size_t size ) const;

	static    large fromString( const char* str );
	static    large fromHex( const char* str );
	static    large fromSignedBinary( const unsigned char* bin, size_t size );
	static    large fromUnsignedBinary( const unsigned char* bin, size_t size );

private:
	typedef    unsigned int         word_t;
	typedef    unsigned long long  dword_t;
	typedef    std::vector<word_t>   vec_t;

	vec_t    _val;
	bool    _neg;

private:
	large( bool neg, const vec_t& val );

	void    normalize();
	int        compare( const large& v ) const;
	int        compare_abs( const large& v ) const;
	large    divide( const large& v, large& remainder ) const;

	int        partial_sub( size_t off, size_t len, const large& v );
	int        partial_sum( size_t off, size_t len, const large& v );
	large    get_partial( size_t off, size_t len ) const;
	void    replace_partial( size_t off, size_t len, const large& v );

	word_t    loword( size_t off = 0 ) const;
	word_t    hiword( size_t off = 0 ) const;
	word_t    dword_hi( dword_t d ) const                {    return    (word_t)(d >> 32);            }
	word_t    dword_lo( dword_t d ) const                {    return    (word_t)(d & 0xFFFFFFFF);    }
};

#endif
