#ifndef ROOM_CLIENT_H
#define ROOM_CLIENT_H

#include "base/BaseClass.h"
#include "protoo/protoo.h"
#include "mediasoup/Room.h"
#include <string>
#include <vector>

#define MAX_HEART_BEAT_TIME_OUT 120000

typedef struct _PeerInfo{
public:
	_PeerInfo(std::string peerName, std::string displayName, std::string role, std::string userObject)
		: _peerName(peerName)
		, _displayName(displayName)
		, _role(role)
		, _userObject(userObject) {}

	_PeerInfo(std::string peerName, std::string displayName, std::string role, std::string userObject, std::string _source)
		: _peerName(peerName)
		, _displayName(displayName)
		, _role(role) 
		, _source(_source)
		, _userObject(userObject) {}

	Json::Value ToJson() {
		Json::Value data(Json::objectValue);
		data["peerName"] = _peerName;
		data["peerId"] = _peerName; //  duplicated, just adapter to app data struct
		data["displayName"] = _displayName;
		data["nickName"] = _displayName; // duplicated, just adapter to app data struct
		data["role"] = _role;
        data["userObject"] = _userObject;
		data["source"] = _source;

		return data;
	}

	std::string _peerName;
	std::string _displayName;
	std::string _role;
	std::string _userObject;
	std::string _source;
}PeerInfo;

class TransportInfo {
public:
	enum TransportLevel {
		LOW,
		WEEK,
		MIDDLE,
		HIGH
	};

	TransportInfo() {}
	TransportInfo(std::string peerName, TransportLevel level, int fps, int packets, int packetLoss, int bytes, float lossRate, int currentDelayMs)
		: _peerName(peerName)
		, _level(level)
		, _fps(fps)
		, _packets(packets)
		, _packetLoss(packetLoss)
		, _bytes(bytes)
		, _lossRate(lossRate)
		, _currentDelayMs(currentDelayMs){}

	Json::Value ToJson() {
		Json::Value data(Json::objectValue);
		data["peerName"] = _peerName;
		data["peerId"] = _peerName; // just adapter to app data struct
		data["level"] = (int)_level;
		data["fps"] = _fps;
		data["packets"] = _packets;
        data["packetLoss"] = _packetLoss;
		data["bytes"] = _bytes;
		data["lossRate"] = _lossRate;
        data["currentDelayMs"] = _currentDelayMs;

		return data;
	}

	std::string _peerName;
	TransportLevel _level = TransportLevel::LOW;
	int _fps = 0;
	int _packets = 0;
	int _packetLoss = 0;
	int _bytes = 0;
	float _lossRate = 0.0;
	int _currentDelayMs = 0;
} ;

enum SdkType {
	DEFAULT = 0,
	CLASS
};

class RoomCallBack {
public:
	virtual ~RoomCallBack() {}

	/**
	 * websockt���ӳɹ�
	 */
	virtual void OnConnected() = 0;

	/**
	 * websocket����ʧ��
	 */
	virtual void OnConnectFail() = 0;

	/**
	 * �յ�websocket��Ϣ
	 * @param method
	 * @param data
	 */
	virtual void OnRecvRequest(std::string method, std::string data) = 0;

	/**
	 * ��������Ƶʧ��
	 * @param error
	 */
	virtual void OnCreateSessionFail(std::string error) = 0;

	/**
	* ��consumerʧ�ܣ������������ڷ�����ĳ����Ա�쳣�˳����£����δ�����ʱ��Ӧ�ò���Ը�����һ����ʾ������������������
	* @param error
	*/
	virtual void OnEnableConsumerFail(std::string error) = 0;

	/**
	* ������Ƶ����״̬
	* @param bSuccess
	*/
	virtual void OnModifyLocalAudio(bool bOpen) = 0;

	/**
	* ������Ƶ����״̬
	* @param bSuccess
	*/
	virtual void OnModifyLocalVideo(bool bOpen) = 0;

	/**
	 *
	 */
	virtual void OnVideoRejectedByServer() = 0;

	/**
	* ������Ļ������״̬
	* @param bSuccess
	*/
	virtual void OnModifyScreenShare(bool bOpen) = 0;

