// Copyright Kacper Hankiewicz 2018

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}
// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();	
	FindPhysicsComponent();
	SetupInputComponent();

}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	///////UGrabber::GetReachLineEnd();
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->SetTargetLocation(GetLine().v2);
	}
}

void UGrabber::Grab()
{
	//UE_LOG(LogTemp, Error, TEXT("CLICKING"))
	auto HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
	AActor *ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		/* Grabs object*/
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ActorHit->GetActorLocation(),
			ActorHit->GetActorRotation()
			);
	}
}


void UGrabber::Release()
{
	//UE_LOG(LogTemp, Error, TEXT("Released"))
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input works"))
		InputComponent->BindAction("Grab", IE_Pressed, this,&UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this,&UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input Component"), *GetOwner()->GetName())
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FCollisionQueryParams TraceParams
	(
		FName(TEXT("")),
		false,
		GetOwner()
	);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT	Hit,
		GetLine().v1,
		GetLine().v2,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	AActor * ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("You are Grabbing %s"), *(ActorHit->GetName()))
	}
	return Hit;
}
FTwoVectors UGrabber::GetLine() const
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	FVector directionVector = playerViewPointRotation.Vector() * Reach;
	FVector lineTraceEnd = playerViewPointLocation + directionVector;
	return FTwoVectors(playerViewPointLocation, lineTraceEnd);
}


/// DRAWING DEBUG LINE	
/*DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255,0,0),
		false,
		0.f,
		0.f,
		5
	);
	UE_LOG(LogTemp, Error, TEXT("Current Location : %s Current Rotation : %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString())*/
