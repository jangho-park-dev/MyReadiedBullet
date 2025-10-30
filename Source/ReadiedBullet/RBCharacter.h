#pragma once

#include "ReadiedBullet.h"
#include "Engine.h"
#include "KeyItem.h"
#include "RBWeapon.h"
#include "RBPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/Character.h"
#include "RBCharacter.generated.h"



UCLASS()
class READIEDBULLET_API ARBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARBCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	//¾É±â
	void BeginCrouch();
	void EndCrouch();

	//ÁÜ
	void BeginZoom();
	void EndZoom();

	

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* MinimapSpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneCaptureComponent2D* MinimapCaptureComp;
	


	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.0, ClampMax = 100))
	float ZoomInterpSpeed;

	/*Default FOV set during begin play*/
	float DefaultFOV;


	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ARBWeapon> StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();

	void RemoteBulletPressed();
	void RemoteBulletReleased();



	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light")
	USpotLightComponent* SpotLightComp;

	FRotator GetCamRotator();

	ARBWeapon* CurrentWeapon;

	AProjectile* CurrentProjectile;

	void SelectSlot1();
	void SelectSlot2();
	void SelectSlot3();

	void SelectSlot1(int id);
	void SelectSlot2(int id);
	void SelectSlot3(int id);

	void LightOnOff();

	void Reload();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP")
	float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP")
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP")
	bool IsDead;

	bool IsReloading = false;
	bool LightTurn = false;
	float ReloadCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Magazine = 90;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ID")
	int m_ID;

	UPROPERTY()
	class ARBNetwork* m_NetWorkActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anim", Meta = (AllowPrivateAccess = true))
	UAnimMontage* ReloadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* ReloadCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* HitCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	class UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Path")
	TArray<UStaticMeshComponent*> PathMeshArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Path")
	bool IsPathMade = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Key")
	bool isKey = false;;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	bool bMenu = true;;


};