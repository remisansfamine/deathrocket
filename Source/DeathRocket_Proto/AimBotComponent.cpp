#include "AimBotComponent.h"

#include "Kismet/GameplayStatics.h"
#include "DeathRocket_ProtoCharacter.h"
#include "HealthComponent.h"

// Sets default values for this component's properties
UAimBotComponent::UAimBotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UAimBotComponent::PickEveryEnemies()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeathRocket_ProtoCharacter::StaticClass(), actors);

	// Save them in memory
	for (AActor* actor : actors)
	{
		ADeathRocket_ProtoCharacter* enemy = Cast<ADeathRocket_ProtoCharacter>(actor);

		//don't take self
		if (enemy->aimBotComp == this)
			continue;

		enemies.Add(actor);
	}
}

// Called when the game starts
void UAimBotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UAimBotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAimBotComponent::Aim(const FVector& cameraDir, const FVector& aimerPos)
{
	if (enemies.Num() == 0)
		PickEveryEnemies();

	float smallestAngle = maximumSight;
	int iterationIndex = 0;
	int targetIndex = 0;

	for (AActor* actor : enemies)
	{
		ADeathRocket_ProtoCharacter* enemy = Cast<ADeathRocket_ProtoCharacter>(actor);

		if (!enemy->healthComp->GetIsAlive())
			continue;

		FVector dirToEnemy = enemy->GetActorLocation() - aimerPos;
		dirToEnemy.Normalize();

		float cos = FVector::DotProduct(cameraDir.GetSafeNormal(), dirToEnemy);
		float angle = acosf(cos);	//angle in rad

		if (angle < smallestAngle)
		{
			smallestAngle = angle;
			targetIndex = iterationIndex;
		}

		++iterationIndex;
	}

	if (smallestAngle <= minimumAccuracy)
		target = enemies[targetIndex];
	else
		target = nullptr;
}

void UAimBotComponent::LoseTarget()
{
	target = nullptr;
}