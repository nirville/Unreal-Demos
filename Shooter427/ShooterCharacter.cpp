// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter() :
	BaseTurnRate(45.f),
	BaseLookupRate(45.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create a camera boom (pulls in towards the character if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f; // The camera follow at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 50.f);

	/* Create a Follow Camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach camera to end of the boom
	FollowCamera->bUsePawnControlRotation = false; // Does not rotate the camera relative to arm

	// Don't rotate character when the controller rotates. Let the controller only affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 520.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay() called!"));

	int myInt{ 42 }; 
	UE_LOG(LogTemp, Warning, TEXT("int myInt: %d"), myInt);

	float myFloat{ 123.123f };
	UE_LOG(LogTemp, Warning, TEXT("float myFloat: %f"), myFloat);

	double myDouble{ 456.12345 };
	UE_LOG(LogTemp, Warning, TEXT("double myDouble: %lf"), myDouble);

	char myChar{ 's' };
	UE_LOG(LogTemp, Warning, TEXT("char myChar: %c"), myChar);

	wchar_t myWideChar{ L'S' };
	UE_LOG(LogTemp, Warning, TEXT("wchar_t myWideChar: %c"), myWideChar);

	bool myBool{ true };
	UE_LOG(LogTemp, Warning, TEXT("bool myBool: %d"), myBool);

	UE_LOG(LogTemp, Warning, TEXT("int: %d, float: %f, bool myBool: %d"), myInt, myFloat, myBool);

	FString myString( TEXT("My String!!!") );
	UE_LOG(LogTemp, Warning, TEXT("FString myString: %s"), *myString);

	UE_LOG(LogTemp, Warning, TEXT("Name of Instance: %s"), *GetName());
}

void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		/* find out which way is forward */
		FRotator Rotation{ Controller->GetControlRotation() };
		FRotator YawRotation{0, Rotation.Yaw, 0};

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		/* find out which way is right */
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{0, Rotation.Yaw, 0};

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()-> GetDeltaSeconds()); // deg/sec * sec/frame
}

void AShooterCharacter::LookupAtRate(float Rate)
{

	AddControllerPitchInput(Rate * BaseLookupRate *GetWorld()-> GetDeltaSeconds()); // deg/sec * sec/frame
}

//int incNum = 0;
// Fire Button is Clicked
void AShooterCharacter::FireWeapon()
{
	//incNum = incNum + 1;
	//UE_LOG(LogTemp, Warning, TEXT("Fire Weapon... %d"), incNum);

	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}

	/** Flash at Weapon Muzzle */
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if(BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		// Get the current display Viewport Size
		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		// Get screen space location of the crosshairs
		FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		CrosshairLocation.Y = CrosshairLocation.Y - 50.f;

		// Holds the location information of Crosshair in the World
		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;

		// Get world position and direction of crosshairs
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0), 
			CrosshairLocation, 
			CrosshairWorldPosition, 
			CrosshairWorldDirection);

		// was deproject successful?
		if (bScreenToWorld)
		{
			FHitResult ScreenTraceHit;
			const FVector Start{ CrosshairWorldPosition };
			const FVector End{ CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f };

			// Set beam end point to line trace end point
			FVector BeamEndPoint{ End };

			// Trace outward from crosshairs world location
			GetWorld()->LineTraceSingleByChannel(
				ScreenTraceHit, 
				Start, 
				End, 
				ECollisionChannel::ECC_Visibility);

			if (ScreenTraceHit.bBlockingHit) // was there a trace hit?
			{
				// Beam end point is now trace hit location 
				BeamEndPoint = ScreenTraceHit.Location;				
			}

			// Perform a second trace, this time the gun barrel
			FHitResult WeaponTraceHit;
			const FVector WeaponTraceStart{SocketTransform.GetLocation()};
			const FVector WeaponTranceEnd{ BeamEndPoint };
			GetWorld()->LineTraceSingleByChannel(
				WeaponTraceHit,
				WeaponTraceStart,
				WeaponTranceEnd,
				ECollisionChannel::ECC_Visibility
			);

			if (WeaponTraceHit.bBlockingHit) { // object between a gun barrel and BeamEndPoint?
				BeamEndPoint = WeaponTraceHit.Location;
			}

			// Spawn impact particles after updating BeamEndPoint
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					BeamEndPoint);
			}
			
			if (BeamParticles) 
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				BeamParticles,
				SocketTransform);
				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEndPoint);
				}
			}
		}
		
		/** Line Tracing when Fire */
		/*
		FHitResult FireHit;
		const FVector Start{ SocketTransform.GetLocation() };
		const FQuat Rotation{ SocketTransform.GetRotation() };
		const FVector RotationAxis{ Rotation.GetAxisX()};
		const FVector End{ Start + RotationAxis * 50'000.f };

		FVector BeamEndPoint = { End };

		GetWorld()->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
		DrawDebugPoint(GetWorld(), FireHit.Location, 15.f, FColor::Red, false, 2.f);

		// True when hitted by Weapon
		if (FireHit.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
			DrawDebugPoint(GetWorld(), FireHit.Location, 15.f, FColor::Red, false, 2.f);

			BeamEndPoint = FireHit.Location;

			// Spawned particle at the hit location
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.Location);
			}
		}

		// Set bullet stroke to end point
		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEndPoint);
			}
		}*/
	}

	/** Play Anim Montage using specific animation */
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireAnimMontage)
	{
		AnimInstance->Montage_Play(HipFireAnimMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Anim Montage selected."));
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookupAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	/* Binding Action Event */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AShooterCharacter::FireWeapon);
}