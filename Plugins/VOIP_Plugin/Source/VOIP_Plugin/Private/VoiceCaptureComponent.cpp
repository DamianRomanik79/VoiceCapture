// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceCaptureComponent.h"
#include <AudioMixerBlueprintLibrary.h>
#include <Components/SynthComponent.h>

// Sets default values for this component's properties
UVoiceCaptureComponent::UVoiceCaptureComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
, RecordingType(EAudioRecordingExportType::SoundWave)
{
	PrimaryComponentTick.bCanEverTick = true;

	// Makes sure microphone is not activated from the start
	bAutoActivate = false;
}


// Called when the game starts
void UVoiceCaptureComponent::BeginPlay()
{
	Super::BeginPlay();

	// Sets sound submix to mute playback from microphone when recording
	// (should be auto-created but it's need to be set for now)
	USoundSubmixBase* SubmixBase = Cast<USoundSubmixBase>(MicrophoneSubmix);
	SetSubmixSend(SubmixBase, 1);
	SoundSubmix = SubmixBase;
}


// Called every frame
void UVoiceCaptureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UVoiceCaptureComponent::StartCapture()
{
	// Check if microphone already recording and proceed accordingly
	if (IsPlaying())
	{
		return false;
	}
	else
	{
		// Starts capturing microphone
		Start();
		// Starts recording sounds output
		UAudioMixerBlueprintLibrary::StartRecordingOutput(GetOwner()->GetWorld(), 6000, MicrophoneSubmix);

		return true;
	};
}

void UVoiceCaptureComponent::StopCapture()
{
	// Stops microphone capturing
	Stop();

	// Creates path to save the file
	FString ContentPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectContentDir(), FString("VoiceRecords")));
	// Gets current time to create name of file
	FDateTime ActualTime = FDateTime::Now();
	FString FileName = ActualTime.ToString();
	FileName = FileName.Replace(TEXT("."), TEXT("_"));
	// For some reason when creating sound wave the folder name must be in file name
	// Needs to be fixed later
	if (RecordingType == EAudioRecordingExportType::SoundWave)
	{
		FileName = "VoiceRecords\\" + FileName;
	};
	// Creates SoundWave or wav file
	USoundWave* SoundWave = UAudioMixerBlueprintLibrary::StopRecordingOutput(GetOwner()->GetWorld(), RecordingType, FileName, ContentPath, MicrophoneSubmix);

	if (RecordingType == EAudioRecordingExportType::SoundWave)
	{
		// Calls delegate if SoundWave has been created
		OnCaptureFinished.Broadcast(SoundWave);
	}
}
