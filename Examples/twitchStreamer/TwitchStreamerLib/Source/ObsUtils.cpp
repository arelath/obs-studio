
#include <Windows.h>
#include <iostream>>

#include "ObsUtils.h"

using namespace std;

void WaitForDebuggerToAttach()
{
	while (!IsDebuggerPresent()) {
		Sleep(3000);
		std::cout << "Waiting for debugger to attach" << std::endl;
	}
}
