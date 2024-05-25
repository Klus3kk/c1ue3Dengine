#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H
#include "cJSON/cJSON.h"
#include "file_operations/tinyfiledialogs.h"

	/**
	 * Saves the current project state to a JSON file.
	 * This function will invoke a file dialog allowing the user to specify the file path and name.
	 */
	void save_project();

	/**
	 * Loads a project state from a JSON file.
	 * This function will invoke a file dialog allowing the user to select the file to load.
	 */
	void load_project();

	/**
	 * Creates a new project by resetting the current state to default values.
	 * This typically involves clearing all data structures and reinitializing them.
	 */
	void new_project();

#ifdef __cplusplus
}
#endif

#endif /* FILE_OPERATIONS_H */
