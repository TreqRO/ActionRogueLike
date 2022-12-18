// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the instance
	// The spring arm component has some collision checking for us built in so that the camera never sticks through the wall.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// Allows the user to move the camera without moving the pawn
	// Boleans start with b
	SpringArmComp->bUsePawnControlRotation = true;
	// Make sure that the spring arm is properly attached to the character (which is in this case the root component)
	SpringArmComp->SetupAttachment(RootComponent);

	//  Rotate our character to whatever we're currently moving towards (Gets rids of the sideway movement.)
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Yaw is the horizontal rotation
	bUseControllerRotationYaw = false;

	// Create the instance
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	// Attach the camera to the spring (hence 3rd person)
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("interactionComp");

	
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Moves the character forward/backwards. Values from -1 to 0 and from 0 to 1 (Image a joystick)
// Move Forward 1, Move Backwards -1
// Bind key in: Edit/Project Settings/Engine/Inputs/Axis Bindings/"MoveForward"(the name from Setup)
void ASCharacter::MoveForward(float Value)
{
	// Move into the the direction of the camera instead of the direction of the pawn. E.g If the camera looked left then
	// we want to move in that direction and just rotate the character in that direction.
	FRotator ControlRot = GetControlRotation();

	// Make sure that we don't accidently mess with our input where we try to move into the floor or into the sky.
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// Specify in which direction to move (uses vectors, not rotators)
	AddMovementInput(ControlRot.Vector(),Value);
}

void ASCharacter::MoveRight(float Value)
{
	//Moves right regardless of the rotation of our pawn but instead based on the rotation of our camera
	// You get the Y (right vector) of the camera and use that for the A and D.

	// Vectors direction
	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	FRotator ControlRot = GetControlRotation();
	// Make sure that we don't accidently mess with our input where we try to move into the floor or into the sky.
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	// Plays the animation we pass in the Editor blue print!
	PlayAnimMontage(AttackAnim);

	//The time handle is just another struct which hold a handle to this timer. For example if we want to stop this timer before it started, we just
	// get the timer and we use that to clear it but we need to know which timer are we talking about.

	//If our character does die we want to cancel that attack since he already died.
	//Example below for this case
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
	

	// Add a timer so the Projectile will spawn when the hand is fully extended during the animation
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);


}

// Trigger attack after delay
void ASCharacter::PrimaryAttack_TimeElapsed()
{
	// Aserts example for pro-active detection of bugs
	// ensure only gives you an error once after compiling but then you can continue playing afterwards
	// ensureAlways give you an error every time you press play (don't do this in Tick Function).
	// Don't use check - Can't continue debugging afterwards
	// ensure doesn't run in packaged games
	if (ensure(ProjectileClass))
	{
		// Get the Right Hand Location 
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		//PROBLEM: PROJECTILES DON'T HIT WHERE THE PLAYER EXPECTS (under crosshair) (01:09 Assign2)
		// SOLUTION: Recalculate new projectile spawn rotation by using:
		// Spawn Location (Hand Position)
		// Impact Location (Line Trace Result)
		// Line trace object from the Center of the Camera to the first Object found (if any)

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		// Fills the hit information
		FHitResult Hit;


		// Camera Location and Rotation
		APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector camLocation = camManager->GetCameraLocation();
		//FVector camForward = camManager->GetCameraRotation().Vector();

		// End Location for the line trace
		FVector End = camLocation + (GetControlRotation().Vector() * 500000);

		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, camLocation, End, ObjectQueryParams);
		FTransform SpawnTM;
		FRotator CalculatedRotationProjectile;

		// If an object was hit
		if (bBlockingHit)
		{

			//AActor* HitActor = Hit.GetActor();
			CalculatedRotationProjectile = (Hit.Location - HandLocation).Rotation();

			//SpawnTM (Spawn Transform Matrix) is a transform. A transform is a struct that holds a rotation, location,  and a scale.
			// ROTATION = The control location (where we are looking at)
			// LOCATION  = It will spawn the projectile from right hand (combine it later with an attack animation (puts his hand forward) )
			//FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
			SpawnTM = FTransform(CalculatedRotationProjectile, HandLocation);
			
		}

		// If nothing was hit, use 'trace end' vector as desired target
		else
		{
			CalculatedRotationProjectile = (Hit.TraceEnd - HandLocation).Rotation();
			SpawnTM = FTransform(CalculatedRotationProjectile, HandLocation);
		}

		//(DRAWS STRAIGHT A LINE FROM Camera Center TO END POINT)
		// Used for debugging
		//FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
		//DrawDebugLine(GetWorld(), camLocation, End, LineColor, false, 2.0f, 0, 2.0f);

		//F actor SpawnParams just holding a ton of optional paramters for us to use.
		FActorSpawnParameters SpawnParams;
		//Allows us to specify the spawn rules (because when the object gets spawned in the world it will check its collision
		// and see like can I move myself a little bit to not overlap with anything when I spawn? But we just want to always spawn since
		// we're spawning ourselves in that character now it will not be able to even adjust itself a little bit.
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//Pass the instigator (the character that had spawn the projectile attack)
		SpawnParams.Instigator = this;

		//Spawn the Primary Attack Projectile
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}


}

