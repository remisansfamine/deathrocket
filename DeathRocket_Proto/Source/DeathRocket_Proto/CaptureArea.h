#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaptureArea.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCaptureAreaDelegate);


UCLASS()
class DEATHROCKET_PROTO_API ACaptureArea : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* areaCollider;

	UFUNCTION()
	void AreaCaptured();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool captured = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float curPercent;

	float tickFactor = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float resetAreaSpeed = 2.f;

	TArray<FColor> capturingTeams;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FColor previousCapturingTeam = FColor::Blue;

public:	

	ACaptureArea();

	virtual void Tick(float DeltaTime) override;

	void TickCapturePercent(const FColor& team, float deltaPercent);
	void UpdateColor();

	bool TryCaptureArea(const FColor& team);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintAssignable, Category = Event)
	FCaptureAreaDelegate OnCaptureCompleted;
	UPROPERTY(BlueprintAssignable, Category = Event)
	FCaptureAreaDelegate OnCaptureProcess;
};
