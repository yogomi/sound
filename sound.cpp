#include <iostream>
#include <string.h>
#include <cmath>
#include <AL/alc.h>
#include <AL/alut.h>
using namespace std;

void errorCheck()
{
    switch(alGetError()){
        case AL_NO_ERROR:
            cout << "NO_ERROR" << endl;
            break;
        case AL_INVALID_NAME:
            cout << "INVALID_NAME" << endl;
            break;
        case AL_INVALID_ENUM:
            cout << "INVALID_ENUM" << endl;
            break;
        case AL_INVALID_VALUE:
            cout << "INVALID_VALUE" << endl;
            break;
        case AL_INVALID_OPERATION:
            cout << "INVALID_OPERATION" << endl;
            break;
        case AL_OUT_OF_MEMORY:
            cout << "OUT_OF_MEMORY" << endl;
            break;
        default:
            cout << "Unknown error code" << endl;
            break;
    }
    return;
}

void sourceStatusCheck(ALint status)
{
    switch(status){
        case AL_INITIAL:
            cout << "AL_INITIAL" << endl;
            break;
        case AL_PLAYING:
            cout << "AL_PLAYING" << endl;
            break;
        case AL_PAUSED:
            cout << "AL_PAUSED" << endl;
            break;
        case AL_STOPPED:
            cout << "AL_STOPPED" << endl;
            break;
        default:
            cout << "Unknown source status code" << endl;
            break;
    }
}

void debugLine()
{
    cout << "========================" << endl;
    return;
}

int main(int argc, char** argv)
{
    alutInit(&argc, argv);
    ALuint buffer, source;
    ALshort buff[22050];
    ALint sample, status;

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
    while(true){
        alcGetIntegerv(rec_device
                , ALC_CAPTURE_SAMPLES
                , (ALCsizei)sizeof(ALint)
                , &sample);
        alcCaptureSamples(rec_device
                , (ALCvoid *)buff
                , sample);
        alGetSourcei(source, AL_SOURCE_STATE, &status);
        debugLine();
        if(status == AL_INITIAL || status == AL_STOPPED){
            alSourcei(source, AL_BUFFER, AL_NONE);
        /*
            for(int i=0; i<sample; i++){
                cout << buff[i] << " ";
            }
            */
            alBufferData(buffer
                    , AL_FORMAT_MONO16
                    , buff
                    , sample * 2
                    , rec_f);
            alSourcei(source, AL_BUFFER, buffer);
            cout << "set buffer" << endl;
        }
        alGetSourcei(source, AL_SOURCE_STATE, &status);
        if(status != AL_PLAYING){
            alSourcePlay(source);
        }
        errorCheck();
        alutSleep(0.04);
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
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcePlay(source);
    alutSleep(2);
    return 0;
}

