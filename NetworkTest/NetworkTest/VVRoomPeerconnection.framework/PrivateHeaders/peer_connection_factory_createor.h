#ifndef PEER_CONNECTION_FACTORY_CREATEOR_H
#define PEER_CONNECTION_FACTORY_CREATEOR_H

#include <string>
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectionfactory.h"
#include "webrtc/modules/video_capture/video_capture.h"
#include "webrtc/modules/video_capture/video_capture_factory.h"
#include "webrtc/media/base/videocapturer.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/media/base/fakevideocapturer.h"
#include "webrtc/api/test/fakeconstraints.h"
#include "webrtc/modules/audio_device/include/fake_audio_device.h"

#ifdef WEBRTC_WIN
#include <windows.h>
#endif
#include <vector>  
#include "peerconnection/screen_share_capture.h"

class PeerConnectionFactoryCreator {
public:
	static PeerConnectionFactoryCreator* GetInstance(bool fake_audio = false);
	static void Release();

	~PeerConnectionFactoryCreator();

	webrtc::PeerConnectionFactoryInterface* GetPeerConnectionFactory();

	bool OpenCamera();

	bool OpenFakeCamera();

	bool OpenScreenShareCapture();

	void CloseCamera();

	void CloseFakeCamera();

	void StopScreenShareCapture();

	void CloseScreenShareCapture();

	void SetInputFormat(int pWidth, int pHeight, int fps);

	void GetOutputFormat(int* pWidth, int* pHeight, int* fps);

	rtc::scoped_refptr<webrtc::AudioTrackInterface> CreateAudioTrack(std::string audioLabel);

	rtc::scoped_refptr<webrtc::VideoTrackInterface> CreateVideoTrackWithSource(std::string videoLabel);

	rtc::scoped_refptr<webrtc::VideoTrackInterface> CreateFakeVideoTrackWithSource(std::string videoLabel);

	rtc::scoped_refptr<webrtc::VideoTrackInterface> CreateScreenTrackWithSource(std::string videoLabel);

	rtc::Thread* signaling_thread() { return _signaling_thread.get(); }

	rtc::Thread* worker_thread() { return _worker_thread.get(); }

	std::vector<std::string>& GetVideoDevices();

	bool SetCameraIndex(int nIndex); 

private:
	PeerConnectionFactoryCreator(bool fake_audio = false);

	cricket::VideoCapturer* OpenVideoCaptureDevice(int cameraId);

	static PeerConnectionFactoryCreator* _self;

	webrtc::PeerConnectionFactoryInterface* _peer_connection_factory;
	//rtc::scoped_refptr<webrtc::VideoTrackSourceInterface> _trackSource;

	std::unique_ptr<rtc::Thread> _network_thread;
	std::unique_ptr<rtc::Thread> _worker_thread;
	std::unique_ptr<rtc::Thread> _signaling_thread;

	cricket::VideoCapturer* _capturer = NULL;

	cricket::VideoCapturer* _fake_capturer = NULL;

	ScreenShareCapturer* _screenShareCapturer = NULL;

	webrtc::FakeConstraints* _capturer_constraints = NULL;

	std::vector<std::string> _video_device_names;

	int _camera_index = 0;

	webrtc::FakeAudioDeviceModule _adm;
};

#ifdef WEBRTC_ANDROID
extern bool OpenAndroidCamera();
extern void CloseAndroidCamera();
extern rtc::scoped_refptr<webrtc::VideoTrackInterface> CreateAndroidVideoTrack(std::string trackId);
extern rtc::scoped_refptr<webrtc::AudioTrackInterface> CreateAndroidAudioTrack(std::string trackId);
#elif  WEBRTC_IOS
extern bool OpenIOSCamera();
extern void CloseIOSCamera();
extern rtc::scoped_refptr<webrtc::VideoTrackInterface> CreateIOSVideoTrack(std::string trackId);
extern rtc::scoped_refptr<webrtc::AudioTrackInterface> CreateIOSAudioTrack(std::string trackId);
#endif // WEBRTC_ANDROID

#endif // PEER_CONNECTION_FACTORY_CREATEOR_H
