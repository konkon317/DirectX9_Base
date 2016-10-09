#include "Application.h"


Application::Application()
{
	char szBuff[256];
	wsprintf(szBuff, "Application");
	std::string st = szBuff;
	MessageBox(NULL, st.c_str(), TEXT("ì¬"), MB_OK);
}

Application::~Application()
{
	char szBuff[256];
	wsprintf(szBuff, "ŠJ•úApplication");
	std::string st = szBuff;
	MessageBox(NULL, st.c_str(), TEXT("ŠJ•ú"), MB_OK);
};
