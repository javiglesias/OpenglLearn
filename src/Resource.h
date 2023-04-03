#pragma once

class cResource
{
public:
	enum RESOURCE_STATUS
	{
		INACTIVE,
		LOADING,
		INITIALIZED,
		READY,
		REMOVED
	};
	RESOURCE_STATUS Status { RESOURCE_STATUS::INACTIVE };
};