/*  Audio Input Device Info
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <set>
#include <QtGlobal>
#include "Common/Cpp/Pimpl.tpp"
#include "AudioInfo.h"


#if QT_VERSION_MAJOR == 5
#include <QAudioDeviceInfo>
namespace PokemonAutomation{


struct AudioInfo::Data{
    std::string device_name;    //  For serialization
    QString display_name;
    QAudioDeviceInfo info;
};
std::vector<AudioInfo> AudioInfo::all_input_devices(){
    QList<QAudioDeviceInfo> list = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    std::set<QString> existing;

    std::vector<AudioInfo> ret;
    for (QAudioDeviceInfo& info : list){
        QString name = info.deviceName();

        //  Skip duplicates.
        if (existing.find(name) != existing.end()){
            continue;
        }
        existing.insert(name);

        std::string id = name.toStdString();

        ret.emplace_back();
        Data& data = ret.back().m_body;

        data.device_name = name.toStdString();
        data.display_name = std::move(name);
        data.info = std::move(info);
    }
    return ret;
}
std::vector<AudioInfo> AudioInfo::all_output_devices(){
    QList<QAudioDeviceInfo> list = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    std::set<QString> existing;

    std::vector<AudioInfo> ret;
    for (QAudioDeviceInfo& info : list){
        QString name = info.deviceName();

        //  Skip duplicates.
        if (existing.find(name) != existing.end()){
            continue;
        }
        existing.insert(name);

        std::string id = name.toStdString();

        ret.emplace_back();
        Data& data = ret.back().m_body;

        data.device_name = name.toStdString();
        data.display_name = std::move(name);
        data.info = std::move(info);
    }
    return ret;
}


}
#elif QT_VERSION_MAJOR == 6
#include <QAudioDevice>
#include <QMediaDevices>
namespace PokemonAutomation{


struct AudioInfo::Data{
    std::string device_name;    //  For serialization
    QString display_name;
    QAudioDevice info;
};
std::vector<AudioInfo> AudioInfo::all_input_devices(){
    QList<QAudioDevice> list = QMediaDevices::audioInputs();
    std::vector<AudioInfo> ret;
    for (QAudioDevice& info : list){
        ret.emplace_back();
        Data& data = ret.back().m_body;

        data.device_name = info.id().data();
        data.display_name = info.description();
        data.info = std::move(info);
    }
    return ret;
}
std::vector<AudioInfo> AudioInfo::all_output_devices(){
    QList<QAudioDevice> list = QMediaDevices::audioOutputs();
    std::vector<AudioInfo> ret;
    for (QAudioDevice& info : list){
        ret.emplace_back();
        Data& data = ret.back().m_body;

        data.device_name = info.id().data();
        data.display_name = info.description();
        data.info = std::move(info);
    }
    return ret;
}


}
#else
#error "Unknown Qt Version."
#endif





namespace PokemonAutomation{


AudioInfo::AudioInfo(){}
AudioInfo::~AudioInfo(){}

AudioInfo::AudioInfo(const std::string& device_name){
    for (AudioInfo& info : all_input_devices()){
        if (device_name == info.device_name()){
            *this = std::move(info);
            return;
        }
    }
    for (AudioInfo& info : all_output_devices()){
        if (device_name == info.device_name()){
            *this = std::move(info);
            return;
        }
    }
}

AudioInfo::operator bool() const{
    return !m_body->device_name.empty();
}
const std::string& AudioInfo::device_name() const{
    return m_body->device_name;
}
const QString& AudioInfo::display_name() const{
    return m_body->display_name;
}
const NativeAudioInfo& AudioInfo::native_info() const{
    return m_body->info;
}
bool AudioInfo::operator==(const AudioInfo& info){
    return device_name() == info.device_name();
}


}




















