#include <obs.h>
#include <obs-config.h>

#include <iostream>
#include <memory>

#include "ObsWrapper.h"

using namespace std;



int main()
{
	cout << "Starting ObsWrapper" << endl;

	auto obs = ObsWrapper::CreateOBS();

	return 0;
}
