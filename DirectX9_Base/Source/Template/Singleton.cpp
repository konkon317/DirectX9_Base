#include "Singleton.h"


const int MaxFinalizerSize = 256;
static int NumFinalizerSize = 0;
static SingletonFinalizer::FinalizerFunc Finalizers[MaxFinalizerSize];

void SingletonFinalizer::AddFinalizer(FinalizerFunc func)
{
	assert(NumFinalizerSize < MaxFinalizerSize);
	Finalizers[NumFinalizerSize] = func;
	NumFinalizerSize++;

}

void  SingletonFinalizer::Finalize()
{
	char szBuff[256];
	wsprintf(szBuff, "%d", NumFinalizerSize);
	std::string st = szBuff;
	MessageBox(NULL,st.c_str(), TEXT("äJï˙"), MB_OK);

	for (int i = NumFinalizerSize - 1; i >= 0; i--)
	{
		(*Finalizers[i])();//ä÷êîÇÃé¿çs
	}
	NumFinalizerSize = 0;
}

