// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine.h"
#include "RBNetwork.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "ReadiedBullet/RBCharacter.h"
#include "ReadiedBullet/RBPlayerController.h"
#include "RBAnimInstance.h"
// Sets default values
ARBNetwork::ARBNetwork()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	memset(m_PacketBuf, 0, MAX_BUFFER);
	m_PrevSize = 0;

	static ConstructorHelpers::FClassFinder<ARBCharacter> Character(TEXT("/Game/Blueprints/BP_RBCharacter.BP_RBCharacter_C"));
	if (Character.Succeeded() && Character.Class != NULL)
	{
		BPCharacter = Character.Class;
	}

	static ConstructorHelpers::FClassFinder<AProjectile> Proj(TEXT("/Game/Blueprints/BP_Projectile.BP_Projectile_C"));
	if (Proj.Succeeded() && Proj.Class != NULL)
	{
		BPProjectile = Proj.Class;
	}
}

// Called when the game starts or when spawned
void ARBNetwork::BeginPlay()
{
	Super::BeginPlay();
	InitClientSocket();
	Connect();
	GetWorldTimerManager().SetTimer(m_SendTimer, this, &ARBNetwork::SendMyTransform, 0.016f, true);
}

// Called every frame
void ARBNetwork::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecvPacket();
}

void ARBNetwork::RecvPacket()
{
	DWORD RecvBytes = 0;
	DWORD flags = 0;

	//if ((pc != nullptr) && (pc->GetPawn() == nullptr))
	//{
	//	pc->OnPossess(m_myCharacter);
	//}
	//if ((m_myCharacter != nullptr) && (m_myCharacter->GetController() ) )
	//{
	//	if (Cast<ARBPlayerController>(m_myCharacter->GetController()))
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("mycharacter %s"), *m_myCharacter->GetController()->GetName());
	//	}
	//	else
	//	{
	//		//UE_LOG(LogTemp, Error, TEXT("mycharacter possess ARBcontroller  "));
	//	}
	//	
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("mycharacter unpossess  "));
	//}
	//if (pc)
	//{
	//	auto ch = Cast<ARBCharacter>(pc->GetPawn());
	//	if (ch != nullptr)
	//	{
	//		//UE_LOG(LogTemp, Error, TEXT("pc possess character id is %d "), ch->m_ID);
	//	}
	//	else
	//	{
	//		//UE_LOG(LogTemp, Error, TEXT("pc unpossess character "));
	//	}
	//}
	WSARecv(m_ClientSocket, &m_WSARecvBuf, 1, &RecvBytes, &flags, NULL, NULL);
	
	
	int iobytes = RecvBytes;
	ProcessPacket(iobytes, m_RecvBuf);
}

