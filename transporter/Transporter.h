#pragma once

#if defined(_MSC_VER)
	#if defined(TRANSPORTER_EXPORTS)
		#define TRANSPORTER_DLL __declspec(dllexport)
	#else
		#define TRANSPORTER_DLL __declspec(dllimport)
	#endif
#else
	#define TRANSPORTER_DLL
#endif
