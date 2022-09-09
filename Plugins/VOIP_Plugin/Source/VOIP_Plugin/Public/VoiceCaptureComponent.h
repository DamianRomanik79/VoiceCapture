// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <AudioCaptureComponent.h>
#include "VoiceCaptureComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCaptureFinished, class USoundWave*, RecordedVoice);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOIP_PLUGIN_API UVoiceCaptureComponent : public UAudioCaptureComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVoiceCaptureComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundSubmix* MicrophoneSubmix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAudioRecordingExportType RecordingType; 

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool StartCapture();

	UFUNCTION(BlueprintCallable)
	void StopCapture();

	UPROPERTY(BlueprintAssignable)
	FOnCaptureFinished OnCaptureFinished;
};
