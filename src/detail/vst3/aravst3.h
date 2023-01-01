#pragma once

#include "../ara/ara.h"

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/base/falignpush.h"

#define ARA_ADDENDUM(x)
namespace ARA
{
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
