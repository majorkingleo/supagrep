/*
 * $Log: range.h,v $
 * Revision 1.2  2006/11/23 16:46:27  wamas
 * MOBERZA -Wshadow dazugeschalten und ausgebessert
 *
 * Revision 1.1.1.1  2006/03/17 19:49:15  wamas
 * own tools reponsitority
 *
 * Revision 1.2  2006/03/09 00:48:27  wamas
 * Added CVS Log Info
 *
 */

#ifndef TOOLS_RANGE_H
#define TOOLS_RANGE_H

#include <exception>

namespace Tools {

template<class EClass> class EnumRange : public EClass
{    
public:
    struct Error : public std::exception {
	const char *err;
	Error( const char *e ) throw() : exception(), err(e) {}
	virtual const char * what() const throw() { return err; }
    };

    typedef typename EClass::ETYPE ETYPE;       

    static const ETYPE INVALID__ = static_cast<ETYPE>(-5000);
    ETYPE value;

    EnumRange( ETYPE value_ ) 
	: value( value_ )
	{
	    if( value <= EClass::FIRST__ || value >= EClass::LAST__ )
		throw( Error( "EnumRange: Out of range!" ) );
	}    

    EnumRange() : value( INVALID__ ) {}

    ETYPE operator()() const {
	if( value == INVALID__ )
	    throw( Error( "EnumRange: Uninitialized value!" ) );

	return value;
    }

    operator ETYPE () const {
	if( value == INVALID__ )
	    throw( Error( "EnumRange: Uninitialized value!" ) );

	return value;
    }

    EnumRange<EClass>& operator=( ETYPE v ) {
	if( v <= EClass::FIRST__ || v >= EClass::LAST__ )
	    throw( Error( "EnumRange: Out of range!" ) );
	value = v;
	return *this;
    }

    EnumRange<EClass>& operator=( const EnumRange<EClass>& er ) {
	value = er.value;
	return *this;
    }
	
};

} // namespace Tools

#endif
