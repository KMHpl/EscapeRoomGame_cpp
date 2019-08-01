// Copyright Kacper Hankiewicz 2018

#pragma once
#include "TwoVectors.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	///range of grabbing
	float Reach = 100.f;
	///sets physics handle
	UPhysicsHandleComponent *PhysicsHandle = nullptr;
	UInputComponent *InputComponent = nullptr;
	void Grab();
	void Release();
	void FindPhysicsComponent();
	///Checks if there are any inputs AT object that has this method 
	void SetupInputComponent();
	///checks if Phy Body is in range
	FHitResult GetFirstPhysicsBodyInReach() const;
	
	//Gets line with beginning and end.
	FTwoVectors GetLine() const;
};
