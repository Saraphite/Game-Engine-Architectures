//For use setting an importance level for a debug message.
typedef enum {
	None,
	Trivial,
	Regular,
	Important,
	Critical
} Importance;

//For use tagging a log message with a particular type. 
typedef enum {
	None,
	Debug,
	Data,
	Performance
} DataTag;

//To define whether or not logging is initialised (maybe a bool?)
typedef enum{
	Active,
	Inactive
} LogInitialised;

//Logging type. Safe is most demanding but protects from crashes.
typedef enum{
	Safe,
	Efficient,

} LoggingType;

void InitializeLogging();

void TerminateLogging();

void PrintLog(const char *message, ...);

void PrintLog(const char *message, DataTag tag);

void PrintLog(const char *message, DataTag tag, Importance importance);

