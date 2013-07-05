#include <iostream>
#include <string.h>
#include <cmath>
#include <AL/alc.h>
#include <AL/alut.h>
using namespace std;

int main(int argc, char** argv)
{
    alutInit(&argc, argv);
    ALuint buffer, source;
    ALbyte buff[22050];

    ALCdevice* rec_device;
    ALCuint rec_f = 44100;
    ALCsizei bufsize = 1024;
    const ALchar *pDeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
    cout << "device list" << endl;
    while(*pDeviceList){
        cout << pDeviceList << endl;
        pDeviceList += strlen(pDeviceList) +1;
    }
    rec_device = alcCaptureOpenDevice(NULL, rec_f
            , AL_FORMAT_MONO16
            , bufsize);
    if(alGetError() != AL_NO_ERROR){
        return 0;
    }
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);
    alcCaptureStart(rec_device);
    ALint sample, status;
    alSourcePlay(source);
    while(true){
        alcGetIntegerv(rec_device
                , ALC_CAPTURE_SAMPLES
                , (ALCsizei)sizeof(ALint)
                , &sample);
        alcCaptureSamples(rec_device
                , (ALCvoid *)buff
                , sample);
        if(sample > 0){
            cout << sample << endl;
        }
        alBufferData(buffer
                , AL_FORMAT_MONO16
                , buff
                , sample
                , rec_f);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcei(source, AL_LOOPING, AL_TRUE);
        alGetSourcei(source, AL_SOURCE_STATE, &status);
        if(status != AL_PLAYING){
            for(int i=0; i<sample; i++){
                cout << (int)buff[i] << " ";
            }
            cout << "Not Playing " << sample << endl;
            alSourcePlay(source);
        }
        alutSleep(1);
    }
    alcCaptureStop(rec_device);
    alcCaptureCloseDevice(rec_device);
    int freq = 10000, Hz =440;
    ALshort data[freq/Hz];
    for(int i=0; i<freq/Hz; ++i){
        data[i] = 32767 * sin(i * 3.14159 * 2 * Hz / freq);
    }
    alBufferData(buffer
            , AL_FORMAT_MONO16
            , data
            , freq/Hz * sizeof(ALshort), freq);
    alSourcePlay(source);
    alutSleep(2);
    return 0;
}

