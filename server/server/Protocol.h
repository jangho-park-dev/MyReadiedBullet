#pragma once
#define SERVERPORT 9000
#define MAXBUFFER 8192
#define MAXPLAYER 2
#define MAXPACKETSIZE 255
#define MAX_ID_LEN 30

enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT };

enum class e_PacketType : uint8_t
{
	e_LoginOK,
	e_Select_GameMode,
	e_LeavePacket,
	e_ReadyPacket,
	e_StartPacket,
	e_PlayerInfoPacket,
	e_EnterPacket,
	e_myCharacterPacket,
	e_BulletSpawnPacket,
	e_BulletSlotPacket,
	e_BulletMovePacket,
	e_BulletRotPacket,
	e_CharacterDeadPacket,
	e_CharacterReloadPacket,
	e_CharacterLightPacket,
};

enum class e_bulletType : uint8_t
{
	e_Bullet1 = 1,
	e_Bullet2,
	e_Bullet3,
};

#pragma pack(push, 1)
struct PlayerPosition
{
	float x;
	float y;
	float z;
};
struct PlayerRotation
{
	float Pitch;
	float Yaw;
	float Roll;
};
struct PlayerVelocity
{
	float vx;
	float vy;
	float vz;
};
struct PlayerInfo
{
	PlayerPosition m_Position;
	PlayerRotation m_Rotation;
	PlayerVelocity m_Velocity;
};
struct BulletSlotData
{
	float x;
	float y;
	float z;
};

struct sc_packet_loginOK
{
	char size;
	e_PacketType type;
	int m_id;
};

struct cs_packet_selectGameMode
{
	char size;
	e_PacketType type;
	unsigned char mode;// 0 = solo , 1 = multi
};

struct cs_packet_leavePacket
{
	char size;
	e_PacketType type;
	int m_id;
};

struct sc_packet_leavePacket
{
	char size;
	e_PacketType type;
	int m_id;
};

struct cs_packet_readyPacket
{
	char size;
	e_PacketType type;
	int m_id;
	bool isReady;
};

struct cs_packet_playerInfo
{
	char size;
	e_PacketType type;
	int m_id;
	PlayerInfo info;
};

struct sc_packet_playerInfo
{
	char size;
	e_PacketType type;
	int m_id;
	PlayerInfo info;
};

struct sc_packet_startPacket
{
	char size;
	e_PacketType type;
	int m_id;
	PlayerPosition pos;
	PlayerRotation rot;
};

struct sc_packet_enterPacket
{
	char size;
	e_PacketType type;
	int m_id;
	PlayerPosition pos;
	PlayerRotation rot;
};

struct sc_packet_myCharacterPacket
{
	char size;
	e_PacketType type;
	int m_id;
	PlayerPosition pos;
	PlayerRotation rot;
};

struct cs_packet_bulletSpawnPacket
{
	char size;
	e_PacketType type;
	e_bulletType bulletType;
	int m_id;
	PlayerPosition pos;
	PlayerRotation rot;
};

struct sc_packet_bulletSpawnPacket
{
	char size;
	e_PacketType type;
	e_bulletType bulletType;
	int m_id;
	PlayerPosition pos;
	PlayerRotation rot;
};

struct cs_packet_bulletSlotPacket
{
	char size;
	e_PacketType type;
	e_bulletType bulletType;
	int m_id;
};

struct sc_packet_bulletSlotPacket
{
	char size;
	e_PacketType type;
	e_bulletType bulletType;
	int m_id;
};

struct cs_packet_bulletRotPacket
{
	char size;
	e_PacketType type;
	int m_id;
	BulletSlotData slot1;
	BulletSlotData slot2;
	BulletSlotData slot3;
};

struct sc_packet_bulletRotPacket
{
	char size;
	e_PacketType type;
	int m_id;
	BulletSlotData slot1;
	BulletSlotData slot2;
	BulletSlotData slot3;
};

struct cs_packet_deadPacket
{
	char size;
	e_PacketType type;
	int m_id;
};

struct sc_packet_deadPacket
{
	char size;
	e_PacketType type;
	int m_id;
};

struct cs_packet_reloadPacket
{
	char size;
	e_PacketType type;
	int m_id;
};

struct sc_packet_reloadPacket
{
	char size;
	e_PacketType type;
	int m_id;
};

struct cs_packet_lightPacket
{
	char size;
	e_PacketType type;
	int m_id;
};

struct sc_packet_lightPacket
{
	char size;
	e_PacketType type;
	int m_id;
};

#pragma pack(pop)


//플레이어 정보//
class Player
{
public:
	Player() {}
	~Player() { }

	PlayerInfo m_PlayerInfo;
	int m_ID;
	bool m_isReady = false;
	char m_gamemode;
	BulletSlotData slot1;
	BulletSlotData slot2;
	BulletSlotData slot3;
};
struct OverlappedEx
{
	WSAOVERLAPPED m_Overlapped;
	WSABUF m_WSABuf;
	char m_DataBuf[MAXBUFFER];
};
struct SOCKETINFO
{
	//WSAOVERLAPPED m_RecvOverlapped; //1개만써야함 ,  recv는1개지만 send용Overlapped는 그때그때 new로 할당해주어서 사용 콜백이불리면 delete
	//WSABUF m_WSASendBuf; //다중 송신을위해서는 여러개 써야함.     위와마찬가지로 new로 할당, 콜백이불리면 delete
	//WSABUF m_WSARecvBuf; // 1개만써야함
	SOCKET m_Socket;
	OverlappedEx* m_SendOverlappedEx;
	OverlappedEx* m_RecvOverlappedEx;

	//char m_SendBuf[MAXBUFFER]; //다중 송신을위해서는 여러개 써야함.   위와마찬가지로 new로 할당, 콜백이불리면 delete
	//char m_RecvBuf[MAXBUFFER]; // 1개만써야함.
	//Position m_PlayerPosition;
	char m_CompletePacketBuf[MAXBUFFER];
	int m_PacketPrevSize;
	int recvBytes = 0;
	int sendBytes = 0;
	Player m_Player;

	SOCKETINFO()
	{
		m_RecvOverlappedEx = new OverlappedEx;
		memset(&(m_RecvOverlappedEx->m_Overlapped), 0x00, sizeof(WSAOVERLAPPED));
		m_RecvOverlappedEx->m_WSABuf.buf = m_RecvOverlappedEx->m_DataBuf;
		m_RecvOverlappedEx->m_WSABuf.len = MAXBUFFER;

	}
	~SOCKETINFO() { if (m_SendOverlappedEx != nullptr) delete m_SendOverlappedEx; delete m_RecvOverlappedEx; }
};

////////////////