void ASCharacter::BlackholeAttack()
{
	// Plays the animation we pass in the Editor blue print!
	PlayAnimMontage(AttackAnim);

	// Add a timer so the Projectile will spawn when the hand is fully extended during the animation
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::BlackholeAttack_TimeElapsed, 0.2f);
}

void ASCharacter::BlackholeAttack_TimeElapsed()
{
	if (ensure(ProjectileBlackHoleClass))
	{
		// Get the Right Hand Location 
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		// Fills the hit information
		FHitResult Hit;

		// Camera Location and Rotation
		APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector camLocation = camManager->GetCameraLocation();
		//FVector camForward = camManager->GetCameraRotation().Vector();

		// End Location for the line trace
		FVector End = camLocation + (GetControlRotation().Vector() * 500000);

		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, camLocation, End, ObjectQueryParams);
		FTransform SpawnTM;
		FRotator CalculatedRotationProjectile;

		// If an object was hit
		if (bBlockingHit)
		{

			//AActor* HitActor = Hit.GetActor();
			CalculatedRotationProjectile = (Hit.Location - HandLocation).Rotation();

			//SpawnTM (Spawn Transform Matrix) is a transform. A transform is a struct that holds a rotation, location,  and a scale.
			// ROTATION = The control location (where we are looking at)
			// LOCATION  = It will spawn the projectile from right hand (combine it later with an attack animation (puts his hand forward) )
			//FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
			SpawnTM = FTransform(CalculatedRotationProjectile, HandLocation);

		}

		// If nothing was hit, use 'trace end' vector as desired target
		else
		{
			CalculatedRotationProjectile = (Hit.TraceEnd - HandLocation).Rotation();
			SpawnTM = FTransform(CalculatedRotationProjectile, HandLocation);
		}

		//F actor SpawnParams just holding a ton of optional paramters for us to use.
		FActorSpawnParameters SpawnParams;

		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//Pass the instigator (the character that had spawn the projectile attack)
		SpawnParams.Instigator = this;

		//Spawn the BlackHole Attack Projectile
		GetWorld()->SpawnActor<AActor>(ProjectileBlackHoleClass, SpawnTM, SpawnParams);
	}
}

void ASCharacter::Dash()
{
	// Plays the animation we pass in the Editor blue print!
	PlayAnimMontage(AttackAnim);

	// Add a timer so the Projectile will spawn when the hand is fully extended during the animation
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::Dash_TimeElapsed, 0.2f);
}

void ASCharacter::Dash_TimeElapsed()
{
	if (ensure(ProjectileDashClass))
	{
		// Get the Right Hand Location 
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		// Fills the hit information
		FHitResult Hit;

		// Camera Location and Rotation
		APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector camLocation = camManager->GetCameraLocation();
		//FVector camForward = camManager->GetCameraRotation().Vector();

		// End Location for the line trace
		FVector End = camLocation + (GetControlRotation().Vector() * 500000);

		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, camLocation, End, ObjectQueryParams);
		FTransform SpawnTM;
		FRotator CalculatedRotationProjectile;

		// If an object was hit
		if (bBlockingHit)
		{

			CalculatedRotationProjectile = (Hit.Location - HandLocation).Rotation();

			//SpawnTM (Spawn Transform Matrix) is a transform. A transform is a struct that holds a rotation, location,  and a scale.
			// ROTATION = The control location (where we are looking at)
			// LOCATION  = It will spawn the projectile from right hand (combine it later with an attack animation (puts his hand forward) )
			//FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
			SpawnTM = FTransform(CalculatedRotationProjectile, HandLocation);

		}

		// If nothing was hit, use 'trace end' vector as desired target
		else
		{
			CalculatedRotationProjectile = (Hit.TraceEnd - HandLocation).Rotation();
			SpawnTM = FTransform(CalculatedRotationProjectile, HandLocation);
		}

		//F actor SpawnParams just holding a ton of optional paramters for us to use.
		FActorSpawnParameters SpawnParams;

		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//Pass the instigator (the character that had spawn the projectile attack)
		SpawnParams.Instigator = this;

		//Spawn the BlackHole Attack Projectile
		GetWorld()->SpawnActor<AActor>(ProjectileDashClass, SpawnTM, SpawnParams);
	}
}


void ASCharacter::PrimaryInteract()
{
	// Opens up the chest if its in the line of sight
	InteractionComp->PrimaryInteract();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// Adds Yaw as input (horizontal rotation)
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	//Making sure that we as the player can spawn the SMagicProjectile (By prssing a key hence we bind it to an action)
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);

	//Making sure that we as the player can spawn the SBlackholeProjectile (By prssing a key hence we bind it to an action)
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &ASCharacter::BlackholeAttack);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	//Makes the character jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);


}



