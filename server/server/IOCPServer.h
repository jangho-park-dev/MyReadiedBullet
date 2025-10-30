#pragma once
#pragma warning(disable:4996)
#include <iostream>
#include <map>
#include <thread>
#include <memory>
#include <mutex>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include "Protocol.h"
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock.lib")
using namespace std;

struct EXOVER
{
	WSAOVERLAPPED Over;
	ENUMOP Op;
	char IO_Buf[MAXBUFFER];
	WSABUF WSABuf;
};
struct CLIENT
{
	SOCKET m_Sock;
	EXOVER m_RecvOver;
	int m_PrevSize;
	char m_PacketBuf[MAXPACKETSIZE];

	//∞‘¿”ƒ‹≈Ÿ√˜¡§∫∏
	char name[MAX_ID_LEN + 1];
	
	Player m_Player;
};


class IOCPServer
{
public:
	HANDLE m_iocp;
	int m_current_user_id;
	//CLIENT m_Clients[MAXPLAYER];
	SOCKET m_ListenSock;
	map<int, CLIENT> m_Clients;
	bool m_IsGameStart;
	thread* volatile m_GameTimerThread;


	void mainLoop();
	void ProcessPacket(int user_id, char* buf);
	/*void ProcessTimer();
	void StartTimer();
	void StopTimer();*/

	
	//void Broad_Cast(int user_id, void* p);
	void Disconnect(int user_id);

	void Send_Packet(int user_id, void* p);

	/*void Enter_Game(int user_id);
	void Select_Team_Process(int user_id);
	void Select_CharacterType_Process(int user_id);
	void Move_Character_Process(int user_id);
	void Shooter_Fire_Process(int user_id);
	void Roller_Paint_Process(int user_id);
	void Bomber_Throw_Process(int user_id);
	void Reloading_Process(int user_id);
	void GameResult_Process(int user_id, int red, int green);

	bool Check_IsAllReady();

	
	void Send_Login_OK_Packet(int user_id);
	void Send_Enter_Packet(int user_id, int Other_id);
	void Send_TeamType_Packet(int user_id, int Other_id);
	void Send_CharacterType_Packet(int user_id, int Other_id);
	void Send_Leave_Packet(int user_id, int Other_id);
	void Send_StartGame();
	void Send_PlayerInfo_Packet(int user_id, int Other_id);
	void Send_ShooterFire_Packet(int user_id, int Other_id);
	void Send_RollerPaint_Packet(int user_id, int Other_id);
	void Send_BomberThrow_Packet(int user_id, int Other_id);
	void Send_Reloading_Packet(int user_id, int Other_id);
	void Send_TimerPacket();
	void Send_EndGame();
	void Send_ResultGame_Packet(int ID, int red, int green);*/
};

