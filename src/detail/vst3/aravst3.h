#pragma once

#include "../ara/ara.h"

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/base/falignpush.h"

// these interfaces are copied from the ARA SDK to maintain compatibility without the full ARA SDK

#define ARA_ADDENDUM(x)
namespace ARA
{
	/***************************************************************************************************/
//! Interface class to be implemented by an object provided by the VST3 factory.
//! The host can use the VST3 factory to directly obtain the ARA factory, which allows for creating
//! and maintaining the model independently of any IAudioProcessor instances, enabling tasks such as
//! automatic tempo detection or audio-to-MIDI conversion.
//! For rendering and editing the model however, there must be an associated IAudioProcessor class
//! provided in the same binary.
//! This match is usually trivial because there typically is only one such class in the binary, but
//! there are cases such as WaveShell where multiple plug-ins live in the same binary, and only a
//! subset of those plug-ins support ARA. In this scenario, the plug-in must use the same class name
//! for the matching pair of ARA::IMainFactory and IAudioProcessor classes - this enables the host
//! to quickly identify the matching pairs without having to create instances of all the
//! IAudioProcessor classes to query their IPlugInEntryPoint::getFactory ()->factoryID to perform
//! the matching.
	class IMainFactory : public Steinberg::FUnknown
	{
	public:
		//! Get the ARA factory.
		//! The returned pointer must remain valid throughout the lifetime of the object that provided it.
		//! The returned ARAFactory must be equal to the ARAFactory provided by the associated
		//! IAudioProcessor class through its IPlugInEntryPoint.
		virtual const ARAFactoryPtr PLUGIN_API getFactory() = 0;

		static const Steinberg::FUID iid;
	};

	//! Class category name for the ARA::IMainFactory.
#if !defined (kARAMainFactoryClass)
#define kARAMainFactoryClass "ARA Main Factory Class"
#endif

	DECLARE_CLASS_IID(IMainFactory, 0xDB2A1669, 0xFAFD42A5, 0xA82F864F, 0x7B6872EA)

	//! ARA 2 extension of IPlugInEntryPoint, from the ARA SDK
	class ARA_ADDENDUM(2_0_Draft) IPlugInEntryPoint2 : public Steinberg::FUnknown
	{
	public:
		//! Extended version of bindToDocumentController ():
		//! bind the VST3 instance to an ARA document controller, switching it from "normal" operation
		//! to ARA mode with the assigned roles, and exposing the ARA plug-in extension.
		//! \p knownRoles encodes all roles that the host considered in its implementation and will explicitly
		//! assign to some plug-in instance(s), while \p assignedRoles describes the roles that this specific
		//! instance will fulfill.
		//! This may be called only once during the lifetime of the IAudioProcessor component, before
		//! the first call to setActive () or setState () or getProcessContextRequirements () or the
		//! creation of the GUI (see IPlugView).
		//! The ARA document controller must remain valid as long as the plug-in is in use - rendering,
		//! showing its UI, etc. However, when tearing down the plug-in, the actual order for deleting
		//! the IAudioProcessor instance and for deleting ARA document controller is undefined.
		//! Plug-ins must handle both potential destruction orders to allow for a simpler reference
		//! counting implementation on the host side.
		virtual const ARAPlugInExtensionInstancePtr PLUGIN_API bindToDocumentControllerWithRoles(ARADocumentControllerRef documentControllerRef,
			ARAPlugInInstanceRoleFlags knownRoles, ARAPlugInInstanceRoleFlags assignedRoles) = 0;
		static const Steinberg::FUID iid;
	};

	DECLARE_CLASS_IID(IPlugInEntryPoint2, 0xCD9A5913, 0xC9EB46D7, 0x96CA53AD, 0xD1DB89F5)

}

#include "pluginterfaces/base/falignpop.h"