	/**
	 * ����������Ƶ�ɹ�
	 * @param track
	 */
	virtual void OnOpenVideoDeviceSuccess(rtc::scoped_refptr<webrtc::VideoTrackInterface> track, std::string source) = 0;

	/**
	 * ����Զ����Ƶ�Ự�ɹ�
	 * @param track
	 * @param displayName
	 * @param peerName
	 * @param role
	 */
	virtual void OnAddRemoteVideoStreamSuccess(rtc::scoped_refptr<webrtc::VideoTrackInterface> track, PeerInfo peerInfo) = 0;

    /**
	 * Զ����Ƶ�ر�
	 * @param peerName
	 * @param kind
	 */
    virtual void OnConsumerStreamClosed(std::string peerName, std::string kind, std::string source) = 0;

    /**
	 * Զ����Ƶ����
	 * @param peerName
	 * @param kind
	 */
	virtual void OnConsumerResume(std::string peerName, std::string kind) = 0;

	/**
	 * Զ����Ƶ��ͣ
	 * @param peerName
	 * @param kind
	 */
	virtual void OnConsumerPause(std::string peerName, std::string kind) = 0;


	/**
	 * ������Ƶ/��Ƶ���ر�/����
	 * @param kind
	 * @param status
	 */
	virtual void OnRecvTrackSlient(std::string kind, bool status) = 0;

	/**
	 * ��ʼ�Ͽ�
	 * @param peerinfos����ǰ��Ա��Ϣ
	 */
	virtual void OnClassStart(std::vector<PeerInfo> peerinfos, std::string extraInfoFromServer) = 0;

	/**
	 * �����Ͽ�
	 */
	virtual void OnClassOver() = 0;

	/**
	 * �³�Ա����
	 * @param peerName
	 * @param displayName
	 */
	virtual void OnNewPeer(PeerInfo peerInfo) = 0;

	/**
	 * ��Ա�˳�
	 * @param peerName
	 */
	virtual void OnPeerClose(std::string peerName) = 0;

	/**
	 * ����״̬�ص�
	 * @param transportinfos
	 */
	virtual void OnTransportStatus(std::vector<TransportInfo*> transportInfos) = 0;

	/**
	* �����Ƶ�豸��Ϣ
	* @param devices
	*/
	virtual void OnGetVideoDevices(std::vector<std::string> devices) = 0;

	/**
	* �����Ƶ�豸��Ϣ
	* @param devices
	*/
	virtual void OnGetAudioDevices(std::vector<std::string> devices) = 0;

	/**
	* ��������ͷ�����ص�
	* @param succ
	*/
	virtual void OnSetVideoDevice(bool succ) = 0;

	virtual void OnGetOutputFormat(int pWidth, int pHeight, int fps) = 0;
};

