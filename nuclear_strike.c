/*
  nuclear strike by localghost
  work of drugs, my damaged brain and kabbala adeptz :)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define VOLUME 133

#define RANGE_MIN 0
#define RANGE_MAX 255

WAVEHDR WaveHdr;
int koef = 160000;

void Errinf()
{
 char szBuf[256]; 
 void *lpMsgBuf;
 DWORD dwCode;

 if( (dwCode = GetLastError()) != 0 ) 
  {
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dwCode,
        NULL,
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    wsprintf( szBuf, "Failed with error %d: %s", dwCode, lpMsgBuf ); 
    MessageBox(NULL, szBuf, "Error", MB_ICONERROR); 

    LocalFree(lpMsgBuf);
  }
 
}


void GenSnd( char *ptr, int sz )
 {
  int i;
  char prev = 1;

  for( i=0; i < sz; i++ )
   {
    if( prev != 0) 
     {
      ptr[i] = prev;
      prev--;
     }
    else
    {
     ptr[i] = (((double) rand() / (double) RAND_MAX) * RANGE_MAX + RANGE_MIN);
     prev = ptr[i]; 
    }

   }
  
 }

void SetFreq( int pos1, int pos2, int hz )
{
  float v, step;

  v = VOLUME;
  step = -v / (koef / hz);
    
   while( pos1 < pos2 )
   {   
    *(unsigned int *)(WaveHdr.lpData + pos1) = (unsigned int)v;
    
    v += step;
    if (abs(v) > VOLUME) step = -step;
    pos1 += 4;
   }
}

int main( int argc, char **argv )
 {
  HWAVEOUT hWaveOut;
  WAVEFORMATEX WaveFormat;
  MMRESULT mmRes;

  ULONG BufferSize = 16000 * 2;

  WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
  WaveFormat.nChannels = 1;
  WaveFormat.nSamplesPerSec = 16000;
  WaveFormat.nBlockAlign = 2;
  WaveFormat.wBitsPerSample = 8;
  WaveFormat.nBlockAlign = WaveFormat.nChannels*WaveFormat.wBitsPerSample / 8;
  WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec*WaveFormat.nBlockAlign;
  WaveFormat.cbSize = 0;

  printf( "\n" );
if( argc < 2 ) printf( " U can also use freq number by typing \"niclear_strike NUMBAH\"\n for example 35000 for nuclear sound, 600000 some techno tunes\n");
else koef = atoi( argv[1] );

mmRes = waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFormat, 0, 0L, CALLBACK_NULL);

if(mmRes != MMSYSERR_NOERROR )
 { 
  Errinf();
  return -1;
 }

WaveHdr.dwFlags = 0;
WaveHdr.dwLoops = 0;
WaveHdr.lpData = malloc(BufferSize);
WaveHdr.dwBufferLength = BufferSize;

srand( (unsigned)time( NULL ) );

printf( " Press CTRL^C to stop the madness, but remember -\n 'Mescalino is the only way to fly'\n" );

while(1) { 
	GenSnd(WaveHdr.lpData, BufferSize-1); 
	SetFreq( 0, BufferSize-1, 40000 ); //freq
	waveOutPrepareHeader(hWaveOut, &WaveHdr, sizeof(WAVEHDR));

 	mmRes = waveOutWrite( hWaveOut, &WaveHdr, sizeof(WAVEHDR));
 	waveOutUnprepareHeader(hWaveOut, &WaveHdr, sizeof(WAVEHDR));
 }

free(WaveHdr.lpData);
  
}
