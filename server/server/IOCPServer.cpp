#pragma once

#include "IOCPServer.h"
#include <iostream>
#include <string>
using namespace std;

void IOCPServer::mainLoop()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	
	m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN ServerAddr;
	memset(&ServerAddr, 0x00, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVERPORT);
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	::bind(m_ListenSock, reinterpret_cast<sockaddr*>(&ServerAddr), sizeof(ServerAddr));

	listen(m_ListenSock, SOMAXCONN);

	//iocp 생성
	m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	// 엑셉트도 iocp로 받기위해 등록.
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_ListenSock), m_iocp, 999, 0);

	//동기 accpet랑 다르게 미리 소켓을 만들어놓고 이를이용해 클라이언트와 통신한다.
	SOCKET Client_Sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EXOVER Accept_over;
	ZeroMemory(&Accept_over.Over, sizeof(Accept_over.Over));
	Accept_over.Op = OP_ACCEPT;
	AcceptEx(m_ListenSock, Client_Sock, Accept_over.IO_Buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &Accept_over.Over);

	while (true)
	{
		DWORD iobytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(m_iocp, &iobytes, &key, &over, INFINITE);
		//완료한 결과를 담고 있는 overlapped구조체를 확장 overlapped구조체로 탈바꿈
		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);
		CLIENT& Client = m_Clients[user_id];
		
		//Accept시 id를 999로
		if(user_id == 999)
			m_Clients[user_id].m_Player.m_ID = user_id;
		switch (exover->Op)
		{
		case OP_RECV:
		{//recv가 완료됐으면 패킷처리

			if (iobytes == 0)
			{
				Disconnect(user_id);
			}
			else
			{
				char* ptr = exover->IO_Buf;
				static size_t in_packet_size = 0;
				while (0 != iobytes)
				{
					if (0 == in_packet_size)
						in_packet_size = ptr[0];
					if (iobytes + Client.m_PrevSize >= in_packet_size)
					{
						memcpy(Client.m_PacketBuf + Client.m_PrevSize, ptr, in_packet_size - Client.m_PrevSize);
						ProcessPacket(user_id, Client.m_PacketBuf);		// 이부분입니다!
						ptr += in_packet_size - Client.m_PrevSize;
						iobytes -= in_packet_size - Client.m_PrevSize;
						in_packet_size = 0;
						Client.m_PrevSize = 0;
					}
					else
					{
						memcpy(Client.m_PacketBuf + Client.m_PrevSize, ptr, iobytes);
						Client.m_PrevSize += iobytes;
						iobytes = 0;
						break;
					}
				}

				ZeroMemory(&Client.m_RecvOver.Over, sizeof(Client.m_RecvOver.Over));
				DWORD flags = 0;

				WSARecv(Client.m_Sock, &Client.m_RecvOver.WSABuf, 1, NULL, &flags, &Client.m_RecvOver.Over, NULL);
			}
		}
		break;
		case OP_SEND:
		{//센드가 완료됐으면 오버랩구조체 메모리를 반환
			if (iobytes == 0)
				Disconnect(user_id);
			delete exover;
		}
		break;
		case OP_ACCEPT:
		{
			int id = m_current_user_id++;

			CreateIoCompletionPort(reinterpret_cast<HANDLE>(Client_Sock), m_iocp, id, 0); //key값으로 id를 준다.

			m_current_user_id = m_current_user_id;// % MAXPLAYER; //아이디가 초과하는것을 방지하기위함.
			CLIENT& newClient = m_Clients[id];
			newClient.m_Player.m_ID = id;
			newClient.m_PrevSize = 0;
			newClient.m_RecvOver.Op = OP_RECV;
			ZeroMemory(&newClient.m_RecvOver.Over, sizeof(newClient.m_RecvOver.Over));
			newClient.m_RecvOver.WSABuf.buf = newClient.m_RecvOver.IO_Buf;
			newClient.m_RecvOver.WSABuf.len = MAXBUFFER;
			newClient.m_Sock = Client_Sock;

			/*sc_packet_loginOK p{};
			p.m_id = m_Clients[id].m_ID;
			p.size = sizeof(p);
			p.type = e_PacketType::e_LoginOK;

			Send_Packet(p.m_id, &p);*/

			// accpet하고난뒤에 초기화할 정보들을 이곳에 추가.
			cout << "Client" << id << " 접속" << endl;

			DWORD flags = 0;
			WSARecv(newClient.m_Sock, &newClient.m_RecvOver.WSABuf, 1, NULL, &flags, &newClient.m_RecvOver.Over, NULL);

			//계속해서 Accept를 받기위함.
			Client_Sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			ZeroMemory(&Accept_over.Over, sizeof(Accept_over.Over));
			AcceptEx(m_ListenSock, Client_Sock, Accept_over.IO_Buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &Accept_over.Over);

		}
		break;
		}
	}

	return;
}
void IOCPServer::Disconnect(int user_id)
{
	m_Clients.erase(user_id);
	cout << "[Disconnect] user_id: " << user_id << endl;
	
	sc_packet_leavePacket p{};
	p.m_id = user_id;
	p.size = sizeof(p);
	p.type = e_PacketType::e_LeavePacket;
	for (auto& client : m_Clients)
	{
		if (client.second.m_Player.m_ID == 999) continue;
		Send_Packet(client.second.m_Player.m_ID, &p);
	}
}
void IOCPServer::ProcessPacket(int user_id, char* buf)
{
	e_PacketType packetType = static_cast<e_PacketType>(buf[1]);
	
	switch (packetType)
	{
	case e_PacketType::e_Select_GameMode:
	{
		cs_packet_selectGameMode* packet = reinterpret_cast<cs_packet_selectGameMode*>(buf);
		m_Clients[user_id].m_Player.m_gamemode = packet->mode;

		sc_packet_loginOK p{};
		p.m_id = user_id;
		p.size = sizeof(p);
		p.type = e_PacketType::e_LoginOK;

		Send_Packet(p.m_id, &p);
	}
	break;
	case e_PacketType::e_LeavePacket:
	{
		cs_packet_leavePacket* packet = reinterpret_cast<cs_packet_leavePacket*>(buf);
		
		Disconnect(packet->m_id);
	}
	break;
	case e_PacketType::e_BulletRotPacket:
	{
		cs_packet_bulletRotPacket* packet = reinterpret_cast<cs_packet_bulletRotPacket*>(buf);

		sc_packet_bulletRotPacket p{};
		p.m_id = packet->m_id;
		/*p.size = sizeof(p);
		p.type = e_PacketType::e_BulletRotPacket;*/

		m_Clients[packet->m_id].m_Player.slot1 = packet->slot1;
		m_Clients[packet->m_id].m_Player.slot2 = packet->slot2;
		m_Clients[packet->m_id].m_Player.slot3 = packet->slot3;

		/*cout << "packet slot1(xyz) : " << m_Clients[packet->m_id].m_Player.slot1.x << ", " << m_Clients[packet->m_id].m_Player.slot1.y << ", " << m_Clients[packet->m_id].m_Player.slot1.z << endl;
		cout << "packet slot2(xyz) : " << m_Clients[packet->m_id].m_Player.slot2.x << ", " << m_Clients[packet->m_id].m_Player.slot2.y << ", " << m_Clients[packet->m_id].m_Player.slot2.z << endl;
		cout << "packet slot3(xyz) : " << m_Clients[packet->m_id].m_Player.slot3.x << ", " << m_Clients[packet->m_id].m_Player.slot3.y << ", " << m_Clients[packet->m_id].m_Player.slot3.z << endl;*/

		/*for (auto& c : m_Clients)
		{
			if (c.second.m_Player.m_ID == 999) continue;
			if (c.second.m_Player.m_ID == packet->m_id) continue;

			Send_Packet(c.second.m_Player.m_ID, &p);
		}*/
	}
	break;
	case e_PacketType::e_ReadyPacket:
	{
		cs_packet_readyPacket* packet = reinterpret_cast<cs_packet_readyPacket*>(buf);

		m_Clients[user_id].m_Player.m_isReady = packet->isReady;
		int readycnt = 0;
		for (auto c : m_Clients)
		{
			if (c.second.m_Player.m_ID == 999) continue;
			if (c.second.m_Player.m_isReady) ++readycnt;
		}

		int team1cnt = 0;
		int team2cnt = 0;
		if (readycnt == 2)	// 2인 기준
		{
			for (auto& c : m_Clients)
			{
				if (c.second.m_Player.m_ID == 999) continue;
				if (team1cnt % 2 == 0)
				{
					c.second.m_Player.m_PlayerInfo.m_Position = PlayerPosition{ 5000.f + 200 * team1cnt++, 310.f, 42500.f };
					c.second.m_Player.m_PlayerInfo.m_Rotation = PlayerRotation{ 0.f, 90.f, 0.f };
				}
				else
				{// 6200 9500 250
					c.second.m_Player.m_PlayerInfo.m_Position = PlayerPosition{ 5200.f + 200 * team2cnt++, 310.f, 42500.f };
					c.second.m_Player.m_PlayerInfo.m_Rotation = PlayerRotation{ 0.f, -90.f, 0.f };
				}
			}

			for (auto& c : m_Clients)
			{
				if (c.second.m_Player.m_ID == 999) continue;

				sc_packet_startPacket p{};
				p.size = sizeof(p);
				p.type = e_PacketType::e_StartPacket;
				p.m_id = c.second.m_Player.m_ID;
				p.pos = c.second.m_Player.m_PlayerInfo.m_Position;
				p.rot = c.second.m_Player.m_PlayerInfo.m_Rotation;

				Send_Packet(c.second.m_Player.m_ID, &p);
			}

			for (auto& c : m_Clients)
			{
				if (c.second.m_Player.m_ID == 999) continue;
				// c는 본인 클라, cl은 타 클라
				for (auto& cl : m_Clients)
				{
					if (cl.second.m_Player.m_ID == 999) continue;

					if (cl.second.m_Player.m_ID != c.second.m_Player.m_ID)
					{
						// 내 클라 슬롯 정보를 다른 클라에게
						sc_packet_bulletRotPacket rp{};
						rp.m_id = c.second.m_Player.m_ID;
						rp.size = sizeof(rp);
						rp.type = e_PacketType::e_BulletRotPacket;
						rp.slot1 = m_Clients[c.second.m_Player.m_ID].m_Player.slot1;
						rp.slot2 = m_Clients[c.second.m_Player.m_ID].m_Player.slot2;
						rp.slot3 = m_Clients[c.second.m_Player.m_ID].m_Player.slot3;
						
						Send_Packet(cl.second.m_Player.m_ID, &rp);

						// 내 클라 정보를 다른 클라에게
						sc_packet_enterPacket sp{};
						sp.m_id = c.second.m_Player.m_ID;
						sp.pos = c.second.m_Player.m_PlayerInfo.m_Position;
						sp.rot = c.second.m_Player.m_PlayerInfo.m_Rotation;
						sp.size = sizeof(sp);
						sp.type = e_PacketType::e_EnterPacket;

						Send_Packet(cl.second.m_Player.m_ID, &sp);

						// 한번씩만 불리긴 함.
						//cout << cl.second.m_Player.m_ID << "가 " << c.second.m_Player.m_ID << "에게" << endl;
						//cout << "enter packet be sended to " << c.second.m_Player.m_ID << endl;
					}
				}
			}

			for (auto& c : m_Clients)
			{
				sc_packet_enterPacket mp{};
				mp.m_id = c.second.m_Player.m_ID;
				mp.pos = c.second.m_Player.m_PlayerInfo.m_Position;
				mp.rot = c.second.m_Player.m_PlayerInfo.m_Rotation;
				mp.size = sizeof(mp);
				mp.type = e_PacketType::e_myCharacterPacket;

				Send_Packet(c.second.m_Player.m_ID, &mp);
			}
		}
	}
	break;
	case e_PacketType::e_PlayerInfoPacket:
	{
		cs_packet_playerInfo* packet = reinterpret_cast<cs_packet_playerInfo*>(buf);

		m_Clients[packet->m_id].m_Player.m_PlayerInfo = packet->info;
		
		sc_packet_playerInfo p{};
		p.info = packet->info;//m_Clients[packet->m_id].m_Player.m_PlayerInfo;
		/*cout << "=====================================================================" << endl;
		cout << "id: " << packet->m_id << endl;
		cout << "pos : " <<p.info.m_Position.x << ", " << p.info.m_Position.y << ", " << p.info.m_Position.z << endl;
		cout << "rot : " <<p.info.m_Rotation.Pitch << ", " << p.info.m_Rotation.Yaw << ", " << p.info.m_Rotation.Roll<< endl;
		cout << "vel : " <<p.info.m_Velocity.vx << ", " << p.info.m_Velocity.vy << ", " << p.info.m_Velocity.vz<< endl;
		cout << "=====================================================================" << endl;*/

		/*cout << "=====================================================================" << endl;
		cout << "id: " << packet->m_id << endl;
		cout << "rot : " << p.info.m_Rotation.Pitch << ", " << p.info.m_Rotation.Yaw << ", " << p.info.m_Rotation.Roll << endl;
		cout << "=====================================================================" << endl;*/

		p.m_id = packet->m_id;
		p.size = sizeof(p);
		p.type = e_PacketType::e_PlayerInfoPacket;
		
		// 위에서 보내온 클라의 정보를
		// 서버가 타 클라들에게 쏴준다.
		for (auto& c : m_Clients)
		{
			if (c.second.m_Player.m_ID == 999) continue;
			if (c.second.m_Player.m_ID == packet->m_id) continue;
			
			Send_Packet(c.second.m_Player.m_ID, &p);
		}
	}
	break;
	case e_PacketType::e_BulletSpawnPacket:
	{
		cs_packet_bulletSpawnPacket* packet = reinterpret_cast<cs_packet_bulletSpawnPacket*>(buf);
		sc_packet_bulletSpawnPacket p{};
		memcpy(&p, packet, sizeof(p));

		for (auto& c : m_Clients)
		{
			if (c.second.m_Player.m_ID == 999) continue;
			//if (c.second.m_Player.m_ID == packet->m_id) continue;

			Send_Packet(c.second.m_Player.m_ID, &p);
		}
	}
	break;
	case e_PacketType::e_BulletSlotPacket:
	{
		cs_packet_bulletSlotPacket* packet = reinterpret_cast<cs_packet_bulletSlotPacket*>(buf);
		sc_packet_bulletSlotPacket p{};
		memcpy(&p, packet, sizeof(p));

		for (auto& c : m_Clients)
		{
			if (c.second.m_Player.m_ID == 999) continue;
			if (c.second.m_Player.m_ID == packet->m_id) continue;

			Send_Packet(c.second.m_Player.m_ID, &p);
		}
	}
	break;
	case e_PacketType::e_CharacterDeadPacket:
	{
		cs_packet_deadPacket* packet = reinterpret_cast<cs_packet_deadPacket*>(buf);
		sc_packet_deadPacket p{};
		memcpy(&p, packet, sizeof(p));

		for (auto& c : m_Clients)
		{
			if (c.second.m_Player.m_ID == 999) continue;
			if (c.second.m_Player.m_ID == packet->m_id) continue;

			Send_Packet(c.second.m_Player.m_ID, &p);
		}
	}
	break;
	case e_PacketType::e_CharacterReloadPacket:
	{
		cs_packet_reloadPacket* packet = reinterpret_cast<cs_packet_reloadPacket*>(buf);
		sc_packet_reloadPacket p{};
		memcpy(&p, packet, sizeof(p));

		for (auto& c : m_Clients)
		{
			if (c.second.m_Player.m_ID == 999) continue;
			if (c.second.m_Player.m_ID == packet->m_id) continue;

			Send_Packet(c.second.m_Player.m_ID, &p);
		}
	}
	break;
	case e_PacketType::e_CharacterLightPacket:
	{
		cs_packet_lightPacket* packet = reinterpret_cast<cs_packet_lightPacket*>(buf);
		sc_packet_lightPacket p{};
		memcpy(&p, packet, sizeof(p));

		for (auto& c : m_Clients)
		{
			if (c.second.m_Player.m_ID == 999) continue;
			if (c.second.m_Player.m_ID == packet->m_id) continue;

			Send_Packet(c.second.m_Player.m_ID, &p);
		}
	}
	break;
	default:
		cout << "Unknown Packet Type Error";
		DebugBreak();
		exit(-1);
	}

}

void IOCPServer::Send_Packet(int user_id, void* p)
{
	char* buf = reinterpret_cast<char*>(p);
	CLIENT& user = m_Clients[user_id];

	//별도의 센드용 확장오버랩구조체를 생성한다.
	EXOVER* exover = new EXOVER;
	exover->Op = OP_SEND;
	ZeroMemory(&exover->Over, sizeof(exover->Over));
	exover->WSABuf.buf = exover->IO_Buf;
	exover->WSABuf.len = buf[0];
	memcpy(exover->IO_Buf, buf, buf[0]);

	WSASend(user.m_Sock, &exover->WSABuf, 1, NULL, 0, &exover->Over, NULL);
}