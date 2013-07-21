#include <iostream>
#include <string.h>
#include <cmath>
#include <AL/alc.h>
#include <AL/alut.h>
using namespace std;

#define BUFFER_SIZE 3
#define rec_f 44100

struct BUFFER{
    ALuint buffer[BUFFER_SIZE];
    ALint status[BUFFER_SIZE];
};

void initialize_buffer(struct BUFFER &buffer)
{
    alGenBuffers(BUFFER_SIZE, buffer.buffer);
    for(int i=0; i<BUFFER_SIZE; i++){
        buffer.status[i] = 0;
    }
    return;
}

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

ALint get_useable_buffer_number(struct BUFFER &buffer)
{
    for(ALint i=0; i<BUFFER_SIZE; i++){
        cout << buffer.status[i] << endl;
        if(buffer.status[i] == 0){
            return i;
        }
    }
    return -1;
}

void que_buffer(ALuint source
        , struct BUFFER &buffer
        , ALvoid *data
        , ALint data_size)
{

    ALint buffer_number = get_useable_buffer_number(buffer);
    if(buffer_number < 0){
        cout << "No useable buffer." << endl;
        return;
    }
    alBufferData(buffer.buffer[buffer_number]
                , AL_FORMAT_MONO16
                , data
                , data_size
                , rec_f);
    alSourceQueueBuffers(source, 1, &buffer.buffer[buffer_number]);
    buffer.status[buffer_number] = 1;
}

void deque_buffer(ALuint source, struct BUFFER &buffer)
{
    ALint used_buffer;
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &used_buffer);
    cout << "used buffer = " << used_buffer << endl;
    if(used_buffer == 0) return;

    alSourceUnqueueBuffers(source, 1, &buffer.buffer[used_buffer]);
    buffer.status[used_buffer] = 0;
    deque_buffer(source, buffer);
    return;
}

int main(int argc, char** argv)
{
    alutInit(&argc, argv);
    ALuint source;
    struct BUFFER buffer;
    ALshort buff[22050];
    ALint sample, status;

    ALCdevice* rec_device;
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
    initialize_buffer(buffer);
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
        deque_buffer(source, buffer);
        que_buffer(source, buffer, buff, sample*2);
        alGetSourcei(source, AL_SOURCE_STATE, &status);
        if(status != AL_PLAYING){
            alSourcePlay(source);
        }
        errorCheck();
        alutSleep(0.05);
    }
    alcCaptureStop(rec_device);
    alcCaptureCloseDevice(rec_device);
    int freq = 10000, Hz =440;
    ALshort data[freq/Hz];
    for(int i=0; i<freq/Hz; ++i){
        data[i] = 32767 * sin(i * 3.14159 * 2 * Hz / freq);
    }
    alBufferData(buffer.buffer[0]
            , AL_FORMAT_MONO16
            , data
            , freq/Hz * sizeof(ALshort), freq);
    alSourcei(source, AL_BUFFER, buffer.buffer[0]);
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcePlay(source);
    alutSleep(2);
    return 0;
}

