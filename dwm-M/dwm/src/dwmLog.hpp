/*** dwmLog.hpp ***/
#ifndef DWMLOG_HPP
#define DWMLOG_HPP



#include <string>
#include <sstream>
#include <string>
#include <iostream>

#include <fstream>
#include <utility>
#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <X11/X.h>

class FileHandler {
	public:
	explicit FileHandler ( std::string filename ) : m_filename (std::move( filename )) {}
	bool
	append ( const std::string &text ) {
		std::ofstream file ( m_filename, std::ios::app );// Open in append mode

		if ( ! file.is_open () ) {
			return false;// Return false if file couldn't be opened
		}
		file << text;// Append text
		file.close ();
		return true;
	}

	bool
	open () {
		std::ofstream file ( m_filename, std::ios::app );// Open in append mode
		bool isOpen = file.is_open ();
		file.close ();
		return isOpen;
	}

	private:
	std::string m_filename;
};

class TIME {
	public:
	static std::string get() {
		// Get the current time
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);

		// Convert time_t to tm as local time
		std::tm buf{};
		localtime_r(&in_time_t, &buf);

		// Use stringstream to format the time
		std::ostringstream ss;
		ss << "[" << std::put_time(&buf, "%Y-%m-%d %H:%M:%S") << "] ";
		return ss.str();
	}
};

class Converter{
	enum {
		ClkTagBar,
		ClkLtSymbol,
		ClkStatusText,
		ClkWinTitle,
		ClkClientWin,
		ClkRootWin,
		ClkLast,
		WmClose,
		WmMove
	}; /* clicks */
	public:
	static std::string
	Xev(const int &num){
		return eventTypeToString(num);
	}
	static const char*
	eventTypeToString(int eventType) {
		switch (eventType)
		{
			case KeyPress: 			return "KeyPress";
			case KeyRelease: 		return "KeyRelease";
			case ButtonPress: 		return "ButtonPress";
			case ButtonRelease: 	return "ButtonRelease";
			case MotionNotify:	 	return "MotionNotify";
			case EnterNotify: 		return "EnterNotify";
			case LeaveNotify: 		return "LeaveNotify";
			case FocusIn: 			return "FocusIn";
			case FocusOut: 			return "FocusOut";
			case KeymapNotify: 		return "KeymapNotify";
			case Expose: 			return "Expose";
			case GraphicsExpose: 	return "GraphicsExpose";
			case NoExpose: 			return "NoExpose";
			case VisibilityNotify: 	return "VisibilityNotify";
			case CreateNotify: 		return "CreateNotify";
			case DestroyNotify: 	return "DestroyNotify";
			case UnmapNotify: 		return "UnmapNotify";
			case MapNotify: 		return "MapNotify";
			case MapRequest: 		return "MapRequest";
			case ReparentNotify:	return "ReparentNotify";
			case ConfigureNotify:	return "ConfigureNotify";
			case ConfigureRequest: 	return "ConfigureRequest";
			case GravityNotify: 	return "GravityNotify";
			case ResizeRequest: 	return "ResizeRequest";
			case CirculateNotify: 	return "CirculateNotify";
			case CirculateRequest: 	return "CirculateRequest";
			case PropertyNotify: 	return "PropertyNotify";
			case SelectionClear: 	return "SelectionClear";
			case SelectionRequest: 	return "SelectionRequest";
			case SelectionNotify: 	return "SelectionNotify";
			case ColormapNotify: 	return "ColormapNotify";
			case ClientMessage: 	return "ClientMessage";
			case MappingNotify: 	return "MappingNotify";
			default: return "Unknown";
		}
	}

	static std::string
	click( unsigned int click) {
		return clickToString( click );
	}
	static const char*
	clickToString( unsigned int click ) {
		switch (click)
		{
			case ClkTagBar:      return "ClkTagBar";
			case ClkLtSymbol:    return "ClkLtSymbol";
			case ClkStatusText:  return "ClkStatusText";
			case ClkWinTitle:    return "ClkWinTitle";
			case ClkClientWin:   return "ClkClientWin";
			case ClkRootWin:     return "ClkRootWin";
			case ClkLast:        return "ClkLast";
			case WmClose:        return "WmClose";
			case WmMove:         return "WmMove";
			default:             return "Unknown Click Type";
		}
	}

};

class dwmLog {
	public:
	static void
	Click( const unsigned int &click ){
		std::string message = Converter::click( click );
		logMessage ( ":[Click]  :[" + message + "]" );
	}

	static void
	XEv ( const int &num ) {
		std::string message = Converter::Xev(num);
		logMessage ( ":[XEvent] :[" + message + "]" );
	}

	static void
	FUNC ( const std::string &message ) {
		logMessage ( ":[FUNC]   :[" + message + "]" );
	}

	static void
	Start() {
		StartMessage( ":[Start]  :[dwm-M]:[STARTED]" );
	}

	static void
	End() {
		EndMessage( ":[End]    :[dwm-M]:[KILLED]\n" );
	}

	static void
	ENDFUNC ( const std::string &message ){
		logMessage ( ":[ENDFUNC]:[" + message + "]" + "\n");
	}

	static void
	INFO ( const std::string &message ) {
		logMessage ( ":[INFO]   :[" + message + "]" );
	}

	template<typename T, typename... Args>
	static void
	INFO ( const T &first, Args... args ) {
		logMessage ( ":[INFO]   :[" + toString ( first, args... ) + "]" );
	}

	static void
	WARNING ( const std::string &message ) {
		logMessage ( ":[WARNING]:[" + message + "]" );
	}

	template<typename T, typename... Args>
	static void
	WARNING ( const T &first, Args... args ) {
		logMessage ( ":[WARNING]:[" + toString ( first, args... ) + "]" );
	}

	static void
	ERROR ( const std::string &message ) {
		logMessage ( ":[ERROR]  :[" + message + "]" );
	}

	template<typename T, typename... Args>
	static void
	ERROR ( const T &first, Args... args ) {
		logMessage ( ":[ERROR]  :[" + toString ( first, args... ) + "]" );
	}

	private:
	// Static function for conversion
	template<typename T>
	static std::string
	toString ( const T &arg ) {
		std::ostringstream stream;
		stream << arg;
		return stream.str ();
	}

	template<typename T, typename... Args>
	static std::string
	toString ( const T &first, Args... args ) {
		return toString ( first ) + " " + toString ( args... );
	}

	static void
	logMessage ( const std::string &message ) {
		FileHandler file ( "/home/mellw/log" );

		if ( file.open () )
			file.append ( TIME::get() + message + "\n" );
	}
	static void
	StartMessage ( const std::string &message ) {
		FileHandler file ( "/home/mellw/log" );

		if ( file.open () )
			file.append ( "\n" + TIME::get() + message + "\n" );
	}
	static void
	EndMessage ( const std::string &message ) {
		FileHandler file ( "/home/mellw/log" );

		if ( file.open () )
			file.append ( TIME::get() + message + "\n" );
	}
};

#endif // DWMLOG_HPP