class RoomClient : public Room::Listener
	, Peer::Listener
	, Consumer::Listener 
	, ProtooRecvRequestCallBack
	, sigslot::has_slots<> 
	, webrtc::Module {
public:
	RoomClient(std::string token, std::string roomId, std::string kind, std::string peerName, std::string displayName, 
		std::string role, std::string device, std::string userObject, RoomCallBack* track_call_back);

	virtual ~RoomClient();

	void setRole(std::string role);

	void SetSdkType(SdkType sdkType);

	void ReInitRoom(std::string roomId, std::string kind, std::string peerName, std::string displayName,
		std::string role, std::string userObject, std::string token);

	void ConnectRequestRoom(std::string serverIP, int serverPort, int timeout_ms);

	void LeaveRoom();

	void SendRequest(std::string data, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata, bool notify = false);

	void QueryRoomData(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void SendRouteRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void SendPPTRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void SendVideoRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void SendChatRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void SendPathRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void SendRequest(std::string data, std::string method
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void SendDrawBoardPagesRequest(std::string data, ProcessCallBack successCallBack,
								   ProcessCallBack failCallBack, void *userdata);

	void SendDrawBoardIndexRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void SendDrawBoardOriRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	/**
	 * ������Ȩ/����
	 * @param data
	 * @param successCallBack
	 * @param failCallBack
	 * @param userdata
	 */
	void SendReqContentControlRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	/**
	 * �����ɫ�ı�
	 * @param data
	 * @param successCallBack
	 * @param failCallBack
	 * @param userdata
	 */
	void SendReqRoleChangeRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	/**
	 * ��Ȩ ��ʦ/���̸�ѧ��/������ѡ
	 * @param data
	 * @param successCallBack
	 * @param failCallBack
	 * @param userdata
	 */
	void SendContentControlAuthRequest(std::string data
			, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);
	/**
	* �����û��Զ�����Ϣ
	* @param data ��Ϣ����
	* @param toSelf ��Ϣ�Ƿ��͸��Լ�
	*/
	void SendUserData(std::string data, std::vector<std::string> targets, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	/**
	* ��ȡ�û��Զ�����Ϣ��ʷ
	*/
	void GetUserDataHistory(ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void CreateMedia();

	void DisableCamera();

	void EnableCamera();

	void TryToEnableCamera(ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata);

	void MuteMic();

	void UnMuteMic();

	void SlientTrack(std::string kind, std::string peerName, bool status);

	void SetPeerPause(std::string peerName, std::string kind, bool pause);

	void GetVideoDevices();

	void GetAudioDevices();

	void SetVideoDevice(int nIndex);

	void AddRolesForAudioVideo(std::string role);

	void SetInputFormat(int pWidth, int pHeight, int fps);

	void EnableScreenShare();

	void DisableScreenShare();

	void FocusOnMember(std::string peerName);

	/// impl Room::Listener
	virtual void RoomSendRequest(Json::Value data, ProcessCallBack successCallBack, ProcessCallBack failCallBack, void* userdata) override;

	virtual void RoomSendNotify(Json::Value data)  override;

	virtual void CreateTransport() override;

	/// impl  Consumer::Listener 
	virtual void Close(int64_t id) override {}

	virtual void Close(int64_t id, std::string originator) override;

	/// impl Peer::Listener
	virtual bool NewConsumer(Consumer* consumer) override;

	/// impl ProtooRecvRequestCallBack
	virtual void RecvData(std::string recMethod, std::string data) override;

	virtual void OnProtooConnected(int code) override;

	virtual void OnProtooDisConnected() override;

private:
	void OnJoinMe(std::string extraInfoFromServer);

	void OnLeaveMe();

	bool Join(std::string displayName, std::string device, std::string extraInfoFromServer);

	bool JoinRoom(std::string displayName, std::string device, std::string extraInfoFromServer);

	bool HandleConsumer(Consumer* consumer);

	bool SetMicProducer();

	bool EnableWebcam();

	void closeMicProducer();

	void closeWebcamProducer();

	void ActiveSpeakerCallBack(Json::Value value);

	void TrackSlient(Json::Value data);

	void ConsumerEffectiveProfileChangedCallBack(Json::Value value);

	void SignalThreadDestroyed();

	void OnHeartBeat();

	//impl webrtc::Module
	virtual int64_t TimeUntilNextProcess() override;

	virtual void Process() override;

private:
	SdkType _sdk_type = SdkType::DEFAULT;

	std::unique_ptr<webrtc::CriticalSectionWrapper> _network_lock;
//	Network* _network = NULL;

	ProtooClient* _protoo = NULL;

	std::string _peerName;
	// Closed flag.
	bool _closed = false;

	// Whether we should produce.
	void *_produce = NULL;

	// Whether simulcast should be used.
	void *_useSimulcast = NULL;

	// Redux store dispatch function.
	void *_dispatch = NULL;

	// Redux store getState function.
	void *_getState = NULL;

	// mediasoup-client Room instance.
	Room* _room = NULL;

	// Transport for sending.
	Transport* _sendTransport = NULL;

	// Transport for receiving.
	Transport* _recvTransport = NULL;

	// Local mic mediasoup Producer.
	Producer* _micProducer = NULL;

	// Local webcam mediasoup Producer.
	Producer* _webcamProducer = NULL;

	Producer* _screenShareProducer = NULL;

	std::unique_ptr<webrtc::ProcessThread> _working_thread;

	RoomCallBack* _room_call_back = NULL;

	std::string _token;

	std::string _roomId;

	std::string _displayName;

	std::string _role;

	std::string _device;

	std::string _userObject;

	std::string _kind;

	std::string _focusOnPeer;

	std::vector<std::string> _ignoreRolesForAudioVideo;

	std::string _joinedResponseData;

	bool _in_room = true;

	int64_t _last_heart_beat = 0;

	std::vector<std::string> _audio_device;
};

#endif