void ARBNetwork::ProcessPacket(int iobytes, char* buf)
{
	
	/*char* ptr = buf;
	static size_t in_packet_size = 0;
	while (iobytes > 0)
	{
		if (0 == in_packet_size)
			in_packet_size = ptr[0];
		if (iobytes + m_PrevSize >= in_packet_size)
		{
			memcpy(m_PacketBuf + m_PrevSize, ptr, in_packet_size - m_PrevSize);
			RecvPacketProcess(m_PacketBuf);
			ptr += in_packet_size - m_PrevSize;
			iobytes -= in_packet_size - m_PrevSize;
			in_packet_size = 0;
			m_PrevSize = 0;
		}
		else
		{
			memcpy(m_PacketBuf + m_PrevSize, ptr, iobytes);
			m_PrevSize += iobytes;
			iobytes = 0;
			break;
		}
	}*/

	int packet_size = 0;
	char* p = buf;
	int rest_byte = iobytes;

	// p가 맨 앞부분(즉, 크기)을 가리키고 있지 않으면?
	if (m_PrevSize != 0)
		packet_size = buf[0];

	while (rest_byte > 0)
	{
		// 새 패킷을 받아왔을 때.
		if (packet_size == 0)
			packet_size = *p;

		if (packet_size <= rest_byte + m_PrevSize)
		{	// 패킷 완성
			// cu.m_prev_size 만큼 뒤에 전송하려면, cu.m_prev_size만큼 뺀 크기를 복사해야 한다.
			memcpy(m_PacketBuf + m_PrevSize, p, packet_size - m_PrevSize);
			p += (packet_size - m_PrevSize);
			rest_byte -= (packet_size - m_PrevSize);
			packet_size = 0;
			//RecvPacketProcess(m_PacketBuf);
			switch ((e_PacketType)m_PacketBuf[1])
			{
			case e_PacketType::e_LoginOK:
			{
				sc_packet_loginOK* packet = reinterpret_cast<sc_packet_loginOK*>(m_PacketBuf);
				m_ID = packet->m_id;
			}
			break;
			case e_PacketType::e_BulletRotPacket:
			{
				sc_packet_bulletRotPacket* packet = reinterpret_cast<sc_packet_bulletRotPacket*>(m_PacketBuf);

				UE_LOG(LogTemp, Error, TEXT("e_BulletRotPacket id : %d"), packet->m_id);
				UE_LOG(LogTemp, Error, TEXT("e_BulletRotPacket slot1 :  %f  /   %f  /   %f"), packet->slot1.x, packet->slot1.y, packet->slot1.z);
				UE_LOG(LogTemp, Error, TEXT("e_BulletRotPacket slot2 :  %f  /   %f  /   %f"), packet->slot2.x, packet->slot2.y, packet->slot2.z);
				UE_LOG(LogTemp, Error, TEXT("e_BulletRotPacket slot3 :  %f  /   %f  /   %f"), packet->slot3.x, packet->slot3.y, packet->slot3.z);
				
				URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				GameInstance->SaveSlot1_InstanceX[packet->m_id] = packet->slot1.x;
				GameInstance->SaveSlot1_InstanceY[packet->m_id] = packet->slot1.y;
				GameInstance->SaveSlot1_InstanceZ[packet->m_id] = packet->slot1.z;

				GameInstance->SaveSlot2_InstanceX[packet->m_id] = packet->slot2.x;
				GameInstance->SaveSlot2_InstanceY[packet->m_id] = packet->slot2.y;
				GameInstance->SaveSlot2_InstanceZ[packet->m_id] = packet->slot2.z;

				GameInstance->SaveSlot3_InstanceX[packet->m_id] = packet->slot3.x;
				GameInstance->SaveSlot3_InstanceY[packet->m_id] = packet->slot3.y;
				GameInstance->SaveSlot3_InstanceZ[packet->m_id] = packet->slot3.z;
			}
			break;
			case e_PacketType::e_StartPacket:
			{
				sc_packet_startPacket* packet = reinterpret_cast<sc_packet_startPacket*>(m_PacketBuf);

				UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
				//UGameplayStatics::LoadStreamLevel(GetWorld(), FName("NewCity"), true, false, FLatentActionInfo{});
				UGameplayStatics::LoadStreamLevel(GetWorld(), FName("NewCity"), true, false, FLatentActionInfo{});
				UGameplayStatics::UnloadStreamLevel(GetWorld(), FName("Room"), FLatentActionInfo{}, false);
				
				FVector pos = FVector(packet->pos.x, packet->pos.y, packet->pos.z);
				FRotator rot = FRotator(packet->rot.Pitch, packet->rot.Yaw, packet->rot.Roll);
				
				////
				m_myPos = pos;
				m_myRot = rot;
				////

				UE_LOG(LogTemp, Error, TEXT("e_StartPacket'id : %d"), packet->m_id);
			
				//pc = GetWorld()->SpawnActor<ARBPlayerController>();

				//m_myCharacter = GetWorld()->SpawnActor<ARBCharacter>(BPCharacter, pos, rot, FActorSpawnParameters{});

				//pc = Cast<ARBPlayerController>(UGameplayStatics::GetGameMode(GetWorld())->SpawnPlayerController(ENetRole::ROLE_None, FVector::ZeroVector, FRotator::ZeroRotator) );
				
				/*if (m_myCharacter->GetController() != nullptr)
				{
					m_myCharacter->GetController()->OnUnPossess();
					m_myCharacter->GetController()->Destroy();
				}*/
				
				//pc->OnPossess(m_myCharacter);
				//m_OtherPlayers.Add(m_ID, m_myCharacter);
				//m_myCharacter->m_ID = packet->m_id;

				//UE_LOG(LogTemp, Error, TEXT("e_StartPacket : %d , %d"), m_myCharacter->m_ID, m_ID);
				if (m_myCharacter == nullptr)
					UE_LOG(LogTemp, Error, TEXT("e_StartPacket : m_myCharacter is nullptr!"));

				
			}
			break;
			case e_PacketType::e_EnterPacket:
			{
				sc_packet_enterPacket* packet = reinterpret_cast<sc_packet_enterPacket*>(m_PacketBuf);
				if (m_ID == packet->m_id) return;
				
				FVector pos = FVector(packet->pos.x, packet->pos.y, packet->pos.z);
				FRotator rot = FRotator(packet->rot.Pitch, packet->rot.Yaw, packet->rot.Roll);
				auto character = GetWorld()->SpawnActor<ARBCharacter>(BPCharacter, pos, rot, FActorSpawnParameters{} );
				//m_OtherPlayers.Add(packet->m_id, m_otherplayer);
				m_OtherPlayers[packet->m_id] = character;
				m_OtherPlayers[packet->m_id]->m_ID = packet->m_id;

				pc = GetWorld()->SpawnActor<ARBPlayerController>();
				pc->OnPossess(m_OtherPlayers[packet->m_id]);

				//m_OtherPlayers.FindRef(packet->m_id)->m_ID = packet->m_id;
				UE_LOG(LogTemp, Error, TEXT("other player's id : %d"), m_OtherPlayers[packet->m_id]->m_ID);

				/*if (m_OtherPlayers.FindRef(packet->m_id) == nullptr)
					UE_LOG(LogTemp, Error, TEXT("e_EnterPacket : m_OtherPlayers is nullptr!"));*/

				//GetWorldTimerManager().SetTimer(m_SendTimer, this, &ARBNetwork::SendMyTransform, 0.016f, true, 1.0f);

				/*m_myCharacter = GetWorld()->SpawnActor<ARBCharacter>(BPCharacter, m_myPos, m_myRot, FActorSpawnParameters{});
				m_myCharacter->m_ID = m_ID;*/

				if (m_myCharacter == nullptr)
					UE_LOG(LogTemp, Error, TEXT("e_StartPacket : m_myCharacter is nullptr!"));
			}
			break;
			case e_PacketType::e_myCharacterPacket:
			{
				sc_packet_myCharacterPacket* packet = reinterpret_cast<sc_packet_myCharacterPacket*>(m_PacketBuf);

				FVector pos = FVector(packet->pos.x, packet->pos.y, packet->pos.z);
				FRotator rot = FRotator(packet->rot.Pitch, packet->rot.Yaw, packet->rot.Roll);

				m_myCharacter = GetWorld()->SpawnActor<ARBCharacter>(BPCharacter, m_myPos, m_myRot, FActorSpawnParameters{});
				m_myCharacter->m_ID = m_ID;

				if (m_myCharacter == nullptr)
					UE_LOG(LogTemp, Error, TEXT("e_myCharacterPacket : m_myCharacter is nullptr!"));
			}
			break;
			case e_PacketType::e_PlayerInfoPacket:
			{
				sc_packet_playerInfo* packet = reinterpret_cast<sc_packet_playerInfo*>(m_PacketBuf);
				//auto character = m_OtherPlayers.FindRef(packet->m_id);
				// 타 클라의 정보를 받아 내 클라에 set해주는 부분.
				/*UE_LOG(LogTemp, Error, TEXT("%f %f %f"), m_myCharacter->GetActorLocation().X,
					m_myCharacter->GetActorLocation().Y, m_myCharacter->GetActorLocation().Z);*/
				//UE_LOG(LogTemp, Error, TEXT("e_PlayerInfoPacket's id : %d"), test);
				//UE_LOG(LogTemp, Error, TEXT("%f %f %f"), packet->info.m_Position.x, packet->info.m_Position.y, packet->info.m_Position.z);
				//UE_LOG(LogTemp, Error, TEXT("e_PlayerInfoPacket : packet id is %d"), packet->m_id);
				//if (m_OtherPlayers.FindRef(packet->m_id) == nullptr)
				//	UE_LOG(LogTemp, Error, TEXT("e_PlayerInfoPacket : Ref is nullptr"));

				auto pos = packet->info.m_Position;
				auto rot = packet->info.m_Rotation;
				auto vel = packet->info.m_Velocity;
				
				if (m_OtherPlayers[packet->m_id] != nullptr)
				{
					m_OtherPlayers[packet->m_id]->SetActorLocation(FVector(pos.x, pos.y, pos.z));
					m_OtherPlayers[packet->m_id]->SetActorRotation(FRotator(rot.Pitch, rot.Yaw, rot.Roll));
					m_OtherPlayers[packet->m_id]->AddMovementInput(FVector(vel.vx, vel.vy, vel.vz));
				}
			}
			break;
			case e_PacketType::e_BulletSpawnPacket:
			{
				cs_packet_bulletSpawnPacket* packet = reinterpret_cast<cs_packet_bulletSpawnPacket*>(m_PacketBuf);


				FVector pos{ packet->pos.x, packet->pos.y, packet->pos.z };
				FRotator rot{ packet->rot.Pitch, packet->rot.Yaw, packet->rot.Roll };
				
				UE_LOG(LogTemp, Error, TEXT("e_BulletSpawnPacket id : %d"), packet->m_id);
				UE_LOG(LogTemp, Error, TEXT("e_BulletSpawnPacket Pos : x %f  /  y %f  /  z %f"), packet->pos.x, packet->pos.y, packet->pos.z);
				UE_LOG(LogTemp, Error, TEXT("e_BulletSpawnPacket Rot : Pit %f  /  Yaw %f  /  Rol %f"), packet->rot.Pitch, packet->rot.Yaw, packet->rot.Roll);


				// 타 클라의 총알을 스폰하기 위해 타클라 정보 받아왔음 (슬롯이 뭔지도 알아야지)
				// 슬롯이 뭔지 어케 알지
				bulletSpawnID = packet->m_id;

				//URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

				switch (packet->bulletType)
				{
				case e_bulletType::e_Bullet1:
				{
					if (m_myCharacter->m_ID != packet->m_id)
					{
						m_OtherPlayers[packet->m_id]->SelectSlot1(packet->m_id);
						GetWorld()->SpawnActor<AProjectile>(BPProjectile, pos, rot, FActorSpawnParameters{});
					}
					else
					{
						m_myCharacter->SelectSlot1(packet->m_id);
						GetWorld()->SpawnActor<AProjectile>(BPProjectile, pos, rot, FActorSpawnParameters{});
					}
				}
				break;
				case e_bulletType::e_Bullet2:
				{
					if (m_myCharacter->m_ID != packet->m_id)
					{
						m_OtherPlayers[packet->m_id]->SelectSlot2(packet->m_id);
						GetWorld()->SpawnActor<AProjectile>(BPProjectile, pos, rot, FActorSpawnParameters{});
					}
					else
					{
						m_myCharacter->SelectSlot2(packet->m_id);
						GetWorld()->SpawnActor<AProjectile>(BPProjectile, pos, rot, FActorSpawnParameters{});
					}
				}
				break;
				case e_bulletType::e_Bullet3:
				{
					if (m_myCharacter->m_ID != packet->m_id)
					{
						m_OtherPlayers[packet->m_id]->SelectSlot3(packet->m_id);
						GetWorld()->SpawnActor<AProjectile>(BPProjectile, pos, rot, FActorSpawnParameters{});
					}
					else
					{
						m_myCharacter->SelectSlot3(packet->m_id);
						GetWorld()->SpawnActor<AProjectile>(BPProjectile, pos, rot, FActorSpawnParameters{});
					}
				}
				break;
				}

				
				
			}
			break;
			case e_PacketType::e_BulletSlotPacket:
			{
				sc_packet_bulletSlotPacket* packet = reinterpret_cast<sc_packet_bulletSlotPacket*>(m_PacketBuf);

				// 타 클라의 슬롯을 여기서 set했다.
				switch (packet->bulletType)
				{
				case e_bulletType::e_Bullet1:
				{
					m_OtherPlayers[packet->m_id]->SelectSlot1(packet->m_id);
				}
				break;
				case e_bulletType::e_Bullet2:
				{
					m_OtherPlayers[packet->m_id]->SelectSlot2(packet->m_id);
				}
				break;
				case e_bulletType::e_Bullet3:
				{
					m_OtherPlayers[packet->m_id]->SelectSlot3(packet->m_id);
				}
				break;
				}
			}
			break;
			case e_PacketType::e_CharacterDeadPacket:
			{
				sc_packet_deadPacket* packet = reinterpret_cast<sc_packet_deadPacket*>(m_PacketBuf);
				
				auto animinstance = Cast<URBAnimInstance>(m_OtherPlayers[packet->m_id]->GetMesh()->GetAnimInstance());
				animinstance->IsDead = true;
			}
			break;
			case e_PacketType::e_CharacterReloadPacket:
			{
				sc_packet_deadPacket* packet = reinterpret_cast<sc_packet_deadPacket*>(m_PacketBuf);

				m_OtherPlayers[packet->m_id]->Reload();
			}
			break;
			case e_PacketType::e_CharacterLightPacket:
			{
				sc_packet_lightPacket* packet = reinterpret_cast<sc_packet_lightPacket*>(m_PacketBuf);

				m_OtherPlayers[packet->m_id]->LightOnOff();
			}
			break;
			}
			m_PrevSize = 0;
		}
		else
		{	// 패킷 미완성
			// 처리해야 될 버퍼 + 전에 받은 크기에, 지금 처리하고 있는 버퍼를, 처리해야 할 남은 크기만큼.
			memcpy(m_PacketBuf + m_PrevSize, p, rest_byte);
			m_PrevSize += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}

void ARBNetwork::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Disconnect();
	memset(m_PacketBuf, 0, MAX_BUFFER);
	m_PrevSize = 0;
	GetWorldTimerManager().ClearTimer(m_SendTimer);
	
}
void ARBNetwork::SendMyTransform()
{
	PlayerPosition pos{};
	PlayerRotation rot{};
	PlayerVelocity vel{};

	
	if (m_myCharacter != nullptr)
	{
		auto apos = m_myCharacter->GetActorLocation();
		pos.x = apos.X;
		pos.y = apos.Y;
		pos.z = apos.Z;

		auto arot = m_myCharacter->GetActorRotation();
		rot.Pitch = arot.Pitch;
		rot.Roll = arot.Roll;
		rot.Yaw = arot.Yaw;

		auto avel = m_myCharacter->GetVelocity();
		vel.vx = avel.X;
		vel.vy = avel.Y;
		vel.vz = avel.Z;

		PlayerInfo info{ pos,rot,vel };

		cs_packet_playerInfo p{};
		p.m_id = m_myCharacter->m_ID;
		p.size = sizeof(p);
		p.type = e_PacketType::e_PlayerInfoPacket;
		p.info = info;

		DWORD SentBytes = 0;
		DWORD flags = 0;

		memcpy(m_SendBuf, &p, sizeof(p));
		m_WSASendBuf.len = sizeof(p);
		int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
	}
}

void ARBNetwork::SendProjectileSpawn(FVector loc, FRotator rot)
{
	PlayerPosition Pos{};
	PlayerRotation Rot{};

	if (m_myCharacter != nullptr)
	{
		Pos.x = loc.X;
		Pos.y = loc.Y;
		Pos.z = loc.Z;

		Rot.Pitch = rot.Pitch;
		Rot.Yaw = rot.Yaw;
		Rot.Roll = rot.Roll;

		URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		GameInstance->SelectSlot[m_ID];

		cs_packet_bulletSpawnPacket bp{};
		bp.m_id = m_myCharacter->m_ID;
		bp.pos = Pos;
		bp.rot = Rot;
		bp.bulletType = (e_bulletType)GameInstance->SelectSlot[m_ID];
		bp.size = sizeof(bp);
		bp.type = e_PacketType::e_BulletSpawnPacket;

		DWORD SentBytes = 0;
		DWORD flags = 0;

		memcpy(m_SendBuf, &bp, sizeof(bp));
		m_WSASendBuf.len = sizeof(bp);
		int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
	}
}

void ARBNetwork::SendBulletType(e_bulletType type)
{
	// 내가 바꾼 슬롯을 다른 클라에게 알리기 위해
	if (m_myCharacter != nullptr)
	{
		cs_packet_bulletSlotPacket bp{};
		bp.m_id = m_myCharacter->m_ID;
		bp.bulletType = type;
		bp.size = sizeof(bp);
		bp.type = e_PacketType::e_BulletSlotPacket;

		DWORD SentBytes = 0;
		DWORD flags = 0;

		memcpy(m_SendBuf, &bp, sizeof(bp));
		m_WSASendBuf.len = sizeof(bp);
		int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
	}
}

void ARBNetwork::SendCharacterDeadState(int id)
{
	if (m_myCharacter != nullptr)
	{
		cs_packet_deadPacket dp{};
		dp.m_id = id;
		dp.size = sizeof(dp);
		dp.type = e_PacketType::e_CharacterDeadPacket;
		

		DWORD SentBytes = 0;
		DWORD flags = 0;

		memcpy(m_SendBuf, &dp, sizeof(dp));
		m_WSASendBuf.len = sizeof(dp);
		int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
	}
}

void ARBNetwork::SendCharacterReloadState(int id)
{
	if (m_myCharacter != nullptr)
	{
		cs_packet_reloadPacket rp{};
		rp.m_id = id;
		rp.size = sizeof(rp);
		rp.type = e_PacketType::e_CharacterReloadPacket;


		DWORD SentBytes = 0;
		DWORD flags = 0;

		memcpy(m_SendBuf, &rp, sizeof(rp));
		m_WSASendBuf.len = sizeof(rp);
		int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
	}
}

void ARBNetwork::SendCharacterLightState(int id)
{
	if (m_myCharacter != nullptr)
	{
		cs_packet_lightPacket lp{};
		lp.m_id = id;
		lp.size = sizeof(lp);
		lp.type = e_PacketType::e_CharacterLightPacket;


		DWORD SentBytes = 0;
		DWORD flags = 0;

		memcpy(m_SendBuf, &lp, sizeof(lp));
		m_WSASendBuf.len = sizeof(lp);
		int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
	}
}

void ARBNetwork::SendBulletRotData()
{
	cs_packet_bulletRotPacket rp{};

	rp.m_id = m_ID;
	rp.size = sizeof(rp);
	rp.type = e_PacketType::e_BulletRotPacket;
	
	// 게임 인스턴스에 저장되어 있는 총알의 회전 방향을 패킷에 저장해 보낸다.
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	rp.slot1.x = GameInstance->SaveSlot1_InstanceX[m_ID];
	rp.slot1.y = GameInstance->SaveSlot1_InstanceY[m_ID];
	rp.slot1.z = GameInstance->SaveSlot1_InstanceZ[m_ID];
	
	rp.slot2.x = GameInstance->SaveSlot2_InstanceX[m_ID];
	rp.slot2.y = GameInstance->SaveSlot2_InstanceY[m_ID];
	rp.slot2.z = GameInstance->SaveSlot2_InstanceZ[m_ID];

	rp.slot3.x = GameInstance->SaveSlot3_InstanceX[m_ID];
	rp.slot3.y = GameInstance->SaveSlot3_InstanceY[m_ID];
	rp.slot3.z = GameInstance->SaveSlot3_InstanceZ[m_ID];

	DWORD SentBytes = 0;
	DWORD flags = 0;

	memcpy(m_SendBuf, &rp, sizeof(rp));
	m_WSASendBuf.len = sizeof(rp);
	int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
}

void ARBNetwork::Disconnect()
{
	cs_packet_leavePacket p{};
	p.m_id = m_ID;
	p.size = sizeof(p);
	p.type = e_PacketType::e_LeavePacket;

	DWORD SentBytes = 0;
	DWORD flags = 0;

	memcpy(m_SendBuf, &p, sizeof(p));
	m_WSASendBuf.len = sizeof(p);
	int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
}

void ARBNetwork::RecvPacketProcess(char* Packet)
{
	//char PacketType = Packet[1];

	switch ((e_PacketType)Packet[1])
	{
	case e_PacketType::e_LoginOK:
	{
		sc_packet_loginOK* packet = reinterpret_cast<sc_packet_loginOK*>(Packet);
		m_ID = packet->m_id;
	}
	break;
	default:
	{
		DebugBreak();
	}
		break;
	}
}
void ARBNetwork::Send_Select_GameMode(uint8 modeType)
{
	cs_packet_selectGameMode p{};
	p.mode = modeType;
	p.size = sizeof(p);
	p.type = e_PacketType::e_Select_GameMode;

	m_gamemode = modeType;

	DWORD SentBytes = 0;
	DWORD flags = 0;

	memcpy(m_SendBuf, &p, sizeof(p));
	m_WSASendBuf.len = sizeof(p);
	int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
	UE_LOG(LogTemp, Error, TEXT("Send_Select_GameMode %d"), retval);
}

void ARBNetwork::SetMyCharacter(ARBCharacter* character)
{
	this->m_myCharacter = character;
}

uint8 ARBNetwork::Get_Mode()
{
	return (uint8)m_gamemode;
}

bool ARBNetwork::Get_Ready()
{
	return m_IsReady;
}

void ARBNetwork::Set_Ready(bool setValue)
{
	m_IsReady = setValue;
}

void ARBNetwork::Send_Ready()
{
	cs_packet_readyPacket p{};

	if (m_IsReady)
		p.isReady = true;
	else
		p.isReady = false;

	p.m_id = m_ID;
	p.size = sizeof(p);
	p.type = e_PacketType::e_ReadyPacket;

	DWORD SentBytes = 0;
	DWORD flags = 0;

	memcpy(m_SendBuf, &p, sizeof(p));
	m_WSASendBuf.len = sizeof(p);
	int retval = WSASend(m_ClientSocket, &m_WSASendBuf, 1, &SentBytes, flags, NULL, NULL);
}

void ARBNetwork::InitClientSocket()
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	m_ClientSocket = WSASocket(AF_INET, SOCK_STREAM, 
		0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == m_ClientSocket)
		return;

	m_WSARecvBuf.buf = m_RecvBuf;
	m_WSARecvBuf.len = MAX_BUFFER;
	m_WSASendBuf.buf = m_SendBuf;
}

void ARBNetwork::Connect()//const char* serverIP)
{
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");//(serverIP);

	int retval = connect(m_ClientSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	UE_LOG(LogTemp, Error, TEXT("%d"), retval);
	if (retval == SOCKET_ERROR)
	{
		return;
	}
	m_IsRun = true;

	u_long sock_on = 1;
	retval =ioctlsocket(m_ClientSocket, FIONBIO, &sock_on);

	return;
}
