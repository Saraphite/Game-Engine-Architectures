#include "altlog.h"
#pragma once
#include <stdarg.h>
#pragma once
#include <time.h>
#pragma once
#include <string.h>
#pragma once
#include "../qcommon/q_shared.h"


#define MAX_CHAR_COUNT 1024

void PrintLog(Severity importance, LogTag tag, bool printTimeStamp, const char *message, ...) {
	if (isActive) {
		if ((consoleSeverityFilters[importance] && consoleTagFilters[tag]) || (fileSeverityFilters[importance] && fileTagFilters[tag])) {
			va_list ap;
			va_start(ap, message);
			char formattedMessage[MAX_CHAR_COUNT];
			vsprintf(formattedMessage, message, ap);
			char outputMessage[MAX_CHAR_COUNT] = "";
			if (printTimeStamp) {
				struct tm *tm;
				time_t t;
				char str_time[100];
				t = time(NULL);
				tm = localtime(&t);
				strftime(str_time, sizeof(str_time), "%H:%M:%S ", tm);
				strcat(outputMessage, str_time);
			}
			switch (importance) {
			case Trivial:
				strcat(outputMessage, "Trivial: ");
				break;
			case Regular:
				strcat(outputMessage, "Regular: ");
				break;
			case Important:
				strcat(outputMessage, "IMPORTANT: ");
				break;
			case Critical:
				strcat(outputMessage, "CRITICAL: ");
				break;
			}
			switch (tag) {
			case Debug:
				strcat(outputMessage, "DEBUG: ");
				break;
			case Error:
				strcat(outputMessage, "ERROR: ");
				break;
			case Warning:
				strcat(outputMessage, "WARNING: ");
				break;
			case Performance:
				strcat(outputMessage, "PERFORMANCE: ");
				break;
			case Other:
				strcat(outputMessage, "OTHER: ");
				break;
			}
			strcat(outputMessage, formattedMessage);
			strcat(outputMessage, "\n");
			va_end(ap);
			switch (outputChannel) {
			case All:
			case File:
				if (fileSeverityFilters[importance] && fileTagFilters[tag]) {

					if (logType == Safe) {
						if (outputFile == NULL) {
							outputFile = fopen("altlog.txt", "w");
						}
						else {
							outputFile = fopen("altlog.txt", "a");
						}
						fprintf(outputFile, "%s", outputMessage);
						fclose(outputFile);
					}
					else {
						fprintf(outputFile, "%s", outputMessage);
					}
				}
				if (outputChannel != All) {
					break;
				}
			case Console:
				if (consoleSeverityFilters[importance] && consoleTagFilters[tag]) {
					Com_Printf(outputMessage);
					//CG_Printf(outputMessage);
				}
				break;
			}
		}
	}
}

void ClearLogTagFilters(Output output) {
	for (int i = 0; i < LOG_TAG_COUNT; i++) {
		switch (output) {
		case All:
		case File:
			fileTagFilters[i] = true;
			if (output != All) {
				break;
			}
		case Console:
			consoleTagFilters[i] = true;
		}
	}
}

void ClearSeverityFilters(Output output) {
	for (int i = 0; i < SEVERITY_TAG_COUNT; i++) {
		switch (output) {
		case All:
		case File:
			fileSeverityFilters[i] = true;
			if (output != All) {
				break;
			}
		case Console:
			consoleSeverityFilters[i] = true;
		}
	}
}

void InitializeLogging(bool clearState) {
	isActive = true; 
	if (clearState)
	{
		ClearSeverityFilters(All);
		ClearLogTagFilters(All);
		logType = Safe;
		outputChannel = All;
	}
}

void TerminateLogging() {
	isActive = false;
	if (logType == Efficient) {
		fclose(outputFile);
	}
}

void SetLoggingType(LoggingType type) {
	logType = type;
	if (type == Efficient) {
		if (outputFile == NULL) {
			outputFile = fopen("altlog.txt", "w");
			outputFile = fopen("altlog.txt", "w");
		}
	}
	else {
		if (outputFile != NULL) {
			fclose(outputFile);
		}
	}
}

void SetSeverityFilterState(Severity severity, bool state, Output output) {
	switch (output) {
	case All:
	case File:
		fileSeverityFilters[severity] = state;
		if (output != All) {
			break;
		}
	case Console:
		consoleSeverityFilters[severity] = state;
	}
}

void SetLogTagFilterState(LogTag tag, bool state, Output output) {
	switch (output) {
	case All:
	case File:
		fileTagFilters[tag] = state;
		if (output != All) {
			break;
		}
	case Console:
		consoleTagFilters[tag] = state;
	}
}

void FilterAllButLogTag(LogTag tag, Output output) {
	for (int i = 0; i < LOG_TAG_COUNT; i++) {
		switch (output) {
		case All:
		case File:
			fileTagFilters[i] = (i == tag) ? true : false;
			if (output != All) {
				break;
			}
		case Console:
			consoleTagFilters[i] = (i == tag) ? true : false;
		}

	}
}

void FilterAllButSeverityTag(Severity importance, Output output) {
	for (int i = 0; i < SEVERITY_TAG_COUNT; i++) {
		switch (output) {
		case All:
		case File:
			fileSeverityFilters[i] = (i == importance) ? true : false;
			if (output != All) {
				break;
			}
		case Console:
			consoleSeverityFilters[i] = (i == importance) ? true : false;
		}
	}
}

void SetOutputChannel(Output output) {
	outputChannel = output;
}

void SetLoggingProfile(struct LoggingProfile profile) {
	logType = profile.logType;
	SetOutputChannel(profile.outputType);
	/*  These are seperated into two seperate for loops to allow them to be edited later with no issues.
		Originally they were in the same for-loop as they (by default) both have 4 options.
		However another programmer might want to add more tags which would cause issues if they're not seperate  */
	for (int i = 0; i < SEVERITY_TAG_COUNT; i++) {
		fileSeverityFilters[i] = profile.fileSeverityFilters[i];
		consoleSeverityFilters[i] = profile.consoleSeverityFilters[i];
	}
	for (int i = 0; i < LOG_TAG_COUNT; i++) { 
		fileTagFilters[i] = profile.fileLogTagFilters[i];
		consoleTagFilters[i] = profile.consoleLogTagFilters[i];
	}
}