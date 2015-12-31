#pragma once
#include <stdio.h>

#define LOG_TAG_COUNT 4 //If a developer wants to add new tags to the alternative logger, they'll also have to increase this number.
#define SEVERITY_TAG_COUNT 4 //If a developer wants to add new tags to the alternative logger, they'll also have to increase this number.

//For use setting a severity level for a debug message.
typedef enum Severity {
	None = 0,
	Trivial = 1,
	Regular = 2,
	Important = 3,
	Critical = 4
} Severity;

//For use tagging a log message with a particular type. 
typedef enum LogTag {
	Debug = 0,
	Error = 1,
	Warning = 2,
	Performance = 3,
	Other = 4
} LogTag;

//For use defining output channels.
typedef enum Output {
	All,
	Console,
	File
} Output;

//Logging type. Safe is most demanding but protects from crashes. 
typedef enum LoggingType {
	Safe,
	Efficient
} LoggingType;

//Because a boolean is wonderful.
typedef enum bool{
	false,
	true
} bool;

struct LoggingProfile {
	LoggingType logType;
	Output outputType;
	bool consoleSeverityFilters[SEVERITY_TAG_COUNT];
	bool fileSeverityFilters[SEVERITY_TAG_COUNT];
	bool consoleLogTagFilters[LOG_TAG_COUNT];
	bool fileLogTagFilters[LOG_TAG_COUNT];
};

bool isActive;
bool fileSeverityFilters[SEVERITY_TAG_COUNT];
bool consoleSeverityFilters[SEVERITY_TAG_COUNT];
bool fileTagFilters[LOG_TAG_COUNT];
bool consoleTagFilters[LOG_TAG_COUNT];

FILE * outputFile;

Output outputChannel;
LoggingType logType;


void PrintLog(Severity severity, LogTag tag, bool printTimeStamp, const char *message, ...);

/* Clears all of the filters that influence what logging tags are printed (Debug, Performance, Warning etc) */
void ClearLogTagFilters(Output output);

/* Clears all of the filters that influence what severity tags are printed (Debug, Performance, Warning etc) */
void ClearSeverityFilters(Output output);

/* Allows logging to happen. Takes a parameter that determines whether or not to clear all filters, revert to Safe logging mode and change the output channel to both file and console. */
void InitializeLogging();

/* Disables logging and if the file is still open, it closes it. (This will happen in Efficient Logging mode)*/
void TerminateLogging();

/* Sets the logging type. 'Safe' will open the file, print the message and close the file in one chunk. This is more demanding. 'Efficient' will open the file the first time a message is printed and will remain open until logging is terminated. */
void SetLoggingType(LoggingType type);

/* Sets the state of the filter for a specific Severity tag. Can specify an output channel. */
void SetSeverityFilterState(Severity severity, bool state, Output output);

/* Sets the state of the filter for a specific Logging tag. Can specify an output channel. */
void SetLogTagFilterState(LogTag tag, bool state, Output output);

/* Sets the filters to ignore everything but the specific log tag. Can specify an output channel. */
void FilterAllButLogTag(LogTag tag, Output output);

/* Sets the filters to ignore everything but the specific severity tag. Can specify an output channel. */
void FilterAllButSeverityTag(Severity severity, Output output);

/* Sets the specific output channel. This allows the programmer to specify where the printed messages are output too. */
void SetOutputChannel(Output output);

/* Sets all of the logging settings to that of a predetermined struct that represents the settings. */
void SetLoggingProfile(struct LoggingProfile);
