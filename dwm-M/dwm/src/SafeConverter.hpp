/*** SafeConverter.hpp ***/
#ifndef SAFE_CONVERTER_HPP
#define SAFE_CONVERTER_HPP

#include <climits>
#include <limits>

class SafeConverter {
	public:
	static int UnsignedToIntWithDefault ( unsigned int value, int defaultValue ) {
		if ( value > static_cast<unsigned int> ( std::numeric_limits<int>::max () ) )
		{
			return defaultValue;
		}
		return static_cast<int> ( value );
	}
	static int UnsignedToInt ( unsigned int value ) {
		if ( value > static_cast<unsigned int> ( std::numeric_limits<int>::max () ) )
		{
			return std::numeric_limits<int>::max ();
		}
		return static_cast<int> ( value );
	}
	static float IntToFloat ( const int &num ) {
		return static_cast<float> ( num );
	}
	static long UnsignedLongToLong( unsigned long &value ) {
		if (value > static_cast<unsigned long>(LONG_MAX)) {
			return LONG_MAX;
		}
		return static_cast<long>(value);
	}
	static int UnsignedLongToInt ( const long &value ) {
		if (value <= static_cast<unsigned long>(INT_MAX))
			return static_cast<int>(value);
		else
			return INT_MAX;
	}
};

#endif // SAFE_CONVERTER_HPP
