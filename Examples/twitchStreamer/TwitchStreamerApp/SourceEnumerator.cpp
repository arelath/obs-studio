#include "SourceEnumerator.h"
#include "obs.h"

#include <iostream>

SourceEnumerator::SourceEnumerator::SourceEnumerator()
{}

void SourceEnumerator::EnumerateSources()
{
	const char *id;
	int index = 0;
	while (obs_enum_source_types(index++, &id))
	{
		std::cout << "Found source " << id << std::endl;
	}
}

