LONGLONG Freq;

LONGLONG GetPTC(void)
{
     LARGE_INTEGER LI;
     QueryPerformanceCounter(&LI); 
     return(LI.QuadPart);
}
LONGLONG GetPTCfreq(void)
{
     LARGE_INTEGER LI;
     QueryPerformanceFrequency(&LI); 
     Freq=LI.QuadPart;
     return(LI.QuadPart);
}
