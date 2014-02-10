#ifndef DEVICE_H
#define DEVICE_H

#include <glm/glm.hpp>
#include "OVR.h"


using namespace OVR;

class Device {
	public:
		/* Variables holding device information */
		Ptr<DeviceManager>	pManager;
		Ptr<HMDDevice>		pHMD;
		Ptr<SensorDevice>	pSensor;
		SensorFusion*		pFusionResult;
		Util::Render::StereoConfig	stereoConfig;
		HMDInfo			Info;
		bool			InfoLoaded;

		glm::mat4 projection;
		glm::mat4 modelview;
		glm::vec3 eyeLocation;
		glm::vec3 origin;
		glm::vec3 up;

		// Eye offsets for rendering in stereo
		struct Eye {
			glm::ivec2 viewportLocation;
			glm::mat4 projectionOffset;
			glm::mat4 modelviewOffset;
			float lensOffset;
		} eyes[2];

		Device();
		~Device();
		void DebugHeadset();
		float ratio();

};

#endif /* DEVICE_H */