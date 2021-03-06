#pragma once

#include "types.h"

#include "util/except.h"

/**
 * The dotnet library instance
 */
extern System_Reflection_Assembly g_corelib;

/**
 * Loading the corelib itself
 *
 * @param buffer        [IN] The corelib binary
 * @param buffer_size   [IN] The corelib binary size
 */
err_t loader_load_corelib(void* buffer, size_t buffer_size);

err_t loader_load_assembly(void* buffer, size_t buffer_size, System_Reflection_Assembly* assembly);

/**
 * Fill the type information of the given type
 */
err_t loader_fill_type(System_Type type);

/**
 * Fill the method information of the given method
 */
err_t loader_fill_method(System_Type type, System_Reflection_MethodInfo method);

/**
 * Setup a type, this is done before we fill the type information and
 * only takes care of matching everything
 */
err_t loader_setup_type(pe_file_t* file, metadata_t* metadata, System_Type type);
