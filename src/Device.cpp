#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "OVR.h"
#include "Device.h"

// Device constructor
// Initialise a new OVR device with info
Device::Device(){

	// Initialise the OVR library
	System::Init();

	std::cout << "---- Oculus Rift VR Trading Desk ----" << std::endl;
	std::cout << "---- Detecting Oculus Rift ----" << std::endl;

	// Create a device manage to access headset
	this->pManager = *DeviceManager::Create();
	this->pFusionResult = new SensorFusion();

	// Get headset from manage
	this->pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	// Check that the headset is available
	if (pHMD){

		std::cout << " [x] Oculus HMD detected " << std::endl;

		// If it is, get device info and sensor
		this->InfoLoaded = pHMD->GetDeviceInfo(&Info);
		pSensor = *pHMD->GetSensor();

	} else {

		std::cout << " [ ] Oculus HMD not detected " << std::endl;

	}

	// Check that the sensor is available
	if (pSensor){

		std::cout << " [x] Oculus Sensor detected " << std::endl;

		// If it is, initialise the FusionResult object
		this->pFusionResult->AttachToSensor(pSensor);
	} else {

		std::cout << " [ ] Oculus Sensor detected " << std::endl;

	}

	// Check if OVR info was loaded
	if (InfoLoaded){

		// Display OVR Headset info
		std::cout << " DisplayDeviceName: " << Info.DisplayDeviceName << std::endl;
		std::cout << " ProductName: " << Info.ProductName << std::endl;
		std::cout << " Manufacturer: " << Info.Manufacturer << std::endl;
		std::cout << " Version: " << Info.Version << std::endl;
		std::cout << " HResolution: " << Info.HResolution << std::endl;
		std::cout << " VResolution: " << Info.VResolution << std::endl;
		std::cout << " HScreenSize: " << Info.HScreenSize << std::endl;
		std::cout << " VScreenSize: " << Info.VScreenSize << std::endl;
		std::cout << " VScreenCenter: " << Info.VScreenCenter << std::endl;
		std::cout << " EyeToScreenDistance: " << Info.EyeToScreenDistance << std::endl;
		std::cout << " LensSeparationDistance: " << Info.LensSeparationDistance << std::endl;
		std::cout << " InterpupillaryDistance: " << Info.InterpupillaryDistance << std::endl;
		std::cout << " DistortionK[0]: " << Info.DistortionK[0] << std::endl;
		std::cout << " DistortionK[1]: " << Info.DistortionK[1] << std::endl;
		std::cout << " DistortionK[2]: " << Info.DistortionK[2] << std::endl;
		std::cout << "--------------------------" << std::endl;

	} else {

		// If the OVR was not found, 
		// initialise Info to values simulating OVR
		this->Info.HResolution = 1280;
		this->Info.VResolution = 800;
		this->Info.HScreenSize = 0.149759993f;
		this->Info.VScreenSize = 0.0935999975f;
		this->Info.VScreenCenter = 0.0467999987f;
		this->Info.EyeToScreenDistance = 0.0410000011f;
		this->Info.LensSeparationDistance = 0.0635000020f;
		this->Info.InterpupillaryDistance = 0.0640000030f;
		this->Info.DistortionK[0] = 1.00000000f;
		this->Info.DistortionK[1] = 0.219999999f;
		this->Info.DistortionK[2] = 0.239999995f;
		this->Info.DistortionK[3] = 0.000000000f;
		this->Info.ChromaAbCorrection[0] = 0.995999992f;
		this->Info.ChromaAbCorrection[1] = -0.00400000019f;
		this->Info.ChromaAbCorrection[2] = 1.01400006f;
		this->Info.ChromaAbCorrection[3] = 0.000000000f;
		this->Info.DesktopX = 0;
		this->Info.DesktopY = 0;

	}

	// Initialise stereoConfig for calculating eye offset
	this->stereoConfig.SetHMDInfo(Info);

	// Set the camera position
	eyeLocation = glm::vec3(0.0f, 0.0f, 0.2f);
	origin = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Set the projection matrix 
	this->projection = glm::perspective(60.0f, ratio(), 0.1f, 100.f);

	// Set the modelview matrix
	this->modelview = glm::lookAt(eyeLocation, origin, up);

	// Set up eyes
	// Viewport location
	eyes[0].viewportLocation = glm::ivec2(0, 0);
	eyes[1].viewportLocation = glm::ivec2(Info.HResolution * 0.5f, 0);
	// Lens offset
	eyes[0].lensOffset = (Info.HScreenSize * 0.25f) - (4.0f * Info.LensSeparationDistance / Info.HScreenSize);
	eyes[1].lensOffset = -eyes[0].lensOffset;
	// Projection offset
	glm::vec3 projectionOffsetVector = glm::vec3(stereoConfig.GetProjectionCenterOffset() / 2.0f, 0, 0);
	eyes[0].projectionOffset = glm::translate(projection, projectionOffsetVector);
	eyes[1].projectionOffset = glm::translate(projection, -projectionOffsetVector);
	// ModelView offset
	glm::vec3 modelviewOffsetVector = glm::vec3(stereoConfig.GetIPD() / 2.0f, 0, 0);
	eyes[0].modelviewOffset = glm::translate(glm::mat4(), modelviewOffsetVector);
	eyes[1].modelviewOffset = glm::translate(glm::mat4(), -modelviewOffsetVector);

}

Device::~Device(){

	pSensor.Clear();
	pHMD.Clear();
	pManager.Clear();

	delete pFusionResult;

	System::Destroy();

}

// Print info from OVR 
void Device::DebugHeadset(){

	std::cout << std::endl << " Press ENTER to debug headset" << std::endl;

	std::cin.get();

	while (pSensor){

		Quatf quaternion = pFusionResult->GetOrientation();

		float yaw, pitch, roll;
		quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);

		std::cout << " Yaw " << RadToDegree(yaw) <<
			", Pitch: " << RadToDegree(pitch) <<
			", Roll: " << RadToDegree(roll) << std::endl;

		Sleep(50);

	}

}

float Device::ratio(){
	return (float)Info.HResolution / (float)Info.VResolution;
}