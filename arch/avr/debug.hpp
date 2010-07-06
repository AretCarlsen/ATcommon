// MEP
//#define DEBUG_MEP(...) __VA_ARGS__
#define DEBUG_MEP(...) ;
// MAP
//#define DEBUG_MAP(...) __VA_ARGS__
#define DEBUG_MAP(...) ;
// AddressGraph
//#define DEBUG_AG(...) __VA_ARGS__
#define DEBUG_AG(...) ;
// SimpleServer
//#define DEBUG_SS(...) __VA_ARGS__
#define DEBUG_SS(...) ;

// Eeprom and state control
//#define DEBUG_EEP(...) __VA_ARGS__
#define DEBUG_EEP(...) ;
// Memory allocation
//#define DEBUG_HEAP(...) __VA_ARGS__
#define DEBUG_HEAP(...) ;
// Rare
#define DEBUG_RARE(...) __VA_ARGS__
//#define DEBUG_RARE(...) ;
// Miscellany
#define DEBUG_MISC(...) __VA_ARGS__
//#define DEBUG_MISC(...) ;

// Analog encoder [server]
//#define DEBUG_AE(...) __VA_ARGS__
#define DEBUG_AE(...) ;
// Motor driver [server]
#define DEBUG_MD(...) __VA_ARGS__
//#define DEBUG_MD(...) ;
// PID engine [server]
//#define DEBUG_PD(...) __VA_ARGS__
#define DEBUG_PD(...) ;
//#define DEBUG_PD_TARG(...) __VA_ARGS__
#define DEBUG_PD_TARG(...) ;
//#define DEBUG_FORCE(...) ;
#define DEBUG_FORCE(...) __VA_ARGS__
//#define DEBUG_HEARTBEAT(...) ;
#define DEBUG_HEARTBEAT(...) __VA_ARGS__

#define DEBUG_NRF(...) __VA_ARGS__
//#define DEBUG_NRF(...) ;
#define DEBUG_GPS(...) __VA_ARGS__
//#define DEBUG_GPS(...) ;

